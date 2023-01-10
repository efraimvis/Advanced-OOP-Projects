//
// Created by Admin on 7/12/2022.
//

#include <stdexcept>
#include "View.h"
#include "SimObject.h"
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;

View::View()
{
    this->size = 25;
    this->scale = 2;
    origin = {0,0};
    objects = std::map<std::string, std::pair<float,float >>();
}

void View::resetDefault()
{
    this->size = 25;
    this->scale = 2;
    origin = {0,0};
}

void View::setOrigin(std::pair<float, float> newOrigin)
{
    origin = newOrigin;
}

void View::setScale(float newScale)
{
    if(scale <= 0) throw std::runtime_error("ERROR: map scale must be a positive number");
    this->scale = newScale;
}

void View::setSize(int newSize)
{
    if(!(6 < newSize && newSize <= 30)) throw std::runtime_error("ERROR: map size must be between 7 and 30");
    this->size = newSize;
}

void View::insertObject(std::string name, std::pair<float, float> position)
{
    if(objects.count(name) == 0)
        objects.emplace(name, position);
    else
        objects.find(name)->second = position;
}

void View::printView()
{
    cout<<"Display size: "<<size<<", scale: "<<std::setprecision(2)<<std::fixed<<scale<<", origin: "<<locationString(origin)<<endl;
    std::string board[size][size];
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            board[i][j] = ". ";
        }
    }

    //place all relevant objects on the board in correct spots
    for(auto object : objects)
    {
        int xCoord = (int)(((object.second.first)-origin.first)/scale);
        int yCoord = size-(int)(((object.second.second)-origin.second)/scale) -1;

        if(xCoord < 0 || yCoord < 0 || xCoord >= size || yCoord >= size) continue;

        board[yCoord][xCoord] = object.first.substr(0,2);
    }

    //print board
    for(int i = 0; i < size; i++)
    {
        if(i%3 == 0)
        {
            std::stringstream ss;
            ss << std::setw(4) << std::setfill(' ') << (origin.second + (size-i-1)*scale);
            cout<<ss.str()<<" ";
        }
        else cout<<"     ";

        for(int j = 0; j < size; j++)
        {
            cout<<board[i][j];
        }
        cout<<endl;
    }

    cout<<" ";
    for(int j = 0; j < size; j++)
    {
        if(j%3 == 0)
        {
            std::stringstream ss;
            ss << std::setw(5) << std::setfill(' ') << (origin.second + j*scale);
            cout<<ss.str()<<" ";
        }
    }
    cout<<endl;
}