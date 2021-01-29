#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
#SingleInstance Force
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

Process, Exist, Lightgun.exe
If (!ErrorLevel = 0) {
Process,Close,Lightgun.exe
sleep, 1000
}
Run, H:\LaunchBox\Utilities\SindenLightgunWindowsP1\SindenLightgun\Lightgun.exe autoborder

;CUSTOM CONFIG
Run, H:\Emulators\RPCS3_Lightgun\rpcs3.exe H:\Emulators\RPCS3\dev_hdd0\disc\BLUS30528\PS3_GAME\USRDIR\pirates_PS3.ppu.self --config "H:\Emulators\RPCS3_Lightgun\config\custom_configs\config_BLUS30528_dsp.yml"

CursorVisible := false
run, H:\LaunchBox\Utilities\nomousy\nomousy.exe /hide

#If rightedge()
*LButton::
*RButton::
*MButton::
*XButton1::
*XButton2::Return
#If

rightedge() {
 CoordMode, Mouse
 MouseGetPos, xpos, ypos
 Return (xpos > A_ScreenWidth - 3)
}

1::
if (CursorVisible){
	CursorVisible := false
	run, H:\LaunchBox\Utilities\nomousy\nomousy.exe /hide
}
else{
	CursorVisible := true
	run, H:\LaunchBox\Utilities\nomousy\nomousy.exe
}
return

;Triangle Button
$Space::
Send {RButton down}
Sleep, 60
Send {MButton down}
Sleep, 60
Send {LButton down}
Sleep, 20
Send {LButton up}
Sleep, 60
Send {MButton up}
Send {RButton up}
Return

;Circle Button
$MButton::
send {LButton down}
sleep, 60
send {RButton down}
sleep, 20
send {RButton up}
sleep, 60
send {LButton up}
Return


$Esc::
	Process,Close,Lightgun.exe
	Process,Close,biped2.exe
    Process,Close,rpcs3.exe
	run, H:\LaunchBox\Utilities\nomousy\nomousy.exe
	sleep, 1000
	Process,Close,nomousy.exe
    ExitApp
return