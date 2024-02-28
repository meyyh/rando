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

//g++ -g keypaste.cpp -o kppp -Wall -Wextra -Wpedantic -Werror -std=c++23

std::string dpserver; // wayland or x11

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

std::string dopopen(std::string command)
{
    std::array<char, 2048> buffer;
    std::string result;

    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        std::cerr << "popen error with " << command << std::endl;
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    {
        result += buffer.data();
    }

    if (ferror(pipe))
    {
        std::cerr << "Error reading from pipe in dopopen." << std::endl;
        pclose(pipe);
        exit(EXIT_FAILURE);
    }

    int status = pclose(pipe);

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
    {
        return result;
    }
    else
    {
        std::cerr << "Command execution failed with exit status: " << WEXITSTATUS(status) << std::endl;
        exit(EXIT_FAILURE);
    }
}

void handle_error(const char* operation) {
    std::cerr << "Error during " << operation << ": " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " /dev/input/by-path/*-event-kbd " << std::endl;
        return EXIT_FAILURE;
    }

    //use /dev/inout/by-path over direct event# because I think the event number can change
    //but both will work
    const char *inputDevicePath = argv[1]; 

    //get display server
    std::string dpserver = dopopen("echo $XDG_SESSION_TYPE");

    int fd = open(inputDevicePath, O_WRONLY | O_NONBLOCK);
    if (!fd) {std::cout << "fd errors\n";}

    //loop through the keys we want to use idk why it does not work without this
    for (int i = 0; i < numKeys; ++i) {
        int code = KEY_ESC + i;
        ioctl(fd, UI_SET_KEYBIT, code);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); //only typing middle section of clipboard idk why without this

    //for issues with string and char
    std::string wayland = "wayland";
    std::string x11 = "x11";

    // Remove trailing newline characters
    dpserver.erase(std::remove(dpserver.begin(), dpserver.end(), '\n'), dpserver.cend());  

    std::string errstr = "command not found";
    std::string cbdata;
    if (dpserver == wayland){
        cbdata += dopopen("wl-paste");

        if (cbdata.find(errstr) != std::string::npos){
            std::cerr << "wl-paste not found" << std::endl;
        }
    }
    else if (dpserver == x11){
        cbdata += dopopen("xclip -o");

        if (cbdata.find(errstr) != std::string::npos) {
            std::cerr << "xclip not found or error" << std::endl;  
        }
    }
    else {
        std::cout << "unknow dpserver: " << dpserver << "\n";
    }

    for (size_t i = 0; i < cbdata.length(); i++) {

        int code = char_to_keycode(cbdata[i]);
        std::string shift_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ@#%&*+()!\":?~|{}$^_<>";



        if (shift_chars.find(cbdata[i]) != std::string::npos) {
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