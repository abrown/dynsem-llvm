#!/bin/bash

compile () {
  echo "Compiling"
  llc dynsem.ll
  clang dynsem.s -o dynsem
}

run () {
  echo "Running"
  ./dynsem
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
