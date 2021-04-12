/*
 * Name: Mike Nguyen
 * netID: mtn190005
 * 
 * */
#include <iostream>
#include <string>
#include <fstream> 
#include <iomanip>
#include <vector>
#include <math.h>
#include <unordered_map>
#include <algorithm>

#include "Order.h"
#include "Auditorium.h"

#define umap unordered_map
#define var(...) " [" << #__VA_ARGS__ ": " << (__VA_ARGS__) << "] "
#define present(c, x) (c.find(x) != c.end()) 

using namespace std;

enum ProgramStatus { RUNNING, STOPPED };

struct User {
    string username, password;
    vector<Order> orders;
    User() { username = "", password = ""; }
    User(string user, string pass) { username = user, password = pass; }
};

bool validInteger(string input, int start = 0, int end = 2147483647) {
	if (input == "") return false;
	for (char c : input) {
		if (c < '0' || c > '9') return false;
	}
	int fin = stoi(input);
	return (fin >= start && fin <= end);
}

bool validCharacter(string input, int start, int end) {
	if (int(input.size()) != 1) return false;
	int temp = toupper(input[0])-65;
	return (temp >= start && temp <= end); 
}

bool validYesNo(string &yesNo) {
	if (int(yesNo.size()) != 1) return false;
	yesNo[0] = char(toupper(yesNo[0]));
	return (yesNo[0]==89 || yesNo[0]==78);
} 

class Program {
public:
    Program() {
        for (string file : {"A1.txt", "A2.txt", "A3.txt"}) auditorium.emplace_back(int(auditorium.size()) + 1, file);
        readUserDatabase();
        startProgram();
    }

    void readUserDatabase(string filename = "userdb.dat") {
        ifstream ifs(filename);
        if (ifs.is_open()) {
            string input;
            while (getline(ifs, input)) {
                if (input == "") break;
                int idx = int(input.find(" "));
                user_database[input.substr(0, idx)] = User(input.substr(0, idx), input.substr(idx+1, int(input.size())-idx-1)); 
            }
            ifs.close();
        }
    }

    void updateAuditoriumFiles() {
        vector<string> final_files{"A1Final.txt", "A2Final.txt", "A3Final.txt"};
        for (int i= 0; i<3; i++) {
            auditorium[i].exportData(final_files[i]);
        }
    }

    void printReport() {
        int total_open = 0, total_reserved = 0, adult = 0, child = 0, senior = 0;
        double total_cost = 0.0;
        
        for (int i= 0; i<3; i++) {
            cout << "Auditorium " << i+1 << '\t';
            cout << auditorium[i].getOpen() << '\t' << auditorium[i].getReserved() << '\t';
            cout << auditorium[i].getAdult() << '\t' << auditorium[i].getChildren() << '\t' << auditorium[i].getSenior() << '\t';
            cout << "$" << auditorium[i].getTotal() << endl;

            total_open += auditorium[i].getOpen(), total_reserved += auditorium[i].getReserved();
            adult += auditorium[i].getAdult(), child += auditorium[i].getChildren(), senior += auditorium[i].getSenior();
            total_cost += auditorium[i].getTotal();
        }

        cout << "Total" << '\t';
        cout << total_open << '\t' << total_reserved << '\t';
        cout << adult << '\t' << child << '\t' << senior << '\t';
        cout << "$" << total_cost << endl;
    }

    void displayAdminMenu() {
        while (program_status != STOPPED) {
            cout << endl;
            cout << "1. Print Report" << endl;
            cout << "2. Logout" << endl;
            cout << "3. Exit" << endl << endl;
            string input = "";
            while (!validInteger(input, 1, 3)) {
                cout << "Enter option: ";
                cin >> input;
            }
            int option = stoi(input);
            if (option == 1) printReport();
            else if (option == 2) return;
            else {
                program_status = STOPPED;
                return;
            }
        }
    }

