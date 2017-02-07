#!/bin/bash

file=$1
compile () {
  echo "Compiling"
  llc "$file.ll"
  clang "$file.s" -o $file
}

run () {
  echo "Running"
  ./$file
}

if [ type inotifywait >/dev/null 2>&1 ]; then
  inotifywait -q -m -e close_write dynsem.ll |
  while read -r filename event; do
    echo "Change detected"
    compile
    run
  done
else 
  compile
  run
fi
