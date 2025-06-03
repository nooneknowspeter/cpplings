FROM archlinux:multilib-devel

LABEL org.opencontainers.image.authors="nooneknowspeter"
LABEL org.opencontainers.image.source="https://github.com/nooneknowspeter/cpplings"
LABEL org.opencontainers.image.title="cpplings"
LABEL org.opencontainers.image.description="A terminal based learning experience for C++, inspired by Rustlings"

# update system
RUN pacman -Syyu --needed --noconfirm git

# makepkg user and workdir
ARG user=someone
RUN useradd --system --create-home $user \
  && echo "$user ALL=(ALL:ALL) NOPASSWD:ALL" > /etc/sudoers.d/$user
USER $user
WORKDIR /home/$user

# install yay
RUN git clone https://aur.archlinux.org/yay.git \
  && cd yay \
  && makepkg -sri --needed --noconfirm \
  && cd \
  # Clean up
  && rm -rf .cache yay
