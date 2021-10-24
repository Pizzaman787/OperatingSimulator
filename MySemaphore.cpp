class MySemaphore
{
    int u = 0; // current number of available users

    public:
        MySemaphore(int i) // makes a sempaphore by setting the max number of users
        {
            u = i;
        }

    // Should be a CRITICAL section (might not be feasable)
    wait() // a basic busy wait (Is a CRITICAL process)
    {
        CriticalSection.wait(); // starts a critical section
        while (u <= 0) // if no user slot is available, busy waits
        {
            ;
        }
        u--;
        CriticalSection.signal(); // says is done with critical section
    }

    signal() // a basic signal (Is a CRITICAL process)
    {
        CriticalSection.wait(); // starts a critical section
        u++;
        CriticalSection.signal(); // says is done with critical section
    }

};