// Class for the EventQueue
// This class is used to store the events for a process. It does this by storing the number of events and
// then using EventNode to be the nodes in the queue that store the events themselves

class EventQueue
{
    int size = 0; // stores the size of the queue (the number of events)
    EventNode* first = NULL;
    EventNode* last = NULL;


    public:
        EventQueue()
        {
            
        }

    addEvent(int i)
    {
        if (first == NULL) // if no events in queue
        {
            EventNode n = new EventNode(i);
            first = &n; // sets the first address
            last = &n; // sets the last address
        }
        else // if there is already events in queue, add it to the end
        {
            EventNode n = new EventNode(i);
            *last.setNext(&n);
            last = &n;
        }
        size = size + 1; // increment size
    }

    popEvent() // removes the first event in queue and goes to the next one
    {
        EventNode* temp = first;
        first = first.getNext();
        delete temp; // need to delete the memory associated with the temp variable
        size = size - 1; // decrement size
    }
}