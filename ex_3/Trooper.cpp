//
// Created by Admin on 7/4/2022.
//

#include "Trooper.h"

Trooper::Trooper(std::string name, std::weak_ptr<Warehouse> spawnPoint, std::list<std::weak_ptr<Warehouse>> allWarehouses): Vehicle(name, spawnPoint.lock()->getLocation()),allWarehouses(allWarehouses)
{
    currentDestinationWarehouse = std::weak_ptr<Warehouse>();
}

void Trooper::generatePatrol(const std::weak_ptr<Warehouse> startWarehouse)
{
    std::list<std::weak_ptr<Warehouse>> warehouselist = allWarehouses;
    std::list<std::weak_ptr<Warehouse>> retval;
    std::weak_ptr<Warehouse> currentClosest = startWarehouse;
    retval.push_back(currentClosest);

    while(!warehouselist.empty())
    {
        warehouselist.remove_if([&currentClosest](std::weak_ptr<Warehouse> p)
        {
            std::shared_ptr<Warehouse> swp = currentClosest.lock();
            std::shared_ptr<Warehouse> sp = p.lock();
            if(swp && sp)
                return swp == sp;
            return false;
        });
        std::weak_ptr<Warehouse> closest =findClosest(currentClosest, warehouselist);
        retval.push_back(closest);
        currentClosest = closest;
    }

    currentPatrol = retval;
}

std::weak_ptr<Warehouse> Trooper::findClosest(std::weak_ptr<Warehouse> warehouse, std::list<std::weak_ptr<Warehouse>> list)
{
    std::weak_ptr<Warehouse> retval = *list.begin();
    for(auto it = list.begin(); it != list.end(); ++it)
    {
        if(getDistance((*it).lock()->getLocation(), warehouse.lock()->getLocation()) <= getDistance(retval.lock()->getLocation(), warehouse.lock()->getLocation()))
        {
            if(getDistance((*it).lock()->getLocation(), warehouse.lock()->getLocation()) == getDistance(retval.lock()->getLocation(), warehouse.lock()->getLocation()))
            {
                if((*it).lock()->getName().compare(retval.lock()->getName()) < 0) retval = (*it);
            }
            else if(getDistance((*it).lock()->getLocation(), warehouse.lock()->getLocation()) < getDistance(retval.lock()->getLocation(), warehouse.lock()->getLocation()))
            {
                retval = (*it);
            }
        }
    }
    return retval;
}

void Trooper::setCourse(int newCourse)
{
    course = newCourse;
    currentState = movingOnCourse;
    speed = 90;
    currentPatrol = std::list<std::weak_ptr<Warehouse>>();
    firstInPatrol = std::weak_ptr<Warehouse>();
    currentDestinationWarehouse = std::weak_ptr<Warehouse>();
}

void Trooper::setDestination(std::pair<float, float> newDestination)
{
    destination = newDestination;
    currentState = movingTo;
    speed = 90;
    currentPatrol = std::list<std::weak_ptr<Warehouse>>();
    firstInPatrol = std::weak_ptr<Warehouse>();
    currentDestinationWarehouse = std::weak_ptr<Warehouse>();
}

void Trooper::setDestination(std::weak_ptr<Warehouse> warehouse)
{
    destination = warehouse.lock()->getLocation();
    currentState = movingTo;
    speed = 90;
    currentDestinationWarehouse = warehouse;
}

void Trooper::startPatrolFrom(std::weak_ptr<Warehouse> warehouse)
{
    destination = warehouse.lock()->getLocation();
    currentState = movingTo;
    speed = 90;
    currentDestinationWarehouse = warehouse;
    firstInPatrol = warehouse;
    generatePatrol(warehouse);
}

void Trooper::stop()
{
    destination = getLocation();
    currentState = stopped;
    speed = 0;
}

std::string Trooper::statusString()
{
    std::stringstream ss;
    ss<<"State Trooper "<<this->name<<" at "<<locationString(getLocation())<<" ";
    if(currentState == movingTo && !currentDestinationWarehouse.expired()) ss<<getStatusString()<<" "<<currentDestinationWarehouse.lock()->getName()<<", speed "<<std::setprecision(2)<<std::fixed<<speed<<" km/h";
    else if(currentState == movingTo && currentDestinationWarehouse.expired()) ss<<getStatusString()<<" "<<locationString(destination)<<", speed "<<std::setprecision(2)<<std::fixed<<speed<<" km/h";
    else if(currentState == movingOnCourse) ss<<getStatusString()<<" "<<std::setprecision(2)<<std::fixed<<course<<" deg, speed "<<speed<<" km/h";
    else ss<<getStatusString();
    return ss.str();
}