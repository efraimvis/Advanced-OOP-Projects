//
// Created by Admin on 7/12/2022.
//

#include "Model.h"
#include <string.h>
#include <fstream>
#include <vector>

Model& Model::getInstance()
{
    static Model instance;
    return instance;
}

void Model::initModel(int argc, const char** argv)
{
    if(!initialized)
    {
        if(argc < 5) throw std::invalid_argument("ERROR: too few program arguments");

        if(!(strcmp(argv[1], "-w") == 0 && strcmp(argv[3], "-t") == 0))
        {
            throw std::invalid_argument("ERROR: invalid program arguments");
        }

        try
        {
            readWarehouses(argv[2]);
        }
        catch(std::invalid_argument& e){throw;}

        //emplace frankfurt if it wasn't already included
        if(warehouses.count("Frankfurt") == 0)
        {
            warehouses.emplace("Frankfurt",std::make_shared<Warehouse>("Frankfurt",std::pair<float, float>{40,10},100000));
        }

        int i = 4;
        while(i < argc)
        {
            try
            {
                readTruckFile(argv[i]);
            }
            catch(std::invalid_argument& e){throw;}

            i++;
        }

        //after all warehouses are created, create a list of weak pointers to warehouses to be used by troopers
        for(auto it = warehouses.begin(); it != warehouses.end(); ++it)
        {
            warehousesForTroopers.push_back((*it).second);
        }
    }
    this->view = std::make_shared<View>();
    this->pushObjectsToView();
    initialized = true;

}


Model::Model()
{
}

std::weak_ptr<View> Model::getView()
{
    return view;
}

void Model::pushObjectsToView()
{
    for(auto warehouse : warehouses)
    {
        view->insertObject(warehouse.first, warehouse.second->getLocation());
    }

    for(auto truck : trucks)
    {
        view->insertObject(truck.first, truck.second->getLocation());
    }

    for(auto trooper : troopers)
    {
        view->insertObject(trooper.first, trooper.second->getLocation());
    }

    for(auto chopper : choppers)
    {
        view->insertObject(chopper.first, chopper.second->getLocation());
    }
}

void Model::moveObjects()
{
    incTime();
    for(auto truck : trucks)
    {
        update(*truck.second);
    }

    for(auto trooper : troopers)
    {
        update(*trooper.second);
    }

    for(auto chopper : choppers)
    {
        update(*chopper.second);
    }
    pushObjectsToView();
}

void Model::printStatus()
{
    for(auto warehouse : warehouses)
    {
        std::cout<<warehouse.second->statusString()<<std::endl;
    }

    for(auto truck : trucks)
    {
        std::cout<<truck.second->statusString()<<std::endl;
    }

    for(auto trooper : troopers)
    {
        std::cout<<trooper.second->statusString()<<std::endl;
    }

    for(auto chopper : choppers)
    {
        std::cout<<chopper.second->statusString()<<std::endl;
    }
}

void Model::readWarehouses(const char *filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        std::stringstream ss;
        ss<<"ERROR: could not open file "<< filename;
        throw std::invalid_argument(ss.str());
    }

    std::string line;
    while(getline(file, line))
    {
        std::string token;
        std::string delim = ", ";
        size_t pos = 0;

        std::vector<std::string> parts;

        while ((pos = line.find(delim)) != std::string::npos)
        {
            token = line.substr(0, pos);

            //get warehouse name, then half a position, then other half of position, and then warehouse load
            parts.push_back(token);

            line.erase(0, pos + delim.length());
        }
        parts.push_back(line);

        if(parts.size() != 4) throw std::invalid_argument("ERROR: incorrect parameters in file");

        std::string warehouseName = parts[0];

        if(warehouses.count(warehouseName) != 0) throw std::invalid_argument("ERROR: incorrect parameters in file");

        if((parts[1])[0] != '(' || (parts[2])[parts[2].size()-1] != ')') throw std::invalid_argument("ERROR: incorrect parameters in file");

        float xCoord = stof( parts[1].substr(1));
        float yCoord = stof( parts[2].substr(0,parts[2].size()-1));

        warehouses.emplace(warehouseName,std::make_shared<Warehouse>(warehouseName,std::pair<float, float>{xCoord,yCoord},stof(parts[3])));
    }

    file.close();
}

