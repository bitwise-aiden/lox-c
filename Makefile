BUILD_DIR := build

default: lox-c

# Compile interpreter
lox-c:
	@ $(MAKE) -f util/c.make NAME=lox-c MODE=release SOURCE_DIR=src
	@ cp build/lox-c lox-c # For convenience, copy the interpreter to the top level.
