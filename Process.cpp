// Class for a process

// will need to store PID, current state, priority, Memory assigned to it, events to do (Like calculate or await I/O input), parent process, and time when the process was created

/* for the events, could use a queue that duplicates the event according to the number assigned to it.
* An example would be, if it needed to do 3 cycles of Calculate, then add three calculates to the queue
*/

#pragma once

#include "Support.cpp"
#include "EventQueue.cpp"
#include "PidQueue.cpp"
#include <time.h>
#include <cstdio>
#include <stdlib.h>

class Process
{
    private:
        static PidQueue* pQueue;
        int pid = 0; // current number assigned to process
        int state = 0; // new = 0, ready = 1, running = 2, waiting = 3, terminating = 4
        int priority = 0; // higher is more prioritized
        EventQueue* queue;
        // MemoryArray // an array for the memory assigned to this process
        Process *parent = NULL;
        time_t timeCreated = 0; // stores the time when the process was created

    public:
        Process(); // the constructor
        Process(char* s); // other constructor
        void setPriority(int i);
        void printStatus();
        void addEvents(char* name);
};
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

// Functions
void Process::setPriority(int i)
{
    priority = i;
}

void Process::printStatus() // for testing purposes
{
    printf("PID: %i\t", pid);
    printf("State: %i\t", state);
    printf("Event: %i\t", queue->readCurrentEvent());
    printf("Priority: %i\t", priority);
    // could add time created
    // could add parent
    printf("\n");
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
        printf("Unable to open requested template.\n"); // doesn't do anything if the template doesn't exist other than notify of bad input
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
            //int minMemory = 0;
            //int maxMemory = 0;
            while(strlen(read) > 0)
            {
                // check if current char is a tab, if it is, remove it
                if (read[0] == '\t')
                {
                    //read = read + 1; // moves the pointer over by 1 character
                    if (read[1] != '\t')
                        strcpy(read, strchr(read, read[1])); // Removes the tab at the beginning
                    else
                        strcpy(read, strchr(read, read[2])); // Removes the two tabs at the beginning
                }
                // else if the event hasn't been found, grab it and put it in event. Then remove said word from read
                else if (!eventFound)
                {
                    char* temp = strchr(read, '\t');
                    if (temp == NULL) // checks if there wans't a tab
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
                        strcpy(read, strchr(read, *temp)); // removes the word from the read string

                        minCycles = atoi(&temp2[0]);
                        minCyclesFound = true;
                    }
                    else
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
                        strcpy(read, strchr(read, *temp)); // removes the word from the read string

                        maxCycles = atoi(&temp2[0]);
                    }
                    //strcpy(read, strchr(read, read[1])); // Removes the tab at the beginning
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
            // generate the number of cycles and add the events to the queue
            else
            {
                int j = rand() % (maxCycles + 1 - minCycles);
                j = j + minCycles;
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
            }
        }
    }
    fclose(texts); // closes the file
}

// Need to add some functions for what the process does for the events, such as:
    // calculateProcess() - just sleeps for one millisecond or doesn't do anything
    // inputOutputProcess() - just sleeps and sets the process to wait
    // criticalProcess() - asks for critical permission
    // criticalEndProcess() - returns the permission for critical it got
    // terminateProcess() - has the process return its pid to the queue, should be called when no other events are in queue and before destorying the process
