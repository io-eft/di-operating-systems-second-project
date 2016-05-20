#include "myentryindex.hpp"

EntryIndex::EntryIndex(int a): currentPosition(0), indexSize(a), itemsinindex(0)
{
    index = new Entry*[a];
    for(int i = 0; i < a; i++)
        index[i] = NULL;
}

EntryIndex::~EntryIndex()
{
    for(int i = 0; i < indexSize; i++)
    {
        if(index[i] != NULL)
            delete index[i];    //needed to free the memory allocated with new
    }
    delete[] index;         //needed to free the memory allocated with new
}

void EntryIndex::printIndex()
{
    for(int i = 0; i < indexSize; i++)
    {
        if(index[i] != NULL)
            index[i]->printEntry();
    }
}

void EntryIndex::insertEntry(Entry* e)
{
    if(currentPosition < indexSize)
    {
        index[currentPosition] = e;
        currentPosition++;
        itemsinindex++;
    }
}

void EntryIndex::mySwap(int a, int b)
{
    Entry* temp = index[a];
    index[a] = index[b];
    index[b] = temp;
}

int EntryIndex::partById(int left, int right, int pivotIndex)   //part by id
{                                                               //algorith found in Wikipedia page for quicksort (http://en.wikipedia.org/wiki/Quicksort)
    unsigned long pivotValue = index[pivotIndex]->getId();

    mySwap(right, pivotIndex);

    int storeIndex = left;

    for(int i = left; i < right; i++)
    {
        if(index[i]->getId() <= pivotValue)
        {
            mySwap(i, storeIndex);
            storeIndex++;
        }
    }
    mySwap(right, storeIndex);

    return storeIndex;
}

void EntryIndex::quicksortById(int left, int right)     //sort by id
{                                                       //algorith found in Wikipedia page for quicksort (http://en.wikipedia.org/wiki/Quicksort)
    if(left < right)
    {
        int pivotIndex = left + (right - left)/2;
        int pivotNewIndex = partById(left, right, pivotIndex);
        quicksortById(left, pivotNewIndex - 1);
        quicksortById(pivotNewIndex + 1, right);
    }
}

int EntryIndex::partBySurname(int left, int right, int pivotIndex)  //part by surname
{                                                                   //algorith found in Wikipedia page for quicksort (http://en.wikipedia.org/wiki/Quicksort)
    string pivotValue = index[pivotIndex]->getSurname();

    mySwap(right, pivotIndex);

    int storeIndex = left;

    for(int i = left; i < right; i++)
    {
        if(index[i]->getSurname() <= pivotValue)
        {
            mySwap(i, storeIndex);
            storeIndex++;
        }
    }
    mySwap(right, storeIndex);

    return storeIndex;
}

void EntryIndex::quicksortBySurname(int left, int right)        //sort by surname
{                                                               //algorith found in Wikipedia page for quicksort (http://en.wikipedia.org/wiki/Quicksort)
    if(left < right)
    {
        int pivotIndex = left + (right - left)/2;
        int pivotNewIndex = partBySurname(left, right, pivotIndex);
        quicksortBySurname(left, pivotNewIndex - 1);
        quicksortBySurname(pivotNewIndex + 1, right);
    }
}

int EntryIndex::partByName(int left, int right, int pivotIndex)     //part by name
{                                                                   //algorith found in Wikipedia page for quicksort (http://en.wikipedia.org/wiki/Quicksort)
    string pivotValue = index[pivotIndex]->getName();

    mySwap(right, pivotIndex);

    int storeIndex = left;

    for(int i = left; i < right; i++)
    {
        if(index[i]->getName() <= pivotValue)
        {
            mySwap(i, storeIndex);
            storeIndex++;
        }
    }
    mySwap(right, storeIndex);

    return storeIndex;
}

void EntryIndex::quicksortByName(int left, int right)               //sort by name
{                                                                   //algorith found in Wikipedia page for quicksort (http://en.wikipedia.org/wiki/Quicksort)
    if(left < right)
    {
        int pivotIndex = left + (right - left)/2;
        int pivotNewIndex = partByName(left, right, pivotIndex);
        quicksortByName(left, pivotNewIndex - 1);
        quicksortByName(pivotNewIndex + 1, right);
    }
}

int EntryIndex::partByIncome(int left, int right, int pivotIndex)   //part by income
{                                                                   //algorith found in Wikipedia page for quicksort (http://en.wikipedia.org/wiki/Quicksort)
    unsigned long pivotValue = index[pivotIndex]->getIncome();

    mySwap(right, pivotIndex);

    int storeIndex = left;

    for(int i = left; i < right; i++)
    {
        if(index[i]->getIncome() <= pivotValue)
        {
            mySwap(i, storeIndex);
            storeIndex++;
        }
    }
    mySwap(right, storeIndex);

    return storeIndex;
}