void Model::readTruckFile(const char* filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        std::stringstream ss;
        ss<<"ERROR: could not open file "<< filename;
        throw std::invalid_argument(ss.str());
    }

    std::string line;

    std::list<DistNode> distRound;
    std::weak_ptr<Warehouse> firstInRound;

    std::string _filename = filename;
    std::string truckName = _filename.substr(0,_filename.find("."));

    if(trucks.count(truckName) != 0) throw std::invalid_argument("ERROR: incorrect parameters in file");

    while(getline(file, line))
    {
        std::string token;
        std::string delim = ",";
        size_t pos = 0;

        std::vector<std::string> parts;

        while ((pos = line.find(delim)) != std::string::npos)
        {
            token = line.substr(0, pos);

            parts.push_back(token);

            line.erase(0, pos + delim.length());
        }
        parts.push_back(line);

        if(parts.size() != 2 && parts.size() != 4) throw std::invalid_argument("ERROR: incorrect parameters in file");

        std::string warehouseName = parts[0];

        if(warehouses.count(parts[0]) == 0) throw std::invalid_argument("ERROR: warehouse does not exist");
        std::weak_ptr<Warehouse> _warehouse = warehouses.find(warehouseName)->second;

        if((parts[1])[2] != ':') throw std::invalid_argument("ERROR: incorrect parameters in file");

        int Ahours = stoi( parts[1].substr(0,2));
        int Aminutes = stof( parts[1].substr(3,parts[1].size()));

        int Dhours, Dminutes;

        int numCrates = 0;

        if(parts.size() == 4)
        {
            if((parts[3])[2] != ':') throw std::invalid_argument("ERROR: incorrect parameters in file");

            Dhours = stoi( parts[3].substr(0,2));
            Dminutes = stof( parts[3].substr(3,parts[3].size()));
            numCrates = stoi(parts[2]);
        }

        if(parts.size() == 2)
        {
            firstInRound = _warehouse;
            distRound.push_back(DistNode{_warehouse,numCrates,Time{Ahours,Aminutes},Time{Ahours,Aminutes}});
        }
        else
        {
            distRound.push_back(DistNode{_warehouse,numCrates,Time{Ahours,Aminutes},Time{Dhours,Dminutes}});
        }

    }

    trucks.emplace(truckName, std::make_shared<Truck>(truckName,distRound, firstInRound));
    firstInRound.lock()->takeCrates(trucks.find(truckName)->second->getCurrentCargo());

    file.close();
}

