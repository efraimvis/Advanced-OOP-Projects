//
// Created by Admin on 4/19/2022.
//

#ifndef HW_1_FOREFATHER_H
#define HW_1_FOREFATHER_H


#include <string>

class Forefather
{
private:
    /*Handles reference counting*/
    struct Value
    {
        int refCount;
        std::string data;
        Value(const std::string initValue):refCount(1)
        {
            data = initValue;
        }
        ~Value()
        {
        }
    };

    Value* value;
public:
    /*Constructor
     * init - initial string value*/
    Forefather(const std::string init);

    /*Copy constructor*/
    Forefather(const Forefather& rhs);

    /*Destructor*/
    ~Forefather();

    /*Returns string value*/
    const std::string getForeString() const{return value->data;}

    /*operator =*/
    Forefather& operator=(const Forefather& rhs);

    /*Returns number of reference to this object*/
    const int getRefCount() const{return value->refCount;}
};


#endif //HW_1_FOREFATHER_H
