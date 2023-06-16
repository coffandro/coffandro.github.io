#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.
OldTitle = Spotify Premium
DefaultTitle = Spotify Premium

NumpadHome::
Numpad7::
Browser_Home::
tooltip
WinGetTitle, Title, ahk_exe Spotify.exe
Array := StrSplit(Title, "-")
if (SubStr(Array[2],0,1) == " ")
{
	Song = % SubStr(Array[2],1,StrLen(var)-1)
}
else
{
	song = % Array[2]
}
Artist = % Array[1]
if (Title == DefaultTitle)
{
	Tooltip, none
}
else
{
	Tooltip, Spotify playing%Song% by %Artist%
}
sleep 1500
tooltip
OldTitle = %Title%
return