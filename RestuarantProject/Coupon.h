#pragma once
#include <string>

class Coupon
{
private:
    int id;
    int customerId;
    std::string code;
    int discount;
    bool used;

public:
    Coupon(
        int id,
        int customerId,
        std::string code,
        int discount,
        bool used
    );

    int getId() const;
    int getCustomerId() const;
    std::string getCode() const;
    int getDiscount() const;
    bool isUsed() const;

    void setUsed(bool value);
};
