//
// Created by Admin on 7/4/2022.
//

#ifndef HW_3_TROOPER_H
#define HW_3_TROOPER_H

#include <list>
#include "Vehicle.h"
#include "Warehouse.h"

class Trooper : public Vehicle
{
    friend class Model;
private:
    std::weak_ptr<Warehouse> firstInPatrol;
    std::weak_ptr<Warehouse> currentDestinationWarehouse;
    std::list<std::weak_ptr<Warehouse>> currentPatrol;
    const std::list<std::weak_ptr<Warehouse>> allWarehouses;

    /*Sets trooper's current patrol
     *
     * startWarehouse - first warehouse in patrol
     */
    void generatePatrol(std::weak_ptr<Warehouse> startWarehouse);

    /*Finds closes warehouse to given warehouse
     *
     * warehouse - given warehouse
     *
     * return : returns pointer to closest warehouse*/
    std::weak_ptr<Warehouse> findClosest(std::weak_ptr<Warehouse> warehouse, std::list<std::weak_ptr<Warehouse>> list);

public:

    /*Constructor
     *
     * name - trooper name
     * spawnPoint - warehouse in which trooper is located upon creation
     * allWarehouses - list of all warehouses in existence*/
    Trooper(std::string name, std::weak_ptr<Warehouse> spawnPoint, std::list<std::weak_ptr<Warehouse>> allWarehouses);
    void setCourse(int newCourse) override;
    void setDestination(std::pair<float, float> newDestination) override;

    /*Sets trooper's destination warehouse
     *
     * warehouse - new destination warehouse*/
    void setDestination(std::weak_ptr<Warehouse> warehouse);

    /*Sets trooper to start new patrol from given warehouse
     *
     * warehouse - given warehouse*/
    void startPatrolFrom(std::weak_ptr<Warehouse> warehouse);

    void stop();
    std::string statusString();
};


#endif //HW_3_TROOPER_H
