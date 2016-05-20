#ifndef __MYENTRYINDEX__HPP
#define __MYENTRYINDEX__HPP
#include "myentry.hpp"

class EntryIndex
{
    Entry** index;
    int currentPosition;
    int indexSize;
    int itemsinindex;

	void mySwap(int a, int b);

	int partById(int left, int right, int pivotIndex);
	int partBySurname(int left, int right, int pivotIndex);
	int partByName(int left, int right, int pivotIndex);
	int partByIncome(int left, int right, int pivotIndex);

	void quicksortById(int left, int right);
	void quicksortBySurname(int left, int right);
	void quicksortByName(int left, int right);
	void quicksortByIncome(int left, int right);

	void shellsortById();
	void shellsortBySurname();
	void shellsortByName();
	void shellsortByIncome();

	void quicksort(int i);
	void shellsort(int a);

    public:
	EntryIndex(int a);
	~EntryIndex();

	void insertEntry(Entry* e);
	void printIndex();
	void printToFifo(FILE* fp);
	
	void mysort(int mode, int field);

	int getItemsInIndex();
};

#endif
