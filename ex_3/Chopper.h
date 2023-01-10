//
// Created by Admin on 7/12/2022.
//

#ifndef HW_3_CHOPPER_H
#define HW_3_CHOPPER_H
#include "Vehicle.h"

class Chopper:public Vehicle
{
    friend class Model;
private:
    int attackRange;
    void setSpeed(int newSpeed);

public:

    /*Constructor
     *
     * name - chopper name
     * startLocation - chopper's start location*/
    Chopper(std::string name, std::pair<float, float> startLocation);

    /*Destructor*/
    ~Chopper(){};

    Chopper(const Chopper& rhs) = delete;
    Chopper(Chopper&& rhs) = delete;
    const Chopper& operator=(const Chopper& rhs) = delete;
    const Chopper& operator=(Chopper&& rhs) = delete;

    void setCourse(int newCourse, int newSpeed);
    void setDestination(std::pair<float, float> newDestination, int newSpeed);

    /*Increments attack range*/
    void incAttackRange();

    /*Decrements attack range*/
    void decAttackRange();

    void stop();
    std::string statusString();
};


#endif //HW_3_CHOPPER_H
