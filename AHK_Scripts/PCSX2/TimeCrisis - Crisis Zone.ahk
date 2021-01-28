#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

Run, H:\LaunchBox\Utilities\Biped\biped2.exe -left 3000 -middle 3000 -right 3000

Process, Exist, Lightgun.exe
If (!ErrorLevel = 0) {
Process,Close,Lightgun.exe
sleep, 1000
}
Run, H:\LaunchBox\Utilities\SindenLightgunWindowsP1\SindenLightgun\Lightgun.exe autoborder

Run, H:\Emulators\PCSX2_MousePatch\PcSx2MP.exe
sleep, 200
SendInput {Space}
SendInput {Tab}
SendInput {Space}
sleep, 1000
Run, H:\Emulators\PCSX2_MousePatch\pcsx2.exe --fullscreen --nogui "H:\LaunchBox\Games\Sony Playstation 2\Time Crisis - Crisis Zone (Europe).iso"
sleep, 5000
SendInput {F3}


$Esc::
{
	process, close, biped2.exe
	process, close, Lightgun.exe
	process, close, PcSx2MP.exe
	process, close, pcsx2.exe
	exitapp
}
return