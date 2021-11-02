// Programmer: Christopher DeVore
// Date: 10-19-21
// Program: Main function for running the simulator

// ensures the imports or includes are only included once
//#pragma once

#include <cstdio>
#include <iostream>

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
    printf("System Started\n");

    Dispatcher* dispatch = new Dispatcher(0, 1); // creates the dispatcher using shortest job first and one thread

    // for testing I add a process
    //dispatch->addProcess(new Process("Basic2plus2"));

    bool pause = 0;
    while(1)
    {
        // scan for input (Creation of a process or status check)
        FILE* inputFile;
        inputFile = fopen("./input.txt", "r"); // opens the file to read
        char input[1024] = "";
        if (inputFile != NULL)
        {
            fgets(input, 1024, inputFile); // puts the first line into the input
            freopen("./input.txt", "w", inputFile); // reopens the file in writing only to overwrite anything in the file (clears the file)
            fclose(inputFile); // closes the file
        }
        else
        {
            // fclose(inputFile); // closes the file that failed to open
            inputFile = fopen("./input.txt", "w"); // opens the file to write to make a blank input file
            fclose(inputFile); // closes the input file
        }
        // process input
        removeEndings(input);
        if (strcmp(input, "status") == 0)
        {
            dispatch->processesStatus();
        }
        else if (strcmp(input, "exit") == 0)
        {
            break;
        }
        else if (strcmp(input, "pause") == 0)
        {
            pause = !pause; // sets pause to whatever it isn't
        }
        else // tries to find the program template to load
        {
            if (strcmp(input, "") != 0) // if the input isn't empty
            {
                Process* p = new Process(input);
                dispatch->addProcess(p);
            }
        }
        // the dispatcher does stuff if not paused
        if (!pause)
        {
            dispatch->doStuff();
        }
        //printf("One cycle done.\n"); 
        //dispatch->processesStatus(); // for testing
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