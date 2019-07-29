const ntdll = 'ntdll.dll';

type
  TUNICODE_STRING  = packed record
                      wLength       : Word;
                      wMaximumLength: Word;
                      pBuffer       : PWideChar;
                    end;
  PUNICODE_STRING = ^TUNICODE_STRING;

  PVOID = ^Pointer;
  ULONG =  Longint;
  PULONG =  ^ULONG;
  DWORD =  Integer;
  BOOL  =  LongBool;
  UCHAR =      Byte;
  SHORT =      Smallint;
  NTSTATUS = LongWord;
  
  TSarray = array of widestring;
  TSarray2 = array[0..7] of pwidechar;
  
  TENVIRONMENT_INFORMATION = packed record
    Unknown: array[0..20] of Cardinal;
    CommandLine: TUNICODE_STRING;
    ImageFile: TUNICODE_STRING;
  end;
  PENVIRONMENT_INFORMATION = ^TENVIRONMENT_INFORMATION;


  PSTARTUP_ARGUMENT = ^TSTARTUP_ARGUMENT;
  TSTARTUP_ARGUMENT = packed record
    Unknown: array [0..2] of Cardinal;
    Environment: PENVIRONMENT_INFORMATION;
  end;
  
  TObjectAttributes = packed record
    Length: ULONG;
    RootDirectory: THandle;
    ObjectName: PUNICODE_STRING;
    Attributes: ULONG;
    SecurityDescriptor: PVOID; // Points to type SECURITY_DESCRIPTOR
    SecurityQualityOfService: PVOID; // Points to type SECURITY_QUALITY_OF_SERVICE
  end;
  OBJECT_ATTRIBUTES = TObjectAttributes;
  POBJECT_ATTRIBUTES = ^OBJECT_ATTRIBUTES;
  
    PRtlUserProcessParameters32 = ^TRtlUserProcessParameters32;
  TRtlUserProcessParameters32 = packed record
    MaximumLength    : Cardinal;                                    // 0x0000
    Length           : Cardinal;                                    // 0x0004
    Flags            : Cardinal;                                    // 0x0008
    DebugFlags       : Cardinal;                                    // 0x000C
    ConsoleHandle    : pointer;                                    // 0x0010
    ConsoleFlags     : Cardinal;                                    // 0x0014
    StandardInput    : pointer;                                    // 0x0018
    StandardOutput   : pointer;                                    // 0x001C
    StandardError    : pointer;                                    // 0x0020
    CurrentDirectory : TUNICODE_STRING;                                 // 0x0024
	CurrentDirectoryHandle: pointer ; 									//2c
    DllPath          : TUNICODE_STRING;                                 // 0x0030
    ImagePathName    : TUNICODE_STRING;                                 // 0x0038
    CommandLine      : TUNICODE_STRING;                                 // 0x0040
    Environment      : pointer;                                    // 0x0048
    StartingX        : Cardinal;                                    // 0x004C
    StartingY        : Cardinal;                                    // 0x0050
    CountX           : Cardinal;                                    // 0x0054
    CountY           : Cardinal;                                    // 0x0058
    CountCharsX      : Cardinal;                                    // 0x005C
    CountCharsY      : Cardinal;                                    // 0x0060
    FillAttribute    : Cardinal;                                    // 0x0064
    WindowFlags      : Cardinal;                                    // 0x0068
    ShowWindowFlags  : Cardinal;                                    // 0x006C
    WindowTitle      : TUNICODE_STRING;                                 // 0x0070
    DesktopInfo      : TUNICODE_STRING;                                 // 0x0078
    ShellInfo        : TUNICODE_STRING;                                 // 0x0080
    RuntimeData      : TUNICODE_STRING;                                 // 0x0088
    CurrentDirectores: array [0..511] of byte;  // 0x0090
    EnvironmentSize  : cardinal;                                    // 0x0290
  end;
  
  PPeb32 = ^TPeb32;
  TPeb32 = packed record
    InheritedAddressSpace             : byte;                    // 0x0000
    ReadImageFileExecOptions          : byte;                    // 0x0001
    BeingDebugged                     : byte;                    // 0x0002
    BitField                          : byte;                    // 0x0003
    Mutant                            : cardinal;                   // 0x0004
    ImageBaseAddress                  : cardinal;                   // 0x0008
    Ldr                               : cardinal;                   // 0x000C
    ProcessParameters                 : PRtlUserProcessParameters32;                   // 0x0010
    SubSystemData                     : cardinal;                   // 0x0014
    ProcessHeap                       : cardinal;                   // 0x0018
    FastPebLock                       : cardinal;                   // 0x001C
    AtlThunkSListPtr                  : cardinal;                   // 0x0020
    IFEOKey                           : cardinal;                   // 0x0024
    CrossProcessFlags                 : cardinal;                   // 0x0028
    u1   							  : pointer;                  // 0x002C
    SystemReserved                    : array [0..0] of cardinal;   // 0x0030
    SpareUlong                        : cardinal;                   // 0x0034
    SparePebPtr0                      : cardinal;                   // 0x0038
    TlsExpansionCounter               : cardinal;                   // 0x003C
    TlsBitmap                         : pointer;                   // 0x0040
    TlsBitmapBits                     : array [0..1] of cardinal;   // 0x0044
    ReadOnlySharedMemoryBase          : pointer;                   // 0x004C
    HotpatchInformation               : pointer;                   // 0x0050
    ReadOnlyStaticServerData          : pointer;                   // 0x0054
    AnsiCodePageData                  : pointer;                   // 0x0058
    OemCodePageData                   : pointer;                   // 0x005C
    UnicodeCaseTableData              : pointer;                   // 0x0060
    NumberOfProcessors                : cardinal;                   // 0x0064
    NtGlobalFlag                      : cardinal;                   // 0x0068
    __alignment_006C                  : cardinal;
    CriticalSectionTimeout            : int64;                   // 0x0070 TLargeInteger
    HeapSegmentReserve                : cardinal;                   // 0x0078
    HeapSegmentCommit                 : cardinal;                   // 0x007C
    HeapDeCommitTotalFreeThreshold    : cardinal;                   // 0x0080
    HeapDeCommitFreeBlockThreshold    : cardinal;                   // 0x0084
    NumberOfHeaps                     : cardinal;                   // 0x0088
    MaximumNumberOfHeaps              : cardinal;                   // 0x008C
    ProcessHeaps                      : pointer;                   // 0x0090
    GdiSharedHandleTable              : pointer;                   // 0x0094
    ProcessStarterHelper              : pointer;                   // 0x0098
    GdiDCAttributeList                : cardinal;                   // 0x009C
    LoaderLock                        : pointer;                   // 0x00A0
    OSMajorVersion                    : cardinal;                   // 0x00A4
    OSMinorVersion                    : cardinal;                   // 0x00A8
    OSBuildNumber                     : word;                   // 0x00AC
    OSCSDVersion                      : word;                   // 0x00AE
    OSPlatformId                      : cardinal;                   // 0x00B0
    ImageSubsystem                    : cardinal;                   // 0x00B4
    ImageSubsystemMajorVersion        : cardinal;                   // 0x00B8
    ImageSubsystemMinorVersion        : cardinal;                   // 0x00BC
    ActiveProcessAffinityMask         : cardinal;                   // 0x00C0
    GdiHandleBuffer                   : array [0..33] of cardinal;  // 0x00C4
    PostProcessInitRoutine            : pointer;                   // 0x014C
    TlsExpansionBitmap                : pointer;                   // 0x0150
    TlsExpansionBitmapBits            : array [0..31] of cardinal;  // 0x0154
    SessionId                         : cardinal;                   // 0x01D4
    AppCompatFlags                    : int64;                   // 0x01D8 TULargeInteger
    AppCompatFlagsUser                : int64;                   // 0x01E0 TULargeInteger
    pShimData                         : pointer;                   // 0x01E8
    AppCompatInfo                     : pointer;                   // 0x01EC
    CSDVersion                        : TUNICODE_STRING;                // 0x01F0
    ActivationContextData             : pointer;                   // 0x01F8
    ProcessAssemblyStorageMap         : pointer;                   // 0x01FC
    SystemDefaultActivationContextData: pointer;                   // 0x0200
    SystemAssemblyStorageMap          : pointer;                   // 0x0204
    MinimumStackCommit                : cardinal;                   // 0x0208
    FlsCallback                       : pointer;                   // 0x020C
    FlsListHead                       : TUNICODE_STRING;             // 0x0210
    FlsBitmap                         : pointer;                   // 0x0218
    FlsBitmapBits                     : array [0..3] of cardinal;   // 0x021C
    FlsHighIndex                      : cardinal;                   // 0x022C
    WerRegistrationData               : pointer;                   // 0x0230
    WerShipAssertPtr                  : pointer;                   // 0x0234
  
  end;                                                            // 0x0238


  TRTL_HEAP_DEFINITION = packed record
    Length,
    Unknown1,
    Unknown2,
    Unknown3,
    Unknown4,
    Unknown5,
    Unknown6,
    Unknown7,
    Unknown8,
    Unknown9,
    Unknown10,
    Unknown11,
    Unknown12: Cardinal;
  end;
  PRTL_HEAP_DEFINITION = ^TRTL_HEAP_DEFINITION;
  

