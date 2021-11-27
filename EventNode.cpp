// This is the class for the node used in EventQueue
#pragma once

class EventNode
{
    private:
        int event = 0; // stores and event that is indicated by a number 
        /* 0 is none 
        *  1 is calculate
        *  2 is i/o
        *  3 is critical
        *  4 is critical end
        */
        EventNode* next = NULL; // stores the pointer to the next node
    

    public:
        EventNode(int i);
        void setNext(EventNode* e);
        EventNode* getNext();
        void setEvent(int i);
        int getEvent();

};
// Constructors
EventNode::EventNode(int i) // creates an event that is not connected to anything
{
    event = i;
}
/*
EventNode(int i, EventNode* e) // creates an event that is in front of another node
{
    event = i;
    next = e;
}
//This one doesn't work as I was unsure how to get "this" and "&" to work together
EventNode(EventNode* e, int i) // creates an event that is behind another node
{
    event = i;
    e->setNext(&this);
}
*/

// Functions
void EventNode::setNext(EventNode* e)
{
    next = e;
}

EventNode* EventNode::getNext()
{
    return next;
}

void EventNode::setEvent(int i)
{
    event = i;
}

int EventNode::getEvent()
{
    return event;
}
