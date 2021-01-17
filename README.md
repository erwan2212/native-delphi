# native-delphi
fpc native2.fpr -Mdelphi <br>


A native app is an app that will be launched as soon as the kernel initialization is completed.

It will be launched (in user mode) by the session manager (smss.exe) thru the registry key HKLM\SYSTEM\CurrentControlSet\Control\SessionManager\BootExecute(run at every boot) or HKLM\SYSTEM\CurrentControlSet\Control\SessionManager\setupexecute(run once only).

A native app can only use NT API functions (ntdll.dll) and not the Windows API functions.

Possible usages :
nativereg createkey \Registry\Machine\SYSTEM\Setup key1
nativereg createvalue \Registry\Machine\SYSTEM\Setup\key1 test0 8 REG_RND_SZ
nativereg createvalue \Registry\Machine\SYSTEM\Setup\key1 test1 toto REG_SZ
nativereg createvalue \Registry\Machine\SYSTEM\Setup\key1 test2 112233AABBCC REG_BINARY
nativereg createvalue \Registry\Machine\SYSTEM\Setup\key1 test3 666 REG_DWORD
nativereg deletevalue \Registry\Machine\SYSTEM\Setup\key1 test1
nativereg deletekey \Registry\Machine\SYSTEM\Setup\key1

The tool is 32 bits (a 64 bits may come later).
It works on XP and up.
