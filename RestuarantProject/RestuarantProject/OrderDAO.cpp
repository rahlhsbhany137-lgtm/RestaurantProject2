#include "OrderDAO.h"
#include "MenuItemDAO.h"
#include <iostream>

OrderDAO::OrderDAO(DatabaseManager& databaseManager)
    : dbManager(databaseManager)
{
}

bool OrderDAO::insertOrder(
    std::shared_ptr<Order> order
)
{
    sqlite3* db = dbManager.getDB();

    if (!order)
        return false;
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    const char* sql =
        "INSERT INTO Orders "
        "(customerId, restaurantId, status, total, discount, shipping, finalPrice, earnedPoints) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    if (
        sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &stmt,
            nullptr
        ) != SQLITE_OK
        )
    {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }

    sqlite3_bind_int(
        stmt,
        1,
        order->getCustomerId()
    );

    sqlite3_bind_int(
        stmt,
        2,
        order->getRestaurantId()
    );

    sqlite3_bind_int(
        stmt,
       3,
        (int)order->getStatus()
    );

    sqlite3_bind_double(
        stmt,
        4,
        order->getTotalPrice()
    );

    sqlite3_bind_double(
        stmt,
        5,
        order->getDiscount()
    );

    sqlite3_bind_double(
        stmt,
        6,
        order->getShippingCost()
    );

    sqlite3_bind_double(
        stmt,
        7,
        order->getFinalPrice()
    );

    sqlite3_bind_int(
        stmt,
        8,
        order->getEarnedPoints()
    );

    int newOrderId = 0;

    bool success =
        (sqlite3_step(stmt)
            ==
            SQLITE_DONE);

    if (success) {
        newOrderId = (int)sqlite3_last_insert_rowid(db);
    }

    sqlite3_finalize(stmt);

    if (!success) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
   
    const auto& items =
        order->getItems();

    const auto& quantities =
        order->getQuantities();

    for (
        size_t i = 0;
        i < items.size();
        ++i
        )
    {
        if (!items[i])
            continue;

        const char* itemSql =
            "INSERT INTO OrderItems "
            "(orderId, itemId, quantity) "
            "VALUES (?, ?, ?);";

        sqlite3_stmt* itemStmt = nullptr;

        if (
            sqlite3_prepare_v2(
                db,
                itemSql,
                -1,
                &itemStmt,
                nullptr
            ) != SQLITE_OK
            )
        {
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            return false;
        }

        sqlite3_bind_int(
            itemStmt,
            1,
            newOrderId
        );

        sqlite3_bind_int(
            itemStmt,
            2,
            items[i]->getId()
        );

        sqlite3_bind_int(
            itemStmt,
            3,
            quantities[i]
        );

        if (sqlite3_step(itemStmt) != SQLITE_DONE)
        {
            sqlite3_finalize(itemStmt);
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            return false;
        }

        sqlite3_finalize(itemStmt);

    }

    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    return true;
}

std::vector<std::shared_ptr<Order>>
OrderDAO::getAllOrders()
{

    std::vector<std::shared_ptr<Order>> result;
    sqlite3* db = dbManager.getDB();
    const char* sql = "SELECT * FROM Orders;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return result;

    MenuItemDAO menuDAO(dbManager);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        int customerId = sqlite3_column_int(stmt, 1);
        int restaurantId = sqlite3_column_int(stmt, 2);
        int status = sqlite3_column_int(stmt, 3);
        double discount = sqlite3_column_double(stmt, 5);
        double shipping = sqlite3_column_double(stmt, 6);
        double finalPrice = sqlite3_column_double(stmt, 7);
        int earnedPoints = sqlite3_column_int(stmt, 8);

        auto order = std::make_shared<Order>(id, customerId, restaurantId);
        order->setStatus(static_cast<OrderStatus>(status));

        order->setInvoice(discount, shipping, finalPrice);

        order->setEarnedPoints(earnedPoints);

        const char* itemSql =
            "SELECT itemId, quantity "
            "FROM OrderItems "
            "WHERE orderId=?;";

        sqlite3_stmt* itemStmt = nullptr;

        if (
            sqlite3_prepare_v2(
                db,
                itemSql,
                -1,
                &itemStmt,
                nullptr
            ) == SQLITE_OK
            )
        {
            sqlite3_bind_int(
                itemStmt,
                1,
                id
            );

            while (
                sqlite3_step(itemStmt)
                ==
                SQLITE_ROW
                )
            {
                int itemId =
                    sqlite3_column_int(
                        itemStmt,
                        0
                    );

                int quantity =
                    sqlite3_column_int(
                        itemStmt,
                        1
                    );

                auto item =
                    menuDAO.getMenuItemById(
                        itemId
                    );

                if (item)
                {
                    order->addItem(
                        item,
                        quantity
                    );
                }
            }

            sqlite3_finalize(itemStmt);
        }

        result.push_back(order);
    }

    sqlite3_finalize(stmt);

    return result;
}

