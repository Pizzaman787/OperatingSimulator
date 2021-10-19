class MySemaphore
{
    int u = 0; // current number of available users

    public:
        MySemaphore(int i) // makes a sempaphore by setting the max number of users
        {
            u = i;
        }

    // Should be a CRITICAL section
    wait() // a basic busy wait
    {
        while (u <= 0) // if no user slot is available, busy waits
        {
            ;
        }
        u--; // 
    }
    // Should be a CRITICAL section
    signal() // a basic signal
    {
        u++;
    }

};