void Model::update(Truck& truck)
{
    if(!(truck.getState() == offRoad))
    {
        Time nextTime = currentTime;
        ++nextTime;

        if(truck.getState() == stopped)
        {
            if(truck.firstInRound.getWarehouse().expired()) return;
        }

        if(truck.isFinished()) return;

        auto iter = truck.distributionRound.begin();

        //for(auto iter = truck.distributionRound.begin(); iter != truck.distributionRound.end(); ++iter)
        while(iter != truck.distributionRound.end())
        {
            //if the truck doesn't reach this warehouse by the next hour
            if((*iter).getArrivalTime() > nextTime)
            {
                truck.destinationWarehouse = (*iter).getWarehouse();
                truck.destination = truck.destinationWarehouse.lock()->getLocation();
                truck.currentState = movingTo;

                int dt = 60;
                truck.location = getNextLocation(truck.getLocation(), truck.getDestination(),truck.getSpeed(), dt);

                return;
            }

            //if the truck should be stopped at this warehouse at the next hour
            else if(!(nextTime < (*iter).getArrivalTime()) && nextTime < (*iter).getDepartureTime())
            {
                truck.destinationWarehouse = (*iter).getWarehouse();
                truck.destination = truck.destinationWarehouse.lock()->getLocation();
                truck.currentState = stopped;
                truck.speed = 0;
                truck.location = truck.destination;
                //truck.currentCargo -= (*iter).getCratesToUnload();

                return;
            }

            //if the truck leaves this warehouse by the next hour
            else
            {
                truck.currentCargo -= (*iter).getCratesToUnload();
                (*iter).getWarehouse().lock()->addCrates((*iter).getCratesToUnload());
                //case 1:there are warehouses in the round after this one
                if(truck.distributionRound.size() > 1)
                {
                    auto nextWarehouse = iter;
                    nextWarehouse++;
                    truck.destinationWarehouse = (*(nextWarehouse)).getWarehouse();
                    truck.destination = truck.destinationWarehouse.lock()->getLocation();
                    truck.currentState = movingTo;
                    truck.speed = truck.getRequiredSpeed((*iter), (*(nextWarehouse)));

                    int dt = 60 - (*iter).getDepartureTime().getMinutes();
                    truck.location = getNextLocation(truck.getLocation(), truck.getDestination(),truck.getSpeed(), dt);

                    ++iter;
                    truck.distributionRound.pop_front();
                }
                else if(truck.distributionRound.size() == 1)
                {
                    truck.destinationWarehouse = truck.firstInRound.getWarehouse();
                    truck.destination = truck.destinationWarehouse.lock()->getLocation();
                    truck.currentState = stopped;
                    truck.speed = 0;

                    truck.location = truck.firstInRound.getWarehouse().lock()->getLocation();

                    truck.distributionRound = std::list<DistNode>();
                }
            }
        }
    }
}

void Model::update(Trooper& trooper)
{
    if(trooper.getState() != stopped)
    {

        int DISTANCE = 90;

        //if trooper is moving on specified course
        if(trooper.getState() == movingOnCourse)
        {
            trooper.speed = 90;
            trooper.location = getNextLocationCourse(trooper.location, trooper.getCourse());
            return;
        }

        //if trooper is moving to specified position (not warehouse)
        if(trooper.getState() == movingTo && trooper.firstInPatrol.expired())
        {
            trooper.speed = 90;
            trooper.location = getNextLocation(trooper.location, trooper.destination,90,60);
            return;
        }

        //if trooper is moving to specified warehouse, need to start patrol when it gets there
        if(trooper.getState() == movingTo && !trooper.firstInPatrol.expired())
        {
            trooper.speed = 90;
            if(trooper.currentDestinationWarehouse.lock() == trooper.firstInPatrol.lock())
            {
                //trooper doesn't reach first warehouse within the hour
                if (getDistance(trooper.location, trooper.firstInPatrol.lock()->getLocation()) * 100 > DISTANCE)
                {
                    trooper.location = getNextLocation(trooper.location, trooper.destination, 90, 60);
                    return;
                }
                //if trooper reaches first warehouse, start patrol
                else if(getDistance(trooper.location, trooper.firstInPatrol.lock()->getLocation()) * 100 <= DISTANCE)
                {
                    double totalDistance = 0;
                    double totalTime = 0;
                    //add distance to first warehosue to total distances, and time to total time
                    totalDistance += getDistance(trooper.location, trooper.firstInPatrol.lock()->getLocation());
                    totalTime += getDistance(trooper.location, trooper.firstInPatrol.lock()->getLocation())/(trooper.speed);

                    //while total distance doesn't go over DISTANCE, continue to warehouses on patrol
                    auto iter = trooper.currentPatrol.begin();
                    while(totalDistance + (getDistance(trooper.location,(*iter).lock()->getLocation())*100) <= DISTANCE && trooper.currentPatrol.size() > 0)
                    {
                        totalDistance += (getDistance(trooper.location,(*iter).lock()->getLocation())*100);
                        totalTime += getDistance(trooper.location, (*iter).lock()->getLocation())/(trooper.speed);
                        trooper.location = (*iter).lock()->getLocation();
                        ++iter;
                        trooper.currentPatrol.pop_front();
                        trooper.setDestination((*iter));
                    }
                    //if trooper can still travel within the hour, set appropriate location
                    if(totalDistance < DISTANCE) trooper.location = getNextLocation(trooper.location, trooper.destination, 90, 60-totalTime);
                    return;
                }
            }
            //if trooper finished patrol, return to first warehouse
        }
        //if trooper is performing a patrol, and has checked the first warehouse
        if(trooper.getState() == movingTo && (trooper.currentDestinationWarehouse.lock() != trooper.firstInPatrol.lock()))
        {
            double totalDistance = 0;
            double totalTime = 0;

            //while total distance doesn't go over DISTANCE, continue to warehouses on patrol
            auto iter = trooper.currentPatrol.begin();
            while(totalDistance + (getDistance(trooper.location,(*iter).lock()->getLocation())*100) <= DISTANCE && trooper.currentPatrol.size() > 0)
            {
                totalDistance += (getDistance(trooper.location,(*iter).lock()->getLocation())*100);
                totalTime += getDistance(trooper.location, (*iter).lock()->getLocation())/(trooper.speed);
                trooper.location = (*iter).lock()->getLocation();
                ++iter;
                trooper.currentPatrol.pop_front();
                trooper.setDestination((*iter));
            }
            //if trooper can still travel within the hour, set appropriate location
            if(totalDistance < DISTANCE) trooper.location = getNextLocation(trooper.location, trooper.destination, 90, 60-totalTime);
            return;
        }
    }
}