    tuple<int, int, int> getTickets() {
        cout << endl;
        string adult_input = "", children_input = "", senior_input = "";
        
        while (!validInteger(adult_input)) {
            adult_input = "";
            cout << "Enter # of adults: ";
            cin >> adult_input;
        }
        
        while (!validInteger(children_input)) {
            children_input = "";
            cout << "Enter # of children: ";
            cin >> children_input;
        }
        
        while (!validInteger(senior_input)) {
            senior_input = "";
            cout << "Enter # of senior: ";
            cin >> senior_input;
        }
        
        return make_tuple(stoi(adult_input), stoi(children_input), stoi(senior_input));
    }

    pair<int, int> getStartPosition(int chosen) {
        string input_row = "", input_col = "";
        
        while (!validInteger(input_row, 1, auditorium[chosen].getRow())) {
            cout << "Enter row number: ";
            cin >> input_row;
        }

        while (!validCharacter(input_col, 0, auditorium[chosen].getCol()-1)) {
            cout << "Enter starting seat letter: ";
		    cin >> input_col;
        }
        return {stoi(input_row)-1, toupper(input_col[0])-65};
    }

    void newOrder(string username, int chosen) {
        auditorium[chosen].printGrid();
        // Prompt for necessary information
        pair<int, int> start_position = getStartPosition(chosen);
        tuple<int, int, int> ticket_count = getTickets();

        int total = get<0>(ticket_count) + get<1>(ticket_count) + get<2>(ticket_count);
        if (!total) return;

        bool ok = auditorium[chosen].checkEmpty(start_position, total);
        if (!ok) {
            pair<int, int> best = auditorium[chosen].bestAvailable(total);
            if (best == make_pair(-1, -1)) cout << "no seats available" << endl;
            else {
                string input_prompt = "";
                cout << best.first+1 << char(best.second+65) << " - " << best.first+1 << char(best.second+64+total) << endl; 
                while (!validYesNo(input_prompt)) {
                    cout << "Make Reservation? (Y/N)";
                    cin >> input_prompt;    
                }
                if (input_prompt == "Y") {
                    user_database[username].orders.emplace_back(chosen+1);
                    auditorium[chosen].reserveSeats(user_database[username].orders.back(), best, ticket_count);
                }
            }
        }
        else {
            user_database[username].orders.emplace_back(chosen+1);
            auditorium[chosen].reserveSeats(user_database[username].orders.back(), start_position, ticket_count);
        }

    }

    void makeReservation(string username) {
        cout << endl;
        for (int i= 0; i<int(auditorium.size()); i++) {
            cout << i+1 << ". Auditorium " << i+1 << endl;
        }
        cout << endl;
        string input = "";
        while (!validInteger(input, 1, 3)) {
            cout << "Enter option: ";
            cin >> input;
        }
        int option = stoi(input);
        newOrder(username, option-1);
    }

    void viewOrder(string username) {
        #define order_list user_database[username].orders
        
        cout << endl;
        if (order_list.empty()) {
            cout << "No orders" << endl;
            return;
        }
        for (Order o : order_list) {
            o.printOrder();
            cout << endl;
        }
    }

    void addTicket(string username, int chosen_order, bool &successful) {
        cout << endl;
        #define current_order user_database[username].orders[chosen_order]

        int chosen_auditorium = current_order.auditorium-1;
        auditorium[chosen_auditorium].printGrid();
        // Prompt for necessary information
        pair<int, int> start_position = getStartPosition(chosen_auditorium);
        tuple<int, int, int> ticket_count = getTickets();

        int total = get<0>(ticket_count) + get<1>(ticket_count) + get<2>(ticket_count);
        if (!total) return;

        bool ok = auditorium[chosen_auditorium].checkEmpty(start_position, total);
        if (!ok) cout << "no seats available" << endl;
        else {
            successful = true;
            auditorium[chosen_auditorium].reserveSeats(current_order, start_position, ticket_count);
        }
    }

