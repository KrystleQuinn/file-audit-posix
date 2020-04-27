# file-audit-posix
This repository contains a simple program that will audit the status of a directory's contents.  

This program logs file status by running a series of system calls.  The system calls come from sys/stat.h header file.  I used POSIX signal processing to define how long the program executes.  The program loops continuously until user input is signaled via ctrl+c; it does not run as a background process.  To iterate through the directory and log file permissions I used C++ Filesystem library.  I used basic multithreading to simulate a timer to record log entries at regular intervals.  This program was written as an excerise and is not meant to be used otherwise.  For production purposes it is recommended to use an API from the OS.

I compiled this program using the GCC version 9.3.0_1 compiler (g++-9) on macOS.  I used CLion as my IDE.  CLion uses CMake and this program uses CXX_STANDARD 20. 

I have included a release as well. 


