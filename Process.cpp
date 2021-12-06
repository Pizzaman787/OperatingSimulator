// Class for a process

// will need to store PID, current state, priority, Memory assigned to it, events to do (Like calculate or await I/O input), parent process, and time when the process was created

/* for the events, could use a queue that duplicates the event according to the number assigned to it.
* An example would be, if it needed to do 3 cycles of Calculate, then add three calculates to the queue
*/

#pragma once
#ifndef PROCESS
#define PROCESS

//#include "Dispatcher.cpp"
#include "Process.h"
#include "Dispatcher.h"
#include "Support.cpp"
#include "CriticalSection.cpp"
#include <cstdio>
#include <stdlib.h>

// I was forced to initialize the static variables outside of the class
PidQueue* Process::pQueue = new PidQueue();

// Constructors
Process::Process() // creation without a parent process
{
    pid = pQueue->requestPID(); // gets a unique PID
    timeCreated = time(NULL); // stores current time in seconds
    queue = new EventQueue();
}

Process::Process(char* s) // creation without a parent process, but with a template for the events
{
    pid = pQueue->requestPID(); // gets a unique PID
    timeCreated = time(NULL); // stores current time in seconds
    queue = new EventQueue();
    addEvents(s); // will make an empty process if template not found
}

Process::Process(char* s, int i) // creation without a parent process, but with a template for the events and priority
{
    pid = pQueue->requestPID(); // gets a unique PID
    timeCreated = time(NULL); // stores current time in seconds
    queue = new EventQueue();
    addEvents(s); // will make an empty process if template not found
    priority = i;
}

Process::Process(Process* p) // creates a fork of the given process with the process being the parent
{
    pid = pQueue->requestPID(); // gets a unique PID
    timeCreated = time(NULL); // stores current time in seconds
    queue = new EventQueue();
    priority = p->getPriority(); // gets the priority of the parent
    parent = p; // sets parent
    memory = p->getMemory();

    // fills the queue with the events in the parent (could make this a function for reusabilty)
    EventNode* en = p->getCurrentEvent();
    while (en != NULL)
    {
        // read what the current event is and add it to the queue
        queue->addEvent(en->getEvent());
        // move to the next node of the parent and repeat (move en to the next in the queue)
        en = en->getNext();
    }
}

// Functions
void Process::setPriority(int i)
{
    priority = i;
}
int Process::getPriority()
{
    return priority;
}

void Process::setState(int i)
{
    state = i;
}

int Process::getState()
{
    return state;
}

int Process::getMemory()
{
    return memory;
}

void Process::printStatus() // for testing purposes
{
    printf("PID: %i\t", pid);
    printf("State: %i\t", state); // new = 0, ready = 1, running = 2, waiting = 3, terminating = 4
    printf("Event: %i\t", queue->readCurrentEvent()); // 0 is none, 1 is calculate, 2 is i/o, 3 is critical, 4 is critical end
    printf("Memory: %i\t", memory);
    printf("Priority: %i\t", priority);
    // could add time created
    // could add parent
    printf("\n");
}

// Adds another layer to the function to allow its call in other classes
int Process::readCurrentEvent()
{
    return (queue->readCurrentEvent());
}

EventNode* Process::getCurrentEvent()
{
    return (queue->getCurrentEvent());
}

// returns the number of elements in the in the queue
int Process::getSize()
{
    return queue->getSize();
}

