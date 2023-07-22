CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11 -lcurl -pthread

main: main.o readData.o sortToGroup.o Group.o Bootstrap.o CalculationFunction.o PriceMap.o Matrix.o visualization.o
	$(CC) $(CFLAGS) -o main main.o readData.o sortToGroup.o Group.o Bootstrap.o CalculationFunction.o PriceMap.o Matrix.o visualization.o

main.o: main.cpp Stock.h Group.h sortToGroup.h readData.h Bootstrap.h CalculationFunction.h PriceMap.h visualization.o Matrix.o
	$(CC) $(CFLAGS) -c main.cpp

Bootstrap.o: Bootstrap.cpp Group.h readData.h CalculationFunction.h Bootstrap.h PriceMap.h
	$(CC) $(CFLAGS) -c Bootstrap.cpp
	
PriceMap.o : PriceMap.cpp Stock.h Group.h PriceMap.h readData.h
	$(CC) $(CFLAGS) -c PriceMap.cpp
 
readData.o: readData.cpp Stock.h Group.h readData.h
	$(CC) $(CFLAGS) -c readData.cpp

CalculationFunction.o: CalculationFunction.cpp CalculationFunction.h Matrix.h
	$(CC) $(CFLAGS) -c CalculationFunction.cpp

Matrix.o: Matrix.cpp Matrix.h
	$(CC) $(CFLAGS) -c Matrix.cpp

sortToGroup.o: sortToGroup.cpp Stock.h Group.h sortToGroup.h
	$(CC) $(CFLAGS) -c sortToGroup.cpp

Group.o: Group.cpp Stock.h Group.h
	$(CC) $(CFLAGS) -c Group.cpp

visualization.o: visualization.cpp visualization.h
	$(CC) $(CFLAGS) -c visualization.cpp

clean:
	rm -rf main *.o
