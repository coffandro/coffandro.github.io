#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.
CoordMode, Mouse, Screen
MouseGetPos, XposA, YposA
XposA-=+105
YposA-=+105
gui, 1:destroy
Gui, 1:Color, EEAA99
Gui, 1:Add, Button, x2 y0 w50 h50 BackgroundTrans gButton1, School login
Gui, 1:Add, Button, x2 y60 w50 h50 BackgroundTrans gButton2, Main Login
Gui, 1:Add, Button, x2 y120 w50 h50 BackgroundTrans gButton3, Second Login
Gui, 1:Add, Button, x62 y120 w50 h50 BackgroundTrans gButton4, Third Login
Gui, 1:Add, Button, x122 y120 w50 h50 BackgroundTrans gButton5, Close
Gui, 1:Add, Button, x122 y60 w50 h50 BackgroundTrans gButton6, Fourth Login
Gui, 1:Add, Button, x122 y0 w50 h50 BackgroundTrans gButton7, Fifth Login
Gui, 1:Add, Button, x62 y0 w50 h50 BackgroundTrans gButton8, Sixth Login
Gui 1:+LastFound +AlwaysOnTop +ToolWindow
WinSet, TransColor, EEAA99
Gui 1:-Caption
Gui, 1:Show, x%XposA% y%YposA% h176 w179, menus
Return

Button5:
exitapp
return

Button1:
run C:\coders_board_login\Login_School.ahk
exitapp
Return
Button2:
run C:\coders_board_login\Login_Main.ahk
exitapp
Return
Button3:
run C:\coders_board_login\Login_Secondary.ahk
exitapp
Return
Button4:
run C:\coders_board_login\login_Tertiary.ahk
exitapp
Return
Button6:
run C:\coders_board_login\Login_Quaternary.ahk
exitapp
Return
Button7:
run C:\coders_board_login\Login_Quinary.ahk
exitapp
Return
Button8:
run C:\coders_board_login\Login_Senary.ahk
exitapp
Return

; Shows a dial around your mouse currently its only my Email logins
; i wont show my mails because i am not an idiot so i have it in text files 
; to use download and run the login_setup script then follow the prompts