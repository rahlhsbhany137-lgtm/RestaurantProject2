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