void EntryIndex::quicksortByIncome(int left, int right)             //sort by income
{                                                                   //algorith found in Wikipedia page for quicksort (http://en.wikipedia.org/wiki/Quicksort)
    if(left < right)
    {
        int pivotIndex = left + (right - left)/2;
        int pivotNewIndex = partByIncome(left, right, pivotIndex);
        quicksortByIncome(left, pivotNewIndex - 1);
        quicksortByIncome(pivotNewIndex + 1, right);
    }
}

void EntryIndex::quicksort(int i)       //quicksort
{
    switch(i)                           //sort by the proper field, given in the input
    {
        case(0): quicksortById(0, indexSize -1);break;
        case(1): quicksortBySurname(0, indexSize -1);break;
        case(2): quicksortByName(0, indexSize -1);break;
        case(3): quicksortByIncome(0, indexSize -1);break;
    }
}

void EntryIndex::shellsort(int i)       //shellsort
{                                       //sort by the proper field, given in the input
    switch(i)
    {
        case(0): shellsortById();break;
        case(1): shellsortBySurname();break;
        case(2): shellsortByName();break;
        case(3): shellsortByIncome();break;
    }
}

void EntryIndex::shellsortById()                        //sory by id
{                                                       //algorith found in Wikipedia page for shellsort (http://en.wikipedia.org/wiki/Shellsort)
    int gap[8] = {701, 301, 132, 57, 23, 10, 4, 1};
    Entry* temp;
    for(int k = 0; k < 8; k++)
    {
        for(int i = gap[k]; i < indexSize; i++)
        {
            temp = index[i];
            int j;
            for(j = i; j >= gap[k] && index[j - gap[k]]->getId() > temp->getId(); j-= gap[k])
            {
                index[j] = index[j-gap[k]];
            }
            index[j] = temp;
        }
    }
}

void EntryIndex::shellsortBySurname()                   //sort by surname
{                                                       //algorith found in Wikipedia page for shellsort (http://en.wikipedia.org/wiki/Shellsort)
    int gap[8] = {701, 301, 132, 57, 23, 10, 4, 1};
    Entry* temp;
    for(int k = 0; k < 8; k++)
    {
        for(int i = gap[k]; i < indexSize; i++)
        {
            temp = index[i];
            int j;
            for(j = i; j >= gap[k] && index[j - gap[k]]->getSurname() > temp->getSurname(); j-= gap[k])
            {
                index[j] = index[j-gap[k]];
            }
            index[j] = temp;
        }
    }
}

void EntryIndex::shellsortByName()                      //sort by name
{                                                       //algorith found in Wikipedia page for shellsort (http://en.wikipedia.org/wiki/Shellsort)
    int gap[8] = {701, 301, 132, 57, 23, 10, 4, 1};
    Entry* temp;
    for(int k = 0; k < 8; k++)
    {
        for(int i = gap[k]; i < indexSize; i++)
        {
            temp = index[i];
            int j;
            for(j = i; j >= gap[k] && index[j - gap[k]]->getName() > temp->getName(); j-= gap[k])
            {
                index[j] = index[j-gap[k]];
            }
            index[j] = temp;
        }
    }
}

void EntryIndex::shellsortByIncome()                //sort by income
{                                                   //algorith found in Wikipedia page for shellsort (http://en.wikipedia.org/wiki/Shellsort)
    int gap[8] = {701, 301, 132, 57, 23, 10, 4, 1};
    Entry* temp;
    for(int k = 0; k < 8; k++)
    {
        for(int i = gap[k]; i < indexSize; i++)
        {
            temp = index[i];
            int j;
            for(j = i; j >= gap[k] && index[j - gap[k]]->getIncome() > temp->getIncome(); j-= gap[k])
            {
                index[j] = index[j-gap[k]];
            }
            index[j] = temp;
        }
    }
}

void EntryIndex::printToFifo(FILE* fp)      //print the input to a file
{
    for(int i = 0; i < indexSize; i++)
    {
        if(index[i] != NULL)
            index[i]->printToFifo(fp);
    }
}

void EntryIndex::mysort(int mode, int field)    //choose the proper type of sorting, based on input
{
    if(mode == 1)
        shellsort(field);
    else
        quicksort(field);
}

int EntryIndex::getItemsInIndex()
{
    return itemsinindex;
}
