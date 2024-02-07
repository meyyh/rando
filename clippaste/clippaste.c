#include <windows.h>
#include <wchar.h>
#include <locale.h>
#include <stdio.h>
 
int main()
{
    setlocale(LC_ALL, ""); // Set the locale to the user's default

    OpenClipboard(NULL);
    HANDLE hData = GetClipboardData(CF_TEXT);

    char* inputString = (char*)GlobalLock(hData);

    // Create a keyboard event structure
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // 1 second delay to release any mod keys
    Sleep(1000);

    // Iterate through each character in the input string
    for (int i = 0; inputString[i] != L'\0'; ++i) {
        wchar_t wideChar;
        
        // Convert each character to its wide character (UTF-16)
        mbtowc(&wideChar, &inputString[i], 1);
        
        // Press a Unicode "key"
        ip.ki.dwFlags = KEYEVENTF_UNICODE;
        ip.ki.wVk = 0;
        ip.ki.wScan = wideChar;
        SendInput(1, &ip, sizeof(INPUT));
 
        // Release key
        ip.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        SendInput(1, &ip, sizeof(INPUT));
    }

    GlobalUnlock(hData);
    CloseClipboard();

    return 0;
}
