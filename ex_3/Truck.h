//
// Created by Admin on 7/3/2022.
//

#ifndef HW_3_TRUCK_H
#define HW_3_TRUCK_H

#include <list>
#include <memory>
#include <iostream>
#include "Warehouse.h"
#include "Vehicle.h"


class DistNode
{
private:
    std::weak_ptr<Warehouse> warehouse;
    int cratesToUnload;
    Time arrival;
    Time departure;
public:
    ~DistNode(){};
    DistNode(){}
    DistNode(std::weak_ptr<Warehouse> _warehouse, int _crates,Time _arrival, Time _departure):warehouse(_warehouse), arrival(_arrival),departure(_departure)
    {
        if(_crates < 0) throw std::invalid_argument("ERROR: can not have negative number of crates to unload");
        cratesToUnload = _crates;
    }
    DistNode(const DistNode& rhs): departure(rhs.departure), arrival(rhs.arrival)
    {
        warehouse = rhs.warehouse;
        cratesToUnload = rhs.cratesToUnload;
        arrival = rhs.arrival;
        departure = rhs.departure;
    }
    DistNode(DistNode&& rhs): arrival(rhs.arrival), departure(rhs.departure)
    {
        warehouse = rhs.warehouse;
        rhs.warehouse.reset();

        cratesToUnload = rhs.cratesToUnload;
    }
    DistNode& operator=(const DistNode& rhs)
    {
        if(&rhs == this)
            return *this;

        warehouse = rhs.warehouse;
        cratesToUnload = rhs.cratesToUnload;
        arrival = rhs.arrival;
        departure = rhs.departure;

        return *this;
    }

    DistNode& operator=(const DistNode&& rhs)
    {
        if(&rhs == this)
            return *this;

        warehouse = std::move(rhs.warehouse);
        cratesToUnload = std::move(rhs.cratesToUnload);
        arrival = std::move(rhs.arrival);
        departure = std::move(rhs.departure);

        return *this;
    }

    const Time& getArrivalTime() const{return arrival;}
    const Time& getDepartureTime() const {return  departure;}
    const int getCratesToUnload() const{return cratesToUnload;}
    std::weak_ptr<Warehouse> getWarehouse() const {return warehouse;}
};

class Truck : public Vehicle
{
    friend class Model;
private:
    DistNode firstInRound;
    std::list<DistNode> distributionRound;
    int currentCargo = 0;
    double getRequiredSpeed(const DistNode& from, const DistNode& to);
    std::weak_ptr<Warehouse> destinationWarehouse;

public:

    /*Constructor
     * name - truck name
     * round - distribution round
     * startLocation - warehouse from which round starts - must match first in distribution round*/
    Truck(std::string name,std::list<DistNode>& round,std::weak_ptr<Warehouse> startLocation);

    Truck(const Truck& rhs) = delete;
    Truck(Truck&& rhs) = delete;
    Truck& operator=(const Truck& rhs) = delete;
    Truck& operator=(Truck&& rhs) = delete;

    /*Getter for current cargo*/
    int getCurrentCargo(){return currentCargo;}

    /*Takes truck off road (after being robbed)*/
    void goOffRoad();

    /*Stops truck (halted, will retain distribution round)*/
    virtual void stop();

    /*Stops truck, without possibility of it moving again*/
    void hardStop();

    std::string statusString();

    /*return : Returns true if truck has finished its round and has returned to first warehouse, false otherwise*/
    bool isFinished();
};


#endif //HW_3_TRUCK_H
