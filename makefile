#makefile

all:
	g++ -std=c++11 -Wall -pedantic -Wno-unused-variable -g -o ims2015-2 ims2015-2.cpp

clean:
	rm ims2015-2

run:
	./ims2015-2 0 400 > test01.out
	./ims2015-2 1 400 > test02.out
