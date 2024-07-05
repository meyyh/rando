#include <array>
#include <fcntl.h>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <cjson/cJSON.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <libvirt/libvirt.h>

#include "pstream.h"

std::array keys = {
    "KEY_ESC", "KEY_DELETE", "KEY_HOME", "KEY_BACKSPACE", "KEY_TAB",  "KEY_PAGEDOWN", "KEY_ENTER", "KEY_PAGEUP", "KEY_CAPSLOCK", "KEY_LEFTSHIFT", "KEY_MENU",
    "KEY_LEFTCTRL", "KEY_FN", "KEY_LEFTMETA", "KEY_LEFTALT", "KEY_SPACE", "KEY_RIGHTALT",  "KEY_RIGHTCTRL", "KEY_LEFT", "KEY_UP", "KEY_DOWN", "KEY_RIGHT", "KEY_RIGHTSHIFT", "KEY_END",
    "KEY_F1", "KEY_F2", "KEY_F3", "KEY_F4", "KEY_F5", "KEY_F6", "KEY_F7", "KEY_F8", "KEY_F9", "KEY_F10", "KEY_F11", "KEY_F12", 
    "KEY_F13", "KEY_F14", "KEY_F15", "KEY_F16", "KEY_F17", "KEY_F18", "KEY_F19", "KEY_F20", "KEY_F21", "KEY_F22", "KEY_F23", "KEY_F24",
    "KEY_1", "KEY_2", "KEY_3", "KEY_4", "KEY_5", "KEY_6", "KEY_7", "KEY_8", "KEY_9", "KEY_0", 
    "KEY_GRAVE", "KEY_MINUS", "KEY_EQUAL", "KEY_LEFTBRACE", "KEY_RIGHTBRACE", "KEY_BACKSLASH", "KEY_SEMICOLON", "KEY_APOSTROPHE",
    "KEY_Q", "KEY_W", "KEY_E", "KEY_R", "KEY_T", "KEY_Y", "KEY_U", "KEY_I", "KEY_O", "KEY_P",  
    "KEY_A", "KEY_S", "KEY_D", "KEY_F", "KEY_G", "KEY_H", "KEY_J", "KEY_K", "KEY_L", 
    "KEY_Z", "KEY_X", "KEY_C", "KEY_V", "KEY_B", "KEY_N", "KEY_M", "KEY_COMMA", "KEY_DOT", "KEY_SLASH",
    
    "KEY_PLAYPAUSE", "KEY_VOLUMEUP", "KEY_VOLUMEDOWN", "KEY_MUTE", "KEY_NEXTSONG", "KEY_PREVIOUSSONG",
    "KEY_UNDO", "KEY_SCROLLLOCK", "KEY_INSERT", "KEY_EJECTCD", "KEY_FORWARD", "KEY_BACK"
};

