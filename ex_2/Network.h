//
// Created by Admin on 5/19/2022.
//

#ifndef HW_2_NETWORK_H
#define HW_2_NETWORK_H

#include "Graph.h"

class Network
{
private:
    std::unique_ptr<Graph> graph;
    std::string outputFile = "output.dat";

    /*Sets network configurations from config file
     * filename - name of config file
     * throws : runtime_error if file could not be opened,
     * or there is an error in file parameters*/
    std::list<std::pair<std::string,int>> readConfigFile(std::string filename);

    /*Modifies network with information from input file
     * filename - name of input file
     * throws : runtime_error if file could not be opened,
     * or there is an error in file parameters*/
    void readInputFile(std::string filename);

    /*Prints current network to output file*/
    void printNetwork();

public:
    /*Constructor
     * args - network parameters (input, config, output files as described in assignment)*/
    Network(int argc,char** args);

    /*Disabled copy constructor (network is singular)*/
    Network(const Network& rhs) = delete;

    /*Disabled copy-assignment operation (network is singular)*/
    Network& operator=(const Network& rhs) = delete;

    /*Disabled move constructor (network is singular)*/
    Network(Network&& rhs) = delete;

    /*Disabled move-assignment operator (network is singular)*/
    Network& operator=(Network&& rhs) = delete;

    /*Runs the program*/
    void run();
};


#endif //HW_2_NETWORK_H
