#include <iostream>
#include <vector>
#include "Model.h"
#include "Controller.h"
using namespace std;

/*Runs program*/
int main(const int argc, const char** argv)
{
    try
    {
        Controller c(argc, argv);
        c.runController();
    }
    catch(std::invalid_argument& e)
    {
        cout<<e.what()<<endl;
    }
    catch(std::runtime_error& re)
    {
        cout<<re.what()<<endl;
    }
    return 0;
}
