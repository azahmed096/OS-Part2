SOURCE:=main.cpp mastermind/generation.cpp mastermind/mastermind.cpp
PROCESS:=8
all: compile

compile:
	mpiCC $(SOURCE) -o hello -ggdb

rund: compile
	mpirun -np $(PROCESS) gdb -ex='set confirm on' -ex=run -ex=quit --args ./hello # mpirun -np $(PROCESS) gdb -ex run ./hello

run: compile
	mpirun -np $(PROCESS) ./hello