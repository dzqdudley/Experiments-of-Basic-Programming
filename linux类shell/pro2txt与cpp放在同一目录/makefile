main:	info.o modify.o head.h main.o
	g++ info.o modify.o main.o head.h -o main
main.o:	main.cpp
	g++ -c -g main.cpp -o main.o
info.o:	info.cpp 
	g++ -c -g info.cpp -o info.o
modify.o: modify.cpp
	g++ -c -g modify.cpp -o modify.o
clean:
	rm *.o
	rm main
