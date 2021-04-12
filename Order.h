/*
 * Name: Mike Nguyen
 * netID: mtn190005
 * 
 * */
#ifndef ORDER_H
#define ORDER_H

#include <vector>

struct Order {
    int auditorium;
    std::vector<std::pair<int, int>> seats;
    int adult, children, senior;
    double total;

    Order(int);
    void printOrder();
    int indexOf(std::pair<int, int>);

};

#endif