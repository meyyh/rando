#Requires AutoHotkey >=2.0

#Persistent
return

<!]:: ; Alt ]
    SetTimer, TypeClipboard, 300 ; in ms
return

TypeClipboard:
   ; SetWorkingDir A_InitialWorkingDir ; set path to dir the .ahk file is in
    Run A_ScriptDir . "\" . clippaste.exe
    SetTimer, TypeClipboard, Off
return
