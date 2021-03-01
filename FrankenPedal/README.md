# FrankenPedal
The FrankenPedal is a modified Infinity USB Transcription Pedal (in my case an IN-USB-2) that has it's factory control board replaced with an Arduino Pro Micro. This allows the pedal to be recognized as a regular HID Keyboard/Mouse rather than the specialized VEC device that is normally registered.

Using flashing your Pro Micro with my INO file and using in combination with the Windows console application will allow you to re-bind your pedals to almost any keyboard or mouse input.

I have made a video about installing the Pro Micro into the Infinity Pedal as well as using the software which is available [HERE](https://www.youtube.com/watch?v=DTvxHF_ZBjA).

### Flashing Firmware

1. Download the latest ZIP file from the Releases page
2. Using the Arduino IDE - Open the FrankenPedal INO file within the Arduino folder
3. Set board to "Arduino Leonardo"
4. Set port to your Arduino
5. Flash the firmware to the Arduino

### FrankenPedal Windows Application

1. Extract the ZIP file downloaded from the Releases page
2. Move to the desired location on your machine
3. Using Command Prompt or PowerShell, run the FrankenPedal.exe application with the correct command line parameter. Use --help to see usage instructions and parameters
