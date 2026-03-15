#include <windows.h>
#include <stdio.h>

void send_key(WORD vk, DWORD flags)
{
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk;
    input.ki.dwFlags = flags;
    SendInput(1, &input, sizeof(INPUT));
}

int main()
{
    if (!OpenClipboard(NULL))
        return 1;

    if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
        return 1;

    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    wchar_t *text = (wchar_t*)GlobalLock(hData);

    if (!text)
        return 1;

    // Give user time to focus target window
    Sleep(1000);

    for (int i = 0; text[i] != L'\0'; i++)
    {
        SHORT vk = VkKeyScanW(text[i]);

        if (vk == -1)
            continue;

        BYTE vkCode = vk & 0xFF;
        BYTE modifiers = (vk >> 8) & 0xFF;

        // Press modifiers
        if (modifiers & 1) send_key(VK_SHIFT, 0);
        if (modifiers & 2) send_key(VK_CONTROL, 0);
        if (modifiers & 4) send_key(VK_MENU, 0);

        // Key down
        send_key(vkCode, 0);

        // Key up
        send_key(vkCode, KEYEVENTF_KEYUP);

        // Release modifiers
        if (modifiers & 4) send_key(VK_MENU, KEYEVENTF_KEYUP);
        if (modifiers & 2) send_key(VK_CONTROL, KEYEVENTF_KEYUP);
        if (modifiers & 1) send_key(VK_SHIFT, KEYEVENTF_KEYUP);
        Sleep(10);
    }

    GlobalUnlock(hData);
    CloseClipboard();

    return 0;
}
