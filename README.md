# jampgame_proxy

Proxy between the server engine and the game module of the game STAR WARS™ Jedi Knight - Jedi Academy™.
This aims to add features to yberion's original work [JKA_YBEProxy](https://github.com/Yberion/JKA_YBEProxy)

## Usage

1. Rename the original `jampgamei386.so` to `jampgame_original.so`.
2. Download the [latest release](https://github.com/VincentMarnier/jampgame_proxy/releases/latest)
3. Put it in the place of the original `jampgamei386.so`

## How it works?

`jampded` use the load the proxy and use it as any `jampgamei386.so`. The proxy forwards instructions to `jampgame_original.so`. Some functions are hooked so we can patch them or add features to them.

## Compilation

You can use `build.sh` to build the proxy by yourself.

## Notes

Feel free to contact me or to make a PR is you want to see anything added to it.
