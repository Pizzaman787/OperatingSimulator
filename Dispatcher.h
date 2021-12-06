// header for Dispatcher.cpp
#pragma once
#ifndef DISPATCHER_H
#define DISPATCHER_H

//#include "ProcessArray.cpp"
class ProcessArray; // forward declaration

class Dispatcher
{
    // variables
    private:
        static ProcessArray* pNeedsSorting; // stores all processes that need to be distributed
        static bool memArray[1024]; // the memory for the system is 1024 megabytes
        static int availableMemory;
        ProcessArray* pReady;
        ProcessArray* pWait;
        void swapArray(int i, int j, int k);
        void shortestJobFirst(Process* p[], int s);
        void readyProcesses();
        void moveReadyProcesses();
        void removeProcess(int i, bool j);
        int sched = 0; // defaults to RoundRobin

    public:
        Dispatcher(int s); // the constructor
        Dispatcher(); // default contructor
        void addProcess(Process* m);
        void processesStatus();
        int totalProcesses();
        void doStuff();
        //void shortestJobFirst(Process* p[], int s);
};

#endif