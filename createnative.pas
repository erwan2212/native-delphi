unit createnative;

interface

uses windows;

implementation

//Section Table 
type 
  TSections = array [0..0] of TImageSectionHeader;

type
  TImage_Import_Descriptor = packed record
    OriginalFirstThunk: Cardinal;
    TimeDateStamp: Cardinal;
    ForwarderChain: Cardinal;
    Name: Cardinal;
    FirstThunk: Cardinal;
  end;

  TImage_Import_Descriptors = array [0..0] of TImage_Import_Descriptor;


function RvaToFileOffset(INH: PImageNtHeaders; dwRVA: Cardinal): Cardinal;
var
  x: Word;
  FNumberOfSections : Cardinal;
  PSections: ^TSections;
begin
  Result := 0;

  FNumberOfSections := INH^.FileHeader.NumberOfSections;

  //ottengo il puntatore alla Section Table
  PSections := pointer(Cardinal(@(INH^.OptionalHeader)) +
                       INH^.FileHeader.SizeOfOptionalHeader);
  for x := 0 to (FNumberOfSections - 1) do
  begin
    if ((dwRVA >= PSections[x].VirtualAddress) and
        (dwRVA < PSections[x].VirtualAddress + PSections[x].SizeOfRawData)
        ) then
    begin
      Result := dwRVA -
                PSections[x].VirtualAddress +
                PSections[x].PointerToRawData;
      Break;
    end;
  end;
end;


procedure Create;
var
  sFile: String;
  iFileHandle: Integer;
  dwSize: Cardinal;
  pMem: Pointer;
  dwRead: Cardinal;
  IDH: PImageDosHeader;
  INH: PImageNtHeaders;
  pEntry: Pointer;
  i, j: Cardinal;
  pDllName: PChar;
  iFSave: Integer;

  //
  PImage_Import_Descriptors: ^TImage_Import_Descriptors;
  pRaw: Cardinal;
  //
