run: main.cpp
	g++ -std=c++11 Main.cpp -o Game
	./Game

threads: main.cpp
	g++ -std=c++11 main.cpp -o Game -lpthread
	./Game

valgrind: main.cpp
	g++ -std=c++11 main.cpp -o Game
	valgrind --leak-check=yes Game
#windows : Base.c
#	gcc -H Base.c -o Game.exe

clean :
	rm -f Game
#uses -f so it deletes what it can without stopping for errors in removal

#Note: Found a way to check if a file exists from https://stackoverflow.com/questions/5553352/how-do-i-check-if-file-exists-in-makefile-so-i-can-delete-it
#ifneq ("$(wildcard $(PATH_TO_FILE))","")
#   FILE_EXISTS = 1
#else
#   FILE_EXISTS = 0
#endif
