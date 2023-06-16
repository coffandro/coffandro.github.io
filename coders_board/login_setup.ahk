#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.
; Directories can be changed if you want
Login_scripts = C:\coders_board_login\Logins
1arc = C:\coders_board_login\Logins\1.txt
2arc = C:\coders_board_login\Logins\2.txt
3arc = C:\coders_board_login\Logins\3.txt
4arc = C:\coders_board_login\Logins\4.txt
5arc = C:\coders_board_login\Logins\5.txt
6arc = C:\coders_board_login\Logins\6.txt
7arc = C:\coders_board_login\Logins\7.txt
; ________________________________________________________________________________________________
; creates login directory
FileCreateDir, %Login_scripts%
; ________________________________________________________________________________________________
; First login prompt
InputBox, code1, name, Please enter your first e-mail, ,300,125
Switch Errorlevel
{
case 0:
MsgBox, Saved
case 1:
MsgBox, you canceled ):
}
FileAppend, %code1%, %1arc%
; ________________________________________________________________________________________________
; Second login prompt
InputBox, code2, name, Please enter your second e-mail, ,300,125
Switch Errorlevel
{
case 0:
MsgBox, Saved
case 1:
MsgBox, you canceled ):
}
FileAppend, %code2%, %2arc%
; ________________________________________________________________________________________________
; Third login prompt
InputBox, code3, name, Please enter your third e-mail, ,300,125
Switch Errorlevel
{
case 0:
MsgBox, Saved
case 1:
MsgBox, you canceled ):
}
FileAppend, %code3%, %3arc%
; ________________________________________________________________________________________________
; Fourth login prompt
InputBox, code4, name, Please enter your fourth e-mail, ,300,125
Switch Errorlevel
{
case 0:
MsgBox, Saved
case 1:
MsgBox, you canceled ):
}
FileAppend, %code4%, %4arc%
; ________________________________________________________________________________________________
; Fifth login prompt
InputBox, code5, name, Please enter your fifth e-mail, ,300,125
Switch Errorlevel
{
case 0:
MsgBox, Saved
case 1:
MsgBox, you canceled ):
}
FileAppend, %code5%, %5arc%
; ________________________________________________________________________________________________
; Sixth login prompt
InputBox, code6, name, Please enter your sixth e-mail, ,300,125
Switch Errorlevel
{
case 0:
MsgBox, Saved
case 1:
MsgBox, you canceled ):
}
FileAppend, %code6%, %6arc%
; ________________________________________________________________________________________________
; Seventh login prompt
InputBox, code7, name, Please enter your seventh e-mail, ,300,125
Switch Errorlevel
{
case 0:
MsgBox, Saved
case 1:
MsgBox, you canceled ):
}
FileAppend, %code7%, %7arc%
; ________________________________________________________________________________________________