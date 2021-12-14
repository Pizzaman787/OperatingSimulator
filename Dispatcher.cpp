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

#ifndef DISPATCHER
#define DISPATCHER

#include "Support.cpp"
#include "Process.h"
#include "Dispatcher.h"
#include "ProcessArray.cpp"

// static initializations
ProcessArray* Dispatcher::pNeedsSorting = new ProcessArray();
int Dispatcher::availableMemory = 1024;
bool Dispatcher::memArray[1024];
MySemaphore* Dispatcher::threadProtection = new MySemaphore(1); // a semaphore with one opening

// Constructors
Dispatcher::Dispatcher(int s) // the s is for which scheduler
{
    pReady = new ProcessArray();
    pWait = new ProcessArray();
    sched = s;
}

Dispatcher::Dispatcher() // default constructor for use to get access to static variables
{
    pReady = new ProcessArray();
    pWait = new ProcessArray();
}

// Functions
void Dispatcher::addProcess(Process* m)
{
    //pReady->addItem(m);
    pNeedsSorting->addItem(m);
    m->setState(0); // new = 0, ready = 1, running = 2, waiting = 3, terminating = 4
}

int Dispatcher::totalProcesses()
{
    return (pReady->getCount() + pWait->getCount() + pNeedsSorting->getCount());
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
        int event = pWait->getProcess(current)->readCurrentEvent(); // check what is next
        if (event == 2) // is going to waiting
        {
            pWait->getProcess(current)->doEvent(); // continues the wait
            current = current + 1;
        }
        else // is not going to be waiting anymore
        {
            pWait->getProcess(current)->setState(1); // has the process move back to being ready
            swapArray(current, 1, 0); // moves to ready array
            end = pWait->getCount();
        }
    }
    // figure out what scheduler to use and have it sort the ready array
    if (sched == 0)
    {
        threadProtection->wait(); // calls the threadProtection semaphore to prevent multiple threads from readying or moving processes
        readyProcesses();
        moveReadyProcesses();
        threadProtection->signal(); // exits semaphore
        shortestJobFirst(pReady->getArray(), pReady->getCount()); // sorts the ready array
    }
    else if (sched == 1)
    {
    	threadProtection->wait(); // calls the threadProtection semaphore to prevent multiple threads from readying or moving processes
		readyProcesses();
		moveReadyProcesses();
		threadProtection->signal(); // exits semaphore
		lowestPriorityFirst(pReady->getArray(), pReady->getCount()); // sorts the ready array
    }
    // have the pReady array do stuff
        // have the processes try to run, but swap if going to i/o or remove if nothing left
    current = 0;
    end = pReady->getCount();
    while (current < end)
    {
        // reference for state: new = 0, ready = 1, running = 2, waiting = 3, terminating = 4
        int cState = pReady->getProcess(current)->doEvent(); // has the process do something and then returns it current state
        if (cState == 4) // is terminating
        {
            removeProcess(current, 0); // removes the terminating process
            end = pReady->getCount(); // updates what would be the end
            current = current - 1; // undoes the later add to make sure the removal of a process doesn't cause skipping
        }
        else if (cState == 3) // is waiting
        {
            swapArray(current, 0, 1); // moves to waiting array if process is in waiting state
            end = pReady->getCount(); // updates what would be the end
            current = current - 1; // undoes the later add to make sure the removal of a process doesn't cause skipping
        }
        else if (cState == 2) // is running
        {
            pReady->getProcess(current)->setState(1); // has the process move back to being ready
        }
        else if (cState == 1) // is ready (as in just finished asking or wrapping up critical)
        {
            //pReady->getProcess(current)->setState(1); // has the process move back to being ready
        }
        else // if somehow new or something
        {
            printf("Error: Impossible state at end of cycle\n"); // no other states should be possible here, so an error is thrown
        }
        current = current + 1;
    }
}

void Dispatcher::removeProcess(int i, bool j) // i is for index and j is for which array (0 is for ready while 1 is for waiting)
{
    Process* m;
    if (j == 0) // ready array
    {
        m = pReady->getProcess(i);
        pReady->removeItem(i);
    }
    else if (j == 1) // waiting array
    {
        m = pWait->getProcess(i);
        pWait->removeItem(i);
    }
    else if (j == 2)
    {
        m = pNeedsSorting->getProcess(i);
        pNeedsSorting->removeItem(i);
    }
    // frees memory assigned to the process
    int k = 0;
    while (k < 1024)
    {
        if (m->processMemory[k] == 1)
        {
            memArray[k] = 0;
            m->processMemory[k] = 0;
        }
        k = k + 1;
    }
    availableMemory = availableMemory + m->getMemory();
    //m->terminateProcess(); // should already be terminating by the process class setting it itself
    //if (showDeletes)
        //printf("Deleting process: Dispatcher %i %i \n", i, j); // for TESTING
    //delete(m); // currently causes an error of double freeing when having 3+ processes
}

