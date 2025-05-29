#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <memory>

using namespace std;

// Abstract Billing Strategy
class BillingStrategy { //parent
public:
    virtual double calculateBill(double baseRate, int nights) const = 0;
    virtual string getBillingType() const = 0;
    virtual ~BillingStrategy() = default; // Virtual destructor for proper cleanup
};

// Concrete Billing Strategies 
class RegularBilling : public BillingStrategy { //child class
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
        return baseRate * nights * 1.10; // 10% service charge
    }
    string getBillingType() const override {
        return "Premium";
    }
};

class CorporateBilling : public BillingStrategy {
public:
    double calculateBill(double baseRate, int nights) const override {
        return baseRate * nights * 0.85; // 15% discount
    }
    string getBillingType() const override {
        return "Corporate";
    }
};

// Room class representing a hotel room
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