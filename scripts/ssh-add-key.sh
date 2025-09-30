#!/usr/bin/env bash

if [[ "$1" != "" ]]; then
  eval "$(ssh-agent)"
  ssh-add "$1"
fi

echo -e "pass your private ssh-key as an argument:
\n
./ssh-add-key.sh ~/.ssh/id_ed25519"

