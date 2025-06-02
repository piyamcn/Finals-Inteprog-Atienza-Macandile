#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <memory>

using namespace std;

class BillingStrategy { 
public:
    virtual double calculateBill(double baseRate, int nights) const = 0;
    virtual string getBillingType() const = 0;
    virtual ~BillingStrategy() = default;
};

class RegularBilling : public BillingStrategy { 
public:
    double calculateBill(double baseRate, int nights) const override {
        return baseRate * nights;
    }
    string getBillingType() const override {
        return "Regular";
    }
};

class PremiumBilling : public BillingStrategy {
public:
    double calculateBill(double baseRate, int nights) const override {
        return baseRate * nights * 1.10; 
    }
    string getBillingType() const override {
        return "Premium";
    }
};

class CorporateBilling : public BillingStrategy {
public:
    double calculateBill(double baseRate, int nights) const override {
        return baseRate * nights * 0.85; 
    }
    string getBillingType() const override {
        return "Corporate";
    }
};

class Room { 
public:
    enum class RoomType { SINGLE, DOUBLE, DELUXE, SUITE };

private:
    int roomNumber;
    RoomType type;
    double baseRate;
    bool isAvailable;
    unique_ptr<BillingStrategy> billingStrategy;
    int maxGuests;

public:
    Room(int number, RoomType roomType, double rate, unique_ptr<BillingStrategy> strategy, int guests)
        : roomNumber(number), type(roomType), baseRate(rate), isAvailable(true), billingStrategy(move(strategy)), maxGuests(guests) {}

    int getRoomNumber() const { return roomNumber; }
    RoomType getType() const { return type; }
    double getBaseRate() const { return baseRate; }
    bool isRoomAvailable() const { return isAvailable; }
    void setAvailability(bool available) { isAvailable = available; }
    void setBaseRate(double newRate) { baseRate = newRate; }
    void setBillingStrategy(unique_ptr<BillingStrategy> strategy) { billingStrategy = move(strategy); }
    int getMaxGuests() const { return maxGuests; }

    double calculateBill(int nights) const {
        if (nights <= 0) throw invalid_argument("Number of nights must be positive.");
        return billingStrategy->calculateBill(baseRate, nights);
    }

    string getRoomTypeString() const {
        switch (type) {
            case RoomType::SINGLE: return "Single";
            case RoomType::DOUBLE: return "Double";
            case RoomType::DELUXE: return "Deluxe";
            case RoomType::SUITE: return "Suite";
            default: return "Unknown";
        }
    }

    string getBillingStrategyString() const {
        return billingStrategy->getBillingType();
    }
};

class Reservation {
private:
    static int idCounter;
    int reservationID;
    string guestName;
    string contactInfo;
    int roomNumber;
    string checkInDate;
    string checkOutDate;
    int numberOfGuests;

public:
    Reservation(const string& name, const string& contact, int roomNum, const string& checkIn, const string& checkOut, int guests)
        : guestName(name), contactInfo(contact), roomNumber(roomNum), checkInDate(checkIn), checkOutDate(checkOut), numberOfGuests(guests) {
        reservationID = ++idCounter; 
    }

    int getReservationID() const { return reservationID; }
    string getGuestName() const { return guestName; }
    string getContactInfo() const { return contactInfo; }
    int getRoomNumber() const { return roomNumber; }
    string getCheckInDate() const { return checkInDate; }
    string getCheckOutDate() const { return checkOutDate; }
    int getNumberOfGuests() const { return numberOfGuests; }

    void updateGuests(int guests) { numberOfGuests = guests; }
    void updateDates(const string& checkIn, const string& checkOut) {
        checkInDate = checkIn;
        checkOutDate = checkOut;
    }
    void updateRoomNumber(int newRoomNumber) {
        roomNumber = newRoomNumber; 
    }
};

int Reservation::idCounter = 0;

class Hotel {
private:
    vector<Room> rooms;
    vector<Reservation> reservations;

public:
    const vector<Room>& getRooms() const {
        return rooms;
    }

    void addRoom(int number, Room::RoomType type, double rate, unique_ptr<BillingStrategy> strategy, int guests) {
        rooms.emplace_back(number, type, rate, move(strategy), guests);
    }

    void deleteRoom(int roomNumber) {
        for (auto it = rooms.begin(); it != rooms.end(); ++it) {
            if (it->getRoomNumber() == roomNumber) {
                rooms.erase(it);
                cout << "Room " << roomNumber << " deleted successfully!\n";
                return;
            }
        }
        cout << "Room not found.\n";
    }

