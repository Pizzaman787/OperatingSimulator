// Code for the critical section
// is essentially a binary semaphore

#pragma once
#ifndef CRITICALSECTION
#define CRITICALSECTION

class CriticalSection
{
    private:
        static int u; // current number of available users is 1

    public:
        CriticalSection();
        int criticalWait();
        void criticalSignal();

};
// forced to initialize static outside of class
int CriticalSection::u = 1;

// constructor
CriticalSection::CriticalSection()
{
    ;
}

// functions
    
int CriticalSection::criticalWait() // a basic busy wait
{
    /*
    while (u <= 0) // if no user slot is available, busy waits
    {
        ; // Note: could have it not be busy wait by having it return a number if it has permission or not
        // and simply have it go into waiting if it can't get permission
    }
    u--;
    */
    if (u <= 0)
    {
        return 0; // 0 means false, as in there isn't space
    }
    else
    {
        u--;
        return 1;
    }
}
    
void CriticalSection::criticalSignal() // a basic signal to be called when the thing that called wait is finished
{
    u++;
}
#endif