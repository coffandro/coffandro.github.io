#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.
; Mail dial
Numpad0::
CoordMode, Mouse, Screen
MouseGetPos, XposA, YposA
XposA-=+105
YposA-=+105
gui, 1:destroy
Gui, 1:Color, EEAA99
Gui, 1:Add, Button, x2 y0 w50 h50 gButton1, School login
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
Gui, 1:Destroy

return

Button1:
Gui, 1:Destroy
run C:\coders_board_login\Login_School.ahk
Return
Button2:
Gui, 1:Destroy
run C:\coders_board_login\Login_Main.ahk
Return
Button3:
Gui, 1:Destroy
run C:\coders_board_login\Login_Secondary.ahk
Return
Button4:
Gui, 1:Destroy
run C:\coders_board_login\login_Tertiary.ahk
Return
Button6:
Gui, 1:Destroy
run C:\coders_board_login\Login_Quaternary.ahk
Return
Button7:
Gui, 1:Destroy
run C:\coders_board_login\Login_Quinary.ahk
Return
Button8:
Gui, 1:Destroy
run C:\coders_board_login\Login_Senary.ahk
Return

; Shows menu for logining into my accounts
; wont show my mails because i am not an idiot
; to use download and run login_setup script
; --------------------------------------------------------------------------------------
