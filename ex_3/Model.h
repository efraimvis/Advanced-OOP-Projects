//
// Created by Admin on 7/12/2022.
//

#ifndef HW_3_MODEL_H
#define HW_3_MODEL_H

#include <map>
#include "Warehouse.h"
#include "Truck.h"
#include "Trooper.h"
#include "Chopper.h"
#include "View.h"
#include "Controller.h"

class Model
{
private:
    Time currentTime;
    std::shared_ptr<View> view;

    /*Constructor*/
    Model();

    bool initialized = false;

    std::map<std::string,std::shared_ptr<Warehouse>> warehouses;
    std::list<std::weak_ptr<Warehouse>> warehousesForTroopers;
    std::map<std::string,std::shared_ptr<Truck>> trucks;
    std::map<std::string,std::shared_ptr<Trooper>> troopers;
    std::map<std::string,std::shared_ptr<Chopper>> choppers;

    /*Increments time by one hour*/
    void incTime();

    /*Updates the view with all objects*/
    void pushObjectsToView();

public:
    enum vehicleType{State_trooper, Chopper_bandit};

    /*Returns instance of singleton Model*/
    static Model& getInstance();

    /*Getter for current time*/
    const Time getCurrentTime() const{return currentTime;}

    Model(const Model& rhs) = delete;
    Model(Model&& rhs) = delete;
    Model& operator=(const Model& rhs) = delete;
    Model& operator=(Model&& rhs) = delete;

    /*Initializes model from parameters (program arguments)
     *
     * argc, argv - program arguments*/
    void initModel(int argc, const char** argv);

    /*Initialized all warehouses from given file
     * filename - directory to file
     *
     * throws : invalid_argument if any parameters in file are invalid, or file could not be found*/
    void readWarehouses(const char* filename);

    /*Initializes a single truck from given file
     *
     * filename - directory to file
     * throws : invalid_argument if any parameters in file are invalid, or file could not be found*/
    void readTruckFile(const char* filename);

    /*Prints the current status of all simulation objects*/
    void printStatus();

    /*Signals given chopper to attack given truck
     *
     * chopperName - chopper name
     * truckName - truck name
     *
    *throws: runtime_error if there are currently no trucks or no choppers, or chopper or truck could not be found*/
    void attackTruck(const std::string chopperName, const std::string truckName);

    /*Sets parameters for given truck at next hour
    * truck - given truck*/
    void update(Truck& truck);

    /*Sets parameters for given trooper at next hour
    * trooper - given trooper*/
    void update(Trooper& trooper);

    /*Sets parameters for given chopper at next hour
    * chopper - given chopper*/
    void update(Chopper& chopper);

    /*Sets new course for vehicle
     *
     * name - vehicle name
     * newCourse - new course to set
     * newSpeed - new vehicle speed (chopper only)
     *
     * throws : runtime_error if vehicle with given name could not be found, or bad parameters*/
    void setVehicleCourse(std::string name, int newCourse, int newSpeed = 0);

    /*Sets vehicle to move to new position
     *
     * name - vehicle name
     * newPosition - new position to set
     * newSpeed - new vehicle speed (chopper only)
     *
     * throws : runtime_error if vehicle with given name could not be found, or bad parameters*/
    void setVehiclePosition(std::string name, std::pair<float, float> newPosition, int newSpeed = 0);

    /*Sets vehicle to move to a specified warehouse (trooper only)
     *
     * vehicleName - vehicle name
     * warehouseName - warehouse name
     *
     * throws : runtime_error if vehicle with given name could not be found, or bad parameters*/
    void setVehicleDestinationWarehouse(std::string vehicleName, std::string warehouseName);

    /*Stops given vehicle
     *
     * name - vehicle name
     *
     * throws : runtime_error if vehicle with given name could not be found, or bad parameters*/
    void stopVehicle(std::string name);

    /*Getter for view*/
    std::weak_ptr<View> getView();

    /*Adds a new vehicle to the model
     *
     * type - vehicle type
     * name - vehicle name
     * location - vehicle start location
     * startWarehouse - name of the vehicle's start warehouse
     *
     * throws : runtime_error if warehouse was not found, or bad parameters*/
    void generateVehicle(vehicleType type, std::string name, std::pair<float, float> location = {0,0} ,std::string startWarehouse = "");

    /*Updates all simulation objects to their values in the next hour*/
    void moveObjects();
};


#endif //HW_3_MODEL_H
