// Class for the PidQueue (is more or less static)
// This class is used to store the PIDs for processes. It does this by storing numbers that can be used in a queue
// if the queue doesn't have a number available, it makes one using the max number before raising the max

#pragma once
#ifndef PIDQUEUE
#define PIDQUEUE

#include "PidNode.cpp"
#include "Support.cpp"

class PidQueue
{
    private:
        static int pidMax;
        static int size; // stores the size of the queue (the number of events)
        static PidNode* first;
        static PidNode* last;

    public:
        PidQueue(); // the constructor
        int requestPID();
        void putBackPID(int p);
};
// I was forced to initialize the static variables outside of the class
int PidQueue::pidMax = 0;
int PidQueue::size = 0;
PidNode* PidQueue::first = NULL;
PidNode* PidQueue::last = NULL;

// Constructor
PidQueue::PidQueue()
{
    pidMax = 0;
    size = 0;
    first = NULL;
    last = NULL;
}

// Functions
int PidQueue::requestPID()
{
    int i = 0;
    if (first == NULL) // if no PIDs in queue, adds one to the queue
    {
        /*
        PidNode* n = new PidNode(pidMax); // creates a new PID equal to the max
        pidMax = pidMax + 1; // increases the counter for the highest PID
        first = n; // sets the first address
        last = n; // sets the last address
        i = requestPID(); // calls again to get the else to trigger on the newly added PID
        */
        i = pidMax; // sets the current pidMax to be given as the pid
        pidMax = pidMax + 1; // increases the max
    }
    else // if there is already PIDs in queue, grab the first one
    {
        PidNode* temp = first;
        if (size > 1) // checks if there is more than one
        {
            first = first->getNext();
        }
        else
        {
            first = NULL;
            last = NULL;
        }
        
        i = temp->getPID();
        if (showDeletes)
            printf("Deleting node: PidQueue request\n"); // for TESTING
        delete temp; // need to delete the memory associated with the temp variable
        size = size - 1; // decrement size
    }
    return i;
}

void PidQueue::putBackPID(int p) // puts the PID back in the queue (should be used with a process that is being terminated)
{
    if (first == NULL) // if no PIDS in queue
    {
        PidNode* n = new PidNode(p);
        first = n; // sets the first address
        last = n; // sets the last address
    }
    else // if there is already PIDs in queue, add it to the end
    {
        PidNode* n = new PidNode(p);
        last->setNext(n);
        last = n;
    }
    size = size + 1; // increment size
}

#endif