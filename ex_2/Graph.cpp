//
// Created by Admin on 5/25/2022.
//

#include <set>
#include "Graph.h"
#include <limits>
#include <iostream>
#include <sstream>
#include <fstream>

Graph::Graph(std::list<std::pair<std::string, int>> config)
{
    if(!config.empty())
    {
        std::set<std::string> used = std::set<std::string>();
        for(std::list<std::pair<std::string, int>>::iterator it = config.begin(); it != config.end(); ++it)
        {
            std::string text = (*it).first;
            int val = (*it).second;
            if(text != "bus" && text != "tram" && text != "rail" && text != "sprinter" && text != "intercity" && text != "stad" && text != "centraal")
                throw std::runtime_error("ERROR: unknown vehicle/station type");
            if(val <= 0) throw std::runtime_error("ERROR: non-positive wait/transit time");

            if(used.count(text) != 0) throw std::runtime_error("ERROR: attempting to change configured value");

            if(text == "bus") bus_stop_time = val;
            if(text == "tram") tram_stop_time = val;
            if(text == "sprinter") sprinter_stop_time = val;
            if(text == "rail") rail_stop_time = val;

            if(text == "stad") stad_transit_time = val;
            if(text == "intercity") intercity_transit_time = val;
            if(text == "centraal") centraal_transit_time = val;

            used.insert(text);
        }
    }
    vertices = std::vector<std::shared_ptr<Vertex>>();
}

Graph::~Graph(){}

int Graph::getVehicleStopTime(const char vehicleType)
{
    switch(vehicleType)
    {
        case 'b':
            return bus_stop_time;
        case 't':
            return tram_stop_time;
        case 's':
            return sprinter_stop_time;
        case 'r':
            return rail_stop_time;
    }
    throw std::runtime_error("getVehicleType: invalid vehicle type");
}


void Graph::addEdge(int weight, std::shared_ptr<Vertex> from, std::shared_ptr<Vertex> to, char vehicleType)
{
    if(weight <= 0) throw std::runtime_error("ERROR: non positive travel time");
    if(from == nullptr) throw std::runtime_error("ERROR: \"from\" vertex is null");
    if(to == nullptr) throw std::runtime_error("ERROR: \"to\" vertex is null");
    if(from == to) throw std::runtime_error("ERROR: cannot add loop from vertex to itself");

    if(vehicleType != 'b' && vehicleType != 't' && vehicleType != 's' && vehicleType != 'r') throw std::runtime_error("ERROR: unknown vehicle type");

    std::shared_ptr<Vertex> from_ptr = nullptr;
    std::shared_ptr<Vertex> to_ptr = nullptr;

    for(std::vector<std::shared_ptr<Vertex>>::iterator it = vertices.begin(); it != vertices.end() ; ++it)
    {
        if((*it) == from)
        {
            from_ptr = (*it);
        }
        else if((*it)->getId() == from->getId())
        {
            throw std::runtime_error("ERROR: cannot add a different vertex with the same name");
        }
    }

    for(std::vector<std::shared_ptr<Vertex>>::iterator it = vertices.begin(); it != vertices.end() ; ++it)
    {
        if((*it) == to)
        {
            to_ptr = (*it);
        }
        else if((*it)->getId() == to->getId())
        {
            throw std::runtime_error("ERROR: cannot add a different vertex with the same name");
        }
    }

    if(from_ptr == nullptr)
    {
        vertices.push_back(from);
    }

    if(to_ptr == nullptr)
    {
        vertices.push_back(to);
    }

    from->addOutNeighbor(weight,to,vehicleType);
    to->addInNeighbor(weight,from,vehicleType);
}


int Graph::getEdgeWeight(int u, int v, char vehicleType, char inOutControl)
{
    int retval = 0;
    std::list<std::pair<int, std::weak_ptr<Vertex>>> uNeighbors;
    if(inOutControl == 'i') uNeighbors = vertices[u]->getInNeighbors(vehicleType);
    if(inOutControl == 'o') uNeighbors = vertices[u]->getOutNeighbors(vehicleType);

    for(std::list<std::pair<int, std::weak_ptr<Vertex>>>::iterator it = uNeighbors.begin(); it != uNeighbors.end(); ++it)
    {
        if((*it).second.lock() == vertices[v])
            return (*it).first;
    }
    return retval;
}

