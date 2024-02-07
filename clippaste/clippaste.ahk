#Persistent
return

<!]:: ; Alt ]
    SetTimer, TypeClipboard, 50
return

TypeClipboard:
    Clipboard := ClipboardAll
    SendInput, %Clipboard%
    SetTimer, TypeClipboard, Off
return
