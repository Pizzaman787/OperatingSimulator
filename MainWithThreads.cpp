// Programmer: Christopher DeVore
// Date: 10-19-21
// Program: Main function for running the simulator

// ensures the imports or includes are only included once
//#pragma once

// NOTES ON INCLUSION:
// - Include headers, then include the cpp files here after them, as this gives the definitions for everything in the final compiled file

#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include "Process.h"
#include "Dispatcher.h"
#include "Process.cpp"
#include "Dispatcher.cpp"
#include "Support.cpp"
#include "MyArrayList.cpp"


using namespace std;

bool pause = 0;
bool alwaysStatus = 0;
bool randomIOinterrupts = 1; // for causing random interrupts in the threads (0 is off and 1 is on)
char inputString[1024] = ""; // string for storing input
int threadCount = 0; // stores the number of threads
MyArrayList<int>* threadInstruction = new MyArrayList<int>(); // ArrayList for thread instruction where 0 is empty, 1 is in use/resting, 2 is for print status, 3 is for exit, 4 is for random I/O interrupt

// forward declarations:
void* runProgram(void* sched); 
void* inputProgram(void* thing);
int getThreadSlot();
// FIX THREADS TO USE threadInstruction array rather than threadKiller and thread count 
//(have threadcount update as new ones are added or removed and have them check for an open slot in threadInstruction, maybe via a function or something)


// Main function
int main(int argc, char* argv[])
{
    /*
    // sets up kernel (asks number of threads)

    // looping system
    while(1)
    {
        // begin a cycle
        
        // Checks if there has been input from the user

        // processes input or just performs the cycle (create processes and stuff)
            // to run a cycle, have the distributor run the processes it gets in the list from the RoundRobin

        // prepare to run again (end the cycle)

        // display UI update if UI is wanted

    }
    */

    srand(time(0)); // seeds rand
    
    // creates input thread
    pthread_t inputThread;
    if (pthread_create(&inputThread, NULL, inputProgram, NULL)) // creates the thread and error checks
    {
        printf("Error: Input thread creation failed. Exiting program.\n");
        exit(-1);
    }

    // creates a running thread (should change to make threads based off a number
    pthread_t runThread;
    int* tempSched;
    *tempSched = 0; // round robin scheduler
    if (pthread_create(&inputThread, NULL, runProgram, (void*)tempSched)) // creates the thread and error checks
    {
        printf("Error: Running thread creation failed. Exiting program.\n");
        exit(-1);
    }

    printf("-System Started-\n"); // prints notification that the program is ready for input

    while(1) // main just checks for input
    {
        //gets(inputString); // should be under 1024 characters, else will go out of bounds
        cin >> inputString;
        removeEndings(inputString);
        // has main exit when exit is called (the thread created exits seperately as it reads the input)
        if (strcmp(inputString, "exit") == 0)
        {
            //pthread_exit(NULL); // closes the thread (or the main thread in this case)
            printf("Exiting...\n");
            exit(0); // closes program and threads
        }
    }

    
    return 0;
}

