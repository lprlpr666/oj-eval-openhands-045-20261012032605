#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <set>

using namespace std;

// We can only use cstdio and iostream, so we need to implement our own heap
// or use priority_queue which is based on heap

class Elevator {
private:
    int current_floor;
    int direction; // 1 for up, -1 for down
    set<int> requests; // Use set to maintain sorted order of requests

public:
    Elevator() : current_floor(0), direction(1) {}

    void addRequest(int floor) {
        requests.insert(floor);
    }

    void cancelRequest(int floor) {
        requests.erase(floor);
    }

    void action() {
        if (requests.empty()) {
            // No requests, stay in place with same direction
            return;
        }

        // Find the next request based on current direction
        auto it = requests.end();
        
        if (direction == 1) { // Moving up
            // Find the smallest floor that is >= current_floor
            it = requests.lower_bound(current_floor);
            
            if (it == requests.end()) {
                // No requests above, reverse direction and go to highest floor below
                direction = -1;
                it = requests.end();
                --it; // Last element (highest floor)
            }
        } else { // Moving down
            // Find the largest floor that is <= current_floor
            it = requests.upper_bound(current_floor);
            if (it != requests.begin()) {
                --it; // Previous element (largest floor <= current_floor)
            } else {
                // No requests below, reverse direction and go to lowest floor above
                direction = 1;
                it = requests.begin();
            }
        }

        // Execute the request
        current_floor = *it;
        requests.erase(it);
    }

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