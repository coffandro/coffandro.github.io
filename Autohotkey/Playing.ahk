
PosX := A_ScreenWidth - 100
PosY := A_ScreenHeight - 200

Gui, 2:destroy
Gui, 2:new,
Gui, 2:Add, Text,, %PosX%
Gui, 2:Add, Text,, %PosY%
Gui, 2:Add, Text,, Hello!
Gui, 2:Show, x%PosX% y%PosY%, Playing