void Dispatcher::swapArray(int i, int j, int k) // i is for index and j is for which array the process is in (0 is ready, 1 is waiting, 2 is Needs Sorting) and k is for destination array
{
    Process* m;
    // grabs process from array
    if (j == 0) // ready array
    {
        m = pReady->getProcess(i);
        pReady->removeItem(i);
    }
    else if (j == 1) // waiting array
    {
        m = pWait->getProcess(i);
        pWait->removeItem(i);
    }
    else if (j == 2) // Needs Sorting array
    {
        m = pNeedsSorting->getProcess(i);
        pNeedsSorting->removeItem(i);
    }
    // moves to new array
    if (k == 0)
        pReady->addItem(m);
    else if (k == 1)
        pWait->addItem(m);
    else if (k == 2)
        pNeedsSorting->addItem(m);
}

void Dispatcher::moveReadyProcesses()
{
    // moves ready items in pNeedsSorting to pReady
    int x = 0;
    while (x < pNeedsSorting->getCount())
    {
        if (pNeedsSorting->getProcess(x)->getState() == 1) // if process is ready
        {
            swapArray(x, 2, 0); // swaps from pNeedsSorting to pReady
        }
    }
}

// this function readies processes if they can be assigned the necessary memory
void Dispatcher::readyProcesses()
{
    // look at process in the pNeedsSorting
    int i = 0;
    while (i < pNeedsSorting->getCount())
    {
        Process* tempP = pNeedsSorting->getProcess(i);
        //check if enough memory is available
        if (availableMemory >= tempP->getMemory())
        {
            //assign slots of memory to the process (this is done one megabyte at a time as pages are one megabyte in this case)
            int j = tempP->getMemory();
            int k = 0;
            while (j > 0)
            {
                if (memArray[k] == 0) // if the slot is empty
                {
                    memArray[k] = 1; // note the slot is now filled
                    tempP->processMemory[k] = 1; // tell the process it has this slot
                    j = j - 1; // note that memory was assigned
                }
                k = k + 1;
            }
            // update available memory
            availableMemory = availableMemory - tempP->getMemory();
            // ready the process
            tempP->setState(1);
        }
        i = i + 1;
    }
}

void Dispatcher::processesStatus()
{
    int ready = pReady->getCount(); // amount of processes in ready array
    int waiting = pWait->getCount(); // amount of processes waiting array

    printf("Printing status of processes: %i\n------------------------------------------------------------\n", totalProcesses());
    printf("For state: new = 0, ready = 1, running = 2, waiting = 3, terminating = 4\n");
    printf("For event: 0 is none, 1 is calculate, 2 is i/o, 3 is critical, 4 is critical end\n");
    // prints the status of all processes in the ready array
    int i = 0;
    while (i < ready)
    {
        pReady->getProcess(i)->printStatus();
        i = i + 1;
    }

    // prints the status of all processes in the waiting array
    i = 0;
    while (i < waiting)
    {
        pWait->getProcess(i)->printStatus();
        i = i + 1;
    }
    printf("\n");
}

void Dispatcher::emptyProcesses() // empties the processes to clear the dispatcher
{
	// clears needs sorting
	int i = 0;
	while (i < pNeedsSorting->getCount())
	{
		pNeedsSorting->getProcess(i)->terminateProcess();
		i = i + 1;
	}
	
	// clears wait
	i = 0;
	while (i < pWait->getCount())
	{
		pWait->getProcess(i)->terminateProcess();
		i = i + 1;
	}
		
	// clears ready
	i = 0;
	while (i < pReady->getCount())
	{
		pReady->getProcess(i)->terminateProcess();
		i = i + 1;
	}
}

void Dispatcher::setScheduler(int i) // changes what dispatcher is being used
{
	sched = i;
}

// shortest job first scheduler
void Dispatcher::shortestJobFirst(Process** p, int s) // takes the array and its size as arguments
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

// lowest priority first scheduler
void Dispatcher::lowestPriorityFirst(Process** p, int s) // takes the array and its size as arguments
{
    // sorts the given array so that they are in order from shortest to longest
    // Bubble sort:
    int i = 0;
    while (i < s)
    {
        int j = 0;
        while (j < (s - 1)) // no comparison for the last one in the array
        {
            if (p[j]->getPriority() > p[j + 1]->getPriority()) // if first is lower priority than second
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

#endif