std::list<std::pair<int, std::string>> Graph::dijkstra(std::weak_ptr<Vertex> from, std::weak_ptr<Vertex> to, char vehicleType, std::string mode)
{
    if(from.expired() || to.expired()) throw std::runtime_error("ERROR: null pointer to vertex");

    int start = -1; //index of "from" in "vertices"
    for(unsigned int i = 0; i < vertices.size(); i++)
    {
        if(from.lock() == vertices[i])
        {
            start = i;
            break;
        }
    }
    if(start == -1) throw std::runtime_error("ERROR: vertex not in graph");

    if(vehicleType != 'b' && vehicleType != 't' && vehicleType != 's' && vehicleType != 'r') throw std::runtime_error("ERROR: unknown vehicle type");
    if(mode != "in" && mode != "out" && mode != "uni" && mode != "multi") throw std::runtime_error("ERROR: unknown mode");

    char inout = (mode == "in")?'i':'o';

    // algorithm starts
    int dist[vertices.size()];
    int prev[vertices.size()];
    std::set<int> Q = std::set<int>();

    for(unsigned int i = 0; i < vertices.size(); i++)
    {
        dist[i] = std::numeric_limits<int>::max();
        prev[i] = -1;
        Q.insert(i);
    }
    dist[start] = 0;

    char selectedVehicles[vertices.size()];
    for(unsigned int i = 0; i < vertices.size(); i++)
    {
        selectedVehicles[i] = 'q';
    }

    while(!Q.empty())
    {
        int u = *(Q.begin());
        for(std::set<int>::iterator i = Q.begin(); i != Q.end(); ++i) //select vertex from Q with minimum distance
        {
            if(dist[(*i)] < dist[u]) u = (*i);
        }

        int posInQ = *(Q.find(u));
        Q.erase(posInQ);

        for(int v : Q)
        {
            int w;
            if(mode != "multi") w = getEdgeWeight(u,v,vehicleType,inout);
            else
            {
                //Get minimum edge weight by vehicle type
                int busWeight = getEdgeWeight(u,v,'b','o');
                int tramWeight = getEdgeWeight(u,v,'t','o');
                int sprinterWeight = getEdgeWeight(u,v,'s','o');
                int railWeight = getEdgeWeight(u,v,'r','o');

                int weights[] = {busWeight, tramWeight, sprinterWeight, railWeight};
                char selected = 'n';

                w = weights[0];

                int minWeightIndex = -1;
                int totalEdgeWeight = 0;
                for(int i = 0; i < 4; i++)
                {
                    totalEdgeWeight += weights[i];
                }

                if(totalEdgeWeight == 0) w = 0;
                else
                {
                    for(int i = 0; i < 4; i++)
                    {
                        if(weights[i] == 0) continue;
                        if(w == 0)
                        {
                            w = weights[i];
                            minWeightIndex = i;
                        }
                        else if(weights[i] <= w)
                        {
                            w = weights[i] = w;
                            minWeightIndex = i;
                        }
                    }
                }

                if(minWeightIndex > -1)
                {
                    switch(minWeightIndex)
                    {
                        case 0:
                            selected = 'b';
                            break;
                        case 1:
                            selected = 't';
                            break;
                        case 2:
                            selected = 's';
                            break;
                        case 3:
                            selected = 'r';
                            break;
                    }
                }

                selectedVehicles[v] = selected;
            }
            if(w == 0) continue;

            int alt = dist[u] + w;
            if(alt < dist[v])
            {
                dist[v] = alt;
                prev[v] = u;
            }
        }
    }
    std::list<std::pair<int, std::string>> retval = std::list<std::pair<int, std::string>>();

    if(mode == "in" || mode == "out")
    {
        for(unsigned int i = 0; i < vertices.size(); i++)
        {
            if(dist[i] < std::numeric_limits<int>::max() && dist[i] > 0) retval.push_back(std::pair<int, std::string>(dist[i], vertices[i]->getId()));
        }

        return retval;
    }

    if(mode == "uni")
    {
        int end = -1;
        for(unsigned int i = 0; i < vertices.size(); i++)
        {
            if(vertices[i]->getId() == to.lock()->getId())
            {
                end = i;
                break;
            }
        }
        if(end == -1) throw std::runtime_error("ERROR: vertex not in graph");

        if(dist[end] == std::numeric_limits<int>::max() || dist[end] <= 0) return retval;

        int totalStopTime = 0;
        int stopTime = getVehicleStopTime(vehicleType);
        int stop = end;
        while(stop != start)
        {
            if(stop == -1) break;
            stop = prev[stop];
            if(stop != end && stop != start) totalStopTime += stopTime;
        }

        retval.push_back(std::pair<int, std::string>(dist[end]+totalStopTime, vertices[end]->getId()));
        return retval;
    }

    if(mode == "multi")
    {
        int end = -1;
        for(unsigned int i = 0; i < vertices.size(); i++)
        {
            if(vertices[i]->getId() == to.lock()->getId())
            {
                end = i;
                break;
            }
        }
        if(end == -1) throw std::runtime_error("ERROR: vertex not in graph");

        if(dist[end] == std::numeric_limits<int>::max() || dist[end] <= 0) return retval;

        int totalTransitTime = 0;
        int stop = end;
        char prevVehicleType = selectedVehicles[stop];
        while(stop != start)
        {
            stop = prev[stop];
            char vertexType = vertices[stop]->getType();
            int stopTransitTime = 0;
            switch(vertexType)
            {
                case 's':
                    stopTransitTime = stad_transit_time;
                    break;
                case 'i':
                    stopTransitTime = intercity_transit_time;
                    break;
                case 'c':
                    stopTransitTime = centraal_transit_time;
                    break;
            }

            if(stop != end && stop != start)
            {
                if(selectedVehicles[stop] != prevVehicleType)
                {
                    totalTransitTime += stopTransitTime;
                }
                prevVehicleType = selectedVehicles[stop];
            }
        }

        retval.push_back(std::pair<int, std::string>(dist[end]+totalTransitTime, vertices[end]->getId()));
        return retval;
    }
    return retval;
}

