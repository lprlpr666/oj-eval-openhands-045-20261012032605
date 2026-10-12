#include <iostream>
#include <cstdio>
#include <queue>
#include <unordered_set>

using namespace std;

// Elevator system using two heaps (priority queues) for up and down requests
// This approach is suggested in the problem hint

class Elevator {
private:
    int current_floor;
    int direction; // 1 for up, -1 for down
    
    // Max heap for down requests (we want largest floor first when going down)
    priority_queue<int> down_requests;
    // Min heap for up requests (we want smallest floor first when going up)
    priority_queue<int, vector<int>, greater<int>> up_requests;
    
    // To handle cancel operations, we need to mark requests as canceled
    // We'll use lazy deletion: when canceling, add to canceled set, and when popping, check if it's canceled
    unordered_set<int> canceled;

public:
    Elevator() : current_floor(0), direction(1) {}

    // Add a request for a floor
    void addRequest(int floor) {
        if (floor > current_floor) {
            up_requests.push(floor);
        } else if (floor < current_floor) {
            down_requests.push(floor);
        }
        // If floor == current_floor, it shouldn't happen by problem constraints
    }

    // Cancel a request for a floor
    void cancelRequest(int floor) {
        canceled.insert(floor);
        // Note: we don't remove from heaps immediately, we'll do lazy deletion
    }

    // Remove canceled requests from the top of heaps
    void cleanHeaps() {
        // Clean up_requests
        while (!up_requests.empty() && canceled.find(up_requests.top()) != canceled.end()) {
            canceled.erase(up_requests.top());
            up_requests.pop();
        }
        // Clean down_requests
        while (!down_requests.empty() && canceled.find(down_requests.top()) != canceled.end()) {
            canceled.erase(down_requests.top());
            down_requests.pop();
        }
    }

    // Execute the next action according to elevator rules
    void action() {
        // First, clean the heaps of any canceled requests at the top
        cleanHeaps();
        
        // Check if there are any valid requests
        if (up_requests.empty() && down_requests.empty()) {
            // No pending requests, stay in place with same direction
            return;
        }

        if (direction == 1) { // Moving up
            if (!up_requests.empty()) {
                // There are requests in the up direction, serve the nearest (smallest)
                current_floor = up_requests.top();
                up_requests.pop();
            } else {
                // No requests in up direction, reverse to down
                direction = -1;
                // Serve the nearest down request (largest, since it's a max heap)
                current_floor = down_requests.top();
                down_requests.pop();
            }
        } else { // Moving down
            if (!down_requests.empty()) {
                // There are requests in the down direction, serve the nearest (largest)
                current_floor = down_requests.top();
                down_requests.pop();
            } else {
                // No requests in down direction, reverse to up
                direction = 1;
                // Serve the nearest up request (smallest)
                current_floor = up_requests.top();
                up_requests.pop();
            }
        }
        
        // After serving, we might have added new requests that are on the same floor
        // But by problem constraints, we won't have a request at current_floor
        // Also, clean the heaps in case the served floor was canceled (though it shouldn't be)
        cleanHeaps();
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