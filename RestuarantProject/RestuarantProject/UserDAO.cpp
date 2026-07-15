#include "UserDAO.h"
#include "../MembershipLevel.h"
#include "../BadgeDAO.h"

UserDAO::UserDAO(DatabaseManager& databaseManager)
    : dbManager(databaseManager)
{
}
bool UserDAO::insertUser(
    std::shared_ptr<User> user
)
{
    sqlite3* db = dbManager.getDB();

    const char* sql =
        "INSERT OR IGNORE INTO Users "
        "(id, username, password, role, restaurantId) "
        "VALUES (?, ?, ?, ?, ?);";

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
    int restaurantId = 0;
    if (user->getRole() == UserRole::RESTAURANT_ADMIN)
    {
        auto admin =
            std::dynamic_pointer_cast<RestaurantAdmin>(user);

        if (admin)
        {
            restaurantId =
                admin->getRestaurantId();
        }
    }

    sqlite3_bind_int(
        stmt,
        1,
        user->getId()
    );

    sqlite3_bind_text(
        stmt,
        2,
        user->getUsername().c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        stmt,
        3,
        user->getPassword().c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int(
        stmt,
        4,
        (int)user->getRole()
    );

    sqlite3_bind_int(
        stmt,
        5,
        restaurantId
    );

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);

    return success;
}

std::vector<std::shared_ptr<User>>
UserDAO::getAllUsers()
{
    sqlite3* db = dbManager.getDB();

    std::vector<std::shared_ptr<User>> result;

    const char* sql =
        "SELECT id, username, password, role, restaurantId, currentLevel FROM Users;";

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

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id =
            sqlite3_column_int(stmt, 0);

        std::string username =
            (const char*)
            sqlite3_column_text(stmt, 1);

        std::string password =
            (const char*)
            sqlite3_column_text(stmt, 2);

        int roleInt =
            sqlite3_column_int(stmt, 3);

        int restaurantId =
            sqlite3_column_int(stmt, 4);

        UserRole role =
            static_cast<UserRole>(roleInt);

        std::shared_ptr<User> user;

        if (role == UserRole::CUSTOMER)
        {
            user =
                std::make_shared<Customer>(
                    id,
                    username,
                    password
                );
        }
        else if (
            role ==
            UserRole::RESTAURANT_ADMIN
            )
        {
            user =
                std::make_shared<
                RestaurantAdmin>(
                    id,
                    username,
                    password,
                    restaurantId
                );
        }
        else
        {
            user =
                std::make_shared<
                SystemAdmin>(
                    id,
                    username,
                    password
                );
        }

        result.push_back(user);
    }

    sqlite3_finalize(stmt);

    return result;
}

std::shared_ptr<User>
UserDAO::getUserById(int id)
{

    sqlite3* db = dbManager.getDB();
    const char* sql =
        "SELECT id, username, password, role, restaurantId, currentLevel FROM Users WHERE id=?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_int(stmt, 1, id);

    std::shared_ptr<User> user = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int uid = sqlite3_column_int(stmt, 0);
        std::string username = (const char*)sqlite3_column_text(stmt, 1);
        std::string password = (const char*)sqlite3_column_text(stmt, 2);
        int roleInt = sqlite3_column_int(stmt, 3);
        int restaurantId = sqlite3_column_int(stmt, 4);

        UserRole role = static_cast<UserRole>(roleInt);

        if (role == UserRole::CUSTOMER)
            user = std::make_shared<Customer>(uid, username, password);
        else if (role == UserRole::RESTAURANT_ADMIN)
            user = std::make_shared<RestaurantAdmin>(uid, username, password, restaurantId);
        else
            user = std::make_shared<SystemAdmin>(uid, username, password);
    }

    sqlite3_finalize(stmt);
    return user;
}

