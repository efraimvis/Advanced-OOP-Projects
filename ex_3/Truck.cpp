//
// Created by Admin on 7/3/2022.
//

#include <sstream>
#include "Truck.h"


Truck::Truck(std::string name,std::list<DistNode>& round,std::weak_ptr<Warehouse> startLocation):Vehicle(name, startLocation.lock()->getLocation())
{
    if(round.empty()) throw std::invalid_argument("ERROR: cannot create truck with empty distribution round");
    if(startLocation.lock()->getLocation() != (*round.begin()).getWarehouse().lock()->getLocation()) throw std::invalid_argument("ERROR: start location must match first warehouse in distribution round");
    this->distributionRound = round;
    this->firstInRound = (*round.begin());
    this->location = startLocation.lock()->getLocation();

    if(++round.begin() != round.end())
    {
        this->destination = (*(++round.begin())).getWarehouse().lock()->getLocation();
        this->destinationWarehouse = (*(++round.begin())).getWarehouse();
        this->speed = getRequiredSpeed(*(round.begin()), *(++round.begin()));
        this->currentState = movingTo;
    }
    else
    {
        stop();
    }
    for(auto it = ++round.begin(); it != round.end(); ++it)
    {
        currentCargo += (*it).getCratesToUnload();
    }
    this->distributionRound.erase(distributionRound.begin());
}

void Truck::stop()
{
    this->destination = this->location;
    this->currentState = stopped;
    this->speed = 0;
}

void Truck::hardStop()
{
    this->destination = this->location;
    this->currentState = stopped;
    this->speed = 0;
    this->distributionRound = std::list<DistNode>();
    this->firstInRound = DistNode();
}

void Truck::goOffRoad()
{
    this->currentState = offRoad;
    this->speed = 0;
    this->destination = this->location;
    this->currentCargo = 0;
}

double Truck::getRequiredSpeed(const DistNode &from, const DistNode &to)
{
    if(to.getArrivalTime() < from.getDepartureTime() || to.getArrivalTime() == from.getDepartureTime())
        throw std::invalid_argument("ERROR: incompatible departure/arrival times");
    double t = to.getArrivalTime()-from.getDepartureTime();
    double s = getDistance(from.getWarehouse().lock()->getLocation(), to.getWarehouse().lock()->getLocation()) * 100;
    return (s/t);
}

std::string Truck::statusString()
{
    std::stringstream ss;
    ss<<"Truck "<<this->name<<" at "<<locationString(getLocation())<<" ";
    if(currentState == movingTo) ss<<getStatusString()<<" "<<destinationWarehouse.lock()->getName();
    else ss<<getStatusString();
    ss<<", Crates: "<<currentCargo;
    return ss.str();
}

bool Truck::isFinished()
{
    if(currentState == stopped)
    {
        if(distributionRound.empty())
        {
            if(destination == firstInRound.getWarehouse().lock()->getLocation()) return true;
        }
    }
    return false;
}
