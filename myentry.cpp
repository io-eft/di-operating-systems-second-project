#include "myentry.hpp"

Entry::Entry(unsigned long i, string s1, string s2, unsigned long inc): id(i), surname(s1), name(s2), income(inc) {}

void Entry::printEntry()
{
    cout << id << " " << surname << " " << name << " " << income << endl;
}

unsigned long Entry::getId(){return id;}

string Entry::getSurname(){return surname;}

string Entry::getName(){return name;}

unsigned long Entry::getIncome(){return income;}

void Entry::printToFifo(FILE* fp)
{
    fprintf(fp, "%lu %s %s %lu\n", id, surname.c_str(), name.c_str(), income);
}

void Entry::printToStream(ofstream* out)
{
    *out << id << " " << surname << " " << name << " " << income << endl;
}

void Entry::print()
{
    cout << id << " " << surname << " " << name << " " << income << endl;
}