//yoinked form https://github.com/malteskoruppa/linuxkb
// convert ASCII chars to key codes
short charToKeycode(char c)
{
  short keycode;

  switch(c) {

    case 'a': case 'A': keycode = KEY_A; break;
    case 'b': case 'B': keycode = KEY_B; break;
    case 'c': case 'C': keycode = KEY_C; break;
    case 'd': case 'D': keycode = KEY_D; break;
    case 'e': case 'E': keycode = KEY_E; break;
    case 'f': case 'F': keycode = KEY_F; break;
    case 'g': case 'G': keycode = KEY_G; break;
    case 'h': case 'H': keycode = KEY_H; break;
    case 'i': case 'I': keycode = KEY_I; break;
    case 'j': case 'J': keycode = KEY_J; break;
    case 'k': case 'K': keycode = KEY_K; break;
    case 'l': case 'L': keycode = KEY_L; break;
    case 'm': case 'M': keycode = KEY_M; break;
    case 'n': case 'N': keycode = KEY_N; break;
    case 'o': case 'O': keycode = KEY_O; break;
    case 'p': case 'P': keycode = KEY_P; break;
    case 'q': case 'Q': keycode = KEY_Q; break;
    case 'r': case 'R': keycode = KEY_R; break;
    case 's': case 'S': keycode = KEY_S; break;
    case 't': case 'T': keycode = KEY_T; break;
    case 'u': case 'U': keycode = KEY_U; break;
    case 'v': case 'V': keycode = KEY_V; break;
    case 'w': case 'W': keycode = KEY_W; break;
    case 'x': case 'X': keycode = KEY_X; break;
    case 'y': case 'Y': keycode = KEY_Y; break;
    case 'z': case 'Z': keycode = KEY_Z; break;


    case '1': keycode = KEY_1; break;
    case '2': keycode = KEY_2; break;
    case '3': keycode = KEY_3; break;
    case '4': keycode = KEY_4; break;
    case '5': keycode = KEY_5; break;
    case '6': keycode = KEY_6; break;
    case '7': keycode = KEY_7; break;
    case '8': keycode = KEY_8; break;
    case '9': keycode = KEY_9; break;
    case '0': keycode = KEY_0; break;

    case '!': keycode = KEY_1; break; // with SHIFT
    case '@': keycode = KEY_2; break; // with SHIFT
    case '#': keycode = KEY_3; break; // with SHIFT
    case '$': keycode = KEY_4; break; // with SHIFT
    case '%': keycode = KEY_5; break; // with SHIFT
    case '^': keycode = KEY_6; break; // with SHIFT
    case '&': keycode = KEY_7; break; // with SHIFT
    case '*': keycode = KEY_8; break; // with SHIFT
    case '(': keycode = KEY_9; break; // with SHIFT
    case ')': keycode = KEY_0; break; // with SHIFT

    case '-': keycode = KEY_MINUS; break;
    case '_': keycode = KEY_MINUS; break; // with SHIFT

    case '=': keycode = KEY_EQUAL; break;
    case '+': keycode = KEY_EQUAL; break; // with SHIFT

    case '\'': keycode = KEY_APOSTROPHE; break; 
    case '"': keycode = KEY_APOSTROPHE; break; // with SHIFT

    case ';': keycode = KEY_SEMICOLON; break;
    case ':': keycode = KEY_SEMICOLON; break; // with SHIFT

    case '/': keycode = KEY_SLASH; break;
    case '?': keycode = KEY_SLASH; break; // with SHIFT

    case ',': keycode = KEY_COMMA; break;
    case '<': keycode = KEY_COMMA; break; // with SHIFT

    case ' ': keycode = KEY_SPACE; break;

    case '.': keycode = KEY_DOT; break;
    case '>': keycode = KEY_DOT; break; // with SHIFT

    case '`': keycode = KEY_GRAVE; break;
    case '~': keycode = KEY_GRAVE; break; // with SHIFT

    case '[': keycode = KEY_LEFTBRACE; break;
    case '{': keycode = KEY_LEFTBRACE; break; // with SHIFT

    case ']': keycode = KEY_RIGHTBRACE; break;
    case '}': keycode = KEY_RIGHTBRACE; break; // with SHIFT
    
    case '\\': keycode = KEY_BACKSLASH; break; // escape escape!
    case '|': keycode = KEY_BACKSLASH; break; // with SHIFT

    case '\n': keycode = KEY_ENTER; break;

    default: keycode = -1;
    }

    return keycode;
}



void emit(int fd, int type, int code, int val)
{
    struct input_event ie;
    ie.type = type;
    ie.code = code;
    ie.value = val;

    struct input_event report;
    report.type = EV_SYN;
    report.code = SYN_REPORT;
    report.value = 0;

    write(fd, &ie, sizeof(ie));
    write(fd, &report, sizeof(report));
}

std::string getClipBoardData(bool useNewLine)
{
    //should return x11 or wayland
    std::string session_type = std::getenv("XDG_SESSION_TYPE");
    std::string getCbData = (session_type == "wayland") ? "wl-paste -n" : (session_type == "x11") ? "xclip -o" : "";

    if (getCbData.empty())
    {
        std::cerr << "Unknown dpserver: " << session_type << "\n";
        std::cerr << "Expecting x11 or wayland\n";
        return "";
    }

    redi::ipstream inStream(getCbData);

    std::string cbData(std::istreambuf_iterator<char>(inStream), {});

    if(useNewLine == false)
    {
        cbData.erase(std::remove(cbData.begin(), cbData.end(), '\n'), cbData.end());
    }

    return cbData;
}

void usage(char *argv[])
{
    std::cerr << "Usage: " << argv[0] << " -d /dev/input/by-path/*-event-kbd " << std::endl;
    std::cerr << "options:" << std::endl;
    std::cerr << "-d        path to input device(see usage above for example)" << std::endl;
    std::cerr << "-n        removes newline characters" << std::endl;
}

