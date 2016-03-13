#!/bin/bash


if [ \( -f "$1/assets" \) ]
then
  rm $1/assets
  echo "%ERROR: $1/assets was regular file. Removed it." >&2
elif [ ! \( -L "$1/assets" \) -a \( -d "$1/assets" \) ]
then
  echo "%ERROR: $1/assets was regular directory. You probably copied assets to $1/assets. Removed it." >&2
  rm -r $1/assets
fi

if [ ! \( -L "$1/assets" \) ]
then
  ln -s ../assets $1/assets
  echo "%Linked $1/assets to assets!" >&2
else
  echo "%Already linked." >&2
fi
exit 0;