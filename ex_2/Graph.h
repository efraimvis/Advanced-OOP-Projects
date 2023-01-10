//
// Created by Admin on 5/25/2022.
//

#ifndef HW_2_GRAPH_H
#define HW_2_GRAPH_H

#include <vector>
#include "Vertex.h"

class Graph
{
private:

    std::vector<std::shared_ptr<Vertex>> vertices;

    /*Returns the edge weight between two vertices, if the edge exists
     * u - index of "from" vertex
     * v - index of "to" vertex
     * vehicleIndex - type of vehicle edge
     * inOutControl - selects whether "in" neighbors or "out" neighbors are scanned,
     * valid arguments : 'i' ("in" neighbors), 'o' ("out" neighbors)
     *
     * return : returns the edge weight if edge exists, or 0 if it doesn't*/
    int getEdgeWeight(int u, int v, char vehicleType, char inOutControl);

    /*Vehicle type stop times*/
    int bus_stop_time = 1;
    int tram_stop_time = 2;
    int sprinter_stop_time = 3;
    int rail_stop_time = 5;

    /*Stop transit times*/
    int intercity_transit_time = 15;
    int centraal_transit_time = 10;
    int stad_transit_time = 5;

    /*Returns appropriate stop time for vehicle type
     * vehicleType - type of vehicle
     *
     * return : returns appropriate stop time for vehicle type*/
    int getVehicleStopTime(const char vehicleType);

    /*Helper function from inbound and outbound
     *
     * station - name of given station
     * inout - selects inbound/outbound
     *
     * throw : runtime_error if station is not in network*/
    void inoutbound(std::string station, char inout);

    /*Performs dijkstra's algorithm on the current graph
     *
     * from - vertex from which to start algorithm
     * to - vertex for which the lightest path needs to be found (only applicable if mode is "uni" or "multi")
     * vehicleType - vehicle type for which the algorithm is performed,
     * valid arguments : b (bus), t (tram), s (sprinter), r (rail)
     * mode - determines output (reachable vertices vs shortest paths),
     *  valid arguments : "inout", "uni", "multi"*/
    std::list<std::pair<int, std::string>> dijkstra(std::weak_ptr<Vertex> from, std::weak_ptr<Vertex> to, char vehicleType, std::string mode);


public:

    /*Constructor
     * config - list of <string, int> pairs representing configuration data,
     * i.e. <"bus",6> means bus_stop_time will be overwritten with 6 etc.
     * valid arguments in config : "bus", "tram", "sprinter", "rail", "stad", "intercity", "centraal"
     * throws : runtime_error if some information in the config list is invalid*/
    Graph(std::list<std::pair<std::string, int>> config);

    /*Destructor*/
    ~Graph();

    /*Disabled copy constructor (graph is singular)*/
    Graph(const Graph& rhs) = delete;

    /*Disabled copy-assignment operator (graph is singular)*/
    Graph& operator=(const Graph& rhs) = delete;

    /*Disabled move constructor (graph is singular)*/
    Graph(Graph&& rhs) = delete;

    /*Disabled move-assignment operator (graph is singular)*/
    Graph& operator=(Graph&& rhs) = delete;

    /*Adds a new route between two vertices (stations), for a certain vehicle type;
     * If one or more of the vertices in the new route don't already exist in the graph, they are added
     *
     * weight - route travel time (int minutes)
     * from - "from" station
     * to - "to" station
     * vehicleType - type of vehicle that can travel this route, valid arguments:
     * b (bus), t (tram), s (sprinter), r (rail)
     *
     * throws : runtime_error if any argument is invalid*/
    void addEdge(int weight, std::shared_ptr<Vertex> from, std::shared_ptr<Vertex> to, char vehicleType);

    /*Returns all the stations accessible outbound from a certain station, by all means of transportation
     *
     * stationName - name of given station
     *
     * throw : runtime_error if station is not in network*/
    void outbound(std::string stationName);

    /*Returns all the stations from which a certain station is accessible, by all means of transportation
     *
     * stationName - name of given station
     *
     * throw : runtime_error if station is not in network*/
    void inbound(std::string stationName);

    /*Displays the duration of the shortest route from one station to another, by all means of transportation
     *
     * from - "from" station
     * to - "to" station
     *
     * throws : runtime_error if one or both station are not in the network*/
    void uniExpress(std::string from, std::string to);

    /*Displays the duration of the shortest overall route from one station to another
     *
     * from - "from" station
     * to - "to" station
     *
     * throws : runtime_error if one or both station are not in the network*/
    void multiExpress(std::string from, std::string to);

    /*Output for every station in the network, all the stations accessible from it, by all means of transportation
     *
     * fileName - name of output file*/
    void print(std::string fileName);

    /*Adds a new route between two vertices (stations), for a certain vehicle type;
     * If one or more of the vertices in the new route don't already exist in the graph, they are added
     *
     * weight - route travel time (int minutes)
     * from - "from" station
     * to - "to" station
     * vehicleType - type of vehicle that can travel this route, valid arguments:
     * b (bus), t (tram), s (sprinter), r (rail)
     *
     * throws : runtime_error if any argument is invalid*/
    void addEdge(int weight, std::string from, char from_type, std::string to, char to_type, char vehicleType);

};


#endif //HW_2_GRAPH_H
