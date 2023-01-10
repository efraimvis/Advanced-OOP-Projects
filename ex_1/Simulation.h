//
// Created by Admin on 4/20/2022.
//

#ifndef HW_1_SIMULATION_H
#define HW_1_SIMULATION_H

#include <vector>
#include "Corona.h"
#include <fstream>
#include <sstream>

class Simulation
{
private:
    std::string target;

    int popSize;

    int idSize;

    int maxCycles;

    Corona* strongest;

    std::vector<Corona*> population;

    std::vector<std::pair<char, Forefather > > ancestors;

    /*Reads from configuration file, and initializes appropriate simulation parameters
     * config - directory to configuration file*/
    void readConfig(const char* config);//MOVE TO PRIVATE!

    /*Reads from file for first generation, and initializes appropriate simulation parameters
     * firstGen - directory to file for first generation*/
    void readFirstGeneration(const char* firstGen);

    /*Selects two indeces of viruses in the population
     * return : Returns a pair of numbers representing two indeces*/
    std::pair<int, int> chooseVirusIndeces();

    /*Returns the result of splicing two ancestor strings from given indeces
     * indeces - pair of integers representing indeces of ancestor strings
     * return : Returns a pair of spliced strings*/
    std::pair<std::string, std::string> splicePair(std::pair<int, int> indeces);

    /*Reads required files to initialize simulation parameters
     * config - directory to config file
     * first_gen - directory to first generation file*/
    void readFiles(const char* config, const char* first_gen);

    /*Compares two Corona viruses according to match strength with current target string
     * first - first virus
     * second - second virus
     * return : Returns result of comparison*/
    bool cmpStrength(Corona *first, Corona *last);

    /*Selects indeces in id string for segment swap
     * return : Returns a pair of numbers representing indeces*/
    std::pair<int, int> chooseSpliceIndeces();

    /*Generates a new Corona virus
     * father - pair representing virus type and forefather
     * id - current id string
     * return : Returns new Corona virus with specified attributes*/
    Corona* generateNewVirus(std::pair<char, Forefather> father, std::string id);

    /*Performs a single step of the simulation*/
    void step();

    /*Prints type and id of strongest virus in population (historically)*/
    void printStrongest();

    /*Prints last generation of viruses*/
    void printLastGeneration();

    /*Prints list of viruses descended from initial population*/
    void printRC();

public:

    /*Runs simulation*/
    void run();

    /*Constructor
     * config - directory to configuration file
     * firstGeneration - directory to file for first generation*/
    Simulation(const char* config, const char* firstGeneration);

    /*Destructor*/
    ~Simulation();

    /*Exception for files*/
    struct FileException: public std::exception
    {
        virtual const std::string getMessage() const = 0;
        ~FileException() throw(){}
    };

    /*Exception for files that could not be opened*/
    struct FileNotFoundException : public FileException
    {
        const std::string fileName;

        /*Constructor
         * name - name of bad file*/
        FileNotFoundException(const char* name):fileName(name){};

        /*Returns error message*/
        const std::string getMessage() const
        {
           std::stringstream ss;
           ss<<"ERROR: Could not open file "<<fileName<<std::endl;
           return ss.str();
        }

        ~FileNotFoundException() throw(){}
    };

    /*Exception for files with bad parameters*/
    struct FileParameterException : public FileException
    {
        std::string message;

        /*Constructor
         * filename - file name
         * line - line number (for fault identification)
         * error - error description*/
        FileParameterException(const char* fileName ,int line, const char* error)
        {
            std::stringstream ss;
            ss<<"Error in file "<<fileName<<", line "<<line<<": "<<error<<std::endl;
            message = ss.str();
        }

        ~FileParameterException() throw(){}

        /*Returns error message*/
        const std::string getMessage() const{return this->message;}
    };

};


#endif //HW_1_SIMULATION_H
