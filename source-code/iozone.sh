wget http://www.iozone.org/src/current/iozone3_394.tar

tar xvf iozone3_394.tar 

cd iozone3_394/src/current

make

make linux

./iozone -g# -s 1024
