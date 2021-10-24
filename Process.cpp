// Class for a process

// will need to store PID, current state, priority, Memory assigned to it, events to do (Like calculate or await I/O input), parent process, and time when the process was created

/* for the events, could use a queue that duplicates the event according to the number assigned to it.
* An example would be, if it needed to do 3 cycles of Calculate, then add three calculates to the queue
*/

class Process
{
    static PidQueue pQueue = new PidQueue();
    int pid = 0; // current number assigned to process
    int state = 0; // new = 0, ready = 1, running = 2, waiting = 3, terminating = 4
    int priority = 0; // higher is more prioritized
    EventQueue queue;
    // MemoryArray // an array for the memory assigned to this process
    Process *parent = NULL;
    time_t timeCreated = 0; // stores the time when the process was created

    public:
        Process() // creation without a parent process
        {
            pid = pQueue.requestPID(); // gets a unique PID
            timeCreated = time(NULL); // stores current time in seconds
            queue = new EventQueue();
        }

        Process(String s) // creation without a parent process, but with a template for the events
        {
            pid = pQueue.requestPID(); // gets a unique PID
            timeCreated = time(NULL); // stores current time in seconds
            queue = new EventQueue();
            addEvents(s);
        }

        Process(int pr) // creation without a parent process, but sets priority
        {
            pid = pQueue.requestPID(); // gets a unique PID
            timeCreated = time(NULL); // stores current time in seconds
            queue = new EventQueue();
            priority = pr;
        }

        Process(Process p) // creates a process with a parent
        {
            pid = pQueue.requestPID(); // gets a unique PID
            timeCreated = time(NULL); // stores current time in seconds
            queue = new EventQueue();
            parent = &p;
        }

        Process(Process p, int pr) // creates a process with a parent and sets priority
        {
            pid = pQueue.requestPID(); // gets a unique PID
            timeCreated = time(NULL); // stores current time in seconds
            queue = new EventQueue();
            parent = &p;
            priority = pr;
        }


    
    setState(int i)
    {
        state = i; // new = 0, ready = 1, running = 2, waiting = 3, terminating = 4
    }

    setPriority(int i)
    {
        priority = i;
    }

    addEvents(String s)
    {
        // searches for the template with a name that matches the string
        
        // reads the template and adds the events to the event queue
    }
};