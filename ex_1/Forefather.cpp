//
// Created by Admin on 4/19/2022.
//

#include "Forefather.h"

Forefather::Forefather(const std::string init):value(new Value(init)){}

Forefather::Forefather(const Forefather &rhs):value(rhs.value)
{
    ++value->refCount;
}

Forefather::~Forefather()
{
    if(--value->refCount == 0) delete value;
}

Forefather& Forefather::operator=(const Forefather& rhs)
{
    if(value == rhs.value) return *this;

    if(--value->refCount == 0) delete value;

    value = rhs.value;
    ++value->refCount;
    return *this;
}


