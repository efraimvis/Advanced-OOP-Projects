//
// Created by Admin on 7/2/2022.
//

#ifndef HW_3_SIMOBJECT_H
#define HW_3_SIMOBJECT_H
#include <cmath>
#include <memory>
#include <iomanip>
#include <iostream>
#include <sstream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Time
{
private:
    int hours;
    int minutes;
public:

    /*Returns hours*/
    const int getHours() const{return hours;}

    /*Returns minutes*/
    const int getMinutes() const{return minutes;}

    /*Default constructor*/
    Time()
    {
        hours = 0;
        minutes = 0;
    }

    /*Constructor
     * _hours - hours
     * _minutes - minutes*/
    Time(int _hours, int _minutes)
    {
        if(!(_hours >= 0 && _hours < 72)) throw std::invalid_argument("ERROR: invalid hours");
        if(!(_minutes >= 0 && _minutes < 60)) throw std::invalid_argument("ERROR: invalid minutes");

        hours = _hours;
        minutes = _minutes;
    }
    /*Destructor*/
    ~Time(){};

    /*Copy c'tor*/
    Time(const Time& rhs)
    {
        hours = rhs.hours;
        minutes = rhs.minutes;
    }

    /*Move c'tor*/
    Time(Time&& rhs)
    {
        hours = rhs.hours;
        minutes = rhs.minutes;

        rhs.hours = 0;
        rhs.minutes = 0;
    }

    /*Cpy assignment operator*/
    Time& operator=(const Time& rhs)
    {
        if(this == &rhs)
            return *this;
        hours = rhs.hours;
        minutes = rhs.minutes;
        return *this;
    }

    /*Move assignment operator*/
    Time& operator=(Time&& rhs)
    {
        if(this == &rhs)
            return *this;
        hours = rhs.hours;
        minutes = rhs.minutes;
        rhs.hours = 0;
        rhs.minutes = 0;
        return *this;
    }

    /*Prefix ++ operator*/
    bool operator++()
    {
        if(hours < 71) hours++;
        else if(hours == 71) hours = 0;
    }

    /*== operator
     *
     * returns true if times are the same*/
    bool operator==(const Time& other) const
    {
        if(this->hours == other.hours && this->minutes == other.minutes) return true;
        return false;
    }

    bool operator< (const Time& other) const
    {
        if(this->hours > other.hours) return false;
        else if(this->minutes >= other.minutes) return false;
        return true;
    }
    bool operator>(const Time& other) const
    {
        if(this->hours < other.hours) return false;
        else if(this->minutes <= other.minutes) return false;
        return true;
    }

    /*Returns time difference in HOURS*/
    double operator-(const Time& other) const {
        double this_with_mins = this->hours + (((double) this->minutes) / 60);
        double other_with_mins = other.hours + (((double) other.minutes) / 60);

        return this_with_mins - other_with_mins;
    }

    /*Return string representing time*/
    std::string toString() const
    {
        std::stringstream ss;
        if(hours < 10) ss<<"0";
        ss<<hours<<":";
        if(minutes < 10) ss<<"0";
        ss<<minutes;
        return ss.str();
    }
};

/*Return distance between two points on map (IN ORDER TO GET KILOMETERS, MULTIPLY BY 100!)*/
static float getDistance(std::pair<float, float> a, std::pair<float, float> b)
{
    return sqrt(pow((b.second-a.second),2)+pow((b.first-a.first),2));
}

/*Returns string representing location*/
static std::string locationString(std::pair<float, float> location)
{
    std::stringstream ss;
    ss<<"("<<std::setprecision(2)<<std::fixed<<location.first<<", "<<location.second<<")";
    return ss.str();
}

/*Calculates partial distance between two points, that can be covered with a given speed in a given time - and returns appropriate point
 *
 * from - start point
 * to - end point
 * speed - speed in kmh
 * dt_in_minutes - given time in minutes
 *
 * return : Returns the point reached given parameters*/
static std::pair<float, float> getNextLocation(std::pair<float, float> from, std::pair<float, float> to, double speed, int dt_in_minutes)
{
    double fullDistance = getDistance(from, to)*100;
    double partialDistance = speed*(((double)dt_in_minutes)/((double)60));
    double multFactor = partialDistance/fullDistance;

    std::pair<float, float> retval = std::pair<float, float>(from.first+((to.first-from.first)*multFactor), from.second+((to.second-from.second)*multFactor));
    return retval;
}

/*Converts a point in polar coordinates to cartesian*/
static std::pair<float, float> toCartesian(const std::pair<float , float> polar)
{
    double r = polar.first;
    double th = polar.second;

    double x = r*cos(th);
    double y = r*sin(th);

    double eps = 0.0001;

    if(fabs(x) < eps) x = 0;
    if(fabs(y) < eps) y = 0;

    std::pair<float, float> retval = std::pair<float, float>(x,y);
    return retval;
}

/*Calculates partial distance from start point that can be covered with a given speed in a given time travelling at given angle - and returns appropriate point
 *
 * from - start point
 * angle - given angle
 * speed - speed in kmh
 * dt_in_minutes - given time in minutes
 *
 * return : Returns the point reached given parameters*/
static std::pair<float, float> getNextLocationCourse(std::pair<float, float> start ,double angle, double speed = 90, int dt_in_minutes = 60)
{
    double angle_deg = fmod((360 - angle + 90), 360);
    double angle_rad = (angle_deg * M_PI)/((double)180);
    double distance = (speed*(((double)dt_in_minutes)/((double)60)))/((double)100);

    std::pair<float, float> intermediate = toCartesian({distance, angle_rad});

    std::pair<float, float> retval = std::pair<float, float>(start.first + intermediate.first, start.second + intermediate.second);
    return retval;
}

class SimObject
{
protected:
    std::string name;
    std::pair<float, float> location;
public:
    const std::string getName() const {return name;}
    const std::pair<float, float> getLocation() const {return location;}
    SimObject(const std::pair<float, float> objLocation):location(objLocation){};
};

#endif //HW_3_SIMOBJECT_H
