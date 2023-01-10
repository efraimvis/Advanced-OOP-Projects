//
// Created by Admin on 5/19/2022.
//

#ifndef HW_2_VERTEX_H
#define HW_2_VERTEX_H


#include <string>
#include <list>
#include <memory>

class Vertex
{
private:
    const std::string id;
    char type;
    std::list<std::pair<int, std::weak_ptr<Vertex>>> outNeighbors[4];
    std::list<std::pair<int, std::weak_ptr<Vertex>>> inNeighbors[4];

    void addNeighbor(std::list<std::pair<int, std::weak_ptr<Vertex>>> neighborList[],int weight, std::weak_ptr<Vertex> other, const char control);

public:
    /*Constructor
     * id - station ID
     * type - station type, one of the following is allowed:
     * i(Intercity), c(Centraal), s(Stad)*/
    Vertex(const std::string id, char type);

    /*Constructor
     * Generates a vertex of an appropriate type based on ID prefix
     * id - station ID, must be prefixed by:
     * nothing (stad), "IC" (intercity), "CS" (centraal)*/
    //Vertex(const std::string id);

    /*Destructor*/
    ~Vertex(){}

    /*Disabled copy constructor (each vertex is singular)*/
    Vertex(const Vertex& rhs) = delete;

    /*Disabled copy-assignment operator (each vertex is singular)*/
    Vertex& operator=(const Vertex& rhs) = delete;

    /*Disabled move constructor (each vertex is singular)*/
    Vertex(Vertex&& rhs) = delete;

    /*Disabled move-assignment operator (each vertex is singular)*/
    Vertex& operator=(Vertex&& rhs) = delete;

    /*Getter for Vertex id*/
    const std::string getId() const{return id;}

    /*Getter for Vertex type*/
    const char getType() const{return type;};

    /*Adds a new out-neighbor (out edge) to the vertex
     * weight - travel time (in minutes), must to positive
     * other - reference to the neighboring vertex
     * control - determines the type of edge being added:
     * valid arguments - b (bus), t (tram), s (sprinter), r (rail)*/
    //void addOutNeighbor(double weight, Vertex& other, const char control);
    void addOutNeighbor(int weight, std::weak_ptr<Vertex> other, const char control);

    /*Adds a new in-neighbor (in edge) to the vertex
     * weight - travel time (in minutes), must be positive
     * other - reference to the neighboring vertex
     * control - determines the type of edge being added:
     * valid arguments - b (bus), t (tram), s (sprinter), r (rail)*/
    //void addInNeighbor(double weight, Vertex& other, const char control);
    void addInNeighbor(int weight, std::weak_ptr<Vertex> other, const char control);

    /*Getter for vertex's out-neighbors, BY THE TYPE OF VEHICLE THAT CAN REACH THEM
     * control - char representing the type of vehicle that can reach the neighbor,
     * valid arguments - b (bus), t (tram), s (sprinter), r (rail)*/
    std::list<std::pair<int, std::weak_ptr<Vertex>>> getOutNeighbors(char control) const;

    /*Getter for vertex's in-neighbors, BY THE TYPE OF VEHICLE THAT CAN REACH THEM
     * control - char representing the type of vehicle that can reach the neighbor,
     * valid arguments - b (bus), t (tram), s (sprinter), r (rail)*/
    std::list<std::pair<int, std::weak_ptr<Vertex>>> getInNeighbors(char control) const;
};


#endif //HW_2_VERTEX_H
