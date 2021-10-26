// This is the class for the node used by PidQueue.cpp
#pragma once

#include <cstdio>

class PidNode
{
    private:
        int pid = 0; // stores and pid that is indicated by a number 
        PidNode* next = NULL; // stores the pointer to the next node
    

    public:
        PidNode(int i); // the constructor
        //PidNode(int i, PidNode* e);
        //PidNode(PidNode* e, int i);
        void setNext(PidNode* e);
        PidNode* getNext();
        int getPID();
};
// Constructors
PidNode::PidNode(int i) // creates an pid that is not connected to anything
{
    pid = i;
}
/* I don't use these constructors
PidNode::PidNode(int i, PidNode* e) // creates an pid that is in front of another node
{
    pid = i;
    next = e;
}
PidNode::PidNode(PidNode* e, int i) // creates an pid that is behind another node
{
    pid = i;
    e->setNext(&this);
}
*/
// Functions
void PidNode::setNext(PidNode* e)
{
    next = e;
}

PidNode* PidNode::getNext()
{
    return next;
}

int PidNode::getPID()
{
    return pid;
}