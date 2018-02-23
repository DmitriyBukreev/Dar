SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=%.o)
CFLAGS = -fsanitize=address -c
LDFLAGS = -fsanitize=address -o

all: dar

dar : precompiled.h.pch $(OBJ)
	gcc $(LDFLAGS) bin/$@ $(OBJ)
	#------------------------------------------

$(OBJ): $(SRC)
	check/checkpatch.pl --no-tree -fix -f $(SRC)
	#------------------------------------------
	cppcheck $(SRC)
	#------------------------------------------
	gcc $(CFLAGS) $(SRC)
	#------------------------------------------

#Testing precompiled headers
precompiled.h.pch: src/precompiled.h
	gcc $< -o $@
	#------------------------------------------