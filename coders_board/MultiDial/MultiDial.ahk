#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.
; --------------------------------------------------------------------------------------
; Variables
FileRead, BtnVar, %A_ScriptDir%\Multidata.txt
;---------------------------------------------------------------------------------------
; Function for making the wheel turn or so to speak
if BtnVar <= -1
{
	BtnVar := 4
	FileDelete, %A_ScriptDir%\Multidata.txt
	FileAppend, %BtnVar%, %A_ScriptDir%\Multidata.txt
	Reload
}
if BtnVar = 0
{
	Btn1tx = School login
	Btn2tx = Main login
	Btn3tx = Second Login
	Btn4tx = Third Login
	Btn5tx = Fourth Login
}
if BtnVar = 1
{
	Btn1tx = Discord
	Btn2tx = Notepad++
	Btn3tx = obsidian
	Btn4tx = Win Spy
	Btn5tx = Firefox
}
if BtnVar = 2
{
	Btn1tx = ß
	Btn2tx = Potens
	Btn3tx = test13
	Btn4tx = test14
	Btn5tx = test15
}
if BtnVar = 3
{
	Btn1tx = test16
	Btn2tx = test17
	Btn3tx = test18
	Btn4tx = test19
	Btn5tx = test20
}
if BtnVar = 4
{
	Btn1tx = test21
	Btn2tx = test22
	Btn3tx = test23
	Btn4tx = test24
	Btn5tx = test25
}
; --------------------------------------------------------------------------------------
; Multi Dial
AppsKey::
CoordMode, Mouse, Screen
MouseGetPos, XposA, YposA
XposA-=+105
YposA-=+105
Gui, 1:destroy
Gui, 1:Color, EEAA99
Gui, 1:Add, Button, x2 y0 w50 h50 BackgroundTrans gButton1, %Btn1tx%
Gui, 1:Add, Button, x2 y60 w50 h50 BackgroundTrans gButton2, %Btn2tx%
Gui, 1:Add, Button, x2 y120 w50 h50 BackgroundTrans gButton3, %Btn3tx%
Gui, 1:Add, Button, x62 y120 w50 h50 BackgroundTrans gForwFunction, Forward wheel
Gui, 1:Add, Button, x122 y120 w50 h50 BackgroundTrans gCloseFunction, Close
Gui, 1:Add, Button, x122 y60 w50 h50 BackgroundTrans gBackFunction, Back wheel
Gui, 1:Add, Button, x122 y0 w50 h50 BackgroundTrans gButton4, %Btn4tx%
Gui, 1:Add, Button, x62 y0 w50 h50 BackgroundTrans gButton5, %Btn5tx%
Gui 1:+LastFound +AlwaysOnTop +ToolWindow
WinSet, TransColor, EEAA99
Gui 1:-Caption
Gui, 1:Show, x%XposA% y%YposA% h176 w179, menus
SetTimer, CloseFunction, 5000
Return

CloseFunction:
Gui, 1:Destroy
return

ForwFunction:
Gui, 1:Destroy
BtnPlus := BtnVar += 1
FileDelete, %A_ScriptDir%\Multidata.txt
FileAppend, %BtnPlus%, %A_ScriptDir%\Multidata.txt
reload
Return

BackFunction:
Gui, 1:Destroy
BtnMinus := BtnVar -= 1
FileDelete, %A_ScriptDir%\Multidata.txt
FileAppend, %BtnMinus%, %A_ScriptDir%\Multidata.txt
reload
Return

Button1:
Gui, 1:Destroy
if BtnVar = 0
{
	Run, C:\coders_board_login\Login_School.ahk
}
if BtnVar = 1
{
	Run, C:\Users\Cornelius Rosenaa\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Discord Inc\Discord.lnk
}
if BtnVar = 2
{
	send ß
}
if BtnVar = 3
{
	Msgbox, BtnVar is 3 and you pressed button 1
}
if BtnVar = 4
{
	Msgbox, BtnVar is 4 and you pressed button 1
}
return

Button2:
Gui, 1:Destroy
if BtnVar = 0
{
	Run, C:\coders_board_login\Login_Main.ahk
}
if BtnVar = 1
{
	Run, C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Notepad++.lnk
}
if BtnVar = 2
{
	Run, C:\coders_board\PotensSlider.ahk
}
if BtnVar = 3
{
	Msgbox, BtnVar is 3 and you pressed button 2
}
if BtnVar = 4
{
	Msgbox, BtnVar is 4 and you pressed button 2
}
return

Button3:
Gui, 1:Destroy
if BtnVar = 0
{
	Run, C:\coders_board_login\Login_Secondary.ahk
}
if BtnVar = 1
{
	Msgbox, BtnVar is 1 and you pressed button 3
}
if BtnVar = 2
{
	Msgbox, BtnVar is 2 and you pressed button 3
}
if BtnVar = 3
{
	Msgbox, BtnVar is 3 and you pressed button 3
}
if BtnVar = 4
{
	Msgbox, BtnVar is 4 and you pressed button 3
}
return

Button4:
Gui, 1:Destroy
if BtnVar = 0
{
	Run, C:\coders_board_login\login_Tertiary.ahk
}
if BtnVar = 1
{
	IfWinNotExist, ahk_class AU3Reveal
	Run, C:\ProgramData\Microsoft\Windows\Start Menu\Programs\AutoHotkey\Window Spy.lnk

}
if BtnVar = 2
{
	Msgbox, BtnVar is 2 and you pressed button 4
}
if BtnVar = 3
{
	Msgbox, BtnVar is 3 and you pressed button 4
}
if BtnVar = 4
{
	Msgbox, BtnVar is 4 and you pressed button 4
}
return

Button5:
Gui, 1:Destroy
if BtnVar = 0
{
	Run, C:\coders_board_login\Login_Quaternary.ahk
}
if BtnVar = 1
{
	Msgbox, BtnVar is 1 and you pressed button 5
}
if BtnVar = 2
{
	Msgbox, BtnVar is 2 and you pressed button 5
}
if BtnVar = 3
{
	Msgbox, BtnVar is 3 and you pressed button 5
}
if BtnVar = 4
{
	Msgbox, BtnVar is 4 and you pressed button 5
}
return
; --------------------------------------------------------------------------------------
if BtnVar >= 5
{
	BtnVar := 0
	FileDelete, %A_ScriptDir%\Multidata.txt
	FileAppend, %BtnVar%, %A_ScriptDir%\Multidata.txt
	Reload
}