#include <windows.h>
#include <stdio.h>
#include <time.h>


int main(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("mount iso and supply drive letter like G:\n");
        return 1;
    }

    srand(time(NULL));   // Initialization, should only be called once.
    int rand_num = rand();

    char extract_path[256]; //if temp path is some weird location might overflow

    // needed to use GetEnvironmentVariable as my temp path was
    // a unix like path for some reason so getenv(TEMP)
    // would return /c/Users/USERNAME/AppData/Local/Temp idk why
    TCHAR temp_path[MAX_PATH];
    DWORD size = GetEnvironmentVariable(TEXT("TEMP"), temp_path, MAX_PATH);
    if (!size || size > MAX_PATH) {
        printf("no TEMP env var or other error with env vars\n");
    } 
    const char *extract_dir_name = "\\wimedit-";

    char num_str[11];  // 10 digits + null terminator
    snprintf(num_str, sizeof(num_str), "%010d", rand_num);  // Format number with at most 10 digits

    strcpy(extract_path, temp_path);
    strcat(extract_path, extract_dir_name);
    strcat(extract_path, num_str);

    SHFILEOPSTRUCT file_op;
    ZeroMemory(&file_op, sizeof(file_op));
    file_op.hwnd = NULL;                  // No parent window
    file_op.wFunc = FO_COPY;              // Copy operation
    file_op.pFrom = argv[1];              // Source path
    file_op.pTo = extract_path;           // Destination path
    file_op.fFlags = FOF_ALLOWUNDO |      // Allow undo
                     FOF_SIMPLEPROGRESS | // Show simple progress dialog
                     FOF_NOCONFIRMMKDIR;  // No "create directory" confirmation

    int result = SHFileOperation(&file_op);
    if (result != 0) {
        printf("Failed to copy files. Error code: %d\n", result);
        return 1;
    }

    printf("Files copied successfully to: %s\n", extract_path);
    


    return 0;
}

//oscdimg.exe -u2 -udfver102 -bC:\Users\meyyh\AppData\Local\Temp\wimedit_1988392313\efi\microsoft\boot\efisys.bin -h C:\Users\meyyh\AppData\Local\Temp\wimedit_1988392313 C:\Users\meyyh\wint.iso
