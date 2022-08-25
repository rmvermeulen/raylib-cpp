#!/usr/bin/env sh
nodemon -e c,h,cpp -x 'make bin/app && make execute || exit 1' -d 1s $*
