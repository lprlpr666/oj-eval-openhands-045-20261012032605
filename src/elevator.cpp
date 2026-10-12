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
            // Look for any request at or above current floor
            it = requests.lower_bound(current_floor);
            if (it == requests.end()) {
                // No requests above or at current floor, reverse direction
                direction = -1;
                // Find the largest request below current floor
                it = requests.lower_bound(current_floor);
                if (it != requests.begin()) {
                    --it;
                } else {
                    // This should not happen since requests is not empty
                    it = requests.begin();
                }
            }
            // If we found a request at or above, we go to the first one (smallest >= current_floor)
            // Note: we won't have a request at current_floor because we just served it or it was canceled
        } else { // Moving down
            // Look for any request at or below current floor
            it = requests.upper_bound(current_floor);
            if (it != requests.begin()) {
                --it; // Largest request <= current_floor
            } else {
                // No requests below or at current floor, reverse direction
                direction = 1;
                // Find the smallest request above current floor
                it = requests.upper_bound(current_floor);
                if (it == requests.end()) {
                    // All requests are at or below current_floor, so go to the largest one
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