// if broken, then it is probably that \0 was added by the removeEndings function
// Will also break if more than two tabs exist in a row
// reads template files, but cannot read templates with lines  or names over 1024 characters properly
void Process::addEvents(char* name) // takes the name of a template as input
{
    int size = 1024;
    char path[size];
    // sets the path to look for the named template in the templates directory
    strcpy(path, "./templates/");
    strcat(path, name);
    removeEndings(path);
    strcat(path, ".txt"); // adds ending
    FILE* texts;
    texts = fopen(path, "r+"); // opens the file to read

    if (texts == NULL) // if unable to open the file
    {
        //printf("%s\n", path); // for testing what the path is
        //printf("Unable to open requested template.\n"); // doesn't do anything if the template doesn't exist other than notify of bad input (disabled for use in threads, as it just spams the terminal when input is unused)
    }
    else
    {
        char read[size] = ""; // for storing what is read on each line
        while (fgets(read, size, texts) != NULL) // reads one line at a time, until it hits the size cap (1024), or until end of file
        {
            removeEndings(read);
            char event[size] = "";
            bool eventFound = false;
            int minCycles = 0;
            bool minCyclesFound = false;
            int maxCycles = 0;
            bool maxCyclesFound = false;
            int minMemory = 0;
            bool minMemoryFound = false;
            int maxMemory = 0;
            bool maxMemoryFound = false;
            while(strlen(read) > 0)
            {
                // check if current char is a tab, if it is, remove it
                if (read[0] == '\t')
                {
                    //read = read + 1; // moves the pointer over by 1 character
                    if (read[1] != '\t')
                    {
                        strcpy(read, strchr(read, read[1])); // Removes the tab at the beginning
                    }
                    else
                    {
                        strcpy(read, strchr(read, read[2])); // Removes the two tabs at the beginning
                    }
                }
                // else if the event hasn't been found, grab it and put it in event. Then remove said word from read
                else if (!eventFound)
                {
                    char* temp = strchr(read, '\t');
                    if (temp == NULL) // checks if there wasn't a tab
                    {
                        temp = strchr(read, '\0');
                    }
                    int wordSize = strlen(read) - strlen(temp);
                    int i = 0;
                    while (i < wordSize) // copies the event to the event string
                    {
                        event[i] = read[i];
                        i = i + 1;
                    } 
                    strcpy(read, strchr(read, *temp)); // removes the word from the read string

                    eventFound = true;
                }
                // else the current thing is a number, then add to min unless found, then do max. Then remove said number from read
                else
                {
                    if (!minCyclesFound)
                    {
                        char* temp = strchr(read, '\t');
                        char temp2[1024] = "";
                        if (temp == NULL) // checks if there wansn't a tab
                        {
                            temp = strchr(read, '\0');
                        }
                    
                        int wordSize = strlen(read) - strlen(temp);
                        int i = 0;
                        while (i < wordSize) // copies the number to the temp2 string
                        {
                            temp2[i] = read[i];
                            i = i + 1;
                        } 
                        temp2[i] = '\0'; // adds the end string or null character
                        strcpy(read, temp); // removes the word from the read string

                        minCycles = atoi(&temp2[0]);
                        minCyclesFound = true;
                    }
                    else if (!maxCyclesFound)
                    {
                        char* temp = strchr(read, '\t');
                        char temp2[1024] = "";
                        if (temp == NULL) // checks if there wansn't a tab
                        {
                            temp = strchr(read, '\0');
                        }
                    
                        int wordSize = strlen(read) - strlen(temp);
                        int i = 0;
                        while (i < wordSize) // copies the number to the temp2 string
                        {
                            temp2[i] = read[i];
                            i = i + 1;
                        } 
                        temp2[i] = '\0'; // adds the end string or null character
                        strcpy(read, temp); // removes the word from the read string

                        maxCycles = atoi(&temp2[0]);
                        maxCyclesFound = true;
                    }
                    else if (!minMemoryFound)
                    {
                        char* temp = strchr(read, '\t');
                        char temp2[1024] = "";
                        if (temp == NULL) // checks if there wansn't a tab
                        {
                            temp = strchr(read, '\0');
                        }
                    
                        int wordSize = strlen(read) - strlen(temp);
                        int i = 0;
                        while (i < wordSize) // copies the number to the temp2 string
                        {
                            temp2[i] = read[i];
                            i = i + 1;
                        } 
                        temp2[i] = '\0'; // adds the end string or null character
                        strcpy(read, temp); // removes the word from the read string

                        minMemory = atoi(&temp2[0]);
                        minMemoryFound = true;
                    }
                    else if (!maxMemoryFound)
                    {
                        char* temp = strchr(read, '\t');
                        char temp2[1024] = "";
                        if (temp == NULL) // checks if there wansn't a tab
                        {
                            temp = strchr(read, '\0');
                        }
                    
                        int wordSize = strlen(read) - strlen(temp);
                        int i = 0;
                        while (i < wordSize) // copies the number to the temp2 string
                        {
                            temp2[i] = read[i];
                            i = i + 1;
                        } 
                        temp2[i] = '\0'; // adds the end string or null character
                        strcpy(read, temp); // removes the word from the read string // THIS BREAKS THE STRING SOMEHOW

                        maxMemory = atoi(&temp2[0]);
                        maxMemoryFound = true;
                    }
                    //strcpy(read, strchr(read, read[1])); // Removes the tab at the beginning
                }

                // checks if it is an event that doesn't need it to check any more (soley for optimization)
                if (eventFound)
                {
                    if (strcmp(event, "CRITICAL") == 0)
                        break;
                    else if (strcmp(event, "CRITICAL_END") == 0)
                        break;
                    else if (strcmp(event, "FORK") == 0)
                        break;
                }
            }
            // check if the event is one without cycles (such as critical and critical end)
            if (strcmp(event, "CRITICAL") == 0)
            {
                queue->addEvent(3);
            }
            else if (strcmp(event, "CRITICAL_END") == 0)
            {
                queue->addEvent(4);
            }
            else if (strcmp(event, "FORK") == 0)
            {
                queue->addEvent(5);
            }
            // generate the number of cycles and add the events to the queue
            else
            {
                //srand(time(NULL)); // seeds rand
                int j = (int)(rand() % (maxCycles + 1 - minCycles)); // gets the range between min and max
                j = j + minCycles; // adds the min to set a minumum number
                int i = 0;
                while (i < j)
                {
                    if (strcmp(event, "CALCULATE") == 0)
                    {
                        queue->addEvent(1);
                    }
                    else if (strcmp(event, "I/O") == 0)
                    {
                        queue->addEvent(2);
                    }
                    i = i + 1;
                }
                
                //printf("MinCycles: %i MaxCycles: %i\t", minCycles, maxCycles); // for testing
                // adds the memory from those events
                int m = (int)(rand() % (maxMemory + 1 - minMemory)); // gets the range between min and max
                m = m + minMemory; // adds the min to set a minumum number
                memory = memory + m; // adds the memory for this part to the total memory of the process
                //printf("End Num: %i ", m); // for testing
                //printf("\tMin mem: %i Max mem: %i\n", minMemory, maxMemory); // for testing
            }
        }
        fclose(texts); // closes the file
    }
}

