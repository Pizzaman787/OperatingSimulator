# OperatingSimulator
A project for CMSC 312

This is a project to create a program that "simulates" the running and management of processes like an operating system would.

Current Notes:
- Critical section only supports one thing in it at a time
- Critical section use returns whether a slot is available (returns an int, but could also return a bool)
- Created in C and C++ (Only properly tested on linux systems (Ubuntu 20.04LTS))
- Only supports templates that have lines and names less than 1024 characters
- Only supports input in the file that is less than 1024 characters or less
- The Process class acts as a PCB
- Status currently can't show processes terminating and running, as they finish or change before status can show them
- Technically a really low chance the I/O interrupt prevents the status command