# this is a repo for the varoius projects that I do that would clutter my github if i made a repo for each of them

## projects
1. clippaste - send the contents of you clipboard as keyboard inputs to avoid issues with weird or escape characters
2. brainfuck to string - enter ><+_., and get back the origional string you put into some online string to brainfuck thing, wow very useful
3. uniart - a way to store unicode/ascii art in a way that maintains its formatting

## clippaste
### windows
- clippaste.c/.exe/.ahk are for windows
- make sure that the exe and ahk are in the same directory
- double click on the ahk and the bind is alt + ]
### linux
- keypaste.cpp is for linux
- you need to be added to the input group then reboot
- make sure you have wl-clipboard or xclip installed

- example hyprland bind
  - bind = CTRL, bracketright, exec, /home/meyyh/gits/rando/clippaste/kppp -d /dev/input/by-path/platform-i8042-serio-0-event-kbd
### bonus image to text

```
grim -g "$(slurp)" /tmp/pic && tesseract /tmp/pic stdout | wl-copy
```  
or 
```
flameshot gui --raw | tesseract -l fin stdin stdout | xclip -selection clipboard
```

## brainfuck to string
- read the header

## uniart
- requires gum and either wl-clipboard or xclip

```./uniart.sh i``` prompts you then stores whatever is in your clipboard into the json then asks for tags 

```./uniart.sh o``` allows you to search based on name or tag with the option to output everything