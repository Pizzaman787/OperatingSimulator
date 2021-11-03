// Programmer: Christopher DeVore
// Date: 10-19-21
// Program: Main function for running the simulator

// ensures the imports or includes are only included once
//#pragma once

#include <cstdio>
#include <iostream>
#include <stdlib.h>

#include "Support.cpp"
#include "Process.cpp"
#include "Dispatcher.cpp"

//function
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
    printf("-System Started-\n");

    Dispatcher* dispatch = new Dispatcher(0, 1); // creates the dispatcher using shortest job first and one thread
    srand(time(NULL)); // seeds rand

    // for testing I add a process
    //dispatch->addProcess(new Process("Basic2plus2"));

    bool pause = 0;
    bool alwaysStatus = 0;
    while(1)
    {
        // scan for input (Creation of a process or status check)
        FILE* inputFile;
        inputFile = fopen("./input.txt", "r"); // opens the file to read
        char input[1024] = "";
        if (inputFile != NULL)
        {
            fgets(input, 1024, inputFile); // puts the first line into the input
        }
        else
        {
            // fclose(inputFile); // closes the file that failed to open
            inputFile = fopen("./input.txt", "w"); // opens the file to write to make a blank input file
        }
        // process input
        removeEndings(input);
        //printf("Input: %s\n", input);
        if (strcmp(input, "status") == 0)
        {
            dispatch->processesStatus();
        }
        else if (strcmp(input, "alwaysStatus") == 0)
        {
            alwaysStatus = !alwaysStatus;
        }
        else if (strcmp(input, "exit") == 0)
        {
            printf("Exiting...\n");
            freopen("./input.txt", "w", inputFile); // reopens the file in writing only to overwrite anything in the file (clears the file)
            fclose(inputFile); // closes the input file
            break;
        }
        // this command breaks due to a bug with %, as it will randomly throw a Floating Point Exception, though no divide by zero or floats are used
        else if (strcmp(input, "random") == 0) // adds a number of random processes from the templates
        {
            fgets(input, 1024, inputFile); // puts the second line into the input
            removeEndings(input);
            int num = atoi(input);
            int randI = 0;
            while (randI < num)
            {
                //int randJ = (rand() % 5); // gets the range between 0 and 5 (it cannot get 5)
                int randJ = rand();
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
        }
        else if (strcmp(input, "pause") == 0)
        {
            pause = !pause; // sets pause to whatever it isn't
            printf("Toggled pause\n");
        }
        else // tries to find the program template to load
        {
            if (strcmp(input, "") != 0) // if the input isn't empty
            {
                Process* p = new Process(input);
                if (p->getSize() > 0) // ensures it is a functional template
                {
                    dispatch->addProcess(p);
                    printf("Template process added\n");
                }
            }
        }
        freopen("./input.txt", "w", inputFile); // reopens the file in writing only to overwrite anything in the file (clears the file)
        fclose(inputFile); // closes the input file
        // the dispatcher does stuff if not paused
        if (!pause)
        {
            dispatch->doStuff();
            if (alwaysStatus)
            {
                dispatch->processesStatus(); // for testing
            }
        }
        //printf("One cycle done.\n"); 
    }
    delete dispatch; // deletes the dispatcher
    return 0;

    // for testing
    /*
    Process* pc[10];
    int pSize = 0;
    while(1)
    {
        // scan for input (Creation of a process or status check)
        char input[1024];
        printf("Waiting for input: ");
        std::cin>>input; // takes user input and sticks it in the input array
        // process input
        removeEndings(input);
        if (strcmp(input, "status") == 0)
        {
            int i = 0;
            while(i < pSize)
            {
                pc[i]->printStatus();
                i = i + 1;
            }
        }
        else if (strcmp(input, "exit") == 0)
        {
            break;
        }
        else
        {
            pc[pSize] = new Process(input);
            pSize = pSize + 1;
        }
        printf("One cycle done.\n"); 
    }
    return 0;
    */
}