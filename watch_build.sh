#!/usr/bin/env sh
nodemon -e c,h,cpp -x 'make bin/app || exit 1' -d 1s $*
