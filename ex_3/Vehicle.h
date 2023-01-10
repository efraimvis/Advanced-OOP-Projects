//
// Created by Admin on 7/2/2022.
//

#ifndef HW_3_VEHICLE_H
#define HW_3_VEHICLE_H


#include <utility>
#include <string>
#include "SimObject.h"

enum State{stopped, parked, offRoad, movingTo, movingOnCourse};


class Vehicle : public SimObject
{
protected:
    double speed;
    int course;
    std::pair<float, float> destination;
    State currentState;

    /*Returns string representing vehicle's current STATE*/
    std::string getStatusString()
    {
        switch(currentState)
        {
            case stopped:
                return "Stopped";
            case offRoad:
                return "Off road";
            case movingTo:
                return "Heading to:";
            case movingOnCourse:
                return "Moving on course:";
        }
        return "";
    }

public:
    /*Constructor
     *
     * name - vehicle name
    * location - vehicle location
    **/
    Vehicle(std::string name,std::pair<float, float> location);

    /*Destructor*/
    ~Vehicle(){}

    Vehicle(const Vehicle& rhs) = delete;
    Vehicle(Vehicle&& rhs) = delete;
    Vehicle& operator=(const Vehicle& rhs) = delete;
    Vehicle& operator=(Vehicle&& rhs) = delete;

    /*Getter for speed*/
    const double getSpeed() const {return speed;}

    /*Getter for course*/
    const int getCourse() const {return course;}

    /*Setter for course
     *
     * newCourse - course to set
     *
     * throws invalid_argument if course is not between 0 and 359*/
    virtual void setCourse(int newCourse);

    /*Setter for destination
     * newDestination - destination to set*/
    virtual void setDestination(std::pair<float, float> newDestination);

    /*Getter for destination*/
    const std::pair<float, float> getDestination() const {return destination;}

    /*Getter for state*/
    const State getState() const{return currentState;}

    /*Stops vehicle*/
    virtual void stop() = 0;

    /*Returns a string representing vehicle's current status*/
    virtual std::string statusString() = 0;
};


#endif //HW_3_VEHICLE_H
