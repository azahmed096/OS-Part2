SOURCE:=main.cpp mastermind/generation.cpp mastermind/mastermind.cpp
SOURCE+=master.cpp player.cpp
PROCESS:=8
WARNINGS:=-Wall -Wextra

all: compile

compile:
	mpiCC $(SOURCE) $(WARNINGS) -o hello -ggdb

rund: compile
	mpirun -np $(PROCESS) gdb -ex='set confirm on' -ex=run -ex=quit --args ./hello # mpirun -np $(PROCESS) gdb -ex run ./hello

report:
	xelatex report.tex

run: compile
	mpirun -np $(PROCESS) ./hello