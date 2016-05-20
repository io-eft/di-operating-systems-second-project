all: mysortapp mysorter internalnode

mysortapp: mysortapp.o myfunction.o 
	g++ -g -Wall mysortapp.o myfunction.o -o mysortapp

mysorter: mysorter.o myentry.o myentryindex.o
	g++ -g -Wall mysorter.o myentry.o myentryindex.o -o mysorter

internalnode: internalnode.o myentry.o myentryqueue.o myfunction.o
	g++ -g -Wall internalnode.o myentry.o myentryqueue.o myfunction.o -o internalnode

mysortapp.o: mysortapp.cpp
	g++ -c mysortapp.cpp

mysorter.o: mysorter.cpp
	g++ -c mysorter.cpp

internalnode.o: internalnode.cpp
	g++ -c internalnode.cpp

myfunction.o: myfunction.cpp
	g++ -c myfunction.cpp

myentryqueue.o: myentryqueue.cpp
	g++ -c myentryqueue.cpp

myentryindex.o: myentryindex.cpp
	g++ -c myentryindex.cpp

myentry.o: myentry.cpp
	g++ -c myentry.cpp

clean:
	rm -rf *o mysortapp

