#pragma once

#include <string>

class MembershipLevel
{
public:

    virtual ~MembershipLevel() {}

    virtual double getDiscount() const = 0;

    virtual double getPointMultiplier() const = 0;

    virtual double getShippingCost(double shipping) const = 0;

    virtual std::string getLevelName() const = 0;

    virtual int getRequiredPoints() const = 0;
};

class NormalLevel : public MembershipLevel
{
public:

    double getDiscount() const override
    {
        return 0;
    }

    double getPointMultiplier() const override
    {
        return 1;
    }

    double getShippingCost(double shipping) const override
    {
        return shipping;
    }

    std::string getLevelName() const override
    {
        return "Normal";
    }

    int getRequiredPoints() const override
    {
        return 100;
    }

};

class SilverLevel : public MembershipLevel
{
public:

    double getDiscount() const override
    {
        return 0.05;
    }

    double getPointMultiplier() const override
    {
        return 1.2;
    }

    double getShippingCost(double shipping) const override
    {
        return shipping * 0.5;
    }

    std::string getLevelName() const override
    {
        return "Silver";
    }

    int getRequiredPoints() const override
    {
        return 300;
    }

};

class GoldLevel : public MembershipLevel {
public:
    double getDiscount()
    {
        return 0.10;
    }

    double getPointMultiplier()
    {
        return 1.5;
    }

    double getShippingCost(double shipping)
    {
        return shipping / 2;
    }

    std::string getLevelName() const override
    {
        return "Gold";
    }

    int getRequiredPoints() const override
    {
        return 700;
    }
};

class VIPLevel : public MembershipLevel {
public:
    double getDiscount()
    {
        return 0.15;
    }

    double getPointMultiplier()
    {
        return 2;
    }

    double getShippingCost(double shipping)
    {
        return 0;
    }

    std::string getLevelName() const override
    {
        return "VIP";
    }

    int getRequiredPoints() const override
    {
        return -1;
    }
};