void Model::update(Chopper& chopper)
{
    if(!(chopper.getState() == stopped))
    {
        //if chopper is moving on specified course
        if (chopper.getState() == movingOnCourse)
        {
            chopper.location = getNextLocationCourse(chopper.location, chopper.getCourse(),chopper.speed);
            return;
        }

        //if chopper is moving to specified position
        if (chopper.getState() == movingTo)
        {
            chopper.location = getNextLocation(chopper.location, chopper.destination, chopper.speed, 60);
            return;
        }
    }
}

void Model::attackTruck(const std::string chopperName, const std::string truckName)
{
    if(trucks.empty() || choppers.empty()) throw std::runtime_error("ERROR: no trucks or no choppers currently");

    auto chopperIter = choppers.find(chopperName);
    if(chopperIter == choppers.end()) //throw runtime_error if chopper not found
    {
        std::stringstream ss;
        ss<<"ERROR: could not find chopper ";
        ss<<chopperName;
        throw std::runtime_error(ss.str());
    }

    auto truckIter = trucks.find(truckName);
    if(truckIter == trucks.end()) //throw runtime_error if truck not found
    {
        std::stringstream ss;
        ss<<"ERROR: could not find truck ";
        ss<<truckName;
        throw std::runtime_error(ss.str());
    }

    if((*truckIter).second->getState() == offRoad) throw std::runtime_error("ERROR: cannot attack offroad truck");

    //find vehicle locations and determine if the is a trooper nearby
    std::pair<float, float> truckPos = (*truckIter).second->getLocation();

    bool trooperNearby = false;
    for(auto it = troopers.begin(); it != troopers.end(); ++it)
    {
        if((getDistance((*it).second->getLocation(), truckPos)*100) < 10) trooperNearby = true;
    }

    //trooper nearby, attack failed
    if(trooperNearby)
    {
        (*chopperIter).second->stop();
        (*chopperIter).second->decAttackRange();
        return;
    }
    std::pair<float, float> chopperPos = (*chopperIter).second->getLocation();
    int chopperRange = (*chopperIter).second->attackRange;

    //successful
    if((getDistance(truckPos, chopperPos)*100) <= chopperRange)
    {
        (*truckIter).second->goOffRoad();
        (*chopperIter).second->incAttackRange();
        (*chopperIter).second->stop();
    }
    else//failed attack
    {
        (*chopperIter).second->decAttackRange();
        (*chopperIter).second->stop();
    }
}

