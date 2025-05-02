# VRisingCapRemove
Standalone mod for VRising that nops some hardcoded checks inside `ServerGameSettings::ToStruct` to allow setting any drop/refinement/stack size rates you want.

## How to install:
Extract `version.dll` into dedicated server folder (`V Rising\VRising_Server` if you play with friends using in-game hosting button).

# Notes: 
While you can set stack sizes multiplier to any value, on clients stack size will not be displayed higher than 4096 (although real item amount will be higher)

Untested on Linux.

Untested in singleplayer.

Do not set `FloorLimit`(these are uncapped by default btw) to values higher than 32767.