begin
  pEntry := GetProcAddress(GetModuleHandle(nil),'NtProcessStartup');
  if (pEntry = nil) then
  begin
    MessageBoxA(0,'NtProcessStartup not exported',nil,0);
    Exit;
  end;

  pEntry := Pointer(DWord(pEntry)-GetModuleHandle(nil));
  //pEntry contiene quindi l'RVA della funzione NtProcessStartup;
  //tale valore verrà poi assegnato all'EntryPoint

  //ottengo il nome dell'eseguibile corrente
  sFile := Paramstr(0);

  //apro il file eseguibile
  iFileHandle := CreateFileA(
                       PChar(sFile),
                       GENERIC_READ,
                       FILE_SHARE_READ,
                       nil,
                       OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL,
                       0
                       );

  if (iFileHandle < 0) then
  begin
    MessageBoxA(0,'Error while opening file',nil,0);
    Exit;
  end;

  //ottengo la dimensione del file eseguibile
  dwSize := GetFileSize(iFileHandle,nil);
  if (dwSize = 0) then
  begin
    CloseHandle(iFileHandle);
    MessageBoxA(0,'Filesize not valid',nil,0);
    Exit;
  end;

  //vado ad allocare una quantità di memoria pari alla dimensione
  //del file eseguibile
  pMem := VirtualAlloc(
                nil,
                dwSize,
                MEM_COMMIT or MEM_RESERVE,
                PAGE_EXECUTE_READWRITE
                );
  if (pMem = nil) then
  begin
    CloseHandle(iFileHandle);
    MessageBoxA(0,'Error while allocating memory',nil,0);
    Exit;
  end;

  //copio il contenuto del file eseguibile nella fetta di memoria
  //appena allocata
  if (not ReadFile(iFileHandle, pMem^, dwSize, dwRead, nil)) or
    (DwRead <> dwSize) then
  begin
    VirtualFree(pMem,dwSize,MEM_DECOMMIT);
    CloseHandle(iFileHandle);
    MessageBoxA(0,'Error while reading file',nil,0);
    Exit;
  end;

  //ora che ho copiato il contenuto, posso anche chiudere
  //l'handle al file eseguibile
  CloseHandle(iFileHandle);

  //ok, d'ora in poi lavorerò con il contenuto di memoria indirizzato
  //da pMem che contiene appunto il file eseguibile

  //ottengo i puntatori al Dos Header ed al NT Image Header ed eseguo
  //i 2 controlli di base per verificare che il file copiato in memoria
  //sia effettivamente un PE: direi che è un controllo praticamente inutile
  IDH := pMem;
  if (IDH^.e_magic <> IMAGE_DOS_SIGNATURE) then
  begin
    VirtualFree(pMem,dwSize,MEM_DECOMMIT);
    MessageBoxA(0,'No PE File',nil,0);
    Exit;
  end;

  INH := Pointer(Integer(IDH)+IDH._lfanew);
  if (INH^.Signature <> IMAGE_NT_SIGNATURE) then
  begin
    VirtualFree(pMem,dwSize,MEM_DECOMMIT);
    MessageBoxA(0,'No PE File',nil,0);
    Exit;
  end;

  with INH^.OptionalHeader do
    begin
      //annullo i puntatori ed azzero le dimensioni relativamente a tutte le
      //Directory fatta eccezione per la Import Directory; le Directory presenti
      //sono le seguenti: resource, tls, export, relocate;
      //questo accorgimento non elimina tali Directory dal PE ma in ogni caso
      //fa in modo che non vengano prese in considerazione
      DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress := 0;
      DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size := 0;

      DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress := 0;
      DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size := 0;

      DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress := 0;
      DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size := 0;

      DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress := 0;
      DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size := 0;

      //assegnazione dell'EntryPoint
      AddressOfEntryPoint := DWord(pEntry);

      //setto il SubSystem Native
      Subsystem := 1;

      //vado a determinare il File Offset dell'Import Directory
      pRaw := RvaToFileOffset(
                   INH,
                   DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress
                   );

      if (pRaw = 0) then
        begin
          VirtualFree(pMem,dwSize,MEM_DECOMMIT);
          MessageBoxA(0,'Error while converting VA to RA',nil,0);
          Exit;
        end;

    end;


  //enumero i Descriptors; tutte le entry corrispondenti ad ntdll.dll
  //le sposto in alto ed alla fine azzero la entry successiva all'ultima entry
  //con ntdll.dll; in questo modo verranno prese in considerazione solo
  //le entry con ntdll.dll: è da ricordare infatti che la entry nulla
  //rappresenta la fine lista nell'elenco dei Descriptors
  PImage_Import_Descriptors := Pointer(DWord(IDH)+DWord(pRaw));
  i := 0;
  j := 0;
  while (PImage_Import_Descriptors[i].Name <> 0) do
    begin
      pDllName := PChar(
                       Cardinal(IDH) +
                       RvaToFileOffset(INH, PImage_Import_Descriptors[i].Name)
                       );
      if (pDllName = 'ntdll.dll') then
        begin
          PImage_Import_Descriptors[j] := PImage_Import_Descriptors[i];
          Inc(j);
        end;
      Inc(i);
    end;
  ZeroMemory(@PImage_Import_Descriptors[j], SizeOf(TImage_Import_Descriptor));

  //salvo il buffer modificato su un nuovo file su disco
  iFSave := CreateFileA(
                  PChar(sFile+'_new.exe'),
                  GENERIC_READ or GENERIC_WRITE,
                  FILE_SHARE_READ or FILE_SHARE_WRITE,
                  nil,
                  CREATE_ALWAYS,
                  FILE_ATTRIBUTE_NORMAL,
                  0
                  );
  if (iFSave < 0) then
  begin
    VirtualFree(pMem,dwSize,MEM_DECOMMIT);
    MessageBoxA(0,'Error while saving file',nil,0);
    Exit;
  end;

  if (not WriteFile(iFSave,pMem^,dwSize,dwRead,nil)) or (dwSize <> dwRead) then
  begin
    VirtualFree(pMem,dwSize,MEM_DECOMMIT);
    CloseHandle(iFSave);
    MessageBoxA(0,'Error while saving file',nil,0);
    Exit;
  end;

  MessageBoxA(0,'native file sucessful created','native',0);

  CloseHandle(iFSave);
  VirtualFree(pMem,dwSize,MEM_DECOMMIT);
end;

initialization
  Create;

end.