// thread function for running
void* runProgram(void* sched)
{
    int thread = getThreadSlot(); // grabs what number of running thread this is
    threadCount = threadCount + 1; // increases thread count

    Dispatcher* dispatch = new Dispatcher(*((int*)sched)); // creates the dispatcher using the argument to determine the sheduler

    while (1)
    {
        // runs the program if not pausesd
        if (!pause)
        {
            dispatch->doStuff();
            if (alwaysStatus)
            {
                printf("Thread: %i\n", thread);
                dispatch->processesStatus(); // for testing
            }

            // Thread instructions
            if (*(threadInstruction->getItem(thread)) == 2) // if told to print status
            {
                printf("Thread: %i\n", thread);
                dispatch->processesStatus();
                *(threadInstruction->getItem(thread)) = 1; // sets back to rest
            }
            else if (*(threadInstruction->getItem(thread)) == 3) // if told to exit
            {
                *(threadInstruction->getItem(thread)) = 0; // resets the thread killer
                pthread_exit(NULL); // closes the thread
            }
            else if (*(threadInstruction->getItem(thread)) == 4) // random I/O interrupt
            {
                int msec = 0, trigger = 10; /* 10ms */
                clock_t before = clock();

                do {
                    clock_t difference = clock() - before;
                    msec = difference * 1000 / CLOCKS_PER_SEC;
                } while ( msec < trigger );

                *(threadInstruction->getItem(thread)) = 1;
            }
        }
    }
}
// thread function for input
void* inputProgram(void* thing)
{
    int thread = getThreadSlot(); // grabs what number of running thread this is
    threadCount = threadCount + 1; // increases thread count

    char* input = inputString; // converts input pointer

    Dispatcher* dispatch = new Dispatcher(); // creates a dispatcher for use of process creation
    
    // loops reading input
    while(*(threadInstruction->getItem(thread)) != 3) // goes until told to exit
    {
        // checks if it should cause a random I/O interrupt
        // would put in main, but main is checking for input, so it pauses when input isn't being given
        if (randomIOinterrupts)
        {
            if ((int)(rand() % 51231241) == 0) // some really low chance
            {
                //printf("I/O interrupt...\n"); // for TESTING
                int l = 0;
                while (l < threadInstruction->getCount())
                {
                    *(threadInstruction->getItem(l)) = 4; // sets them to be interrupted
                    l = l + 1;
                }
            }
        }

        // Thread Instructions 
        if (*(threadInstruction->getItem(thread)) == 2) // input thread told to check status
        {
            *(threadInstruction->getItem(thread)) = 1; // just moves it back to idle, as the input thread doesn't show status
        }
        else if (*(threadInstruction->getItem(thread)) == 4) // random I/O interrupt
        {
            //printf("I/O interrupt1...\n"); // for TESTING
            int msec = 0, trigger = 10; /* 10ms */
            clock_t before = clock();

            do {
                clock_t difference = clock() - before;
                msec = difference * 1000 / CLOCKS_PER_SEC;
            } while ( msec < trigger );

            *(threadInstruction->getItem(thread)) = 1;
            //printf("I/O interrupt2...\n"); // for TESTING
        }

        // Managing input
        if (strcmp(input, "status") == 0)
        {
            //dispatch->processesStatus();
            // sets all the threads to print their status
            int i = 0;
            while (i < threadInstruction->getCount())
            {
                *(threadInstruction->getItem(i)) = 2;
                i = i + 1;
            }
            strcpy(input, ""); // clears input
        }
        else if (strcmp(input, "alwaysStatus") == 0)
        {
            alwaysStatus = !alwaysStatus;
            strcpy(input, ""); // clears input
        }
        else if (strcmp(input, "exit") == 0)
        {
            //strcpy(input, ""); // clears input (Doesn't do this, as there is a slim chance it does it before main has a chance of closing)
            // actually exiting is determined in main, so it doesn't actually do anything in here
        }
        // this command breaks due to a bug with %, as it will randomly throw a Floating Point Exception, though no divide by zero or floats are used
        else if (strcmp(input, "random") == 0) // adds a number of random processes from the templates
        {
            //fgets(input, 1024, inputFile); // puts the second line into the input
            strcpy(input, ""); // clears input
            printf("Awaiting number of processes: \n");
            bool safeExit = 0;
            while(!safeExit) // created for the idea of having more than one if check, but only one in it right now
            {
                if(strlen(input) > 0) // ensures something is in input
                {
                    safeExit = 1;
                }
                // idea: can have it check to ensure it is a number before allowing exit
            }
            removeEndings(input);
            int num = atoi(input);
            int randI = 0;
            while (randI < num)
            {
                //int randJ = (rand() % 5); // gets the range between 0 and 5 (it cannot get 5)
                int randJ = (int)(rand() % 5);
                if (randJ == 0)
                {
                    char tempS[] = "Basic2plus2";
                    dispatch->addProcess(new Process(tempS));
                }
                else if (randJ == 1)
                {
                    char tempS[] = "CriticalStuff";
                    dispatch->addProcess(new Process(tempS));
                }
                else if (randJ == 2)
                {
                    char tempS[] = "LaunchBigProgram";
                    dispatch->addProcess(new Process(tempS));
                }
                else if (randJ == 3)
                {
                    char tempS[] = "LaunchSimpleProgram";
                    dispatch->addProcess(new Process(tempS));
                }
                else if (randJ == 4)
                {
                    char tempS[] = "RapidKeyboardInputs";
                    dispatch->addProcess(new Process(tempS));
                }
                else // defaults 
                {
                    char tempS[] = "Basic2plus2";
                    dispatch->addProcess(new Process(tempS));
                }

                randI = randI + 1;
            }
            printf("Processes added using random command.\n");
            strcpy(input, ""); // clears input
        }
        else if (strcmp(input, "pause") == 0)
        {
            pause = !pause; // sets pause to whatever it isn't
            printf("Toggled pause\n");
            strcpy(input, ""); // clears input
        }
        else // tries to find the program template to load
        {
            if (strcmp(input, "") != 0) // if the input isn't empty, tries making a template
            {
                Process* p = new Process(input);
                if (p->getSize() > 0) // ensures it is a functional template
                {
                    dispatch->addProcess(p);
                    printf("Template process added\n");
                    strcpy(input, ""); // clears input
                }
                else
                {
                    delete p; // deletes p if it wasn't added to the process list
                }
            }
        }
    }
    if (showDeletes)
        printf("Deleting Dispatcher: Main\n"); // for TESTING
    delete dispatch; // deletes the dispatcher
    *(threadInstruction->getItem(thread)) = 0; // resets the thread instruction to empty
    pthread_exit(NULL); // closes the thread
}

int getThreadSlot()
{
    int* j = new int; // have to add to memory or else the primitive is removed when this function finishes running
    *j = 1;
    int i = threadInstruction->addItem(j);
    return i;
}