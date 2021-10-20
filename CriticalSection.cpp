// Code for the critical section
// is essentially a binary semaphore

class CriticalSection
{
    static int u = 1; // current number of available users is 1

    public:
        CriticalSection()
        {
            ;
        }

    
    wait() // a basic busy wait
    {
        while (u <= 0) // if no user slot is available, busy waits
        {
            ;
        }
        u--;
    }
    
    signal() // a basic signal to be called when the thing that called wait is finished
    {
        u++;
    }

};