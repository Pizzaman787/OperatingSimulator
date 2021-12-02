run: Main.cpp
	g++ -std=c++11 Main.cpp -o Game
	./Game

threads: MainWithThreads.cpp
	g++ -std=c++11 MainWithTHreads.cpp -o Game -lpthread
	./Game

threads2: MainWithThreads.cpp
	g++ -c MainWithTHreads.cpp -o main.o
	g++ -c Dispatcher.cpp -o Dispatcher.o
	g++ -c Process.cpp -o Process.o
	g++ -std=c++11 main.o Dispatcher.o Process.o -o Game -lpthread
	./Game

threads3: MainWithThreads.cpp
	g++ -std=c++11 MainWithTHreads.cpp Dispatcher.cpp Process.cpp -o Game -lpthread
	./Game

valgrind: Main.cpp
	g++ -std=c++11 Main.cpp -o Game
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
