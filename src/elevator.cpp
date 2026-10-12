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
            // Find the smallest floor that is > current_floor (strictly greater)
            it = requests.upper_bound(current_floor);
            
            if (it == requests.end()) {
                // No requests above, reverse direction and go to highest floor below or equal
                direction = -1;
                it = requests.lower_bound(current_floor);
                if (it != requests.begin()) {
                    --it; // Previous element (largest floor < current_floor)
                } else {
                    // No requests below, but we have requests (since set is not empty)
                    // This means all requests are at or above current_floor, but we already checked above
                    // So this should not happen
                    it = requests.begin();
                }
            }
        } else { // Moving down
            // Find the largest floor that is < current_floor (strictly less)
            it = requests.lower_bound(current_floor);
            if (it != requests.begin()) {
                --it; // Previous element (largest floor < current_floor)
            } else {
                // No requests below, reverse direction and go to lowest floor above or equal
                direction = 1;
                it = requests.upper_bound(current_floor);
                if (it == requests.end()) {
                    // All requests are <= current_floor, so go to the highest one
                    it = requests.end();
                    --it;
                }
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