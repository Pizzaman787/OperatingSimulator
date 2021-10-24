

class EventNode
{
    int event = 0; // stores and event that is indicated by a number 
    /* 0 is none 
    *  1 is calculate
    *  2 is i/o
    *  3 is critical
    *  4 is critical end
    */
    EventNode* next = NULL; // stores the pointer to the next node
    

    public:
        EventNode(int i) // creates an event that is not connected to anything
        {
            event = i;
        }

        EventNode(int i, EventNode* e) // creates an event that is in front of another node
        {
            event = i;
            next = e;
        }
        EventNode(EventNode* e, int i) // creates an event that is behind another node
        {
            event = i;
            *e.setNext(&this);
        }

    setNext(EventNode* e)
    {
        next = e;
    }

    EventNode* getNext()
    {
        return next;
    }

    int getEvent()
    {
        return event;
    }
}