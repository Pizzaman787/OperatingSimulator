// Class for the ProcessArray
// This class is used to store the processes for the distrubutor and roundrobin

#pragma once
#ifndef PROCESSARRAY
#define PROCESSARRAY

#include "Process.h"

class ProcessArray
{
    private:
        int size = 10; // stores the current size of the array
        int count = 0; // stores the number of items in the array
        float upperBound = .75f; // the bound for when the array needs to be made bigger
        float lowerBound = .35f; // the bound for when the array needs to be made smaller (doesn't do anything if size is <= 10)
        Process** array; // the pointer to the array that stores the pointers

    public:
        ProcessArray(); // the constructor
        void addItem(Process* p);
        void removeItem(int i); // takes the index of the process to remove
        void copyElements(Process* a[]);
        void growArray();
        void shrinkArray();
        int getCount();
        Process* getProcess(int i);
        Process** getArray();
};

// Contructor
ProcessArray::ProcessArray()
{
    array = new Process*[size]; // starts the arraylist at 10 slots
    int i = 0;
    while (i < size)
    {
        array[i] = NULL; // ensures all are null by default
        i = i + 1;
    }
}

// Functions
void ProcessArray::addItem(Process* p)
{
    // add the process to the first empty slot
    array[count] = p;
    // increments count
    count = count + 1;
    // if count is above the upperbound, increase the array size
    if (((float)count / (float)size) >= upperBound)
    {
        growArray();
    }
}

Process** ProcessArray::getArray()
{
    return array;
}

void ProcessArray::removeItem(int i) // removes the item at the index given
{
    // removes the item at the index
    array[i] = NULL;
    // slide the items over to remove empty slots
    Process* p = NULL;
    int j = size - 1;
    while (j > 0)
    {
        if (array[j] != NULL)
        {
            p = array[j];
            if (j > 0)
            {
                if (array[j - 1] == NULL) // checks if the slot before it is empty
                {
                    array[j - 1] = p; // moves it over
                    array[j] == NULL;
                }
            }
        }
        j = j - 1;
    }
    // decrements count
    count = count - 1;
    // if the count is below the lower bound, decrease array size
    if (((float)count / (float)size) < lowerBound && size > 10) // won't shrink if not above 10 to prevent really tiny array
    {
        shrinkArray();
    }
}

void ProcessArray::copyElements(Process* a[])
{
    // copy the elements from the current array into the array given
    int i = 0;
    while (i < count)
    {
        a[i] = array[i];
        i = i + 1;
    }
}

void ProcessArray::growArray()
{
    // create a new process array that is double the size of the current one
    Process** temp = new Process*[(size * 2)];
    // copy the elements from the current array into the array created
    copyElements(temp);
    // have the current array pointer be moved to that of the new array
    Process** temp2 = array; // stores old array pointer
    array = temp;
    // delete the old array
    if (showDeletes)
        printf("Deleting Old Array: ProcessArray grow\n"); // for TESTING
    delete temp2;
    // update size
    size = size * 2;
}

void ProcessArray::shrinkArray()
{
    // create a new process array that is half the size of the current one
    Process** temp = new Process*[(size / 2)];
    // copy the elements from the current array into the array created
    copyElements(temp);
    // have the current array pointer be moved to that of the new array
    Process** temp2 = array; // stores old array pointer
    array = temp;
    // delete the old array
    if (showDeletes)
        printf("Deleting Old Array: ProcessArray shrink\n"); // for TESTING
    delete temp2;
    // update size
    size = size / 2;
}

int ProcessArray::getCount()
{
    return count;
}

Process* ProcessArray::getProcess(int i)
{
    return array[i];
}

#endif