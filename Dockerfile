FROM archlinux:multilib-devel

LABEL org.opencontainers.image.authors="nooneknowspeter"
LABEL org.opencontainers.image.source="https://github.com/nooneknowspeter/cpplings"
LABEL org.opencontainers.image.title="cpplings"
LABEL org.opencontainers.image.description="A terminal based learning experience for C++, inspired by Rustlings"
LABEL org.opencontainers.image.licenses="MIT"
LABEL org.opencontainers.image.documentation="https://github.com/nooneknowspeter/cpplings/blob/main/README.md"

# update system and install required packages
RUN pacman -Syyu --needed --noconfirm git curl direnv

# # makepkg user and workdir
# ARG user=someone
# RUN useradd --system --create-home $user \
#   && echo "$user ALL=(ALL:ALL) NOPASSWD:ALL" > /etc/sudoers.d/$user
# USER $user
# WORKDIR /home/$user
#
# # install yay
# RUN git clone https://aur.archlinux.org/yay.git \
#   && cd yay \
#   && makepkg -sri --needed --noconfirm \
#   && cd \
#   # Clean up
#   && rm -rf .cache yay

# install nix
RUN sh <(curl --proto '=https' --tlsv1.2 -L https://nixos.org/nix/install) --daemon --yes

# restart shell
RUN exec bash

# create volume
VOLUME [ "/cpplings" ]
WORKDIR /cpplings

