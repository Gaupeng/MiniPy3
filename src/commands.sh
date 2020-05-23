make clean
make
./a.out < ../test/testInput.py > ../test/output.txt
./a.out < ../test/testInput2.py > ../test/output2.txt
./a.out < ../test/testInput3.py > ../test/output3.txt
python3 assembler.py