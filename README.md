# jampgame_proxy

Proxy between the server engine and the game module of the game STAR WARS™ Jedi Knight - Jedi Academy™.
This aims to add features to yberion's original work [JKA_YBEProxy](https://github.com/Yberion/JKA_YBEProxy)

## Compilation

You can use `build.sh` to build the proxy by yourself.

Patchnote : https://hackmd.io/E6LOdJOVQBi4pr1S7z11UA

Todo : https://hackmd.io/LDI7ekrzREu7WFHZJKooMQ

Features : https://hackmd.io/kvj--DTaTmOofjxL5bud-Q

Trampoline hook : https://hackmd.io/7SyusRFMR-m06e-nQ2ehDw

## Notes

If you deal with `static` functions, you better copy/paste them in the Proxy instead of making a trampoline of them because it won't work properly.  
Then after that, each function that call this `static` function should be redefined in a trampoline.  

I'm not sure yet but it seems that sometimes with Discord (and even without) opened it would crash with typing from in-game `rcon map_restart 0` / `rcon map mp/ffa2` (any map) with one of the following hook/trampoline:  

- `SV_PacketEvent`
- `SV_ConnectionlessPacket`
- `SVC_RemoteCommand`