// Need to add some functions for what the process does for the events, such as:
// calculateProcess() - just sleeps for one millisecond or doesn't do anything
// inputOutputProcess() - just sleeps and sets the process to wait
// criticalProcess() - asks for critical permission
// criticalEndProcess() - returns the permission for critical it got
// terminateProcess() - has the process return its pid to the queue, should be called when no other events are in queue and before destorying the process

//Note: should return the processes to ready after a function is run

void Process::calculateProcess()
{
    state = 2; // goes into running
    // pretend to do something (does 2 + 2)
    int a = 2;
    int b = 2;
    int c = a + b;

    if (artificialDelays) // adds more delay
    {
        // wait for a period of time
        int msec = 0, trigger = 10; /* 10ms */
        clock_t before = clock();

        do {
            clock_t difference = clock() - before;
            msec = difference * 1000 / CLOCKS_PER_SEC;
        } while ( msec < trigger );
    }
}

void Process::inputOutputProcess()
{
    state = 3; // goes into waiting
    // wait for a period of time
    int msec = 0, trigger = 10; /* 10ms */
    clock_t before = clock();

    do {
        clock_t difference = clock() - before;
        msec = difference * 1000 / CLOCKS_PER_SEC;
    } while ( msec < trigger );
    // sleep(1); // sleeps for 1 second

    if (artificialDelays) // adds more delay
    {
        // wait for a period of time
        int msec = 0, trigger = 20; /* 20ms */
        clock_t before = clock();

        do {
            clock_t difference = clock() - before;
            msec = difference * 1000 / CLOCKS_PER_SEC;
        } while ( msec < trigger );
    }
   
}

int Process::criticalProcess()
{
    // ask for critical permission
    CriticalSection* crit = new CriticalSection();
    int i = crit->criticalWait(); // 0 means couldn't get a space, 1 means got a space
    return i;
}

void Process::criticalEndProcess()
{
    // return critical permission
    CriticalSection* crit = new CriticalSection();
    crit->criticalSignal();
}

void Process::terminateProcess()
{
    state = 4; // is terminating
    // returns it pid
    pQueue->putBackPID(pid);
    // waits to be destroyed by the dispatcher
}

void Process::forkProcess()
{
    Process* tempP = new Process(this); // creates new child process using the fork constructor
    // adds process to the dispatcher
    Dispatcher* dispatchTemp = new Dispatcher();
    dispatchTemp->addProcess(tempP);
    delete(dispatchTemp);
}

// this function gets the process to run its current event and returns which state the process was in at the end of the process
int Process::doEvent()
{
    // reference for state: new = 0, ready = 1, running = 2, waiting = 3, terminating = 4
    // figure out current event
    int e = queue->readCurrentEvent(); // 0 is empty, 1 is calculate, 2 is i/o, 3 is critical, 4 is critical end, 5 is forking
    // do the current event
    if (e == 0) // no events left
    {
        terminateProcess();
    }
    else if (e == 1) // calculate
    {
        queue->popEvent(); // move to next event
        calculateProcess();
    }
    else if (e == 2) // i/o
    {
        queue->popEvent(); // move to next event
        inputOutputProcess();
    }
    else if (e == 3) // critical section start
    {
        int i = criticalProcess();
        if (i) // won't move to the next event if not given permission
        {
            queue->popEvent(); // move to next event
        }
    }
    else if (e == 4) // critical section end
    {
        queue->popEvent(); // move to next event
        criticalEndProcess();
    }
    else if (e == 5) // forking
    {
        queue->popEvent();
        forkProcess();
    }
    // return which event just completed
    return state;
}

#endif