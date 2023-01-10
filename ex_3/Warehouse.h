//
// Created by Admin on 7/3/2022.
//

#ifndef HW_3_WAREHOUSE_H
#define HW_3_WAREHOUSE_H


#include "SimObject.h"

class Warehouse: public SimObject
{
private:
    int currentCrates;

public:

    /*Constructor
     *
     * name - warehose name
     * location - warehouse locations
     * numCreates - number of crates in warehouse*/
    Warehouse(std::string name, std::pair<float, float> location, int numCrates);

    /*Destructor*/
    ~Warehouse(){}

    Warehouse(const Warehouse& rhs) = delete; //two warehouses with same parameters undesirable
    Warehouse(Warehouse&& rhs) = delete;
    Warehouse& operator=(const Warehouse& rhs) = delete;
    Warehouse& operator=(Warehouse&& rhs) = delete;

    /*Getter for number of crates*/
    const int getCurrentCrates() const{return currentCrates;}

    /*Removes given amount of crates from warehouse
     *
     * numCrates - number of crates to take
     * throws : invalid_argument if number of crates is more than currently in stock, or a non-positive number*/
    void takeCrates(int numCrates);

    /*Adds given amount of crates to warehouse
     *
     * numCrates - number of crates to add
     *
     * throw : throws invalid_argument if number of crates is non-positive */
    void addCrates(int numCrates);

    std::string statusString();
};


#endif //HW_3_WAREHOUSE_H
