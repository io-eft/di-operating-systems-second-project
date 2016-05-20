#ifndef __MYENTRYQUEUE__HPP
#define __MYENTRYQUEUE__HPP
#include "myentry.hpp"

class EntryQueue
{
    struct Node
    {
        Entry* content;
        Node* next;
        Node(Entry* e);
        ~Node();
    };
    Node* head;
    Node* tail;
    int items;

  public:
    EntryQueue();
    ~EntryQueue();

    bool isEmpty();

    void insertEntry(Entry* e);

    Entry* getTop();
    void popTop();

    void printTopToFifo(FILE* fp);
    void printQueue(ofstream* out);

    void flushQueue();

	int getItems(){return items;}
};

#endif
