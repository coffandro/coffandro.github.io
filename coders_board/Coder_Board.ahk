#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.
#MaxHotkeysPerInterval 1000
#Persistent  ; Keep this script running until the user explicitly exits it.
;==============================================================================
;Variabels
DS3Blender := 0
;==============================================================================
;numpad functions
Numpad7::Volume_Up
Numpad4::Volume_Down
Numpad8::Media_Play_Pause
NumpadDot::Delete
NumpadAdd::
Sendinput, ^S
Sleep, 1000
Sendinput, ^!+{R}
Sleep, 100
Sendinput, {Enter}
return
Numpad5::
SendInput, !{Tab}
return
Numpad6::
Run, https://web2.0calc.com
return
;==============================================================================
;CodersBoard functions
$>^::Capslock
PgUp::
Send, {Volume_Up}
return
PgDn::
Send, {Volume_Down}
return
Launch_App2::
Run, https://web2.0calc.com
return
Home::
Send, {Media_Play_Pause}
return
;==============================================================================
;Temporary functions
;if GetKeyState("NumLock", "T")