void toKeyboard(std::string cbData, char *inputDevicePath)
{
    int fd = open(inputDevicePath, O_WRONLY | O_NONBLOCK);
    if (fd < 0)
    {
        std::cout << "fd errors\n";
        return;
    }

    int numKeys = sizeof(keys) / sizeof(keys[0]);

    //loop through the keys we want to use. this is required
    for (int i = 0; i < numKeys; ++i)
    {
        int code = KEY_ESC + i;
        ioctl(fd, UI_SET_KEYBIT, code);
    }

    //only typing middle section of clipboard without this, idk why
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); 

    for (size_t i = 0; i < cbData.length(); i++)
    {
        short code = charToKeycode(cbData[i]);
        std::string shiftChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ@#%&*+()!\":?~|{}$^_<>";
        
        if (shiftChars.find(cbData[i]) != std::string::npos)
        {   //if uppercase
            //press
            emit(fd, EV_KEY, KEY_LEFTSHIFT, 1);
            emit(fd, EV_KEY, code, 1);

            //unpress
            emit(fd, EV_KEY, code, 0);
            emit(fd, EV_KEY, KEY_LEFTSHIFT, 0);

            fsync(fd);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } 
        else
        {   //if lowercase
            //press
            emit(fd, EV_KEY, code, 1);

            //unpress
            emit(fd, EV_KEY, code, 0);

            fsync(fd);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } 
    
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    close(fd);
}

void toVm(std::string cbData, virDomainPtr libvirtDomain)
{
    for (size_t i = 0; i < cbData.length(); i++)
    {
        short code = charToKeycode(cbData[i]);
        std::string shiftChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ@#%&*+()!\":?~|{}$^_<>";

        if (shiftChars.find(cbData[i]) != std::string::npos)
        {
            //if uppercase
            unsigned int keycodes[] = { KEY_LEFTSHIFT, static_cast<unsigned int>(code) };
            virDomainSendKey(libvirtDomain, VIR_KEYCODE_SET_LINUX, 0, keycodes, 2, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        else
        {
            unsigned int keycodes[] = { static_cast<unsigned int>(code) };
            virDomainSendKey(libvirtDomain, VIR_KEYCODE_SET_LINUX, 0, keycodes, 1, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } 
    }
}


std::string getVmName()
{
    //get the list of currently open hyprland windows in json format
    redi::ipstream inStream("hyprctl clients -j");
    std::string str;
    std::string hyprlandClients;
    while (std::getline(inStream, str))
    {
        str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
        hyprlandClients += str;
    }


    cJSON *clients_json = cJSON_Parse(hyprlandClients.c_str());
    if(clients_json == NULL)
    {
        std::cerr << "error parsing json\n";
        exit(EXIT_FAILURE);
    }

    //look for the json entry with focus_id of 1 this will be the previously "selected" window
    //if its title contains QEMU/KVM get the first word of the title which happens to be the vms name
    cJSON *window = NULL;
    cJSON_ArrayForEach(window, clients_json)
    {
        cJSON *focus_ID = cJSON_GetObjectItemCaseSensitive(window, "focusHistoryID");
        if(focus_ID->valueint == 1)
        {
            cJSON *title = cJSON_GetObjectItemCaseSensitive(window, "title");
            if(strstr(title->valuestring, "QEMU/KVM") != nullptr)
            {
                //returns the first "word" by getting all chars before it finds a space
                std::string vm_name = strtok(title->valuestring, " ");
                return vm_name;
            }
            else{return "";}
        }
    }
    return "";
}


int main(int argc, char *argv[])
{
    //if false everything is on one line
    bool useNewline = true;
    bool dpresent = false;
    char *inputDevicePath = NULL;
    std::string vmName;

    int opt;
    while ((opt = getopt(argc, argv, "d:nh")) != -1) {
        switch (opt) {
            case 'd':
                inputDevicePath = optarg;
                dpresent = true;
                break;
            case 'n':
                useNewline = false;
                break;
            case 'h':
            default:
                usage(argv);
                return 0;
        }
    }

    if(dpresent == false)
    {
        usage(argv);
        return 1;
    }


    std::string cbData = getClipBoardData(useNewline);

    //return the name of the qemu/kvm vm if not a vm window returns empty string
    vmName = getVmName();

    if(vmName == "")
    {
        toKeyboard(cbData, inputDevicePath);
    }
    else
    {
        virConnectPtr libvirtConn = virConnectOpen("qemu:///system");
        virDomainPtr libvirtDomain = virDomainLookupByName(libvirtConn, vmName.c_str());
        toVm(cbData, libvirtDomain);
    }
   
    return 0;
}
//g++ -g keypaste.cpp -o kppp -lcjson -lvert -Wall -Wextra -Wpedantic -std=c++17
