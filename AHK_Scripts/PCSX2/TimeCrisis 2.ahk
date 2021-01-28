#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

FileCopyDir, H:\Emulators\PCSX2\Game Configs\Time Crisis 2 (USA), H:\Emulators\PCSX2\inis, 1
FileCopy, H:\Emulators\PCSX2\shaders\BorderWidescreen.ini, H:\Emulators\PCSX2\shaders\active_shader.ini, 1

Process, Exist, Lightgun.exe
If (!ErrorLevel = 0) {
Process,Close,Lightgun.exe
sleep, 1000
}
Run, H:\LaunchBox\Utilities\SindenLightgunWindowsP1\SindenLightgun\Lightgun.exe

Run, H:\Emulators\PCSX2\pcsx2.exe "H:\LaunchBox\Games\Sony Playstation 2\Time Crisis 2 (USA)\Time Crisis 2 (USA).bin" --fullscreen --nogui, H:\Emulators\PCSX2
sleep, 6000
WinActivate, ahk_exe pcsx2.exe
sleep, 1000
Send {F3 down}
sleep, 100
Send {F3 up}

$Esc::
{
	FileCopy, H:\Emulators\PCSX2\shaders\default_shader.ini, H:\Emulators\PCSX2\shaders\active_shader.ini, 1
	process, close, Lightgun.exe
	process, close, pcsx2.exe
	exitapp
}
return