    void updateRoomRate(int roomNumber, double newRate) {
        for (auto& room : rooms) {
            if (room.getRoomNumber() == roomNumber) {
                room.setBaseRate(newRate);
                cout << "Room " << roomNumber << " rate updated to $" << newRate << " successfully!\n";
                return;
            }
        }
        cout << "Room not found.\n";
    }

    void updateRoomBillingStrategy(int roomNumber, unique_ptr<BillingStrategy> strategy) {
        for (auto& room : rooms) {
            if (room.getRoomNumber() == roomNumber) {
                room.setBillingStrategy(move(strategy));
                cout << "Room " << roomNumber << " billing strategy updated successfully!\n";
                return;
            }
        }
        cout << "Room not found.\n";
    }

    void showAvailableRooms() const {
        cout << "===== AVAILABLE ROOMS =====\n";
        cout << "Room #    Type           Base Rate      Billing Type   Max Guests\n";
        cout << "------------------------------------------------------------------\n";
        for (const auto& room : rooms) {
            if (room.isRoomAvailable()) {
                cout << setw(8) << room.getRoomNumber() << "  "
                     << setw(10) << room.getRoomTypeString() << "  "
                     << "$" << fixed << setprecision(2) << setw(10) << room.getBaseRate() << "  "
                     << setw(12) << room.getBillingStrategyString() << "  "
                     << setw(10) << room.getMaxGuests() << "\n";
            }
        }
        cout << "===========================\n";
    }

    void showAllRooms() const {
        cout << "===== ALL ROOMS =====\n";
        cout << "Room #    Type           Base Rate      Status         Billing Type   Max Guests\n";
        cout << "--------------------------------------------------------------------------------\n";
        for (const auto& room : rooms) {
            cout << setw(8) << room.getRoomNumber() << "  "
                 << setw(10) << room.getRoomTypeString() << "  "
                 << "$" << fixed << setprecision(2) << setw(10) << room.getBaseRate() << "  "
                 << setw(12) << (room.isRoomAvailable() ? "Available" : "Occupied") << "  "
                 << setw(12) << room.getBillingStrategyString() << "  "
                 << setw(10) << room.getMaxGuests() << "\n";
        }
        cout << "===========================\n";
    }

   void makeReservation(const string& guestName, const string& contactInfo, int roomNumber, const string& checkIn, const string& checkOut, int guests) {
    for (auto& room : rooms) {
        if (room.getRoomNumber() == roomNumber) {
            if (guests > room.getMaxGuests()) {
                cout << "Error: Room " << roomNumber << " can only accommodate " << room.getMaxGuests() << " guests.\n";
                return;
            }
            if (room.isRoomAvailable()) {
                room.setAvailability(false);
                reservations.emplace_back(guestName, contactInfo, roomNumber, checkIn, checkOut, guests);
                cout << "Reservation created successfully!\n"; 
                return;
            } else {
                cout << "Room not available for reservation.\n";
                return;
            }
        }
    }
    cout << "Room not found.\n";
}




    void cancelReservation(int reservationID) {
        for (auto it = reservations.begin(); it != reservations.end(); ++it) {
            if (it->getReservationID() == reservationID) {
                for (auto& room : rooms) {
                    if (room.getRoomNumber() == it->getRoomNumber()) {
                        room.setAvailability(true);
                        break;
                    }
                }
                reservations.erase(it);
                cout << "Reservation " << reservationID << " cancelled successfully!\n";
                return;
            }
        }
        cout << "Reservation not found.\n";
    }

    void showAllReservations() const {
        cout << "===== ALL RESERVATIONS =====\n";
        cout << "ID        Guest Name      Room #    Check-in      Check-out\n";
        cout << "-------------------------------------------------------------\n";
        for (const auto& reservation : reservations) {
            cout << setw(8) << reservation.getReservationID() << "  | "
                 << setw(12) << reservation.getGuestName() << "  | "
                 << setw(8) << reservation.getRoomNumber() << "  | "
                 << setw(12) << reservation.getCheckInDate() << "  | "
                 << setw(12) << reservation.getCheckOutDate() << "\n";
        }
        cout << "===========================\n";
    }

