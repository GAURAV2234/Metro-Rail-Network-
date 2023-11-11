#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <string>
#include <unordered_map>
#include <cmath>
#include <iomanip> // For rounding distances
using namespace std;

struct Station {
    int node; // Station node index
    double distance; // Distance from the source (using double for geographic distances)
};

struct CompareStation {
    bool operator()(const Station& a, const Station& b) {
        return a.distance > b.distance;
    }
};

struct Coordinates {
    double latitude;
    double longitude;
};

// Function to calculate the Haversine distance between two coordinates
double haversineDistance(const Coordinates& coord1, const Coordinates& coord2) {
    const double earthRadius = 6371.0; // Earth's radius in kilometers
    const double degToRad = M_PI / 180.0;

    double lat1 = coord1.latitude * degToRad;
    double lon1 = coord1.longitude * degToRad;
    double lat2 = coord2.latitude * degToRad;
    double lon2 = coord2.longitude * degToRad;

    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return earthRadius * c;
}

int main() {
    // Define station names and example coordinates for each station
    vector<string> stationNames = {
        "Chennai Central", "Chennai Egmore", "Chennai Beach", "Chennai Tambaram", "Chennai Mambalam",
        "Chennai Perambur", "Chennai Kodambakkam", "Chennai Fort", "Chennai Park", "Chennai Korukkupet",
        "Chennai Royapuram", "Chennai Nungambakkam", "Chennai Chetpet", "Chennai Vyasarpadi",
        "Chennai Basin Bridge", "Chennai Perambur Carriage Works", "Chennai Ennore", "Chennai Ambattur",
        "Chennai Villivakkam", "Chennai Pattaravakkam"
    };

    vector<Coordinates> stationCoordinates = {
        {13.0830, 80.2753}, // Chennai Central
        {13.0832, 80.2658}, // Chennai Egmore
        {13.0733, 80.2783}, // Chennai Beach
        {12.9276, 80.1116}, // Chennai Tambaram
        {13.0392, 80.2201}, // Chennai Mambalam
        {13.1093, 80.2533}, // Chennai Perambur
        {13.0509, 80.2110}, // Chennai Kodambakkam
        {13.0926, 80.2870}, // Chennai Fort
        {13.0742, 80.2654}, // Chennai Park
        {13.1164, 80.2871}, // Chennai Korukkupet
        {13.1122, 80.2951}, // Chennai Royapuram
        {13.0585, 80.2201}, // Chennai Nungambakkam
        {13.0758, 80.2457}, // Chennai Chetpet
        {13.1183, 80.2595}, // Chennai Vyasarpadi
        {13.1087, 80.2703}, // Chennai Basin Bridge
        {13.1096, 80.2593}, // Chennai Perambur Carriage Works
        {13.2144, 80.3273}, // Chennai Ennore
        {13.1127, 80.1646}, // Chennai Ambattur
        {13.1174, 80.2072}, // Chennai Villivakkam
        {13.1402, 80.2479}  // Chennai Pattaravakkam
    };

    cout << "List of stations:" << endl;
    for (size_t i = 0; i < stationNames.size(); ++i) {
        cout << i << ": " << stationNames[i] << endl;
    }

    // Create a map to associate station names with node indices
    unordered_map<string, int> stationNodes;
    for (size_t i = 0; i < stationNames.size(); ++i) {
        stationNodes[stationNames[i]] = i;
    }

    // Initialize all distances to infinity
    vector<double> stationDistances(stationNames.size(), numeric_limits<double>::max());

    // Create a priority queue for Dijkstra's algorithm
    priority_queue<Station, vector<Station>, CompareStation> pq;

    int sourceNode, targetNode;

    cout << "Enter the source station (0 to " << stationNames.size() - 1 << "): ";
    cin >> sourceNode;
    cout << "Enter the target station (0 to " << stationNames.size() - 1 << "): ";
    cin >> targetNode;

    if (sourceNode == targetNode) {
        cout << "Source and target stations are the same: " << stationNames[sourceNode] << endl;
        return 0;
    }

    stationDistances[sourceNode] = 0;
    pq.push({sourceNode, 0.0});

    // Track the path to the target station
    vector<int> prevStation(stationNames.size(), -1);

    // Implement Dijkstra's algorithm
    while (!pq.empty()) {
        Station current = pq.top();
        pq.pop();

        if (current.node == targetNode) {
            break; // Found the target station
        }

        for (size_t i = 0; i < stationNames.size(); ++i) {
            if (i != current.node) {
                double newDistance = current.distance + haversineDistance(stationCoordinates[current.node], stationCoordinates[i]);
                if (newDistance < stationDistances[i]) {
                    stationDistances[i] = newDistance;
                    prevStation[i] = current.node;
                    pq.push({static_cast<int>(i), newDistance});
                }
            }
        }
    }

    if (stationDistances[targetNode] == numeric_limits<double>::max()) {
        cout << "No path found between " << stationNames[sourceNode] << " and " << stationNames[targetNode] << "." << endl;
    } else {
        cout << endl; // Blank line
        cout << "Target Station: " << stationNames[targetNode] << endl;
        cout << endl; // Blank line
        cout << "Shortest distance between " << stationNames[sourceNode] << " and " << stationNames[targetNode] << ": "
             << fixed << setprecision(2) << stationDistances[targetNode] << " kilometers" << endl;

        // Print the connecting stations and distances
        cout << endl; // Blank line
        cout << "Connecting Stations:" << endl;
        int currentStation = targetNode;
        vector<string> connectingStations;
        vector<double> connectingDistances;
        while (currentStation != sourceNode) {
            connectingStations.push_back(stationNames[currentStation]);
            connectingDistances.push_back(haversineDistance(stationCoordinates[currentStation], stationCoordinates[prevStation[currentStation]]));
            currentStation = prevStation[currentStation];
        }
        for (int i = connectingStations.size() - 1; i >= 0; --i) {
            cout << connectingStations[i] << " (" << fixed << setprecision(2) << connectingDistances[i] << " km) <- ";
        }
        cout << stationNames[sourceNode] << endl;
    }

    return 0;
}
