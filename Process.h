// header class for Process.cpp
#pragma once
#ifndef PROCESS_H
#define PROCESS_H

#include "EventQueue.cpp"
#include "PidQueue.cpp"
#include <time.h>

class Process
{
    private:
        static PidQueue* pQueue;
        int pid = 0; // current number assigned to process
        int state = 0; // new = 0, ready = 1, running = 2, waiting = 3, terminating = 4
        int priority = 0; // higher is more prioritized
        int memory = 0; // stores the memory of the process
        EventQueue* queue;
        // MemoryArray // an array for the memory assigned to this process
        Process *parent = NULL;
        time_t timeCreated = 0; // stores the time when the process was created
        void calculateProcess();
        void inputOutputProcess();
        void terminateProcess();
        int criticalProcess();
        void criticalEndProcess();
        void forkProcess();

    public:
        Process(); // the constructor
        Process(char* s); // other constructor for using a template
        Process(char* s, int i); // template and priority
        Process(Process* p); // forking constructor
        void setPriority(int i);
        int getPriority();
        void setState(int i);
        int getState();
        void printStatus();
        int getSize();
        int getMemory();
        void addEvents(char* name);
        int doEvent();
        int readCurrentEvent();
        EventNode* getCurrentEvent();
};

#endif