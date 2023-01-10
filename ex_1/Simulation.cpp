//
// Created by Admin on 4/20/2022.
//

#include <random>
#include <algorithm>
#include <iostream>
#include "Simulation.h"

Simulation::~Simulation()
{
    for(Corona* c : population) delete c;
}

bool Simulation::cmpStrength(Corona *first, Corona *last)
{
    return(first->getMatchStrength(target) < last->getMatchStrength(target));
}

void Simulation::readConfig(const char* config)
{
    if(!config) throw FileNotFoundException("NULL STRING");
    std::ifstream file(config);
    if(!file.is_open()) throw FileNotFoundException(config);

    std::string line;
    int lineCounter = 1;

    if(!getline(file, line))throw FileParameterException(config ,lineCounter,"File empty");
    int id_size = atoi(line.c_str());
    if(!(id_size >= 3 && id_size <= 1000)) throw FileParameterException(config ,lineCounter,"Invalid IdString dimension");
    this->idSize = id_size;
    lineCounter++;

    if(!getline(file, line))throw FileParameterException(config ,lineCounter,"Invalid number of parameters");
    std::stringstream currentLine(line);
    std::stringstream targetString;
    std::string token;
    int j = 0;
    while(getline(currentLine, token, ' '))
    {
        if(j >= id_size)
        {
            throw FileParameterException(config, lineCounter, "Invalid number of characters in target string");
        }
        if(token != "T" && token != "A" && token != "C" && token != "G")
        {
            if(j < id_size-1 && token.length() == 2 && (token[0] == 'A' || token[0] == 'C' || token[0] == 'T' || token[0] == 'G') && isspace(token[1])) throw FileParameterException(config, lineCounter, "Invalid number of characters in target string");
            else if(!(j == id_size-1 && token.length() == 2 && (token[0] == 'A' || token[0] == 'C' || token[0] == 'T' || token[0] == 'G') && isspace(token[1]))) throw FileParameterException(config, lineCounter, "Invalid character in target string");
        }
        targetString<<token;
        j++;
    }
    if(j != id_size) throw FileParameterException(config, lineCounter, "Invalid number of characters in target string");
    target = targetString.str();
    lineCounter++;

    if(!getline(file, line))throw FileParameterException(config ,lineCounter,"Invalid number of parameters");
    int numCycles = 0;
    try
    {
        numCycles = std::stoi(line);
    }
    catch(std::invalid_argument){throw FileParameterException(config ,lineCounter,"Invalid file parameters");}
    if(!(numCycles >= 0 && numCycles <= 1000000)) throw FileParameterException(config ,lineCounter,"Invalid number of cycles");
    maxCycles = numCycles;

    if(getline(file, line))throw FileParameterException(config ,lineCounter,"Invalid number of parameters");
}

void Simulation::readFirstGeneration(const char *firstGen)
{
    if(!firstGen) throw FileNotFoundException("NULL STRING");
    std::ifstream file(firstGen);
    if(!file.is_open()) throw FileNotFoundException(firstGen);

    std::string line;
    int lineCounter = 1;

    if(!getline(file, line))throw FileParameterException(firstGen ,lineCounter,"File empty");
    int pop_size = atoi(line.c_str());
    if(!(pop_size >= 2 && pop_size <= 200)) throw FileParameterException(firstGen ,lineCounter,"Invalid population size");
    this->popSize = pop_size;

    while(getline(file, line))
    {
        lineCounter++;
        std::stringstream currentLine(line);
        std::stringstream id_string;
        std::string token;
        char virusType;
        int letterCounter = 0;

        while(getline(currentLine, token, ' '))
        {
            if(letterCounter > idSize) throw FileParameterException(firstGen, lineCounter, "Invalid number of characters in id string");
            if(letterCounter == 0)
            {
                if(token != "a" && token != "d" && token != "o")
                {
                    throw FileParameterException(firstGen, lineCounter,"Unknown virus type");
                }
                virusType = token[0];
            }
            else if(token != "T" && token != "A" && token != "C" && token != "G")
            {
                if(!(token.length() == 2 && (token[0] == 'A' || token[0] == 'C' || token[0] == 'T' || token[0] == 'G') && isspace(token[1]))) throw FileParameterException(firstGen, lineCounter, "Invalid character in id string");
            }
            if(letterCounter > 0) id_string<<token;
            letterCounter++;
        }
        if(letterCounter <= idSize) throw FileParameterException(firstGen, lineCounter, "Invalid number of characters in id string");

        ancestors.push_back(std::pair<char, Forefather>(virusType,Forefather(id_string.str())));
        switch(virusType)
        {
            case 'a':
            {
                population.push_back(new Alpha(ancestors[ancestors.size() - 1].second));
                break;
            }
            case 'd':
            {
                population.push_back(new Delta(ancestors[ancestors.size() - 1].second));
                break;
            }
            case 'o':
            {
                population.push_back(new Omicron(ancestors[ancestors.size() - 1].second));
                break;
            }
        }
    }
    if(lineCounter != popSize+1) throw FileParameterException(firstGen, lineCounter, "Incorrect number of viruses in population");
}

