//
// Created by Admin on 7/3/2022.
//

#include <stdexcept>
#include "Warehouse.h"

void Warehouse::takeCrates(int numCrates)
{
    if(numCrates < 0) throw std::invalid_argument("ERROR: number of crates must be non-negative");
    if((currentCrates-numCrates) < 0) throw std::invalid_argument("ERROR: number of crates cannot be more than current amount in warehouse");

    this->currentCrates = currentCrates-numCrates;
}

void Warehouse::addCrates(int numCrates)
{
    if(numCrates < 0) throw std::invalid_argument("ERROR: number of crates must be non-negative");

    this->currentCrates += numCrates;
}


Warehouse::Warehouse(std::string name, std::pair<float, float> location, int numCrates): SimObject(location)
{
    if(name.empty()) throw std::invalid_argument("ERROR: invalid name");
    if(numCrates < 0) throw std::invalid_argument("ERROR: number of crates must be non-negative");
    this->name = name;
    this->currentCrates = numCrates;
}


std::string Warehouse::statusString()
{
    std::stringstream ss;
    ss<<"Warehouse "<<this->name<<" at position "<<locationString(getLocation())<<" "<<"Inventory: "<<this->currentCrates;
    return ss.str();
}