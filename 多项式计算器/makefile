main:	function.o head.h main.o
	g++ function.o main.o head.h -o main
main.o:	main.cpp
	g++ -c -g main.cpp -o main.o
function.o:	function.cpp 
	g++ -c -g function.cpp -o function.o
clean:
	rm *.o
	rm main
