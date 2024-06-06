#include <windows.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    if(argc == 1){
        printf("mount iso and supply drive letter like D:\n");
        return 1;
    }
    
    char *wim_path = "\\sources\\install.wim";
    char *wim_drive = argv[1];
    char *full_wim_path;
    full_wim_path = malloc(strlen(wim_drive)+strlen(wim_path));
    strcpy(full_wim_path, wim_drive);
    strcat(full_wim_path, wim_path);
    free(full_wim_path);

    printf(full_wim_path);
    //DismGetImageInfo()



    //DismMountImage()
    return 0;
}