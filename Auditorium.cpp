/*
 * Name: Mike Nguyen
 * netID: mtn190005
 * 
 * */
#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <algorithm>
#include <math.h>

#include "Order.h"
#include "Auditorium.h"

const double ADULT_PRICE = 10.0, CHILD_PRICE = 5.0, SENIOR_PRICE = 7.5;

Auditorium::Auditorium() {
    id = 0;
    row = 0, col = 0, reserved = 0, open = 0;
    total = 0.0, center_col = 0.0, center_row = 0.0;
}

Auditorium::Auditorium(int ID, std::string filename) {
    id = ID;
    row = 0, col = 0, reserved = 0;
    adult = 0, children = 0, senior = 0;
    total = 0.0;

    std::ifstream ifs(filename);
    if (ifs.is_open()) {
        std::string input;
        while (getline(ifs, input)) {
            if (input == "") break;

            if (!col) col = int(input.size());
            col -= int(input[col-1]) == 13 || input[col-1] == '\n';
            
            grid.push_back(std::vector<char>(col));
            
            for (int i= 0; i<col; i++) {
                grid[row][i] = input[i];
                if (input[i] != '.') {
                    reserved++;
                    if (input[i] == 'A') adult++, total += ADULT_PRICE;
                    else if (input[i] == 'S') senior++, total += SENIOR_PRICE;
                    else children++, total += CHILD_PRICE;
                } 
            }
            
            row++;
        }
        open = row*col - reserved;
        center_row = (double(row)-1)/2, center_col = (double(col)-1)/2;
        ifs.close();
    }
}

void Auditorium::printGrid() {
    std::cout << std::endl << "  ";
    for (int i= 0; i<col; i++) std::cout << char(i+65);
    std::cout << std::endl;

    for (int i= 0; i<row; i++) {
        std::cout << i+1 << " "; 
        for (int j = 0; j<col; j++) {
            std::cout << (grid[i][j] == '.' ? '.' : '#');
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

bool Auditorium::checkEmpty(std::pair<int, int> start_position, int seats_required) {
    for (int i= start_position.second; i < start_position.second + seats_required; i++) {
        if (grid[start_position.first][i] != '.') return false;
    }
    return true;
}

void Auditorium::markStatus(std::vector<std::pair<int, int>> &order_seats, int curr_row, int &pos, int seats_required, char status, double cost, int& order_cnt, double &order_cost) {
    order_cnt += seats_required;
    for (int i= 0; i<seats_required; i++) {
        grid[curr_row][pos] = status;
        total += cost, order_cost += cost;

        if (status == 'A') adult++;
        else if (status == 'C') children++;
        else if (status == 'S') senior++;
        
        reserved++, open--;
        order_seats.emplace_back(curr_row, pos);
        pos++;
    }
}

void Auditorium::reserveSeats(Order &curr_order, std::pair<int, int> start_position, std::tuple<int, int, int> cnt) {
    int curr_row = start_position.first, curr_pos = start_position.second;

    markStatus(curr_order.seats, curr_row, curr_pos, std::get<0>(cnt), 'A', ADULT_PRICE, curr_order.adult, curr_order.total);
    markStatus(curr_order.seats, curr_row, curr_pos, std::get<1>(cnt), 'C', CHILD_PRICE, curr_order.children, curr_order.total);
    markStatus(curr_order.seats, curr_row, curr_pos, std::get<2>(cnt), 'S', SENIOR_PRICE, curr_order.senior, curr_order.total);

    std::sort(curr_order.seats.begin(), curr_order.seats.end(), [](std::pair<int, int> a, std::pair<int, int> b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    });
}

void Auditorium::unreserveSeat(Order &current_order, int chosen_idx) {
    std::pair<int, int> target_seat = current_order.seats[chosen_idx];
    #define curr_seat grid[target_seat.first][target_seat.second]
    if (curr_seat == 'A') {
        adult--, current_order.adult--;
        total -= ADULT_PRICE, current_order.total -= ADULT_PRICE;
    }
    else if (curr_seat == 'C') {
        children--, current_order.children--;
        total -= CHILD_PRICE, current_order.total -= CHILD_PRICE;
    }
    else {
        senior--, current_order.senior--;
        total -= SENIOR_PRICE, current_order.total -= SENIOR_PRICE;
    }
    reserved--, open++;
    curr_seat = '.';
    current_order.seats.erase(current_order.seats.begin() + chosen_idx);
}

bool Auditorium::shouldUpdate(std::pair<int, int> new_best, std::pair<int, int> curr_best, int seats_required) {
    if (curr_best == std::make_pair(-1, -1)) return true;
    
    double step = (double(seats_required)-1)/2;
    double dist_currBest = std::pow((center_row - curr_best.first),2) + std::pow((center_col - curr_best.second - step), 2);
    double dist_newBest = std::pow((center_row - new_best.first),2) + std::pow((center_col - new_best.second - step), 2);
    
    if (dist_currBest != dist_newBest) return dist_currBest > dist_newBest;
    
    dist_currBest = std::fabs(curr_best.first - center_row), dist_newBest = std::fabs(new_best.first - center_row);
    if (dist_currBest != dist_newBest) return dist_currBest > dist_newBest;
    
    return curr_best.first > new_best.first;
}

std::pair<int, int> Auditorium::bestAvailable(int seats_required) {
    std::pair<int, int> final_best = std::make_pair(-1, -1);
    for (int i= 0; i<row; i++) {
        int ptr = 0;
        for (int j = 0; j<col; j++) {
            if (grid[i][ptr] != grid[i][j]) ptr = j;
            if (grid[i][ptr] == grid[i][j] && j-ptr+1 == seats_required && grid[i][ptr] == '.') {
                if (shouldUpdate(std::make_pair(i, ptr), final_best, seats_required)) final_best = std::make_pair(i, ptr);
                ptr++;
            }
        }
    }
    return final_best;
}

void Auditorium::exportData(std::string filename) {
    std::ofstream ofs(filename);
    for (int i= 0; i<row; i++) {
        for (int j = 0; j<col; j++) ofs << grid[i][j];
        ofs << std::endl;
    }
    ofs.close();
}

int Auditorium::getOpen() { return open; }
int Auditorium::getReserved() { return reserved; }

int Auditorium::getAdult() { return adult; }
int Auditorium::getChildren() { return children; }
int Auditorium::getSenior() { return senior; }

double Auditorium::getTotal() { return total; }

int Auditorium::getRow() { return row; }
int Auditorium::getCol() { return col; }
int Auditorium::getID() { return id; }

void Auditorium::setOpen(int o) { open = o; }
void Auditorium::setReserved(int r) { reserved = r; }

void Auditorium::setAdult(int a) { adult = a; }
void Auditorium::setChildren(int c) { children = c; }
void Auditorium::setSenior(int s) { senior = s; }

void Auditorium::setTotal(double t) { total = t; }

void Auditorium::setRow(int r) { row = r; }
void Auditorium::setCol(int c) { col = c; }
void Auditorium::setID(int i) { id = i; }
