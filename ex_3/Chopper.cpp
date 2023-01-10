//
// Created by Admin on 7/12/2022.
//

#include "Chopper.h"

Chopper::Chopper(std::string name, std::pair<float, float> startLocation): Vehicle(name, startLocation)
{
    this->attackRange = 2;
}

void Chopper::setSpeed(int newSpeed)
{
    if(!(newSpeed > 0 && newSpeed <= 170)) throw std::invalid_argument("ERROR: speed must be between 1 and 170 kmh");
    this->speed = newSpeed;
}

void Chopper::setCourse(int newCourse, int newSpeed)
{
    try
    {
        Vehicle::setCourse(newCourse);
        setSpeed(newSpeed);
        this->currentState = movingOnCourse;
    }
    catch(std::invalid_argument& e){throw;}
}

void Chopper::setDestination(std::pair<float, float> newDestination, int newSpeed)
{
    try
    {
        Vehicle::setDestination(newDestination);
        setSpeed(newSpeed);
        this->currentState = movingTo;
    }
    catch(std::invalid_argument& e){throw;}
}

void Chopper::incAttackRange()
{
    if(attackRange < 20) attackRange++;
}

void Chopper::decAttackRange()
{
    if(attackRange > 0) attackRange--;
}

void Chopper::stop()
{
    destination = this->location;
    currentState = stopped;
    speed = 0;
}

std::string Chopper::statusString()
{
    std::stringstream ss;
    ss<<"Chopper "<<this->name<<" at "<<locationString(getLocation())<<" ";
    if(currentState == movingTo) ss<<getStatusString()<<" "<<locationString(getDestination())<<", speed "<<std::setprecision(2)<<std::fixed<<speed<<" km/h";
    else if(currentState == movingOnCourse) ss<<getStatusString()<<" "<<std::setprecision(2)<<std::fixed<<course<<" deg, speed "<<speed<<" km/h";
    else ss<<getStatusString();
    return ss.str();
}