    void viewReservationDetails(int reservationID) const {
    for (const auto& reservation : reservations) {
        if (reservation.getReservationID() == reservationID) {
            cout << "===== RESERVATION DETAILS =====\n";
            cout << "Reservation #" << reservation.getReservationID() << "\n";
            cout << "Guest: " << reservation.getGuestName() << "\n";
            cout << "Contact: " << reservation.getContactInfo() << "\n";
            cout << "Room: " << reservation.getRoomNumber() << "\n";
            cout << "Check-in: " << reservation.getCheckInDate() << "\n";
            cout << "Check-out: " << reservation.getCheckOutDate() << "\n";
            cout << "Guests: " << reservation.getNumberOfGuests() << "\n";

            int checkInDay, checkInMonth, checkInYear;
            int checkOutDay, checkOutMonth, checkOutYear;
            sscanf(reservation.getCheckInDate().c_str(), "%d/%d/%d", &checkInDay, &checkInMonth, &checkInYear);
            sscanf(reservation.getCheckOutDate().c_str(), "%d/%d/%d", &checkOutDay, &checkOutMonth, &checkOutYear);

            int nights = 0;

            if (checkInYear > checkOutYear || 
                (checkInYear == checkOutYear && checkInMonth > checkOutMonth) || 
                (checkInYear == checkOutYear && checkInMonth == checkOutMonth && checkInDay >= checkOutDay)) {
                throw runtime_error("Invalid date range.");
            }

            const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

            nights += daysInMonth[checkInMonth - 1] - checkInDay; 

            for (int month = checkInMonth; month < checkOutMonth - 1; ++month) {
                nights += daysInMonth[month];
            }

            nights += checkOutDay;

            double totalBill = 0.0;
            for (const auto& room : rooms) {
                if (room.getRoomNumber() == reservation.getRoomNumber()) {
                    totalBill = room.calculateBill(nights); 
                    break;
                }
            }
            cout << "Total Bill: $" << fixed << setprecision(2) << totalBill << "\n";
            cout << "===============================\n";
            return;
        }
    }
    cout << "Reservation not found.\n";
}


void updateReservation(int reservationID) {
    for (auto& reservation : reservations) {
        if (reservation.getReservationID() == reservationID) {
            cout << "Update Options:\n";
            cout << "1. Change number of guests\n";
            cout << "2. Change room\n";
            cout << "3. Change dates\n";
            cout << "4. Back\n";
            int option;
            option = getValidatedInt("Select update option (1-4): ");

            switch (option) {
                case 1: { 
                    cout << "Current number of guests: " << reservation.getNumberOfGuests() << "\n";
                    int newGuests = getValidatedInt("Enter new number of guests: ");

                    for (const auto& room : rooms) {
                        if (room.getRoomNumber() == reservation.getRoomNumber()) {
                            if (newGuests > room.getMaxGuests()) {
                                cout << "Error: Room " << room.getRoomNumber() << " can only accommodate " << room.getMaxGuests() << " guests.\n";
                                return; 
                            }
                            break; 
                        }
                    }
                    
                    reservation.updateGuests(newGuests);
                    cout << "Number of guests updated successfully.\n";
                    break;
                }
                case 2: { 
                    cout << "Current room: " << reservation.getRoomNumber() << "\n";
                    showAvailableRooms();
                    int newRoomNumber = getValidatedInt("Enter new room number: ");

                    for (auto& room : rooms) {
                        if (room.getRoomNumber() == newRoomNumber) {
  
                            if (reservation.getNumberOfGuests() > room.getMaxGuests()) {
                                cout << "Error: Room " << newRoomNumber << " can only accommodate " << room.getMaxGuests() << " guests.\n";
                                return; 
                            }

                          
                            for (auto& oldRoom : rooms) {
                                if (oldRoom.getRoomNumber() == reservation.getRoomNumber()) {
                                    oldRoom.setAvailability(true);
                                    break;
                                }
                            }

                      
                            reservation.updateRoomNumber(newRoomNumber); 
                            room.setAvailability(false); 
                            cout << "Room changed successfully.\n";
                            return;
                        }
                    }
                    cout << "Room not available.\n";
                    break;
                }
                case 3: {
    cout << "Current check-in date: " << reservation.getCheckInDate() << "\n";
    cout << "Current check-out date: " << reservation.getCheckOutDate() << "\n";
    string newCheckIn, newCheckOut;
    cout << "Enter new check-in date (DD/MM/YYYY): ";
    cin >> newCheckIn;
    cout << "Enter new check-out date (DD/MM/YYYY): ";
    cin >> newCheckOut;
    reservation.updateDates(newCheckIn, newCheckOut); 
    break;
}

                case 4: 
                    return;
                default:
                    cout << "Invalid option.\n";
            }
            return;
        }
    }
    cout << "Reservation not found.\n";
}

  

 
    int getValidatedInt(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            cin >> value;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                cout << "Invalid input. Please enter a number.\n";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
    }
};