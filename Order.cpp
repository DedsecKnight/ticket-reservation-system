/*
 * Name: Mike Nguyen
 * netID: mtn190005
 * 
 * */
#include <iostream>
#include <vector>

#include "Order.h"

Order::Order(int aud = 0) {
    auditorium = aud;
    adult = 0, children = 0, senior = 0;
    total = 0.0;
}

void Order::printOrder() {
    std::cout << "Auditorium " << auditorium << ", ";
    for (int i= 0; i<int(seats.size()); i++) {
        std::cout << seats[i].first + 1 << char(seats[i].second+65);
        if (i < int(seats.size())-1) std::cout << ",";
        else std::cout << std::endl;
    }
    std::cout << adult << " adult, " << children << " child, " << senior << " senior" << std::endl;
}

int Order::indexOf(std::pair<int, int> target_seat) {
    int low = 0, high = int(seats.size())-1;
    while (low <= high) {
        int mid = low + (high - low)/2;
        if (seats[mid] == target_seat) return mid;
        else if (seats[mid] > target_seat) high = mid-1;
        else low = mid+1;
    }
    return -1;
}