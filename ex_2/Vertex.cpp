//
// Created by Admin on 5/19/2022.
//

#include <iostream>
#include "Vertex.h"

/*Constructor
 * id - station ID
 * type - station type, one of the following is allowed:
 * i(Intercity), c(Centraal), s(Stad)*/
Vertex::Vertex(const std::string id, char type):id(id)
{
    if(type != 'i' && type != 'c' && type != 's') throw std::runtime_error("ERROR: invalid station type");
    this->type = type;
}

/*Getter for out-vertex's neighbors, BY THE TYPE OF VEHICLE THAT CAN REACH THEM
 * control - char representing the type of vehicle that can reach the neighbor,
 * valid arguments - b (bus), t (tram), s (sprinter), r (rail)*/
std::list<std::pair<int, std::weak_ptr<Vertex>>> Vertex::getOutNeighbors(char control) const
{
    if(control != 'b' && control != 't'  && control != 's' && control != 'r') throw std::runtime_error("ERROR: invalid vehicle type");

    int index;
    switch(control)
    {
        case 'b':
            index = 0;
            break;
        case 't':
            index = 1;
            break;
        case 's':
            index = 2;
            break;
        case 'r':
            index = 3;
            break;
    }
    return outNeighbors[index];
}

/*Getter for vertex's in-neighbors, BY THE TYPE OF VEHICLE THAT CAN REACH THEM
 * control - char representing the type of vehicle that can reach the neighbor,
 * valid arguments - b (bus), t (tram), s (sprinter), r (rail)*/
std::list<std::pair<int, std::weak_ptr<Vertex>>> Vertex::getInNeighbors(char control) const
{
    if(control != 'b' && control != 't'  && control != 's' && control != 'r') throw std::runtime_error("ERROR: invalid vehicle type");

    int index;
    switch(control)
    {
        case 'b':
            index = 0;
            break;
        case 't':
            index = 1;
            break;
        case 's':
            index = 2;
            break;
        case 'r':
            index = 3;
            break;
    }
    return inNeighbors[index];
}

void Vertex::addNeighbor(std::list<std::pair<int, std::weak_ptr<Vertex>>> neighborList[],int weight, std::weak_ptr<Vertex> other, const char control)
{
    if(weight <= 0) throw std::runtime_error("ERROR: non-positive weight");
    if(other.expired()) throw std::runtime_error("ERROR: null pointer to neighbors");
    if(control != 'b' && control != 't' && control != 's' && control != 'r') throw std::runtime_error("ERROR: unknown control character");

    int index;
    std::shared_ptr<Vertex> newNeighbor = other.lock();

    switch(control)
    {
        case 'b':
            index = 0;
            break;
        case 't':
            index = 1;
            break;
        case 's':
            index = 2;
            break;
        case 'r':
            index = 3;
            break;
    }

    for(std::list<std::pair<int, std::weak_ptr<Vertex>>>::iterator it = neighborList[index].begin(); it != neighborList[index].end(); ++it)
    {
        std::shared_ptr<Vertex> listElement = (*it).second.lock();

        if(listElement->getId() == newNeighbor->getId())
        {
            if(newNeighbor->getType() != listElement->getType()) throw std::runtime_error("ERROR: attempt to change station type");
            if(weight < (*it).first)
            {
                it->first = weight;
            }
            return;
        }
    }

    neighborList[index].push_back(std::pair<int, std::weak_ptr<Vertex>>(weight, other));
}

/*Adds a new neighbor (out edge) to the vertex
 * weight - travel time (in minutes)
 * other - reference to the neighboring vertex
 * control - determines the type of edge being added:
 * valid arguments - b (bus), t (tram), s (sprinter), r (rail)*/
void Vertex::addOutNeighbor(int weight, std::weak_ptr<Vertex> other, const char control)
{
    try
    {
        addNeighbor(outNeighbors,weight, other,control);
    }
    catch(std::runtime_error& e){throw;}
    /**
    if(weight <= 0) throw std::runtime_error("ERROR: non-positive weight");
    if(other.expired()) throw std::runtime_error("ERROR: null pointer to neighbors");
    if(control != 'b' && control != 't' && control != 's' && control != 'r') throw std::runtime_error("ERROR: unknown control character");

    int index;
    std::shared_ptr<Vertex> newNeighbor = other.lock();

    switch(control)
    {
        case 'b':
            index = 0;
            break;
        case 't':
            index = 1;
            break;
        case 's':
            index = 2;
            break;
        case 'r':
            index = 3;
            break;
    }

    for(std::list<std::pair<int, std::weak_ptr<Vertex>>>::iterator it = outNeighbors[index].begin(); it != outNeighbors[index].end(); ++it)
    {
        std::shared_ptr<Vertex> listElement = (*it).second.lock();

        if(listElement->getId() == newNeighbor->getId())
        {
            if(newNeighbor->getType() != listElement->getType()) throw std::runtime_error("ERROR: attempt to change station type");
            if(weight < it->first)
            {
                it->first = weight;
                return;
            }
        }
    }

    outNeighbors[index].push_back(std::pair<int, std::weak_ptr<Vertex>>(weight, other));
     **/
}

/*Adds a new neighbor (in edge) to the vertex
     * weight - travel time (in minutes), must be positive
     * other - reference to the neighboring vertex
     * control - determines the type of edge being added:
     * valid arguments - b (bus), t (tram), s (sprinter), r (rail)*/
//void addInNeighbor(double weight, Vertex& other, const char control);
void Vertex::addInNeighbor(int weight, std::weak_ptr<Vertex> other, const char control)
{
    try
    {
        addNeighbor(inNeighbors,weight, other,control);
    }
    catch(std::runtime_error& e){throw;}
}