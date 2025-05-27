#include <iostream>
#include <vector>

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
