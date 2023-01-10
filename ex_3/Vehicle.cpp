//
// Created by Admin on 7/2/2022.
//

#include <stdexcept>
#include "Vehicle.h"

Vehicle::Vehicle(std::string name, std::pair<float, float> location): SimObject(location)
{
    if(name.length() > 12) throw std::invalid_argument("ERROR: Vehicle name can not be more than 12 characters long");
    this->SimObject::name = name;
    this->currentState = stopped;
    this->speed = 0;
    this->destination = this->location;
    this->course = 0;
}

void Vehicle::setCourse(int newCourse)
{
    if(!(newCourse >= 0 && newCourse < 360)) throw std::invalid_argument("ERROR: course must be a number between 0 and 359");
    this->course = newCourse;
    this->currentState = movingOnCourse;
}

void Vehicle::setDestination(std::pair<float, float> newDestination)
{
    this->destination = newDestination;
    this->currentState = movingTo;
}


