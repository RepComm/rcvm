
gcc ./src/main.c \
  -g \
  -L./ \
  -Wl,-rpath='${ORIGIN}' \
  -Wall \
  -o rcvm

chmod +x ./rcvm

echo done compiling
