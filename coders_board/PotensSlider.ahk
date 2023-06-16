#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.
CoordMode, Mouse, Screen
MouseGetPos, XposA, YposA
XposA-=+145
YposA-=+105
gui, 2:destroy
Gui, 2:Color, EEAA99
Gui, 2:Add, Slider, x50 y60 w130 h50 vMySlider, 10
Gui, 2:Add, Button, x122 y100 w50 h50 BackgroundTrans gButton4, Send
Gui, 2:Add, Button, x62 y100 w50 h50 BackgroundTrans gCloseFunction, Close
Gui 2:+LastFound +AlwaysOnTop +ToolWindow
WinSet, TransColor, EEAA99
Gui 2:-Caption
Gui, 2:Show, x%XposA% y%YposA% h176 w179, menus
SetTimer, CloseFunction, 5000
Return

CloseFunction:
exitapp
return

Button4:
Send, MySlider
exitapp
Return