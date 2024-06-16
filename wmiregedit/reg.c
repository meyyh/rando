#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#include "include/wimlib.h"


//wimlib stuffs
#ifdef _WIN32
#  define main		wmain
   typedef wchar_t	tchar;
#  define TS		"ls"
#else
   typedef char		tchar;
#  define TS		"s"
#endif

static enum wimlib_progress_status
extract_progress(enum wimlib_progress_msg msg,
		 union wimlib_progress_info *info, void *progctx)
{
	switch (msg) {
	case WIMLIB_PROGRESS_MSG_EXTRACT_STREAMS:
		printf("Extracting files: %.2f%% complete\n",
		       TO_PERCENT(info->extract.completed_bytes,
				  info->extract.total_bytes));
		break;
	default:
		break;
	}
	return WIMLIB_PROGRESS_STATUS_CONTINUE;
}


char* replaceWord(const char* s, const char* oldW, 
                const char* newW) 
{ 
    char* result; 
    int i, cnt = 0; 
    int newWlen = strlen(newW); 
    int oldWlen = strlen(oldW); 
 
    // Counting the number of times old word 
    // occur in the string 
    for (i = 0; s[i] != '\0'; i++) { 
        if (strstr(&s[i], oldW) == &s[i]) { 
            cnt++; 
 
            // Jumping to index after the old word. 
            i += oldWlen - 1; 
        } 
    } 
 
    // Making new string of enough length 
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1); 
 
    i = 0; 
    while (*s) { 
        // compare the substring with the result 
        if (strstr(s, oldW) == s) { 
            strcpy(&result[i], newW); 
            i += newWlen; 
            s += oldWlen; 
        } 
        else
            result[i++] = *s++; 
    } 
 
    result[i] = '\0'; 
    return result; 
}

void make_iso(BOOL local_oscdimg, char **argv, char extract_path[])
{
    char oscdimg_cmd[4096];

    if(local_oscdimg == TRUE){
        strcpy(oscdimg_cmd, ".\\oscdimg.exe");
    }
    else if(local_oscdimg == FALSE){
        strcpy(oscdimg_cmd, "oscdimg.exe");
    }
    else{
        printf("wut\n");
    }

    strcat(oscdimg_cmd, " -m -o -u2 -udfver102 -bootdata:2#p0,e,b");
    strcat(oscdimg_cmd, extract_path);
    strcat(oscdimg_cmd, "\\boot\\etfsboot.com#pEF,e,b");
    strcat(oscdimg_cmd, extract_path);
    strcat(oscdimg_cmd, "\\efi\\microsoft\\boot\\efisys.bin -h ");
    strcat(oscdimg_cmd, extract_path);
    strcat(oscdimg_cmd, " ");
    strcat(oscdimg_cmd, argv[2]);
    printf("%s\n", oscdimg_cmd);
    
    //example might look like this
    //oscdimg.exe -m -o -u2 -udfver102 -bootdata:2#p0,e,bC:\Users\USERNAME\AppData\Local\Temp\wimedit_1988392313\boot\etfsboot.com#pEF,e,bC:\Users\USERNAME\AppData\Local\Temp\wimedit_1988392313\efi\microsoft\boot\efisys.bin -h C:\Users\USERNAME\AppData\Local\Temp\wimedit_1988392313 C:\Users\USERNAME\wint2.iso

    // system() processes escape characters again
    char *new_oscdimg_cmd = replaceWord(oscdimg_cmd, "\\", "\\\\");
    system(new_oscdimg_cmd);
}

void edit_wim(char extract_path[])
{
    char wim_path[1024];
    strcpy(wim_path, extract_path);
    strcat(wim_path, "\\sources\\install.wim");
    
    WIMStruct *wim = NULL;
    int ret = wimlib_open_wim(wim_path,
                              0,        // 0 is defaults
                              &wim);
    if(ret != 0)
    {
        wimlib_free(wim);
        fprintf(stderr, "wimlib error %d: %" TS"\n",
			ret, wimlib_get_error_string((enum wimlib_error_code)ret));
        wimlib_global_cleanup();
    }

    wimlib_register_progress_function(wim, extract_progress, NULL);

    wimlib_get_image_name;
    
}

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("mount iso and supply drive letter like G: and full path of the new iso\n");
        printf("reg.exe G: C:\\win.iso\n");
        return 1;
    }
    BOOL local_oscdimg = FALSE;

    //check if oscdimg.exe is installed if not ask to download
    LPCTSTR oscdimg_path = "C:\\Program Files (x86)\\Windows Kits\\10\\Assessment and Deployment Kit\\Deployment Tools\\amd64\\Oscdimg\\oscdimg.exe";
    int full_oscd = GetFileAttributes(oscdimg_path);
    int path_oscd = system("where.exe oscdimg.exe");
    char response[2];
    
    if(path_oscd != 0 && full_oscd == -1){
        printf("oscdimg.exe not found do you want to download it from\n"
               "https://msdl.microsoft.com/download/symbols/oscdimg.exe/3D44737265000/oscdimg.exe\n"
               "(y/n): "
        );
        scanf("%1s", response);

        if(strcmp(response, "y") == 0 || strcmp(response, "Y") == 0){
            system("powershell -Command \"Invoke-WebRequest -Uri https://msdl.microsoft.com/download/symbols/oscdimg.exe/3D44737265000/oscdimg.exe -OutFile oscdimg.exe\"");
            local_oscdimg = TRUE;
        }else{
            printf("oscdimg.exe not found make sure its installed and added to PATH\n");
            printf("you can get it by installing the windows adk\n");
            return 1;
        }
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
        printf("no TEMP environment variable or other error with environment variables\n");
        return 1;
    } 
    const char *extract_dir_name = "\\wimedit-";

    char num_str[11];  // 10 digits + null terminator
    snprintf(num_str, sizeof(num_str), "%010d", rand_num);  // Format number with at most 10 digits

    strcpy(extract_path, temp_path);
    strcat(extract_path, extract_dir_name);
    strcat(extract_path, num_str);
    
    SHFILEOPSTRUCT copy_op;
    ZeroMemory(&copy_op, sizeof(copy_op));
    copy_op.hwnd = NULL;                  // No parent window
    copy_op.wFunc = FO_COPY;              // Copy operation
    copy_op.pFrom = argv[1];              // Source path
    copy_op.pTo = extract_path;           // Destination path
    copy_op.fFlags = FOF_NOCOPYSECURITYATTRIBS | 
                     FOF_SIMPLEPROGRESS | // Show simple progress dialog
                     FOF_NOCONFIRMMKDIR;  // No "create directory" confirmation
    
    int result = SHFileOperation(&copy_op);
    if (result != 0) {
        printf("Failed to copy files. Error code: %d\n", result);
        return 1;
    }

    printf("Files copied successfully to: %s\n", extract_path);
    
    edit_wim(extract_path);
    make_iso(local_oscdimg, argv, extract_path);

    // DOES NOT REMOVE DIR FIX LATER
    RemoveDirectoryA(extract_path);
    return 0;
}

//gcc reg.c -o reg -Iinclude -Llibs -lVirtdisk -Wall -Wextra -Wpedantic
