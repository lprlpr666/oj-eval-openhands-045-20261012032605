#include <iostream>
#include <cstdio>
#include <set>

using namespace std;

// Elevator system for a billion-floor building
// Supports add, cancel, action, and locate operations

class Elevator {
private:
    int current_floor;
    int direction; // 1 for up, -1 for down
    set<int> requests; // Sorted set of pending requests

public:
    Elevator() : current_floor(0), direction(1) {}

    // Add a request for a floor
    void addRequest(int floor) {
        requests.insert(floor);
    }

    // Cancel a request for a floor
    void cancelRequest(int floor) {
        requests.erase(floor);
    }

    // Execute the next action according to elevator rules
    void action() {
        if (requests.empty()) {
            // No pending requests, stay in place with same direction
            return;
        }

        auto it = requests.end();
        
        if (direction == 1) { // Moving up
            // Find the first request at or above current floor
            it = requests.lower_bound(current_floor);
            
            if (it == requests.end()) {
                // No requests at or above current floor
                // Reverse direction to down
                direction = -1;
                // Find the highest request below current floor
                it = requests.lower_bound(current_floor);
                if (it != requests.begin()) {
                    --it; // Now points to largest floor < current_floor
                } else {
                    // This should not happen as requests is not empty
                    // But to be safe, go to the first request
                    it = requests.begin();
                }
            }
            // If we found a request at or above, it points to the smallest such request
        } else { // Moving down
            // Find the first request strictly greater than current floor
            it = requests.upper_bound(current_floor);
            
            if (it == requests.begin()) {
                // No requests at or below current floor (since upper_bound returns begin)
                // Reverse direction to up
                direction = 1;
                // Find the lowest request above current floor
                it = requests.upper_bound(current_floor);
                if (it == requests.end()) {
                    // All requests are at or below current_floor, so go to the highest one
                    it = requests.end();
                    --it;
                }
            } else {
                // There are requests at or below current floor
                --it; // Now points to largest floor <= current_floor
            }
        }

        // Execute the request
        current_floor = *it;
        requests.erase(it);
    }

    // Return current floor
    int locate() const {
        return current_floor;
    }
};

int main() {
    int n;
    cin >> n;
    
    Elevator elevator;
    
    for (int i = 0; i < n; i++) {
        string operation;
        cin >> operation;
        
        if (operation == "action") {
            elevator.action();
        } else if (operation == "add") {
            int floor;
            cin >> floor;
            elevator.addRequest(floor);
        } else if (operation == "cancel") {
            int floor;
            cin >> floor;
            elevator.cancelRequest(floor);
        } else if (operation == "locate") {
            cout << elevator.locate() << endl;
        }
    }
    
    return 0;
}