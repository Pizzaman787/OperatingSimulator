class PidNode
{
    int pid = 0; // stores and pid that is indicated by a number 
    PidNode* next = NULL; // stores the pointer to the next node
    

    public:
        PidNode(int i) // creates an pid that is not connected to anything
        {
            pid = i;
        }

        PidNode(int i, PidNode* e) // creates an pid that is in front of another node
        {
            pid = i;
            next = e;
        }
        PidNode(PidNode* e, int i) // creates an pid that is behind another node
        {
            pid = i;
            *e.setNext(&this);
        }

    setNext(PidNode* e)
    {
        next = e;
    }

    PidNode* getNext()
    {
        return next;
    }

    int getPID()
    {
        return pid;
    }
}