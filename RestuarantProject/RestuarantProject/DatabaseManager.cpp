#include "DatabaseManager.h"
#include "User.h"
#include "Admin.h"
#include "Models.h"
#include <iostream>
#include <vector>
#include <memory>

DatabaseManager::DatabaseManager()
{
    db = nullptr;
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

bool DatabaseManager::openDatabase(const std::string& dbName)
{
    if (sqlite3_open(dbName.c_str(), &db))
    {
        std::cout << "Cannot open database!\n";
        return false;
    }

    char* error = nullptr;

    if (sqlite3_exec(
        db,
        "PRAGMA foreign_keys = ON;",
        nullptr,
        nullptr,
        &error
    ) != SQLITE_OK)
    {
        std::cout << "Failed to enable foreign keys: "
            << error << std::endl;

        sqlite3_free(error);

        return false;
    }

    std::cout << "Database opened successfully.\n";
    return true;
}

void DatabaseManager::closeDatabase()
{
    if (db)
    {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DatabaseManager::execute(const std::string& sql)
{

    if (!db)
    {
        std::cout
            << "Database not opened\n";

        return false;
    }

    char* errMsg = nullptr;

    int result = sqlite3_exec(
        db,
        sql.c_str(),
        nullptr,
        nullptr,
        &errMsg
    );

    if (result != SQLITE_OK)
    {
        std::cout << "SQL Error: "
            << errMsg
            << std::endl;

        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

void DatabaseManager::createTables()
{
    std::string usersTable =
        "CREATE TABLE IF NOT EXISTS Users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT,"
        "password TEXT,"
        "role INTEGER,"
        "restaurantId INTEGER,"
        "points INTEGER DEFAULT 0,"
        "currentLevel TEXT DEFAULT 'Normal'"
        ");";

    execute(usersTable);

    std::string restaurantsTable =
        "CREATE TABLE IF NOT EXISTS Restaurants ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT,"
        "address TEXT,"
        "phone TEXT,"
        "description TEXT,"
        "prepTime INTEGER,"
        "status INTEGER,"
        "adminId INTEGER,"
        "FOREIGN KEY(adminId) REFERENCES Users(id)"
        ");";

    execute(restaurantsTable);

    std::string menuItemsTable =
        "CREATE TABLE IF NOT EXISTS MenuItems ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "restaurantId INTEGER,"
        "name TEXT,"
        "description TEXT,"
        "price REAL,"
        "type INTEGER,"
        "available INTEGER,"
        "cookTime INTEGER,"
        "volume INTEGER,"
        "FOREIGN KEY(restaurantId) REFERENCES Restaurants(id)"
        ");";

    execute(menuItemsTable);

    std::string ordersTable =
        "CREATE TABLE IF NOT EXISTS Orders ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "customerId INTEGER,"
        "restaurantId INTEGER,"
        "status INTEGER,"
        "total REAL,"
        "discount REAL,"
        "shipping REAL,"
        "finalPrice REAL,"
        "earnedPoints INTEGER,"
        "FOREIGN KEY(customerId) REFERENCES Users(id),"
        "FOREIGN KEY(restaurantId) REFERENCES Restaurants(id)"
        ");";

    execute(ordersTable);

    std::string orderItemsTable =
        "CREATE TABLE IF NOT EXISTS OrderItems("
        "orderId INTEGER,"
        "itemId INTEGER,"
        "quantity INTEGER,"
        "FOREIGN KEY(orderId) REFERENCES Orders(id),"
        "FOREIGN KEY(itemId) REFERENCES MenuItems(id)"
        ");";
    
       execute(orderItemsTable);

       std::string levelHistoryTable =
           "CREATE TABLE IF NOT EXISTS LevelHistory("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "customerId INTEGER,"
           "oldLevel TEXT,"
           "newLevel TEXT,"
           "changeDate TEXT DEFAULT CURRENT_TIMESTAMP,"
           "FOREIGN KEY(customerId) REFERENCES Users(id)"
           ");";

       execute(levelHistoryTable);

       std::string couponTable =
           "CREATE TABLE IF NOT EXISTS Coupons("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "customerId INTEGER,"
           "code TEXT,"
           "discount INTEGER,"
           "used INTEGER,"
           "FOREIGN KEY(customerId) REFERENCES Users(id)"
           ");";

       execute(couponTable);

       std::string badgesTable =
           "CREATE TABLE IF NOT EXISTS Badges ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "customerId INTEGER,"
           "badge TEXT,"
           "FOREIGN KEY(customerId) REFERENCES Users(id)"
           ");";
      
        execute(badgesTable);

    std::cout << "Tables created.\n";
}

sqlite3* DatabaseManager::getDB()
{
    return db;
}