#!/bin/bash

set -e

if [ "$#" -lt 1 ]; then
  echo "Usage: ./$0 option[build | run]"
  exit 1
fi

case "$1" in
  build)
    gcc src/main.c src/ai.c src/fitness.c src/nutrition.c src/user.c -L C:/Users/diyen/Downloads/curl-8.10.0_1-win64-mingw/curl-8.10.0_1-win64-mingw/lib -I C:/Users/diyen/Downloads/curl-8.10.0_1-win64-mingw/curl-8.10.0_1-win64-mingw/include -lcurl -o build/app.exe
    echo "Built app."
    ;;
  run)
    echo "Running app."
    ./build/app.exe
    ;;
  *)
    echo "Invalid argument: $1"
    echo "Usage: ./$0 option[build | run]"
    exit 1
    ;;
esac
