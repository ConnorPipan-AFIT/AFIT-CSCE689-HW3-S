#include <vector>
#include <thread>
#include <iostream>
#include <mutex>

#include "PCalc_T.h"

std::mutex mutex;

PCalc_T::PCalc_T(unsigned int array_size, unsigned int num_threads) : PCalc(array_size)
{
    ThreadCount = num_threads;
}


void PCalc_T::markNonPrimes()
{
    //Idea:
    //1. We have one thread which finds the next prime number.
    //When it finds one, it creates (numthreads) threads, which each individually mark the composites
    //So if you have 2 threads and you're marking composites of 3, you get
    //T1    T2
    //----------
    //3     6
    //9     12
    //15    18

    //etc

    //Mark the twos first so we're not repeating any work

    //We start by creating our threads
    std::vector<std::thread> threadVector;
    for(int t = 1; t <= ThreadCount; t++)
        {
            //Assign work per thread
            int starting_value = 2 + (t*2);
            //std::cout << "Thread " << t << " is starting " << 2 << "at value " << starting_value << "\n";
            std::thread worker_thread(&PCalc_T::markCompositeThread, this, 2, starting_value);
            threadVector.push_back(std::move(worker_thread));
        }
        
        //(Note: we actually end up using num_threads + 1 threads here- so if we want to work using
        //4 threads, we actually have 5 running. This may affect performance for whichever poor thread gets scheduled on the same core
        //as the main one/OS, but since our main thread is doing nothing I don't think the performance hit will be too bad)

        //Wait for the threads to finish
        for(std::thread &thread : threadVector)
        {
            thread.join();
        }
        //threadVector.clear();


    //Look for our next prime number
    for(int i = 3; i < this->array_size(); i += 2)
    {
        //If this number appears to be prime,
        if(this->at(i))
        {
            //Sieve our multiples/composites
            int t = 1;
            for(auto &thread : threadVector)
            {
                int starting_value = i + (t*i);
                //std::cout << "Thread " << t << " is starting " << i << "at value " << starting_value << "\n";
                //Reuse our threads and do new work
                thread = std::thread(&PCalc_T::markCompositeThread, this, i, starting_value);
                t = t + 1;
            }

            //Wait for the threads to finish
            for(std::thread &thread : threadVector)
            {
                thread.join();
            }
            //threadVector.clear();

              
        }
        //If this number appears to be composite, do nothing and check the next number
        else { continue; } 
    }
    return;
}

void PCalc_T::markCompositeThread(const int prime, const int starting_value)
{
    //Marks numbers composite- designed to work with other threads
    //So instead of checking every multiple of prime P (2P, 3P, 4P etc) we check every (number of threads)-th

    unsigned int arraySize = this->array_size();

    for(unsigned int m = starting_value; m < arraySize; m = m + (prime * ThreadCount))
    {
        this->at(m) = false;
    }

    return;
}


void PCalc_T::cleanup()
{
    return;
}