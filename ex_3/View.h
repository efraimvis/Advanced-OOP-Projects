//
// Created by Admin on 7/12/2022.
//

#ifndef HW_3_VIEW_H
#define HW_3_VIEW_H


#include <utility>
#include <list>
#include <map>

class View
{
private:
    int size;
    float scale;
    std::pair<float, float> origin;
    std::map<std::string, std::pair<float, float>> objects;
public:

    /*Constructor*/
    View();

    /*Destructor*/
    ~View(){};

    View(const View& rhs) = delete;
    View(View&& rhs) = delete;
    View operator=(const View& rhs) = delete;
    View operator=(View&& rhs) = delete;

    /*Getter for size*/
    const int getSize() const{return size;};

    /*Setter for size
     *
     * throws : invalid_argument if not 6 < size <= 30*/
    void setSize(int newSize);

    /*Getter for scale*/
    const float getScale() const{return scale;}

    /*Setter for scale*/
    void setScale(float newScale);

    /*Getter for origin*/
    const std::pair<float, float> getOrigin(){return origin;}

    /*Setter for origin*/
    void setOrigin(std::pair<float, float> newOrigin);

    /*Resets all view parameters to defaults*/
    void resetDefault();

    /*Inserts an object into the view*/
    void insertObject(std::string name, std::pair<float, float> position);

    /*Shows view*/
    void printView();
};


#endif //HW_3_VIEW_H
