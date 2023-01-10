#include <iostream>
#include "Simulation.h"

/*Performs simulation of lifecycle of population of Corona Viruses*/
int main(int argc, char** argv)
{
    try
    {
        if(argc != 3)
        {
            std::cerr<<"Error: invalid program parameters"<<std::endl;
            return 1;
        }
        Simulation s = Simulation(argv[1], argv[2]);
        s.run();
    }
    catch(std::exception& e)
    {
    }
    return 0;
}
