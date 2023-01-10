//
// Created by Admin on 7/12/2022.
//

#include <vector>
#include "Controller.h"
using std::cout;
using std::endl;

Controller::Controller(int argc, const char **argv)
{
    try
    {
        Model::getInstance().initModel(argc, argv);
    }
    catch(std::invalid_argument& ia)
    {
        cout<<ia.what()<<endl;
        exit(1);
    }
}

void Controller::runController()
{
    while(1)
    {
        cout<<"Time: "<<Model::getInstance().getCurrentTime().getHours()<<": Enter command: ";

        std::string command;
        std::string token;
        getline(std::cin, command);

        if(command == "exit") exit(0);

        std::stringstream ss(command);
        std::vector<std::string> parts;
        while(getline(ss, token, ' '))
        {
            parts.push_back(token);
        }

        if(parts.empty())
        {
            cout<<"ERROR: invalid command"<<endl;
            continue;
        }

        //if first word is a valid view command, handle view commands
        if(parts[0] == "default" || parts[0] == "size" || parts[0] == "zoom" || parts[0] == "pan" || parts[0] == "show")
        {
            std::string verb = parts[0];

            if(verb == "default")
            {
                if(parts.size() != 1)
                {
                    cout<<"ERROR: invalid command"<<endl;
                    continue;
                }
                Model::getInstance().getView().lock()->resetDefault();
            }
            else if(verb == "size")
            {
                if(parts.size() != 2)
                {
                    cout<<"ERROR: invalid command"<<endl;
                    continue;
                }
                try
                {
                    Model::getInstance().getView().lock()->setSize(stoi(parts[1]));
                }
                catch(std::runtime_error& re)
                {
                    cout<<re.what()<<endl;
                    continue;
                }
            }
            else if(verb == "zoom")
            {
                if(parts.size() != 2)
                {
                    cout<<"ERROR: invalid command"<<endl;
                    continue;
                }
                try
                {
                    Model::getInstance().getView().lock()->setScale(stof(parts[1]));
                }
                catch(std::runtime_error& re)
                {
                    cout<<re.what()<<endl;
                    continue;
                }
            }
            else if(verb == "pan")
            {
                if(parts.size() != 3)
                {
                    cout<<"ERROR: invalid command"<<endl;
                    continue;
                }

                float xCoord = stof(parts[1].substr(1,parts[1].size()-1));
                float yCoord = stof(parts[2].substr(0,parts[2].size()-1));

                Model::getInstance().getView().lock()->setOrigin({xCoord, yCoord});
            }
            else
            {
                if(parts.size() != 1)
                {
                    cout<<"ERROR: invalid command"<<endl;
                    continue;
                }
                Model::getInstance().getView().lock()->printView();
            }
            continue;
        }


        //if first word is a valid model command, handle model commands
        if(parts[0] == "status" || parts[0] == "go" || parts[0] == "create")
        {
            if(parts[0] == "status")
            {
                if(parts.size() != 1)
                {
                    cout<<"ERROR: invalid command"<<endl;
                    continue;
                }
                Model::getInstance().printStatus();
            }
            else if(parts[0] == "go")
            {
                if(parts.size() != 1)
                {
                    cout<<"ERROR: invalid command"<<endl;
                    continue;
                }
                Model::getInstance().moveObjects();
            }
            else
            {
                if(!(parts.size() >= 4 && parts.size() <= 5))
                {
                    cout<<"ERROR: invalid command"<<endl;
                    continue;
                }

                std::string vehicleName = parts[1];
                std::string vehicleType = parts[2];

                //if vehicle is a state trooper
                if(vehicleType == "State_trooper")
                {
                    if(parts.size() != 4)
                    {
                        cout<<"ERROR: invalid command"<<endl;
                        continue;
                    }
                    try
                    {
                        Model::getInstance().generateVehicle(Model::State_trooper, vehicleName,{0,0},parts[3]);
                    }
                    catch(std::invalid_argument& ia)
                    {
                        cout<<ia.what()<<endl;
                        continue;
                    }
                }

                //if vehicle is a chopper
                else if(vehicleType == "Chopper")
                {
                    if(parts.size() != 5)
                    {
                        cout<<"ERROR: invalid command"<<endl;
                        continue;
                    }

                    float xCoord = stof(parts[3].substr(1,parts[3].size()-1));
                    float yCoord = stof(parts[4].substr(0,parts[4].size()-1));
                    try
                    {
                        Model::getInstance().generateVehicle(Model::Chopper_bandit, vehicleName,{xCoord,yCoord});
                    }
                    catch(std::invalid_argument& ia)
                    {
                        cout<<ia.what()<<endl;
                        continue;
                    }
                }
                else
                {
                    cout<<"ERROR: invalid command"<<endl;
                    continue;
                }
            }
            continue;
        }

        //if first word is a vehicle name, handle vehicle commands
        if(parts.size() < 2)
        {
            cout<<"ERROR: invalid command"<<endl;
            continue;
        }

        std::string vehicleName = parts[0];
        std::string verb = parts[1];

        if(verb == "course" || verb == "position" || verb == "destination" || verb == "attack" || verb == "stop")
        {
            if(verb == "course")
            {
                if(parts.size() > 4)
                {
                    cout<<"ERROR: invalid command"<<endl;
                    continue;
                }
                int speed = 0;
                if(parts.size() == 4) speed = stof(parts[3]);

                try
                {
                    Model::getInstance().setVehicleCourse(vehicleName, stof(parts[2]), speed);
                }
                catch(std::runtime_error& re)
                {
                    cout<<re.what()<<endl;
                    continue;
                }
            }
            if(verb == "position")
            {
                if(parts.size() > 5)
                {
                    cout<<"ERROR: invalid command"<<endl;
                    continue;
                }

                float xCoord = stof(parts[2].substr(1,parts[2].size()-1));
                float yCoord = stof(parts[3].substr(0,parts[3].size()-1));
                int speed = -1;

                if(parts.size() == 5) speed = stof(parts[4]);

                try
                {
                    Model::getInstance().setVehiclePosition(vehicleName,{xCoord, yCoord},speed);
                }
                catch(std::runtime_error& re)
                {
                    cout<<re.what()<<endl;
                    continue;
                }
            }

            if(verb == "destination")
            {
                if(parts.size() != 3)
                {
                    cout<<"ERROR: invalid command"<<endl;
                    continue;
                }

                try
                {
                    Model::getInstance().setVehicleDestinationWarehouse(vehicleName,parts[2]);
                }
                catch(std::runtime_error& re)
                {
                    cout<<re.what()<<endl;
                    continue;
                }
            }

            if(verb == "attack")
            {
                if(parts.size() != 3)
                {
                    cout<<"ERROR: invalid command"<<endl;
                    continue;
                }

                try
                {
                    Model::getInstance().attackTruck(vehicleName, parts[2]);
                }
                catch(std::runtime_error& re)
                {
                    cout<<re.what()<<endl;
                    continue;
                }
            }

            if(verb == "stop")
            {
                if(parts.size() != 2)
                {
                    cout<<"ERROR: invalid command"<<endl;
                    continue;
                }
                try
                {
                    Model::getInstance().stopVehicle(vehicleName);
                }
                catch(std::runtime_error& re)
                {
                    cout<<re.what()<<endl;
                    continue;
                }
            }
        }
        else
        {
            cout<<"ERROR: invalid command"<<endl;
            continue;
        }
    }
}