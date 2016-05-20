#ifndef __MYENTRY__HPP
#define __MYENTRY__HPP
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

using namespace std;

class Entry
{
    unsigned long id;
    string surname;
    string name;
    unsigned long income;
    
    public:
	Entry(unsigned long i, string s1, string s2, unsigned long inc);
	void printEntry();
	unsigned long getId();
	string getSurname();
	string getName();
	unsigned long getIncome();
	void printToFifo(FILE* fp);
	void printToStream(ofstream* out);
	void print();
};


#endif