function NtCreateKey(var KeyHandle: THandle; DesiredAccess: ULONG; var
    ObjectAttributes: OBJECT_ATTRIBUTES; TitleIndex: ULONG; Class_:
    PUNICODE_STRING; CreateOptions: ULONG; var Disposition: ULONG): NTSTATUS;
    stdcall; external ntdll name 'NtCreateKey';

function NtSetValueKey(KeyHandle: THandle; ValueName: PUNICODE_STRING;
    TitleIndex: ULONG; Type_: ULONG; Data: pointer; DataSize: ULONG): NTSTATUS; //data:pvoid
    stdcall; external ntdll name 'NtSetValueKey';  
	
function NtDeleteKey(KeyHandle: THandle): NTSTATUS; stdcall; external ntdll name 'NtDeleteKey';

function NtDeleteValueKey(KeyHandle: THandle;ValueName: PUNICODE_STRING): NTSTATUS; stdcall; external ntdll name 'NtDeleteValueKey';

function NtOpenKey(var KeyHandle:THandle;DesiredAccess: ULONG;var ObjectAttributes: OBJECT_ATTRIBUTES): NTSTATUS; stdcall;
    external ntdll name 'NtOpenKey';

function NtClose(KeyHandle: THandle): NTSTATUS; stdcall; external ntdll name 'NtClose';	
  
