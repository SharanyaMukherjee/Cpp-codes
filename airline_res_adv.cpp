#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Base class representing a flight
class Flight {
protected:
    string flightNumber;
    string destination;
    string departure;
    int totalSeats;
    int availableSeats;

public:
    // Constructor
    Flight(string flightNumber, string destination, string departure, int totalSeats)
        : flightNumber(flightNumber), destination(destination), departure(departure), totalSeats(totalSeats), availableSeats(totalSeats) {}

    // Virtual destructor
    virtual ~Flight() {}

    // Function to display flight information
    virtual void display() {
        cout << "Flight Number: " << flightNumber << ", Destination: " << destination << ", Departure: " << departure
             << ", Total Seats: " << totalSeats << ", Available Seats: " << availableSeats << endl;
    }

    // Function to reserve a seat
    virtual bool reserveSeat() {
        if (availableSeats > 0) {
            availableSeats--;
            return true;
        }
        return false;
    }

    // Function to write flight details to file
    virtual void writeToFile(ofstream& outFile) const {
        outFile << flightNumber << "," << destination << "," << departure << "," << totalSeats << "," << availableSeats << endl;
    }
};

// Derived class for specific type of flights (e.g., Economy, Business, etc.)
class EconomyFlight : public Flight {
private:
    int economySeats;

public:
    // Constructor
    EconomyFlight(string flightNumber, string destination, string departure, int totalSeats, int economySeats)
        : Flight(flightNumber, destination, departure, totalSeats), economySeats(economySeats) {}

    // Override display function to include specific details
    void display() override {
        cout << "Economy Flight: ";
        Flight::display();
        cout << ", Economy Class Seats: " << economySeats << endl;
    }

    // Override reserveSeat function to handle economy class specifics
    bool reserveSeat() override {
        if (economySeats > 0) {
            economySeats--;
            availableSeats--;
            return true;
        }
        return false;
    }

    // Override writeToFile function to include specific details
    void writeToFile(ofstream& outFile) const override {
        outFile << "Economy,"; // Indicating it's an Economy flight
        Flight::writeToFile(outFile);
    }
};

// Function to display all flights
void displayFlights(const vector<Flight*>& flights) {
    for (const auto& flight : flights) {
        flight->display();
    }
}

// Function to save flights to file
void saveFlightsToFile(const vector<Flight*>& flights, const string& filename) {
    ofstream outFile(filename);

    if (!outFile) {
        cerr << "Error: Unable to open file " << filename << " for writing." << endl;
        return;
    }

    for (const auto& flight : flights) {
        flight->writeToFile(outFile);
    }

    outFile.close();
}

// Function to load flights from file
void loadFlightsFromFile(vector<Flight*>& flights, const string& filename) {
    ifstream inFile(filename);

    if (!inFile) {
        cerr << "Error: Unable to open file " << filename << " for reading." << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string type;
        getline(ss, type, ',');

        string flightNumber;
        string destination;
        string departure;
        int totalSeats;
        int availableSeats;

        getline(ss, flightNumber, ',');
        getline(ss, destination, ',');
        getline(ss, departure, ',');
        ss >> totalSeats;
        ss.ignore(); // Ignore the comma after totalSeats
        ss >> availableSeats;

        if (type == "Economy") {
            flights.push_back(new EconomyFlight(flightNumber, destination, departure, totalSeats, availableSeats));
        } else {
            flights.push_back(new Flight(flightNumber, destination, departure, totalSeats));
        }
    }

    inFile.close();
}

int main() {
    // Initialize random seed for random reservation attempts
    srand(static_cast<unsigned int>(time(nullptr)));

    vector<Flight*> flights;

    // Load existing flights from file or create new flights
    loadFlightsFromFile(flights, "Flights.txt");

    // Example: Display available flights
    displayFlights(flights);

    // Menu loop for reservation system
    while (true) {
        system("cls"); // Clear screen

        cout << "\t Welcome To Airline Management System" << endl;
        cout << "\t ************************************" << endl;
        cout << "\t 1. Reserve A Seat." << endl;
        cout << "\t 2. Exit." << endl;
        cout << "\t Enter Your Choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            system("cls");

            cout << "Available Flights:" << endl;
            displayFlights(flights);

            cout << "Enter Flight Number to Reserve: ";
            string flightNumber;
            cin >> flightNumber;

            // Attempt to reserve a seat on the selected flight
            bool reservationSuccess = false;
            for (auto& flight : flights) {
                if (flight->getFlightNumber() == flightNumber) {
                    reservationSuccess = flight->reserveSeat();
                    break;
                }
            }

            if (reservationSuccess) {
                cout << "Seat Reserved Successfully!" << endl;
            } else {
                cout << "Sorry, Seat Reservation Failed or No Seats Available!" << endl;
            }

            // Save updated flight information to file
            saveFlightsToFile(flights, "Flights.txt");

            system("pause");
        } else if (choice == 2) {
            cout << "Goodbye!" << endl;
            break;
        } else {
            cout << "Invalid choice. Please enter 1 or 2." << endl;
            system("pause");
        }
    }

    // Clean up allocated memory for flights
    for (auto& flight : flights) {
        delete flight;
    }

    return 0;
}