void Graph::inoutbound(std::string station, char inout)
{
    std::string bound = inout=='i'?"in":"out";
    std::weak_ptr<Vertex> vertex;
    for(unsigned int i = 0; i< vertices.size(); i++)
    {
        if(vertices[i]->getId() == station)
        {
            vertex = vertices[i];
            break;
        }
    }
    if(vertex.expired())
    {
        std::stringstream err;
        err<<station<<" does not exist in the current network. ";
        throw std::runtime_error(err.str());
    }

    char controls[] = {'b','t', 's', 'r'};
    for(int i = 0; i< 4 ;i++)
    {
        std::string vehicleType;
        std::list<std::pair<int, std::string>> transport = dijkstra(vertex, vertex ,controls[i],bound);
        switch(i)
        {
            case 0:
                vehicleType = "bus: ";
                break;
            case 1:
                vehicleType = "tram: ";
                break;
            case 2:
                vehicleType = "sprinter: ";
                break;
            case 3:
                vehicleType = "rail: ";
                break;
        }
        std::cout<<vehicleType;
        if(transport.empty())
        {
            std::cout<<"no "<<bound<<"bound travel"<<std::endl;
            continue;
        }
        for(std::list<std::pair<int, std::string>>::iterator it = transport.begin(); it != transport.end(); ++it)
        {
            if(it != transport.begin()) std::cout<<"    ";
            std::cout<<(*it).second;
        }
        std::cout<<std::endl;
    }
}

void Graph::outbound(std::string stationName)
{
    try
    {
        inoutbound(stationName, 'o');
    }
    catch(std::runtime_error& e){throw;}
}

void Graph::inbound(std::string stationName)
{
    try
    {
        inoutbound(stationName, 'i');
    }
    catch(std::runtime_error& e){throw;}
}

void Graph::uniExpress(std::string from, std::string to)
{
    std::weak_ptr<Vertex> from_ptr;
    std::weak_ptr<Vertex> to_ptr;
    for(unsigned int i = 0; i< vertices.size(); i++)
    {
        if(vertices[i]->getId() == from)
        {
            from_ptr = vertices[i];
            break;
        }
    }
    for(unsigned int i = 0; i< vertices.size(); i++)
    {
        if(vertices[i]->getId() == to)
        {
            to_ptr = vertices[i];
            break;
        }
    }

    if(from_ptr.expired())
    {
        std::stringstream err;
        err<<from<<" does not exist in the current network. ";
        throw std::runtime_error(err.str());
    }
    if(to_ptr.expired())
    {
        std::stringstream err;
        err<<to<<" does not exist in the current network. ";
        throw std::runtime_error(err.str());
    }

    char controls[] = {'b','t', 's', 'r'};
    for(int i = 0; i< 4 ;i++)
    {
        std::string vehicleType;
        std::list<std::pair<int, std::string>> transport = dijkstra(from_ptr, to_ptr ,controls[i],"uni");
        switch(i)
        {
            case 0:
                vehicleType = "bus: ";
                break;
            case 1:
                vehicleType = "tram: ";
                break;
            case 2:
                vehicleType = "sprinter: ";
                break;
            case 3:
                vehicleType = "rail: ";
                break;
        }
        std::cout<<vehicleType;
        if(transport.empty())
        {
            std::cout<<"route unavailable"<<std::endl;
            continue;
        }
        for(std::list<std::pair<int, std::string>>::iterator it = transport.begin(); it != transport.end(); ++it)
        {
            if(it != transport.begin()) std::cout<<"    ";
            std::cout<<(*it).first;
        }
        std::cout<<std::endl;
    }
}