function NtDelayExecution(Alertable:boolean;Interval:PInt64): Cardinal; stdcall; external ntdll;

function NtTerminateProcess(
                    dwInHandle: Cardinal; 
                    dwInExitCode: Cardinal
                    ): Cardinal; stdcall; external ntdll;
					
function RtlCreateHeap(
                    dwInFlags: Cardinal; 
                    pInBase: Pointer; 
                    dwInReserve: Cardinal;
                    dwInCommit: Cardinal; 
                    bInLock: Boolean; 
                    RtlHeapParams: PRTL_HEAP_DEFINITION
                    ): Cardinal; stdcall; external ntdll;
					
function NtDisplayString(sString: PUNICODE_STRING): Cardinal; stdcall; external ntdll;

function RtlAllocateHeap(
               dwInHeapHandle: Cardinal; 
               dwInFlags: Cardinal; 
               dwInSize: Cardinal
               ): Pointer; stdcall; external ntdll;

function RtlFreeHeap(
               dwInHeapHandle: Cardinal; 
               dwInFlags: Cardinal; 
               pInMemoryPointer: Pointer
               ): Cardinal; stdcall; external ntdll;

function swprintf(
                sDestText: PWideChar; 
                sFormat: PWideChar; 
                sSourceText: PWideChar
                ): Cardinal; cdecl; external ntdll;
				
function wcslen(sText: PWideChar): Cardinal; cdecl; external ntdll;

function  RtlUnicodeStringToInteger(
    Str : PUNICODE_STRING;
    Base : ULONG;
    Value : PULONG
  ): NTSTATUS; stdcall; external ntdll;
  
function RtlRandomEx(Seed:pulong):ulong;stdcall; external ntdll;  