#include <iostream>
#include "PCalc_SP.h"


PCalc_SP::PCalc_SP(unsigned int array_size) : PCalc(array_size)
{

}
void PCalc_SP::markNonPrimes()
{

    //When we init PCalc, we get an array of bools set to true- we just need to go through and set them false


    for(int i = 2; i < this->array_size(); i++)
    {
        
        //Find the next-lowest prime X (starting at 2)
        auto isPrime = this->at(i);
        if(isPrime) //If this values is true (which means this number is prime),
        {
            //std::cout << i << "is prime.\n";
            //Mark all multiples of this number as false (not prime)
            for(int m = (2*i); m < this->array_size(); m = i + m)
            {
                //std::cout << m << " is therefore NOT prime.\n";
                this->at(m) = false;
            }
        }
        else
        {
            //This number is not a prime- find the next one
            continue;
        }
    }
    
    
    return;
}


void PCalc_SP::cleanup()
{
    return;
}



