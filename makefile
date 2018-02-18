all:
	#------------------------------------------
	check/checkpatch.pl --no-tree -f src/main.c
	#------------------------------------------
	cppcheck src/main.c
	#------------------------------------------
	gcc -c src/main.c
	gcc -o bin/Hello main.o
	#------------------------------------------
	./bin/Hello

clean:
	rm main.o