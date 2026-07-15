#pragma once

#include <vector>
#include <memory>

#include "../RestuarantProject/RestuarantProject/DatabaseManager.h"
#include "Coupon.h"
#include "RestuarantProject/sqlite3.h"

class CouponDAO
{
private:
    DatabaseManager& dbManager;

public:

    CouponDAO(DatabaseManager& db);

    bool insertCoupon(
        std::shared_ptr<Coupon> coupon
    );

    std::vector<std::shared_ptr<Coupon>>
        getCouponsByCustomer(
            int customerId
        );

    bool markAsUsed(
        int couponId
    );
};