# native-delphi
fpc native2.fpr -Mdelphi <br>
<br>

A native app is an app that will be launched as soon as the kernel initialization is completed.<br>
<br>
It will be launched (in user mode) by the session manager (smss.exe) thru the registry key HKLM\SYSTEM\CurrentControlSet\Control\SessionManager\BootExecute(run at every boot) or HKLM\SYSTEM\CurrentControlSet\Control\SessionManager\setupexecute(run once only).<br>
<br>
A native app can only use NT API functions (ntdll.dll) and not the Windows API functions.<br>
<br>
Possible usages :<br>
nativereg createkey \Registry\Machine\SYSTEM\Setup key1<br>
nativereg createvalue \Registry\Machine\SYSTEM\Setup\key1 test0 8 REG_RND_SZ<br>
nativereg createvalue \Registry\Machine\SYSTEM\Setup\key1 test1 toto REG_SZ<br>
nativereg createvalue \Registry\Machine\SYSTEM\Setup\key1 test2 112233AABBCC REG_BINARY<br>
nativereg createvalue \Registry\Machine\SYSTEM\Setup\key1 test3 666 REG_DWORD<br>
nativereg deletevalue \Registry\Machine\SYSTEM\Setup\key1 test1<br>
nativereg deletekey \Registry\Machine\SYSTEM\Setup\key1<br>
<br>
The tool is 32 bits (a 64 bits may come later).<br>
It works on XP and up.<br>
