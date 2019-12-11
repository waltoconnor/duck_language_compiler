echo "Compiling $1"
./bin/parser $1

name=$(basename $1)

gcc -c ./ir_out/Builtins.c -o ./ir_out/Builtins.o
gcc ./ir_out/$name.c ./ir_out/Builtins.o -o a.out

echo "./ir_out/$name.c had been compiled and placed in ./a.out"