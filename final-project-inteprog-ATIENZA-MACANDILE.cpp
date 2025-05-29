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
};