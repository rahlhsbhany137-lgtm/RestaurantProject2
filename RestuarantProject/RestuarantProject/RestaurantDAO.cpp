#include "RestaurantDAO.h"
#include "MenuItemDAO.h"
#include <iostream>

RestaurantDAO::RestaurantDAO(
    DatabaseManager& databaseManager
)
    : dbManager(databaseManager)
{
   
}
bool RestaurantDAO::insertRestaurant(
    std::shared_ptr<Restaurant> restaurant
)
{
    sqlite3* db = dbManager.getDB();

    const char* sql =
        "INSERT OR IGNORE INTO Restaurants "
        "(id, name, address, phone, description, prepTime, status, adminId) "
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
        return false;
    }

    sqlite3_bind_int(
        stmt,
        1,
        restaurant->getId()
    );

    sqlite3_bind_text(
        stmt,
        2,
        restaurant->getName().c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        stmt,
        3,
        restaurant->getAddress().c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        stmt,
        4,
        restaurant->getPhone().c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        stmt,
        5,
        restaurant->getExtraDesc().c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int(
        stmt,
        6,
        restaurant->getPrepTimeMinutes()
    );

    sqlite3_bind_int(
        stmt,
        7,
        restaurant->isActiveStatus() ? 1 : 0
    );

    sqlite3_bind_int(
        stmt,
        8,
        restaurant->getAdminUserId()
    );

    bool success =
        (
            sqlite3_step(stmt)
            ==
            SQLITE_DONE
            );

    sqlite3_finalize(stmt);

    return success;
}

std::vector<
    std::shared_ptr<Restaurant>
>
RestaurantDAO::getAllRestaurants()
{
    sqlite3* db = dbManager.getDB();
    std::vector<
        std::shared_ptr<Restaurant>
    > result;

    const char* sql =
        "SELECT * FROM Restaurants;";

    sqlite3_stmt* stmt;

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
        return result;
    }

    while (
        sqlite3_step(stmt)
        ==
        SQLITE_ROW
        )
    {
        int id =
            sqlite3_column_int(stmt, 0);

        const unsigned char* n = sqlite3_column_text(stmt, 1);
        std::string name = n ? (const char*)n : "";

        const unsigned char* a = sqlite3_column_text(stmt, 2);
        std::string address = a ? (const char*)a : "";

        const unsigned char* p = sqlite3_column_text(stmt, 3);
        std::string phone = p ? (const char*)p : "";

        const unsigned char* d = sqlite3_column_text(stmt, 4);
        std::string desc = d ? (const char*)d : "";


        int prep =
            sqlite3_column_int(stmt, 5);

        int status =
            sqlite3_column_int(stmt, 6);
        

        int adminId =
            sqlite3_column_int(stmt, 7);

        auto restaurant =
            std::make_shared<
            Restaurant>(
                id,
                name,
                address,
                prep,
                phone,
                desc,
                adminId
            );

        restaurant->setStatus(
            static_cast<RestaurantStatus>(status)
        );

        result.push_back(
            restaurant
        );
    }

    sqlite3_finalize(stmt);

    return result;
}

std::shared_ptr<Restaurant>
RestaurantDAO::getRestaurantById(int id)
{
    sqlite3* db = dbManager.getDB();
    const char* sql =
        "SELECT * FROM Restaurants WHERE id=?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_int(stmt, 1, id);

    std::shared_ptr<Restaurant> restaurant = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int rid = sqlite3_column_int(stmt, 0);

        std::string name = (const char*)sqlite3_column_text(stmt, 1);
        std::string address = (const char*)sqlite3_column_text(stmt, 2);
        std::string phone = (const char*)sqlite3_column_text(stmt, 3);
        std::string desc = (const char*)sqlite3_column_text(stmt, 4);

        int prep = sqlite3_column_int(stmt, 5);
        int status = sqlite3_column_int(stmt, 6);
        int adminId = sqlite3_column_int(stmt, 7);

        restaurant = std::make_shared<Restaurant>(
            rid, name, address, prep, phone, desc, adminId
        );

        restaurant->setStatus(static_cast<RestaurantStatus>(status));

        MenuItemDAO menuDAO(dbManager);

        auto menu =
            menuDAO.getMenuItemsByRestaurant(
                restaurant->getId()
            );

        restaurant->setMenu(menu);
    }

    sqlite3_finalize(stmt);
    return restaurant;
}

bool RestaurantDAO::updateRestaurant(std::shared_ptr<Restaurant> r)
{
    sqlite3* db = dbManager.getDB();
    const char* sql =
        "UPDATE Restaurants SET name=?, address=?, phone=?, description=?, prepTime=?, status=?, adminId=? WHERE id=?;";

    sqlite3_stmt* stmt;


    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, r->getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, r->getAddress().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, r->getPhone().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, r->getExtraDesc().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, r->getPrepTimeMinutes());
    sqlite3_bind_int(stmt, 6, r->isActiveStatus() ? 1 : 0);
    sqlite3_bind_int(stmt, 7, r->getAdminUserId());
    sqlite3_bind_int(stmt, 8, r->getId());

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);

   
    sqlite3_finalize(stmt);
    return ok;
}

bool RestaurantDAO::deleteRestaurant(int id)
{
    sqlite3* db = dbManager.getDB();
    const char* sql = "DELETE FROM Restaurants WHERE id=?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, id);

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return ok;
}

bool RestaurantDAO::updateRestaurantStatus(
    int restaurantId,
    bool status
)
{
    sqlite3* db = dbManager.getDB();

    const char* sql =
        "UPDATE Restaurants SET status=? WHERE id=?;";

    sqlite3_stmt* stmt = nullptr;



    std::cout << "Updating Restaurant ID = "
        << restaurantId << std::endl;

    std::cout << "New Status = "
        << status << std::endl;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cout << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, status ? 1 : 0);
    sqlite3_bind_int(stmt, 2, restaurantId);

    int rc = sqlite3_step(stmt);

    std::cout
        << "Changed Rows = "
        << sqlite3_changes(db)
        << std::endl;


    if (rc != SQLITE_DONE)
    {
        std::cout << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}