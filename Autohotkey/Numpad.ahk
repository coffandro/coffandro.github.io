#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

; First Line
NumpadDiv::F13

NumpadMult::
SendInput {Media_Play_Pause}
return

; ------------------------------------------------------------------------------------------
; Main Numpad Keys
NumpadIns::
Numpad0::
SendInput !{Tab}
return

NumpadEnd::
Numpad1::
return

NumpadDown::
Numpad2::
Send #n
Sleep 750
SendInput, {Enter}
Sleep 100
SendInput, {Escape}
return

NumpadPgdn::
Numpad3::
Run https:remove.bg
return

NumpadLeft::
Numpad4::
Run C:\Users\Cornelius Rosenaa\Documents\Skema.docx
return

NumpadClear::
Numpad5::
WinClose A
return

NumpadRight::
Numpad6::
Send, #{Dot}
return

NumpadUp::
Numpad8::
return

NumpadPgup::
Numpad9::
return

; ------------------------------------------------------------------------------------------
; Surrounding
NumpadDel::
NumpadDot::
WinMinimize A
return

NumpadEnter::
Return

NumpadAdd::
SendInput {Media_Next}
return

RemoveToolTip:
SetTimer, RemoveToolTip, Off
ToolTip
return