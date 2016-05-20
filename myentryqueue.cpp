#include "myentryqueue.hpp"

EntryQueue::Node::Node(Entry* e): next(NULL)
{
    content = e;
}

EntryQueue::Node::~Node()
{
    delete content; //needed to free the memory allocated with new
}

EntryQueue::EntryQueue(): head(NULL), tail(NULL), items(0) {}

EntryQueue::~EntryQueue()
{
    Node* temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        delete temp;    //needed to free the memory allocated with new
    }
}

bool EntryQueue::isEmpty()
{/*
    if(head == NULL)
        return true;
    else
        return false;*/
    if(items == 0)
        return true;
    else
        return false;
}

void EntryQueue::insertEntry(Entry* e)
{
    Node* n = new Node(e);
    if(isEmpty())
        head = n;
    else
        tail->next = n;
    tail = n;
    items++;
}

Entry* EntryQueue::getTop()
{
    return head->content;
}

void EntryQueue::popTop()
{
    if(!isEmpty())
    {
        Entry* temp = head->content;
        head = head->next;
        delete temp;
    }
    items--;
}

void EntryQueue::flushQueue()
{
    while(!isEmpty())
        popTop();
}

void EntryQueue::printTopToFifo(FILE* fp)
{
    head->content->printToFifo(fp);
}

void EntryQueue::printQueue(ofstream* out)
{
    Node* temp = head;
    while(temp != NULL)
    {
        temp->content->printToStream(out);
        temp = temp->next;
    }
}
