#include "CouponDAO.h"
#include "RestuarantProject/sqlite3.h"

CouponDAO::CouponDAO(DatabaseManager& db)
    : dbManager(db)
{
}

bool CouponDAO::insertCoupon(
    std::shared_ptr<Coupon> coupon
)
{
    sqlite3* db = dbManager.getDB();

    const char* sql =
        "INSERT INTO Coupons "
        "(customerId, code, discount, used) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(
        db,
        sql,
        -1,
        &stmt,
        nullptr
    ) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_int(
        stmt,
        1,
        coupon->getCustomerId()
    );

    sqlite3_bind_text(
        stmt,
        2,
        coupon->getCode().c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int(
        stmt,
        3,
        coupon->getDiscount()
    );

    sqlite3_bind_int(
        stmt,
        4,
        coupon->isUsed()
    );

    bool ok =
        (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);

    return ok;
}

std::vector<std::shared_ptr<Coupon>>
CouponDAO::getCouponsByCustomer(
    int customerId
)
{
    sqlite3* db = dbManager.getDB();

    std::vector<std::shared_ptr<Coupon>> result;

    const char* sql =
        "SELECT * FROM Coupons "
        "WHERE customerId=?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(
        db,
        sql,
        -1,
        &stmt,
        nullptr
    ) != SQLITE_OK)
    {
        return result;
    }

    sqlite3_bind_int(
        stmt,
        1,
        customerId
    );

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id =
            sqlite3_column_int(stmt, 0);

        int customerId =
            sqlite3_column_int(stmt, 1);

        std::string code =
            (const char*)sqlite3_column_text(stmt, 2);

        int discount =
            sqlite3_column_int(stmt, 3);

        bool used =
            sqlite3_column_int(stmt, 4);

        auto coupon =
            std::make_shared<Coupon>(
                id,
                customerId,
                code,
                discount,
                used
            );

        result.push_back(coupon);
    }

    sqlite3_finalize(stmt);

    return result;
}

bool CouponDAO::markAsUsed(
    int couponId
)
{
    sqlite3* db = dbManager.getDB();

    const char* sql =
        "UPDATE Coupons "
        "SET used=1 "
        "WHERE id=?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(
        db,
        sql,
        -1,
        &stmt,
        nullptr
    ) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_int(
        stmt,
        1,
        couponId
    );

    bool ok =
        (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);

    return ok;
}