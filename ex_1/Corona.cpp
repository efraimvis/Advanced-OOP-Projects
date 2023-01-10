//
// Created by Admin on 4/19/2022.
//

#include "Corona.h"
#include <random>
#include <ctime>

const double Corona::getMatchStrength(const std::string target)
{
    unsigned int matches = 0;
    for(unsigned int i = 0;i<target.length();i++)
    {
        if(idString[i] == target[i]) matches++;
    }

    double strength = (double)((double)matches/(double)target.length());
    if(matches == target.length()) {throw matchFoundException(getType(),idString);}

    return strength;
}

void Alpha::mutate()
{
    //std::srand( ( unsigned int )std::time( 0 ) );
    for(unsigned int i = 0;i<idString.length();i++)
    {
        if(!(idString[i] == 'T' || idString[i] == 'A')) continue;

        std::random_device rd;     // only used once to initialise (seed) engine
        std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
        std::uniform_int_distribution<int> uni(0,100); // guaranteed unbiased

        auto random_integer = uni(rng);
        if(random_integer < 37)
        {
            if(idString[i] == 'T') idString[i] = 'A';
            else idString[i] = 'T';
        }

        /**
        int random_integer = rand()%101;
        if(random_integer < 37)
        {
            if(idString[i] == 'T') idString[i] = 'A';
            else idString[i] = 'T';
        }
        **/
    }
}

void Delta::mutate()
{
    //std::srand( ( unsigned int )std::time( 0 ) );
    for(unsigned int i = 0;i<idString.length();i++)
    {
        if(!(idString[i] == 'A' || idString[i] == 'C' || idString[i] == 'G')) continue;

        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(0,idString.length());

        auto random_integer = uni(rng);
        if(random_integer == 0)
        {
            if(idString[i] == 'A') idString[i] = 'T';
            else if(idString[i] == 'C') idString[i] = 'G';
            else idString[i] = 'C';
        }

        /**
        int random_integer = rand()%(idString.length()+1);
        if(random_integer == 0)
        {
            if(idString[i] == 'A') idString[i] = 'T';
            else if(idString[i] == 'C') idString[i] = 'G';
            else idString[i] = 'C';
        }
         **/
    }
}

void Omicron::mutate()
{
    //std::srand( ( unsigned int )std::time( 0 ) );
    for(unsigned int i = 0;i<idString.length();i++)
    {
        if(!(idString[i] == 'T' || idString[i] == 'C' || idString[i] == 'G')) continue;

        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(0,idString.length());

        unsigned int random_integer = uni(rng);
        if(random_integer == 0 || random_integer == idString.length()-1)
        {
            if(idString[i] == 'T') idString[i] = 'A';
            else if(idString[i] == 'C') idString[i] = 'G';
            else idString[i] = 'C';
        }

        /**
        int random_integer = rand()%(idString.length()+1);
        if(random_integer == 0 || random_integer == idString.length()-1)
        {
            if(idString[i] == 'T') idString[i] = 'A';
            else if(idString[i] == 'C') idString[i] = 'G';
            else idString[i] = 'C';
        }
         **/
    }
}

Corona::~Corona(){}