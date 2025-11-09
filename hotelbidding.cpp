// CIS 476 Project 2
// Gunnar Bergstrom

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// Bid Request Object
class BidRequest {
public:
    string customerName;
    double amount;

    BidRequest(string name, double amt)
        : customerName(name), amount(amt) {}
};

// Abstract Handler
class RoomHandler {
protected:
    RoomHandler* nextHandler;
    int roomsAvailable;

public:
    RoomHandler(int count) : nextHandler(nullptr), roomsAvailable(count) {}

    void setNextHandler(RoomHandler* next) {
        nextHandler = next;
    }

    bool isAvailable() const {
        return roomsAvailable > 0;
    }

    void reduceRoomCount() {
        if (roomsAvailable > 0) roomsAvailable--;
    }

    int getRoomsAvailable() const {
        return roomsAvailable;
    }

    virtual void handleRequest(BidRequest& request) = 0;
};

// Suite Room Handler
class SuiteHandler : public RoomHandler {
public:
    SuiteHandler(int count) : RoomHandler(count) {}

    void handleRequest(BidRequest& request) override {
        if (isAvailable() && request.amount >= 280) {
            reduceRoomCount();
            cout << "Bid accepted for SUITE by "
                 << request.customerName << " ($" << request.amount
                 << "). Remaining Suites: " << roomsAvailable << endl;
        } else if (!isAvailable()) {
            cout << "No Suites left. Passing to next handler...\n";
            if (nextHandler) nextHandler->handleRequest(request);
        } else {
            cout << "Bid too low for Suite. Passing to next handler...\n";
            if (nextHandler) nextHandler->handleRequest(request);
        }
    }
};

// Deluxe Room Handler
class DeluxeHandler : public RoomHandler {
public:
    DeluxeHandler(int count) : RoomHandler(count) {}

    void handleRequest(BidRequest& request) override {
        if (isAvailable() && ((request.amount >= 150 && request.amount < 280) || request.amount >= 280)) {
            reduceRoomCount();
            cout << "Bid accepted for DELUXE by "
                 << request.customerName << " ($" << request.amount
                 << "). Remaining Deluxe: " << roomsAvailable << endl;
        } else if (!isAvailable()) {
            cout << "No Deluxe rooms left. Passing to next handler...\n";
            if (nextHandler) nextHandler->handleRequest(request);
        } else {
            cout << "Bid too low for Deluxe. Passing to next handler...\n";
            if (nextHandler) nextHandler->handleRequest(request);
        }
    }
};

// Standard Room Handler
class StandardHandler : public RoomHandler {
public:
    StandardHandler(int count) : RoomHandler(count) {}

    void handleRequest(BidRequest& request) override {
        if (isAvailable() && ((request.amount >= 80 && request.amount < 150) || request.amount >= 150)) {
            reduceRoomCount();
            cout << "Bid accepted for STANDARD by "
                 << request.customerName << " ($" << request.amount
                 << "). Remaining Standard: " << roomsAvailable << endl;
        } else if (!isAvailable()) {
            cout << "No Standard rooms left. Bid rejected for "
                 << request.customerName << ".\n";
        } else {
            cout << "Bid too low for Standard. Rejected for "
                 << request.customerName << " ($" << request.amount << ")\n";
        }
    }
};

// Main
int main() {
    cout << fixed << setprecision(2);
    // Initialize rooms
    SuiteHandler suite(10);     // 10 Suites
    DeluxeHandler deluxe(15);   // 15 Deluxe
    StandardHandler standard(45); // 45 Standard

    // Chain of Responsibility
    suite.setNextHandler(&deluxe);
    deluxe.setNextHandler(&standard);

    cout << "HOTEL ROOM BIDDING SYSTEM" << endl;
    cout << "Room Availability: " << endl;
    cout << "  Suite:   10 " << endl;
    cout << "  Deluxe:  15" << endl;
    cout << "  Standard:45" << endl;

    while (true) {
        string name;
        double bid;

        cout << "Enter customer name (or 'exit' to exit): ";
        getline(cin, name);

        if (name == "exit" || name == "EXIT")
            break;

        cout << "Enter bid amount ($): ";
        cin >> bid;

        // Validate bid 
        if (cin.fail() || bid <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid bid amount. Please try again.\n" << endl;
            continue;
        }
        cin.ignore(); 

        BidRequest request(name, bid);
        cout << "\nProcessing bid from " << name << " ($" << bid << "): " << endl;
        suite.handleRequest(request);
        cout << "-------------------------------------------" << endl;

        cout << "Current Availability -> Suites: " << suite.getRoomsAvailable()
             << ", Deluxe: " << deluxe.getRoomsAvailable()
             << ", Standard: " << standard.getRoomsAvailable() << "\n" << endl;

        // Stops if rooms are sold out
        if (suite.getRoomsAvailable() == 0 && deluxe.getRoomsAvailable() == 0 && standard.getRoomsAvailable() == 0) {
            cout << "All rooms sold out, ending bidding." << endl;
            break;
        }
    }

    // Final room count
    cout << "REMAINING ROOMS" << endl;
    cout << "Suites:   " << suite.getRoomsAvailable() << endl;
    cout << "Deluxe:   " << deluxe.getRoomsAvailable() << endl;
    cout << "Standard: " << standard.getRoomsAvailable() << endl;

    return 0;
}