    void deleteTicket(string username, int chosen_order, bool &successful) {
        cout << endl;
        #define current_order user_database[username].orders[chosen_order]
        int chosen_auditorium = current_order.auditorium - 1;
        // Prompt for necessary information
        pair<int, int> target_position = getStartPosition(chosen_auditorium);
        int target_idx = current_order.indexOf(target_position);

        if (target_idx != -1) {
            if (int(current_order.seats.size()) == 1) cancelOrder(username, chosen_order, successful);
            else {
                auditorium[chosen_auditorium].unreserveSeat(current_order, target_idx);
                successful = true;
            }
        }
        else cout << "Invalid seats entered" << endl;

    }

    void cancelOrder(string username, int chosen_order, bool &successful) {
        #define current_order user_database[username].orders[chosen_order]
        #define order_list user_database[username].orders
        int chosen_audiitorium = current_order.auditorium - 1;

        while (!current_order.seats.empty()) {
            auditorium[chosen_audiitorium].unreserveSeat(current_order, int(current_order.seats.size())-1);
        }

        order_list.erase(order_list.begin() + chosen_order);
        successful = true;
    }

    void modifyOrder(string username, int chosen) {
        bool successful = false;
        while (!successful) {
            cout << endl;
            cout << "1. Add tickets to order" << endl;
            cout << "2. Delete tickets from order" << endl;
            cout << "3. Cancel Order" << endl << endl;

            string input = "";
            while (!validInteger(input, 1, 3)) {
                cout << "Enter option: ";
                cin >> input;
            }
            int option = stoi(input);
            if (option == 1) addTicket(username, chosen, successful);
            else if (option == 2) deleteTicket(username, chosen, successful);
            else cancelOrder(username, chosen, successful);
        }
    }

    void updateOrder(string username) {
        cout << endl;
        int order_size = int(user_database[username].orders.size());
        if (!order_size) {
            cout << "No orders" << '\n';
            return;
        }
        for (int i = 0; i<order_size; i++) {
            cout << "Order " << i+1 << endl;
            user_database[username].orders[i].printOrder();
            cout << endl;
        }
        string input = "";
        while (!validInteger(input, 1, order_size)) {
            cout << "Enter option: ";
            cin >> input;
        }
        int option = stoi(input);
        modifyOrder(username, option-1);
    }

    void displayReceipt(string username) {
        cout << endl;
        #define order_list user_database[username].orders
        double total_cost = 0.0;
        if (order_list.empty()) cout << "No orders" << '\n';
        for (Order o : order_list) {
            o.printOrder();
            cout << "Order Total: $" << setprecision(2) << fixed << o.total << endl << endl;
            total_cost += o.total;
        }
        cout << "Customer Total: $" << setprecision(2) << fixed << total_cost << endl;
    }

    void displayMainMenu(string username) {
        while (true) {
            cout << endl;
            cout << "1. Reserve Seats" << endl;
            cout << "2. View Orders" << endl;
            cout << "3. Update Order" << endl;
            cout << "4. Display Receipt" << endl;
            cout << "5. Log Out" << endl << endl;

            string input = "";
            while (!validInteger(input, 1, 5)) {
                cout << "Enter option: ";
                cin >> input;
            }
            int option = stoi(input);
            switch(option) {
                case 1: 
                    makeReservation(username);
                    break;
                case 2:
                    viewOrder(username);
                    break;
                case 3: 
                    updateOrder(username);
                    break;
                case 4: 
                    displayReceipt(username);
                    break;
                default:
                    return;
            }
        }
    }

    void loginUser()  {
        cout << endl;
        while (program_status != STOPPED) {
            string username = "", password = "";
            
            while (!present(user_database, username)) {
                cout << "Enter username: ";
                cin >> username;
            }

            int cnt = 0;
            while (cnt < 3) {
                cout << "Enter password: ";
                cin >> password;
                if (password != user_database[username].password) {
                    cout << "Invalid password" << endl;
                    cnt++;
                }
                else break;
            }
            if (cnt >= 3) continue;
            if (username == "admin") displayAdminMenu();
            else displayMainMenu(username);
        }
    }

    void startProgram() {
        program_status = RUNNING;
        loginUser();
        updateAuditoriumFiles();
    }
private:
    vector<Auditorium> auditorium;
    umap<string, User> user_database;
    ProgramStatus program_status;
};

int main() {
    Program p;
}

