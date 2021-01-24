#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.

#SingleInstance force 

SetWorkingDir H:\Arcade PC\Time Crisis 5\TC5\Binaries\Win64

Process, Exist, Lightgun.exe
If (!ErrorLevel = 0) {
Process,Close,Lightgun.exe
sleep, 1000
}
Run, H:\LaunchBox\Utilities\SindenLightgunWindowsSoftwareV1.04\SindenLightgun\Lightgun.exe

;Biped Left: T, Middle: MiddleMouse, Right: Y
Run, H:\LaunchBox\Utilities\Biped\biped2.exe -left 20 -middle 2000 -right 21, ,Min

Run, H:\Arcade PC\Time Crisis 5\DemulShooter_v10.1.5\demulshooterX64.exe -target=es3 -rom=tc5
sleep, 1000

Run, H:\Arcade PC\Time Crisis 5\TC5\Binaries\Win64\TimeCrisisGame-Win64-Shipping.exe -NOINI -Language=JPN -playside=1

;Insert Coin
1::<+t
2Joy9::<+t

;Crosshair On/Off
5::<+h

;Bypass screen button
MButton::
WheelUpDown:                        
    Send,{WheelDown 1}
Return

$Esc::
    Process,Close,TimeCrisisGame-Win64-Shipping.exe
    Process,Close,biped2.exe
    sleep, 2000
    Process,Close,Lightgun.exe
    ExitApp
return