Simulation::Simulation(const char* config, const char* firstGeneration)
{
    this->ancestors = std::vector<std::pair<char, Forefather>>();
    this->population = std::vector<Corona*>();
    try
    {
        readConfig(config);
        readFirstGeneration(firstGeneration);
        this->strongest = population[0];
    }
    catch(FileException& fe)
    {
        std::cerr<<fe.getMessage()<<std::endl;
        throw;
    }
}


std::pair<int, int> Simulation::chooseSpliceIndeces()
{
    if(idSize == 3) return std::pair<int, int>(1,2);
    else
    {
        int s=0;
        int t = 0;
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(1,idSize-2);

        auto random_integer = uni(rng);
        s = random_integer;

        do
        {
            std::uniform_int_distribution<int> uni(2,idSize-1);
            auto random_integer = uni(rng);
            t = random_integer;
        } while(t == s);

        return std::pair<int, int>(s,t);
    }
}

std::pair<int, int> Simulation::chooseVirusIndeces()
{
    if(popSize == 2) return std::pair<int, int>(0,1);
    else
    {
        int s=0;
        int t = 0;
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(0,popSize-1);

        auto random_integer = uni(rng);
        s = random_integer;

        do
        {
            std::uniform_int_distribution<int> uni(0,popSize-1);
            auto random_integer = uni(rng);
            t = random_integer;
        } while(t == s);
        return std::pair<int, int>(s,t);
    }
}

std::pair<std::string, std::string> Simulation::splicePair(std::pair<int, int> indeces)
{

    std::string u = ancestors[indeces.first].second.getForeString();
    std::string v = ancestors[indeces.second].second.getForeString();

    std::pair<int, int> spliceIndeces = chooseSpliceIndeces();

    for(int i = spliceIndeces.first+1;i<= spliceIndeces.second;i++)
    {
        char temp = u[i];
        u[i] = v[i];
        v[i] = temp;
    }

    return std::pair<std::string, std::string>(u,v);
}

Corona* Simulation::generateNewVirus(std::pair<char, Forefather> father, std::string id)
{
    switch(father.first)
    {
        case 'a':
        {
            Alpha* c = new Alpha(father.second);
            c->setId(id);
            return c;
        }

        case 'd':
        {
            Delta* c = new Delta(father.second);
            c->setId(id);
            return c;
        }

        case 'o':
        {
            Omicron* c = new Omicron(father.second);
            c->setId(id);
            return c;
        }
    }
    return 0;
}

void Simulation::step()
{
    for(Corona* virus : population) virus->mutate();

    try
    {
        std::sort(population.begin(), population.end(), [this](Corona *first, Corona *last)
        {
            return(first->getMatchStrength(target) < last->getMatchStrength(target));
        });
    }
    catch(Corona::matchFoundException& mfe){throw;}


    if((population[popSize-1])->getMatchStrength(target) > strongest->getMatchStrength(target)) strongest = population[popSize-1];

    std::pair<int, int> virusIndeces = chooseVirusIndeces();

    std::pair<std::string, std::string> newIdStrings = splicePair(virusIndeces);

    delete population[0];
    delete population[1];

    population[0] = generateNewVirus(ancestors[virusIndeces.first], newIdStrings.first);
    population[1] = generateNewVirus(ancestors[virusIndeces.second], newIdStrings.second);

    if(population[0]->getMatchStrength(target) > strongest->getMatchStrength(target)) strongest = population[0];
    if(population[1]->getMatchStrength(target) > strongest->getMatchStrength(target)) strongest = population[1];
}

void Simulation::printLastGeneration()
{
    for(int i = 0; i< popSize; i++)
    {
        std::cout<<population[i]->getType()<<" ";
        for(int j = 0;j<idSize-1;j++)
        {
            std::cout<<population[i]->getId()[j]<<" ";
        }
        std::cout<<population[i]->getId()[idSize-1]<<std::endl;
    }

    std::cout<<std::endl;
}

void Simulation::printRC()
{
    for(int i = 0; i< popSize; i++)
    {
        std::cout<<ancestors[i].first<<" ";
        for(int j = 0;j<idSize;j++)
        {
            std::cout<<ancestors[i].second.getForeString()[j]<<" ";
        }
        std::cout<<ancestors[i].second.getRefCount()-1<<std::endl;
    }

    std::cout<<std::endl;
}

void Simulation::printStrongest()
{
    std::cout<<strongest->getType()<<" ";
    for(int j = 0;j<idSize-1;j++)
    {
        std::cout<<strongest->getId()[j]<<" ";
    }
    std::cout<<strongest->getId()[idSize-1];
}

void Simulation::run()
{
    std::string strongest= "";
    for(int i = 0; i<maxCycles; i++)
    {
        try
        {
            step();
        }
        catch(Corona::matchFoundException& mfe)
        {
            strongest = mfe.getMessage();
            break;
        }
    }
    printLastGeneration();
    if(maxCycles > 0)
    {
        printRC();
        if(strongest.empty()) printStrongest();
        else std::cout<<strongest;
    }
}