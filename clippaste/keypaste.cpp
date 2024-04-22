#include <array> //used for buffer
#include <fcntl.h> //open fd things

#include <chrono>//time in
#include <thread>// ms

#include <syslog.h> //log to journal

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <regex>

#include <linux/input.h>
#include <linux/uinput.h>

// based on the layout of the hp elitebook 840 G5
const char *keys[] = {
    "KEY_ESC", "KEY_F1", "KEY_F2", "KEY_F3", "KEY_F4","KEY_F5","KEY_F6","KEY_F7","KEY_F8","KEY_F9","KEY_F10","KEY_F11","KEY_F12", "KEY_DELETE",
    "KEY_GRAVE", "KEY_1", "KEY_2", "KEY_3", "KEY_4", "KEY_5", "KEY_6", "KEY_7", "KEY_8", "KEY_9", "KEY_0", "KEY_MINUS", "KEY_EQUAL","KEY_BACKSPACE", "KEY_HOME",
    "KEY_TAB", "KEY_Q", "KEY_W", "KEY_E", "KEY_R", "KEY_T", "KEY_Y", "KEY_U", "KEY_I", "KEY_O", "KEY_P", "KEY_LEFTBRACE", "KEY_RIGHTBRACE", "KEY_BACKSLASH", "KEY_PAGEUP",
    "KEY_CAPSLOCK", "KEY_A", "KEY_S", "KEY_D", "KEY_F", "KEY_G", "KEY_H", "KEY_J", "KEY_K", "KEY_L", "KEY_SEMICOLON", "KEY_APOSTROPHE", "KEY_ENTER", "KEY_PAGEDOWN",
    "KEY_LEFTSHIFT", "KEY_Z", "KEY_X", "KEY_C", "KEY_V", "KEY_B", "KEY_N", "KEY_M", "KEY_COMMA", "KEY_DOT", "KEY_SLASH", "KEY_RIGHTSHIFT", "KEY_END",
    "KEY_LEFTCTRL", "KEY_FN", "KEY_LEFTMETA", "KEY_LEFTALT", "KEY_SPACE", "KEY_RIGHTALT", "KEY_MENU", "KEY_RIGHTCTRL", "KEY_LEFT", "KEY_UP", "KEY_DOWN", "KEY_RIGHT",
    
    "KEY_PLAYPAUSE", "KEY_VOLUMEUP", "KEY_VOLUMEDOWN", "KEY_MUTE", "KEY_NEXTSONG", "KEY_PREVIOUSSONG",
    "KEY_UNDO", "KEY_SCROLLLOCK", "KEY_INSERT", "KEY_EJECTCD", "KEY_FORWARD", "KEY_BACK",
    "KEY_F13", "KEY_F14", "KEY_F15", "KEY_F16","KEY_F17","KEY_F18","KEY_F19","KEY_F20","KEY_F21","KEY_F22","KEY_F23","KEY_F24"
};//bottom 3 rows are just extras


//yoinked form https://github.com/malteskoruppa/linuxkb
// convert ASCII chars to key codes
short char_to_keycode(char c) {

  short keycode;

  switch(c) {

    // normal keyboard
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

    case '\'': keycode = KEY_APOSTROPHE; break; // \ used to escape ' 
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

    default: keycode = -1;
    }

    return keycode;
}

int numKeys = sizeof(keys) / sizeof(keys[0]);

void emit(int fd, int type, int code, int val)
{
   struct input_event ie;

   ie.type = type;
   ie.code = code;
   ie.value = val;

   write(fd, &ie, sizeof(ie));
}

std::string getClipBoardData()
{
    //x11 or wayland
    std::string session_type = std::getenv("XDG_SESSION_TYPE");

    std::string cbData = "";
    if (session_type == "wayland"){
        FILE *pipe = popen("wl-paste", "r");
        if (!pipe) {
            std::cerr << "wl-paste error is it installed?\n";
        }

        std::array<char, 2048> buffer;
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
                cbData += buffer.data();
        }
        pclose(pipe);
    }
    else if (session_type == "x11"){
        FILE *pipe = popen("xclip -o", "r");
        if (!pipe) {
            std::cerr << "xclip error is it installed?\n";
        }

        std::array<char, 2048> buffer;
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
                cbData += buffer.data();
        }
        pclose(pipe);
    }
    else {
        std::cerr << "unknow dpserver: " << session_type << "\n";
        std::cerr << "expecting x11 or wayland \n";
    }
    return cbData;
}

void usage(int argc, char *argv[]){
    std::cerr << "Usage: " << argv[0] << " -d /dev/input/by-path/*-event-kbd " << std::endl;
    std::cerr << "options:" << std::endl;
    std::cerr << "-d        path to input device(see usage above for example)" << std::endl;
    std::cerr << "-n        removes newline characters" << std::endl;
}

int main(int argc, char *argv[])
{
    //if false remove newline characters
    bool useNewline = true;
    const char *inputDevicePath = "";

    for (int i = 0; i < argc; ++i) {
        if(strcmp(argv[i], "-d") == 0)
        {
            int j = i + 1;
            if(argv[j] == NULL){
                
                usage(argc, argv);
                return 1;
            }
            inputDevicePath = argv[j];
        } 
        else if(argv[i] == "-n")
        {
            useNewline = false;
        } 
        else if(argv[i] == "-h" || argv[i] == "--help")
        {
            std::cout << "wut" << std::endl;
            usage(argc, argv);
            return 0;
        }
    }

    int fd = open(inputDevicePath, O_WRONLY | O_NONBLOCK);
    if (!fd) {std::cout << "fd errors\n";}

    //loop through the keys we want to use idk why it does not work without this
    for (int i = 0; i < numKeys; ++i) {
        int code = KEY_ESC + i;
        ioctl(fd, UI_SET_KEYBIT, code);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); //only typing middle section of clipboard idk why without this

    std::string cbData = getClipBoardData();

    for (size_t i = 0; i < cbData.length(); i++) {

        short code = char_to_keycode(cbData[i]);
        std::string shift_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ@#%&*+()!\":?~|{}$^_<>";
        
        if (shift_chars.find(cbData[i]) != std::string::npos) {
            //if not lowercase

            //press
            emit(fd, EV_KEY, KEY_LEFTSHIFT, 1);
            emit(fd, EV_SYN, SYN_REPORT, 0);
            emit(fd, EV_KEY, code, 1);
            emit(fd, EV_SYN, SYN_REPORT, 0);

            //unpress
            emit(fd, EV_KEY, code, 0);
            emit(fd, EV_SYN, SYN_REPORT, 0);
            emit(fd, EV_KEY, KEY_LEFTSHIFT, 0);
            emit(fd, EV_SYN, SYN_REPORT, 0);

            fsync(fd);

            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        } else if (cbData[i] == '\n'){
            emit(fd, EV_KEY, KEY_ENTER, 1);
            emit(fd, EV_SYN, SYN_REPORT, 0);

            emit(fd, EV_KEY, KEY_ENTER, 0);
            emit(fd, EV_SYN, SYN_REPORT, 0);

            fsync(fd);

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } else {
            //if lowercase
            
            //press
            emit(fd, EV_KEY, code, 1);
            emit(fd, EV_SYN, SYN_REPORT, 0);

            //unpress
            emit(fd, EV_KEY, code, 0);
            emit(fd, EV_SYN, SYN_REPORT, 0);

            fsync(fd);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } 
    
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    close(fd);
   
    return 0;
}
//g++ -g keypaste.cpp -o kppp -Wall -Wextra -Wpedantic -Werror -std=c++23