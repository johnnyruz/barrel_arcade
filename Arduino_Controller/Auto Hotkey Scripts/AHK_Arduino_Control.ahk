#SingleInstance Force

; IsTriggerPulled := false

#!l::
Run arduino_control.bat l, H:\LaunchBox\Utilities\AutoHotkey\Scripts\batch, Hide
return

#!.::
Run arduino_control.bat ., H:\LaunchBox\Utilities\AutoHotkey\Scripts\batch, Hide
return

#!f::
Run, arduino_control.bat f, H:\LaunchBox\Utilities\AutoHotkey\Scripts\batch, Hide
return

#!v::
Run, arduino_control.bat v, H:\LaunchBox\Utilities\AutoHotkey\Scripts\batch, Hide
return

#!g::
Run, arduino_control.bat g, H:\LaunchBox\Utilities\AutoHotkey\Scripts\batch, Hide
return

#!b::
Run, arduino_control.bat b, H:\LaunchBox\Utilities\AutoHotkey\Scripts\batch, Hide
return

#!a::
Run, arduino_control.bat a, H:\LaunchBox\Utilities\AutoHotkey\Scripts\batch, Hide
return

#!z::
Run, arduino_control.bat z, H:\LaunchBox\Utilities\AutoHotkey\Scripts\batch, Hide
return

#!d::
Run, arduino_control.bat zvb., H:\LaunchBox\Utilities\AutoHotkey\Scripts\batch, Hide
return


; Testing Sinden Trigger Pull Recoil
; Likely will just wire into Arduino
; #!e::
; if !IsTriggerPulled {
;     Run, arduino_control.bat e, H:\LaunchBox\Utilities\AutoHotkey\Scripts\batch, Hide
;     SetTimer, WaitForButtonUpE, 10
;     IsTriggerPulled := true
; }
; return

; WaitForButtonUpE:
; if GetKeyState("E")  ; The button is still, down, so keep waiting.
;     return
; Otherwise, the button has been released.
; SetTimer, WaitForButtonUpE, Off
; IsTriggerPulled := false
; return

;#!=::
;Run, arduino_control.bat +, H:\LaunchBox\Utilities\AutoHotkey\Scripts\batch, Hide
;return

;#!-::
;Run, arduino_control.bat -, H:\LaunchBox\Utilities\AutoHotkey\Scripts\batch, Hide
;return