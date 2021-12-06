#pragma once
#ifndef MYSEMAPHORE
#define MYSEMAPHORE

class MySemaphore
{
    private:
        int u = 0; // current number of available users

    public:
        MySemaphore(int i); // makes a sempaphore by setting the max number of users
        void wait();
        void signal();
};

// Constructors
MySemaphore::MySemaphore(int i)
{
    u = i;
}
// Should be a CRITICAL section (might not be feasable)
void MySemaphore::wait() // a basic busy wait
{

    while (u <= 0) // if no user slot is available, busy waits
    {
        // Note: could have it not be busy wait by having it return a number if it has permission or not
        // and simply have it go into waiting if it can't get permission
    }
    u--;
}

void MySemaphore::signal() // a basic signal to be called when the thing that called wait is finished
{
    u++;
}

#endif