void Model::generateVehicle(vehicleType type, std::string name, std::pair<float, float> location,std::string startWarehouse)
{
    try
    {
        if(!(trucks.count(name) == 0 && troopers.count(name) == 0 && choppers.count(name) == 0))
        {
            std::stringstream ss;
            ss<<"ERROR: name "<<name<<" already taken by a different vehicle";
            throw std::runtime_error(ss.str());
        }
        if(type == State_trooper)
        {
            if(Model::getInstance().troopers.count("name") != 0) throw std::invalid_argument("ERROR: vehicle already exists");
            auto _startWarehouse = Model::getInstance().warehouses.find(startWarehouse);
            if(_startWarehouse == Model::getInstance().warehouses.end())
            {
                std::stringstream ss;
                ss<<"ERROR: warehouse "<<startWarehouse<<" does not exist";
                throw std::invalid_argument(ss.str());
            }
            try
            {
                Model::getInstance().troopers.emplace(name, std::make_shared<Trooper>(name, (*_startWarehouse).second, Model::getInstance().warehousesForTroopers));
            }catch(std::invalid_argument& ie){throw;}
        }
        if(type == Chopper_bandit)
        {
            if(Model::getInstance().choppers.count("name") != 0) throw std::invalid_argument("ERROR: vehicle already exists");
            try
            {
                Model::getInstance().choppers.emplace(name, std::make_shared<Chopper>(name, location));
            } catch(std::invalid_argument& ie){throw;}
        }
    }
    catch(std::invalid_argument& ia){throw;}
}

void Model::setVehicleCourse(std::string name, int newCourse, int newSpeed)
{
    try
    {
        if(troopers.count(name) != 0)
        {
            auto trooper = troopers.find(name);
            (*trooper).second->setCourse(newCourse);
        }
        else if(choppers.count(name) != 0)
        {
            auto chopper = choppers.find(name);
            (*chopper).second->setCourse(newCourse, newSpeed);
        }
        else
        {
            std::stringstream ss;
            ss<<"ERROR: could not find vehicle "<<name;
            throw std::runtime_error(ss.str());
        }
    }
    catch(std::invalid_argument& ia){throw;}
}

void Model::setVehiclePosition(std::string name, std::pair<float, float> newPosition, int newSpeed)
{
    try
    {
        if(troopers.count(name) != 0)
        {
            auto trooper = troopers.find(name);
            (*trooper).second->setDestination(newPosition);
        }
        else if(choppers.count(name) != 0)
        {
            auto chopper = choppers.find(name);
            (*chopper).second->setDestination(newPosition, newSpeed);
        }
        else
        {
            std::stringstream ss;
            ss<<"ERROR: could not find vehicle "<<name;
            throw std::runtime_error(ss.str());
        }
    }
    catch(std::invalid_argument& ia){throw;}
}

void Model::setVehicleDestinationWarehouse(std::string vehicleName, std::string warehouseName)
{
    try
    {
        if(troopers.count(vehicleName) != 0 && warehouses.count(warehouseName) != 0)
        {
            auto trooper = troopers.find(vehicleName);
            (*trooper).second->startPatrolFrom(warehouses.find(warehouseName)->second);
        }
        else if(troopers.count(vehicleName) == 0)
        {
            std::stringstream ss;
            ss<<"ERROR: could not find vehicle "<<vehicleName;
            throw std::runtime_error(ss.str());
        }
        else if(warehouses.count(warehouseName) == 0)
        {
            std::stringstream ss;
            ss<<"ERROR: could not find warehouse "<<warehouseName;
            throw std::runtime_error(ss.str());
        }
    }
    catch(std::invalid_argument& ia){throw;}
}

void Model::stopVehicle(std::string name)
{
    try
    {
        if(troopers.count(name) != 0)
        {
            auto trooper = troopers.find(name);
            (*trooper).second->stop();
        }
        else if(choppers.count(name) != 0)
        {
            auto chopper = choppers.find(name);
            (*chopper).second->stop();
        }
        else if(trucks.count(name) != 0)
        {
            auto truck = trucks.find(name);
            (*truck).second->hardStop();
        }
        else
        {
            std::stringstream ss;
            ss<<"ERROR: could not find vehicle "<<name;
            throw std::runtime_error(ss.str());
        }
    }
    catch(std::invalid_argument& ia){throw;}
}

void Model::incTime()
{
    ++currentTime;
}

