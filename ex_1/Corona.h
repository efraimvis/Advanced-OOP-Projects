//
// Created by Admin on 4/19/2022.
//

#ifndef HW_1_CORONA_H
#define HW_1_CORONA_H

#include <sstream>
#include "Forefather.h"

class Corona
{
private:
    Forefather forefather;

protected:
    std::string idString;
public:

    /*Exception for virus match with target string*/
    struct matchFoundException : public std::exception
    {
        std::string matchingString;
        /*Constructor
         * virusType - type of virus
         * virusId - id string of virus*/
        matchFoundException(const char virusType, const std::string virusId)
        {
            std::stringstream ss;
            ss<<virusType<<" ";
            for(unsigned int i = 0;i<(virusId.length()-1);i++) ss<<virusId[i]<<" ";
            ss<<virusId[virusId.length()-1]<<std::endl;
            matchingString = ss.str();
        }

        /*Returns type and id of virus that matches target string*/
        const std::string getMessage() const{return matchingString;}

        ~matchFoundException()throw(){};
    };

    /*Getter for virus id*/
    const std::string getId() const{return idString;}

    /*Setter for virus id*/
    void setId(const std::string id){this->idString = id;}

    /*Constructor
     * f - forefather of virus*/
    Corona(Forefather& f): forefather(f),idString(f.getForeString()){}

    /*Destructor*/
    virtual ~Corona();

    /*Mutates virus according to prescribed rules*/
    virtual void mutate(){};

    /*Returns virus type*/
    virtual char getType()=0;

    /*Returns match strength of virus id to given target string
     * target - given target string
     * return : Returns real number between 0 and 1, where 1 represents full match*/
    const double getMatchStrength (const std::string target);
};

/*Derived class of Corona virus, representing Alpha strain*/
class Alpha : public Corona
{
public:
    /*Constructor
     * f - forefather for virus*/
    Alpha(Forefather& f) : Corona(f){};

    /*Destructor*/
    ~Alpha(){}

    void mutate();
    char getType(){return 'a';};
};

/*Derived class of Corona virus, representing Delta strain*/
class Delta : public Corona
{
public:
    /*Constructor
     * f - forefather for virus*/
    Delta(Forefather& f) : Corona(f){};

    /*Destructor*/
    ~Delta(){}
    void mutate();
    char getType(){return 'd';};
};

/*Derived class of Corona virus, representing Omicron strain*/
class Omicron : public Corona
{
public:
    /*Constructor
     * f - forefather for virus*/
    Omicron(Forefather& f) : Corona(f){};

    /*Destructor*/
    ~Omicron(){}
    void mutate();
    char getType(){return 'o';};
};

#endif //HW_1_CORONA_H
