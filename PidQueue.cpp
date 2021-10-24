// Class for the PidQueue (is more or less static)
// This class is used to store the PIDs for processes. It does this by storing numbers that can be used

class PidQueue
{
    static pidMax = 0;
    static int size = 0; // stores the size of the queue (the number of events)
    static PidNode* first = NULL;
    static PidNode* last = NULL;


    public:
        PidQueue()
        {
            
        }

    int requestPID()
    {
        int i = 0;
        if (first == NULL) // if no PIDs in queue
        {
            PidNode n = new PidNode();
            n.setPID(pidMax); // creates a new PID equal to the max
            pidMax = pidMax + 1; // increases the counter for the highest PID
            first = &n; // sets the first address
            last = &n; // sets the last address

        }
        else // if there is already events in queue, add it to the end
        {
            PidNode* temp = first;
            first = first.getNext();
            i = temp.getPID();
            delete temp; // need to delete the memory associated with the temp variable
            size = size - 1; // decrement size
        }
        size = size + 1; // increment size
        return i;
    }

    putbackPID(int p) // puts the PID back in the queue (should be used with a process that is being terminated)
    {
        if (first == NULL) // if no PIDS in queue
        {
            PidNode n = new PidNode();
            n.setPID(p);
            first = &n; // sets the first address
            last = &n; // sets the last address
        }
        else // if there is already PIDs in queue, add it to the end
        {
            PidNode n = new PidNode();
            n.setPID(p);
            *last.setNext(&n);
            last = &n;
        }
        size = size + 1; // increment size
    }
}