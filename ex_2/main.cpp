#include <iostream>
#include "Network.h"

using namespace std;

int main(int argc, char** argv)
{
    try
    {
        if(argc < 3) throw runtime_error("ERROR: too few program arguments");
        Network n{argc, argv};
        n.run();
    }
    catch(std::runtime_error& e)
    {
        cout<<e.what()<<endl;
    }
    return 0;
}
