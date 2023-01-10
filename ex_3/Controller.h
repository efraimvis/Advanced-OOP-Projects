//
// Created by Admin on 7/12/2022.
//

#ifndef HW_3_CONTROLLER_H
#define HW_3_CONTROLLER_H
#include "Model.h"

class Controller
{
    friend class Model;
private:

public:

    /*Constructor
     *
     * argc, argv - program arguments*/
    Controller(int argc, const char** argv);

    /*Destructor*/
    ~Controller(){}

    Controller(const Controller& rhs) = delete;
    Controller(Controller&& rhs) = delete;
    Controller& operator=(const Controller& rhs) = delete;
    Controller& operator=(Controller&& rhs) = delete;

    /*Runs controller (and by extension simulation)*/
    void runController();
};


#endif //HW_3_CONTROLLER_H
