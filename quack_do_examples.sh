
./bin/parser ./working_samples/GoodWalk.qk
./bin/parser ./working_samples/Pt_Print.qk
./bin/parser ./working_samples/schroedinger2.qk
./bin/parser ./working_samples/Sqr.qk
./bin/parser ./working_samples/SqrDecl.qk
./bin/parser ./working_samples/Submarine.qk
./bin/parser ./working_samples/while_test.qk

gcc -c ./ir_out/Builtins.c -o ./ir_out/Builtins.o

#gcc ./ir_out/GoodWalk.qk.c ./ir_out/Builtins.o -o ./ir_out/GoodWalk
gcc ./ir_out/Pt_Print.qk.c ./ir_out/Builtins.o -o ./ir_out/Pt_Print
gcc ./ir_out/schroedinger2.qk.c ./ir_out/Builtins.o -o ./ir_out/schroedinger2
gcc ./ir_out/Sqr.qk.c ./ir_out/Builtins.o -o ./ir_out/Sqr
gcc ./ir_out/SqrDecl.qk.c ./ir_out/Builtins.o -o ./ir_out/SqrDecl
gcc ./ir_out/Submarine.qk.c ./ir_out/Builtins.o -o ./ir_out/Submarine
gcc ./ir_out/while_test.qk.c ./ir_out/Builtins.o -o ./ir_out/while_test

echo "GoodWalk loops infinitely"
echo "Pt_Print output:"
./ir_out/Pt_Print
echo ""
echo "========="
echo "schroedinger2 output:"
./ir_out/schroedinger2
echo
echo "========="
echo "Sqr output:"
./ir_out/Sqr
echo
echo "========="
echo "SqrDecl output:"
./ir_out/SqrDecl
echo
echo "========="
echo "Submarine output:"
./ir_out/Submarine
echo
echo "========="
echo "while_test output:"
./ir_out/while_test
echo
echo "========="