inotifywait -q -m -e close_write dynsem.ll |
while read -r filename event; do
  echo "Change detected"
  llc dynsem.ll
  clang dynsem.s -o dynsem
  echo "Running..."
  ./dynsem
done
