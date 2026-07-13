#include "Coupon.h"

Coupon::Coupon(
    int id,
    int customerId,
    std::string code,
    int discount,
    bool used
)
{
    this->id = id;
    this->customerId = customerId;
    this->code = code;
    this->discount = discount;
    this->used = used;
}

int Coupon::getId() const
{
    return id;
}

int Coupon::getCustomerId() const
{
    return customerId;
}

std::string Coupon::getCode() const
{
    return code;
}

int Coupon::getDiscount() const
{
    return discount;
}

bool Coupon::isUsed() const
{
    return used;
}

void Coupon::setUsed(bool value)
{
    used = value;
}