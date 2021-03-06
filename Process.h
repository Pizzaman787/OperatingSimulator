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
        int mailbox = -1; // number represents a message, like 1 is another process is wrapping up while -1 is empty
        EventQueue* queue;
        Process *parent = NULL;
        time_t timeCreated = 0; // stores the time when the process was created
        void calculateProcess();
        void inputOutputProcess();
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
        bool processMemory[1024]; // shows what megabytes are in use by this process (this is public to make memory assignment easier)
        void addEvents(char* name);
        int doEvent();
        int readCurrentEvent();
        EventNode* getCurrentEvent();
        void terminateProcess();
        void giveMail(int i);
};

#endif