void Graph::multiExpress(std::string from, std::string to)
{
    std::weak_ptr<Vertex> from_ptr;
    std::weak_ptr<Vertex> to_ptr;
    for(unsigned int i = 0; i< vertices.size(); i++)
    {
        if(vertices[i]->getId() == from)
        {
            from_ptr = vertices[i];
            break;
        }
    }
    for(unsigned int i = 0; i< vertices.size(); i++)
    {
        if(vertices[i]->getId() == to)
        {
            to_ptr = vertices[i];
            break;
        }
    }

    if(from_ptr.expired())
    {
        std::stringstream err;
        err<<from<<" does not exist in the current network. ";
        throw std::runtime_error(err.str());
    }
    if(to_ptr.expired())
    {
        std::stringstream err;
        err<<to<<" does not exist in the current network. ";
        throw std::runtime_error(err.str());
    }

    std::list<std::pair<int, std::string>> transport = dijkstra(from_ptr, to_ptr ,'t',"multi");
    if(transport.empty())
    {
        std::cout<<"route unavailable"<<std::endl;
        return;
    }

    std::cout<<"shortest route from "<<from<<" to "<<to<<" takes: ";

    for(std::list<std::pair<int, std::string>>::iterator it = transport.begin(); it != transport.end(); ++it)
    {
        std::cout<<(*it).first;
    }
    std::cout<<" minutes"<<std::endl;
}

void Graph::print(std::string fileName)
{
    std::ofstream file(fileName.c_str());
    if(!file.is_open()) throw std::runtime_error("ERROR: could not open file");
    for(unsigned int i = 0; i < vertices.size(); i++)
    {
        file<<"from "<<vertices[i]->getId()<<std::endl;

        char controls[] = {'b','t', 's', 'r'};
        for(int j = 0; j< 4 ;j++)
        {
            std::string vehicleType;
            std::list<std::pair<int, std::string>> transport = dijkstra(vertices[i], vertices[i] ,controls[j],"out");
            if(transport.empty()) continue;
            switch(j)
            {
                case 0:
                    vehicleType = "bus: ";
                    break;
                case 1:
                    vehicleType = "tram: ";
                    break;
                case 2:
                    vehicleType = "sprinter: ";
                    break;
                case 3:
                    vehicleType = "rail: ";
                    break;
            }
            file<<vehicleType;
            for(std::list<std::pair<int, std::string>>::iterator it = transport.begin(); it != transport.end(); ++it)
            {
                if(it != transport.begin()) file<<"    ";
                file<<(*it).second;
            }
            file<<std::endl;
        }
        file<<"_______________________________________________________\n"<<std::endl;
    }
    file.close();
}

void Graph::addEdge(int weight, std::string from, char from_type, std::string to, char to_type, char vehicleType)
{
    std::shared_ptr<Vertex> from_ptr;
    std::shared_ptr<Vertex> to_ptr;
    for(unsigned int i = 0; i< vertices.size(); i++)
    {
        if(vertices[i]->getId() == from)
        {
            from_ptr = vertices[i];
            break;
        }
    }
    for(unsigned int i = 0; i< vertices.size(); i++)
    {
        if(vertices[i]->getId() == to)
        {
            to_ptr = vertices[i];
            break;
        }
    }

    if(from_ptr == nullptr)
    {
        from_ptr = std::shared_ptr<Vertex>(new Vertex(from, from_type));
    }
    if(to_ptr == nullptr)
    {
        to_ptr = std::shared_ptr<Vertex>(new Vertex(to, to_type));
    }

    addEdge(weight, from_ptr, to_ptr, vehicleType);
}