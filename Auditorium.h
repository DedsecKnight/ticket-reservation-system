/*
 * Name: Mike Nguyen
 * netID: mtn190005
 * 
 * */
#ifndef AUDITORIUM_H
#define AUDITORIUM_H
#include <string>
#include <vector>

#include "Order.h"

class Auditorium {
public:
    Auditorium();
    Auditorium(int, std::string);
    
    void printGrid();
    bool checkEmpty(std::pair<int, int>, int);
    void markStatus(std::vector<std::pair<int, int>>&, int, int&, int, char, double, int&, double&);

    void reserveSeats(Order&, std::pair<int, int>, std::tuple<int, int, int>);
    void unreserveSeat(Order&, int);

    bool shouldUpdate(std::pair<int, int>, std::pair<int, int>, int);
    std::pair<int, int> bestAvailable(int);
    void exportData(std::string);

    int getOpen();
    int getReserved();

    int getAdult();
    int getChildren();
    int getSenior();

    double getTotal();

    int getRow();
    int getCol();
    int getID();

    void setOpen(int);
    void setReserved(int);

    void setAdult(int);
    void setChildren(int);
    void setSenior(int);

    void setTotal(double);

    void setRow(int);
    void setCol(int);
    void setID(int);



private:
    int id, row, col;
    int open, reserved;
    int adult, children, senior;
    double total, center_row, center_col;
    std::vector<std::vector<char>> grid;
};

#endif