std::shared_ptr<Order>
OrderDAO::getOrderById(int orderId)
{
    sqlite3* db = dbManager.getDB();
    const char* sql =
        "SELECT * FROM Orders WHERE id=?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_int(stmt, 1, orderId);

    std::shared_ptr<Order> order = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        int customerId = sqlite3_column_int(stmt, 1);
        int restaurantId = sqlite3_column_int(stmt, 2);
        int status = sqlite3_column_int(stmt, 3);
        double discount = sqlite3_column_double(stmt, 5);
        double shipping = sqlite3_column_double(stmt, 6);
        double finalPrice = sqlite3_column_double(stmt, 7);
        int earnedPoints = sqlite3_column_int(stmt, 8);

        order = std::make_shared<Order>(id, customerId, restaurantId);
        order->setStatus(static_cast<OrderStatus>(status));
        order->setInvoice(discount, shipping, finalPrice);
        order->setEarnedPoints(earnedPoints);
        
        MenuItemDAO menuDAO(dbManager);

        const char* itemSql =
            "SELECT itemId, quantity "
            "FROM OrderItems "
            "WHERE orderId=?;";

        sqlite3_stmt* itemStmt = nullptr;

        if (
            sqlite3_prepare_v2(
                db,
                itemSql,
                -1,
                &itemStmt,
                nullptr
            ) == SQLITE_OK
            )
        {
            sqlite3_bind_int(
                itemStmt,
                1,
                orderId
            );

            while (
                sqlite3_step(itemStmt)
                ==
                SQLITE_ROW
                )
            {
                int itemId =
                    sqlite3_column_int(
                        itemStmt,
                        0
                    );

                int quantity =
                    sqlite3_column_int(
                        itemStmt,
                        1
                    );

                auto item =
                    menuDAO.getMenuItemById(
                        itemId
                    );

                if (item)
                {
                    order->addItem(
                        item,
                        quantity
                    );
                }
            }
            sqlite3_finalize(itemStmt);
        }
    }
    sqlite3_finalize(stmt);
    return order;
}

std::vector<std::shared_ptr<Order>>
OrderDAO::getOrdersByCustomerId(int customerId)
{
    sqlite3* db = dbManager.getDB();
    std::vector<std::shared_ptr<Order>> result;

    const char* sql =
        "SELECT * FROM Orders WHERE customerId=?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return result;

    sqlite3_bind_int(stmt, 1, customerId);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        auto order = getOrderById(id);

        if (order)
        {
            result.push_back(order);
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

std::vector<std::shared_ptr<Order>>
OrderDAO::getOrdersByRestaurantId(int restaurantId)
{
    sqlite3* db = dbManager.getDB();
    std::vector<std::shared_ptr<Order>> result;

    const char* sql =
        "SELECT * FROM Orders WHERE restaurantId=?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return result;

    sqlite3_bind_int(stmt, 1, restaurantId);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        auto order = getOrderById(id);

        if (order)
        {
            result.push_back(order);
        }
    }

    sqlite3_finalize(stmt);
    return result;
}

bool OrderDAO::updateOrderStatus(
    int orderId,
    OrderStatus status
)
{
    sqlite3* db = dbManager.getDB();
    const char* sql =
        "UPDATE Orders "
        "SET status=? "
        "WHERE id=?;";

    sqlite3_stmt* stmt = nullptr;

    if (
        sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &stmt,
            nullptr
        ) != SQLITE_OK
        )
    {
        return false;
    }

    sqlite3_bind_int(
        stmt,
        1,
        (int)status
    );

    sqlite3_bind_int(
        stmt,
        2,
        orderId
    );

    bool success =
        (sqlite3_step(stmt)
            ==
            SQLITE_DONE);

    sqlite3_finalize(stmt);

    return success;
}

bool OrderDAO::deleteOrder(
    int orderId
)
{
    sqlite3* db = dbManager.getDB();
    const char* deleteItemsSql =
        "DELETE FROM OrderItems "
        "WHERE orderId=?;";

    sqlite3_stmt* stmt1 = nullptr;

    if (
        sqlite3_prepare_v2(
            db,
            deleteItemsSql,
            -1,
            &stmt1,
            nullptr
        ) == SQLITE_OK
        )
    {
        sqlite3_bind_int(
            stmt1,
            1,
            orderId
        );

        sqlite3_step(stmt1);

        sqlite3_finalize(stmt1);
    }

    const char* deleteOrderSql =
        "DELETE FROM Orders "
        "WHERE id=?;";

    sqlite3_stmt* stmt2 = nullptr;

    if (
        sqlite3_prepare_v2(
            db,
            deleteOrderSql,
            -1,
            &stmt2,
            nullptr
        ) != SQLITE_OK
        )
    {
        return false;
    }

    sqlite3_bind_int(
        stmt2,
        1,
        orderId
    );

    bool success =
        (sqlite3_step(stmt2)
            ==
            SQLITE_DONE);

    sqlite3_finalize(stmt2);

    return success;
}
