// This is the Dispatcher class
// Its purpose is to hold all processes that are ready and distrubute them to the CPU aka running the processes
// The processes in the dispatcher are schuduled according to the scheduler, which is RoundRobin or Shortest Job First

// when the dispatcher recieves the processes, they go from new to ready and it should take only ready processes to put into the scheduler
// when the dispatcher chooses a process to go, that process goes from ready to running
// if the process goes into i/o, then it sets itself to waiting (should not be put into the next ready queue and instead is checked every cycle to see if done waiting (aka has moved to a different event) and is then set to ready
// if all of the events for a process are done, then the terminate for a process is called, where the process sets itself to terminating and then the dispatcher deletes the process from memory

//Note: Might need to make a class that creates processes and gives them to the dispatcher, but that class could just be main which pretends to be the OS
// For multithreading, can have the dispatcher have different "cpu's" it can assign processes to

// An arraylist for ready processes
// An arraylist for waiting processes

#pragma once

#include "Support.cpp"
#include "Process.cpp"

class Dispatcher
{
    // variables
    ProcessArray* pReady;
    ProcessArray* pWait;

    public:
        Dispatcher(int s, int t); // the constructor
        void addProcess(Process* m);
        void processesStatus();
        int totalProcesses();
        void doStuff();
        //void shortestJobFirst(Process* p[], int s);
};

// Constructors
Dispatcher::Dispatcher(int s, int t) // the s is for which scheduler to use and t is for threads
{
    pReady = new ProcessArray();
    pWait = new ProcessArray();
}

// Functions
void Dispatcher::addProcess(Process* m)
{
    pReady->addItem(m);
    m->setState(1); // new = 0, ready = 1, running = 2, waiting = 3, terminating = 4
}

int Dispatcher::totalProcesses()
{
    return (pReady->getCount() + pWait->getCount());
}

// this is the function to have the processes get running by the dispatcher
// could possibly do mutlithreading by actually making threads in the script and having multiple doStuff functions happening at the same time
void Dispatcher::doStuff()
{
    // have the pWait array do stuff (has the waiting processes continue to wait or move to being ready)
        // have the processes try to wait, but swap if going to something other than i/o or remove if nothing left
    int current = 0;
    int end = pWait->getCount();
    while (current < end)
    {
        // reference for event: 0 is empty, 1 is calculate, 2 is i/o, 3 is critical, 4 is critical end
        int event = pWait.readCurrentEvent(current); // check what is next
        if (event == 2) // is going to waiting
        {
            pWait.doEvent(current); // continues the wait
        }
        else // is not going to be waiting anymore
        {
            pReady->getProcess(current)->setState(1); // has the process move back to being ready
            swapArray(current, 1); // moves to ready array
        }
    }

    // figure out what scheduler to use and have it sort the ready array
    if (s == 0)
    {
        shortestJobFirst(pReady); // sorts the ready array
    }

    // have the pReady array do stuff
        // have the processes try to run, but swap if going to i/o or remove if nothing left
    current = 0;
    end = pReady->getCount();
    while (current < end)
    {
        // reference for state: new = 0, ready = 1, running = 2, waiting = 3, terminating = 4
        int state = pReady.doEvent(current); // has the process do something
        if (state == 4) // is terminating
        {
            removeProcess(current, 0); // removes the terminating process
        }
        else if (state == 3) // is waiting
        {
            swapArray(current, 0); // moves to waiting array if process is in waiting state
        }
        else if (state == 2) // is running
        {
            pReady->getProcess(current)->setState(1); // has the process move back to being ready
        }
        else if (state == 1) // is ready (as in just finished asking or wrapping up critical)
        {
            //pReady->getProcess(current)->setState(1); // has the process move back to being ready
        }
        else // if somehow new
        {
            printf("Error: Impossible state at end of cycle\n"); // no other states should be possible here, so an error is thrown
        }
    }
}

void Dispatcher::removeProcess(int i, bool j) // i is for index and j is for which array (0 is for ready while 1 is for waiting)
{
    Process* m;
    if (!j) // ready array
    {
        Process* m = pReady->getprocess(i);
        pReady->removeItem(i);
    }
    else // waiting array
    {
        Process* m = pWait->getprocess(i);
        pWait->removeItem(i);
    }
    m->terminateProcess();
    delete(m);
}

void Dispatcher::swapArray(int i, bool j) // i is for index and j is for which array the process is in (0 is for ready while 1 is for waiting)
{
    Process* m;
    if (!j) // ready array
    {
        m = pReady->getProcess(i);
        pReady->removeItem(i);
        pWait->addItem(m);
    }
    else // waiting array
    {
        m = pWait->getProcess(i);
        pWait->removeItem(i);
        pReady->addItem(m);
    }
}

void Dispatcher::processesStatus()
{
    int ready = pReady->getCount(); // amount of processes in ready array
    int waiting = pWait->getCount(); // amount of processes waiting array

    printf("Printing status of processes: %i\n------------------------------------------------------------\n", totalProcesses());
    // prints the status of all processes in the ready array
    int i = 0;
    while (i < ready)
    {
        pReady[i]->getProcess(i)->printStatus();
    }

    // prints the status of all processes in the waiting array
    i = 0;
    while (i < waiting)
    {
        pWait[i]->getProcess(i)->printStatus();
    }
    printf("\n");
}

// shortest job first scheduler
void Dispatcher::shortestJobFirst(Process* p[], int s) // takes the array and its size as arguments
{
    // sorts the given array so that they are in order from shortest to longest
    // Bubble sort:
    int i = 0;
    while (i < s)
    {
        int j = 0;
        while (j < (s - 1)) // no comparison for the last one in the array
        {
            if (p[j]->getSize() > p[j + 1]->getSize()) // if first is bigger than second
            {
                // swap positions
                Process* temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}