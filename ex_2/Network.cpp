//
// Created by Admin on 5/19/2022.
//

#include <cstring>
#include <sstream>
#include "Network.h"
#include "iostream"
#include "fstream"
using namespace std;


Network::Network(int argc, char **args)
{
    if(argc < 3) throw runtime_error("ERROR: too few program arguments");
    std::list<pair<string,int>> config;
    std::vector<string> inFiles;

    std::string configFile;

    if(strcmp(args[1], "-i") != 0) throw runtime_error("ERROR: invalid program arguments");
    int i = 2;

    while(i < argc)
    {
        if(strcmp(args[i], "-c") == 0 || strcmp(args[i], "-o") == 0) break;

        inFiles.push_back(args[i]);
        i++;
    }

    bool readConfig = false;
    bool readOutput = false;

    while(i < argc)
    {
        if(strcmp(args[i], "-c") == 0 && !readConfig)
        {
            i++;
            if(i < argc)
            {
                configFile = args[i];
                readConfig = true;
            }
            else
            {
                throw runtime_error("ERROR: invalid program arguments");
            }
        }
        else if(strcmp(args[i], "-c") == 0 && readConfig)
        {
            throw runtime_error("ERROR: invalid program arguments");
        }
        else if(strcmp(args[i], "-o") == 0 && !readOutput)
        {
            i++;
            if(i < argc)
            {
                outputFile = args[i];
                readOutput = true;
            }
            else
            {
                throw runtime_error("ERROR: invalid program arguments");
            }
        }
        else if(strcmp(args[i], "-o") == 0 && readOutput)
        {
            throw runtime_error("ERROR: invalid program arguments");
        }

        i++;

    }

    if(!readConfig) configFile = "";

    //reading files names.
    try
    {
        config=readConfigFile(configFile);
    }
    catch(runtime_error& error)
    {
        throw;
    }

    graph=unique_ptr<Graph>(new Graph(config));

    for(std::string fileName : inFiles)
    {
        try
        {
            readInputFile(fileName);
        }
        catch(runtime_error& e){throw;}
    }

    graph->print("outFile.txt");

}

std::list<pair<string,int>> Network::readConfigFile(std::string filename)
{
    if(filename == "") return std::list<pair<string,int>>();
    ifstream config(filename);
    if(!config.is_open()) throw runtime_error("ERROR opening the specified file.");
    string line;
    std::list<pair<string,int>> retval=list<pair<string,int>>();

    while(getline(config,line))
    {
        stringstream currentline(line);
        string first;
        int second;
        currentline>>first;
        if(first.find("bus",0)==0||first.find("sprinter",0)==0||first.find("tram",0)==0||first.find("rail",0)==0||first.find("stad",0)==0||first.find("centraal",0)==0||first.find("intercity",0)==0)
        {
            currentline>>second;
            if(second<=0) throw runtime_error("Bad config input");
            retval.push_back(pair<string,int>(first,second));

        }
        else
        {
            throw runtime_error("Bad config input");
        }

    }
    config.close();
    return retval;

}

void Network::readInputFile(std::string filename)
{
    char vehicleType;

    if(filename.find("bus",0) ==0) vehicleType = 'b';
    else if(filename.find("sprinter",0) ==0) vehicleType = 's';
    else if(filename.find("tram",0) ==0) vehicleType = 't';
    else if(filename.find("rail",0) ==0) vehicleType = 'r';
    else throw runtime_error("ERROR opening the specified file.");

    ifstream file(filename.c_str());
    if(!file.is_open()) throw std::runtime_error("ERROR opening the specified file.");

    //read file
    std::string line;
    while(getline(file, line))
    {
        stringstream currentLine(line);
        std::string from, to;
        int time;
        char from_type, to_type;

        currentLine>>from >> to>> time;

        if(time <= 0) throw std::runtime_error("ERROR opening the specified file.");

        if(from.find("CS",0) == 0) from_type = 'c';
        else if(from.find("IC",0) == 0) from_type = 'i';
        else from_type = 's';

        if(to.find("CS",0) == 0) to_type = 'c';
        else if(to.find("IC",0) == 0) to_type = 'i';
        else to_type = 's';

        try
        {
            graph->addEdge(time,from, from_type,to,to_type, vehicleType);
        }
        catch(runtime_error){}
    }
    file.close();
}

void Network::run()
{
    while(true)
    {
        string line;
        getline(cin, line);
        stringstream ss(line);
        vector<string> words;
        string token;

        while(!ss.eof())
        {
            ss >> token;
            words.push_back(token);
        }

        if(words.empty()) cerr<<"ERROR: invalid input"<<endl;
        else if(words.size() == 1)
        {
            if(words[0] == "EXIT") return;
            else if(words[0] == "print") graph->print(outputFile);
            else
            {
                cerr<<"ERROR: invalid input"<<endl;
                continue;
            }
        }
        else if(words.size() == 2)
        {
            try
            {
                if (words[0] == "inbound") graph->inbound(words[1]);
                else if (words[0] == "outbound") graph->outbound(words[1]);
                else if(words[0] == "load") readInputFile(words[1]);
                else
                {
                    cerr<<"ERROR: invalid input"<<endl;
                    continue;
                }
            }
            catch(runtime_error& e)
            {
                cerr<<e.what()<<endl;
                continue;
            }
        }
        else if(words.size() == 3)
        {
            try
            {
                if(words[0] == "uniExpress") graph->uniExpress(words[1], words[2]);
                else if(words[0] == "multiExpress") graph->multiExpress(words[1], words[2]);
                else
                {
                    cerr<<"ERROR: invalid input"<<endl;
                    continue;
                }
            }
            catch(runtime_error& e)
            {
                cerr<<e.what()<<endl;
                continue;
            }
        }
    }
}

