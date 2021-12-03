// Class for the MyArrayList
// This class is used to store anything in an array that grows and shrinks according to the number of elements in it
// The base size for this array is 10, and it doesn't shrink below 10, as that could cause a lot of shrinking and growing, which drastically reduces efficiency time wise

#pragma once
#ifndef MYARRAYLIST_CPP
#define MYARRAYLIST_CPP

#include <iostream>

template <typename T>
class MyArrayList
{
    private:
        int size = 10; // stores the current size of the array
        int count = 0; // stores the number of items in the array
        float upperBound = .75f; // the bound for when the array needs to be made bigger
        float lowerBound = .35f; // the bound for when the array needs to be made smaller (doesn't do anything if size is <= 10)
        T** array; // the pointer to the array that stores the pointers

    public:
        MyArrayList(); // the constructor
        int addItem(T* p);
        void removeItem(int i); // takes the index of the process to remove
        void copyElements(T* a[]);
        void growArray();
        void shrinkArray();
        int getCount();
        T* getItem(int i);
        T** getArray();
};

// Contructor
template <typename T>
MyArrayList<T>::MyArrayList()
{
    array = new T*[size]; // starts the MyArrayList at 10 slots
    int i = 0;
    while (i < size)
    {
        array[i] = NULL; // ensures all are null by default
        i = i + 1;
    }
}

// Functions
template <typename T>
int MyArrayList<T>::addItem(T* p) // returns the index the item is in
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
    return (count - 1);
}

template <typename T>
T** MyArrayList<T>::getArray()
{
    return array;
}

template <typename T>
void MyArrayList<T>::removeItem(int i) // removes the item at the index given
{
    // removes the item at the index
    array[i] = NULL;
    // slide the items over to remove empty slots
    T* p = NULL;
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

template <typename T>
void MyArrayList<T>::copyElements(T* a[])
{
    // copy the elements from the current array into the array given
    int i = 0;
    while (i < count)
    {
        a[i] = array[i];
        i = i + 1;
    }
}

template <typename T>
void MyArrayList<T>::growArray()
{
    // create a new process array that is double the size of the current one
    T** temp = new T*[(size * 2)];
    // copy the elements from the current array into the array created
    copyElements(temp);
    // have the current array pointer be moved to that of the new array
    T** temp2 = array; // stores old array pointer
    array = temp;
    // delete the old array
    delete temp2;
    // update size
    size = size * 2;
}

template <typename T>
void MyArrayList<T>::shrinkArray()
{
    // create a new process array that is half the size of the current one
    T** temp = new T*[(size / 2)];
    // copy the elements from the current array into the array created
    copyElements(temp);
    // have the current array pointer be moved to that of the new array
    T** temp2 = array; // stores old array pointer
    array = temp;
    // delete the old array
    delete temp2;
    // update size
    size = size / 2;
}

template <typename T>
int MyArrayList<T>::getCount()
{
    return count;
}

template <typename T>
T* MyArrayList<T>::getItem(int i)
{
    return array[i];
}
#endif