std::shared_ptr<User>
UserDAO::getUserByCredentials(const std::string& username,
    const std::string& password)
{
    sqlite3* db = dbManager.getDB();
    const char* sql =
        "SELECT id, username, password, role,"
        "restaurantId, points,  currentLevel FROM Users WHERE username=? AND password=?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    std::shared_ptr<User> user = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        std::string u = (const char*)sqlite3_column_text(stmt, 1);
        std::string p = (const char*)sqlite3_column_text(stmt, 2);
        int roleInt = sqlite3_column_int(stmt, 3);
        int restaurantId = sqlite3_column_int(stmt, 4);
        int points = sqlite3_column_int(stmt, 5);
        std::string currentLevel = (const char*)sqlite3_column_text(stmt, 6);

        UserRole role = static_cast<UserRole>(roleInt);

        if (role == UserRole::CUSTOMER)
        {
            auto customer = std::make_shared<Customer>(id, u, p);
            customer->setPoints(points);

            BadgeDAO badgeDAO(dbManager);

            auto badges = badgeDAO.getBadgesByCustomer(id);

            for (const auto& badge : badges)
            {
                customer->addBadge(badge);
            }

            if (currentLevel == "Normal")
            {
                customer->setLevel(
                    std::make_unique<NormalLevel>()
                );
            }
            else if (currentLevel == "Silver")
            {
                customer->setLevel(
                    std::make_unique<SilverLevel>()
                );
            }
            else if (currentLevel == "Gold")
            {
                customer->setLevel(
                    std::make_unique<GoldLevel>()
                );
            }
            else if (currentLevel == "VIP")
            {
                customer->setLevel(
                    std::make_unique<VIPLevel>()
                );
            }

            user = customer;
        }


        else if (role == UserRole::RESTAURANT_ADMIN)
        {
            user = std::make_shared<RestaurantAdmin>(id, u, p, restaurantId);
        }

        else
        {
            user = std::make_shared<SystemAdmin>(id, u, p);
        }
    }

    sqlite3_finalize(stmt);
    return user;
}

bool UserDAO::updateUser(std::shared_ptr<User> user)
{
    sqlite3* db = dbManager.getDB();
    const char* sql =
        "UPDATE Users SET username=?, password=?, role=?, restaurantId=? WHERE id=?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    int restaurantId = 0;

    if (user->getRole() == UserRole::RESTAURANT_ADMIN)
    {
        auto admin =
            std::dynamic_pointer_cast<RestaurantAdmin>(user);

        if (admin)
        {
            restaurantId =
                admin->getRestaurantId();
        }
    }

    sqlite3_bind_text(stmt, 1, user->getUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user->getPassword().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, (int)user->getRole());
    sqlite3_bind_int(stmt, 4, restaurantId);
    sqlite3_bind_int(stmt, 5, user->getId());

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return ok;
}



bool UserDAO::deleteUser(int id)
{
    sqlite3* db = dbManager.getDB();
    const char* sql = "DELETE FROM Users WHERE id=?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, id);

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);

    return ok;
}

bool UserDAO::updateCustomerLevelAndPoints(Customer* customer)
{
    sqlite3* db = dbManager.getDB();

    const char* sql =
        "UPDATE Users SET points=?, currentLevel=? WHERE id=?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_int(stmt, 1, customer->getPoints());

    sqlite3_bind_text(
        stmt,
        2,
        customer->getLevel()->getLevelName().c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_int(stmt, 3, customer->getId());

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);

    return ok;
}

bool UserDAO::updateLastOrderDate(int customerId)
{
    sqlite3* db = dbManager.getDB();

    const char* sql =
        "UPDATE Users "
        "SET lastOrderDate = CURRENT_TIMESTAMP "
        "WHERE id=?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_int(stmt, 1, customerId);

    bool ok =
        (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);

    return ok;
}

std::string UserDAO::getLastOrderDate(int customerId)
{
    sqlite3* db = dbManager.getDB();

    const char* sql =
        "SELECT lastOrderDate "
        "FROM Users "
        "WHERE id=?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return "";

    sqlite3_bind_int(stmt, 1, customerId);

    std::string result = "";

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const unsigned char* txt =
            sqlite3_column_text(stmt, 0);

        if (txt)
            result =
            (const char*)txt;
    }

    sqlite3_finalize(stmt);

    return result;
}
