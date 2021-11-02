// This is the class for the RoundRobin algorithm
// This code acts as the short term schedular for the distributor

#pragma once

class RoundRobin
{
    private:
        int q = 10; // the number of milliseconds allowed per process per cycle
        int n = 0; // the number of processes given to the round robin algorithm
        // each process gets 1/n cpu time, but no more than q, so whichever is lower
    

    public:
        RoundRobin();
        ProcessArray* giveProcesses(ProcessArray* p);

};

// Constructors
RoundRobin::RoundRobin()
{
    ;
}

// Functions
void giveProcesses(ProcessArray* p) // takes an arraylist of processes to sort
{
    // sorts the processes

}