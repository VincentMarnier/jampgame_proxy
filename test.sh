#!/bin/sh

SCRIPT_DIR=$(dirname "$0")
DOCKER_IMAGE_NAME=jampgame-proxy-test


if [ -z "$JKA_GAMEDATA" ]; then
  read -p "Enter GameData's path: " JKA_GAMEDATA
fi

if [ -z "$(docker images -q $DOCKER_IMAGE_NAME 2> /dev/null)" ]; then
  docker build -t $DOCKER_IMAGE_NAME - < "$SCRIPT_DIR/test.Dockerfile"
fi

docker run --rm -it \
  -p 29099:29099/udp -p 29099:29099/tcp \
  -v "$JKA_GAMEDATA/base/assets0.pk3:/jka/base/assets0.pk3" \
  -v "$JKA_GAMEDATA/base/assets1.pk3:/jka/base/assets1.pk3" \
  -v "$JKA_GAMEDATA/base/assets2.pk3:/jka/base/assets2.pk3" \
  -v "$JKA_GAMEDATA/base/assets3.pk3:/jka/base/assets3.pk3" \
  -v "$SCRIPT_DIR/build/src/jampgamei386.so:/jka/base/jampgamei386.so" \
  $DOCKER_IMAGE_NAME \
  /jka/linuxjampded \
    +set dedicated 1 \
    +set net_port 29099 \
    +set sv_hostname "^7[^5TEST^7] ^5jampgame_proxy" \
    +set rconpassword "proxy" \
    +set sv_maxclients 32 \
    +set timelimit 0 \
    +set fraglimit 0 \
    +set g_gametype 6 \
    +set sv_pure 0 \
    +set fraglimit 1 \
    +set timelimit 0 \
    +set g_forcepowerdisable 163837 \
    +map mp/duel1 \
    +addbot cultist
    