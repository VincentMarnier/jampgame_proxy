FROM i386/debian:trixie

WORKDIR /jka
RUN apt update && apt install -y wget unzip
RUN wget https://mrwonko.de/jk3files/files/20490_jalinuxded_1.011.zip
RUN unzip 20490_jalinuxded_1.011.zip
RUN rm 20490_jalinuxded_1.011.zip
RUN chmod +x linuxjampded
RUN mkdir base
RUN mv jampgamei386.so base/jampgame_original.so
RUN mv libcxa.so.1 /usr/lib
