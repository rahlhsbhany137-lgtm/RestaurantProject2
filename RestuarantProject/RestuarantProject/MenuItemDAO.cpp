#include "MenuItemDAO.h"

MenuItemDAO::MenuItemDAO(DatabaseManager& databaseManager)
    : dbManager(databaseManager)
{
}
std::shared_ptr<MenuItem>
MenuItemDAO::getMenuItemById(int itemId)
{
    sqlite3* db = dbManager.getDB();

    const char* sql =
        "SELECT * FROM MenuItems WHERE id=?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_int(stmt, 1, itemId);

    std::shared_ptr<MenuItem> item = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);

        std::string name = (const char*)sqlite3_column_text(stmt, 2);
        std::string desc = (const char*)sqlite3_column_text(stmt, 3);
        double price = sqlite3_column_double(stmt, 4);
        int type = sqlite3_column_int(stmt, 5);
        int available = sqlite3_column_int(stmt, 6);
        int cookTime = sqlite3_column_int(stmt, 7);
        int volume = sqlite3_column_int(stmt, 8);

        if (type == (int)ItemType::FOOD)
        {
            item = std::make_shared<Food>(
                id, name, desc, price, cookTime, available
            );
        }
        else if (type == (int)ItemType::DRINK)
        {
            item = std::make_shared<Drink>(
                id, name, desc, price, volume, available
            );
        }
    }

    sqlite3_finalize(stmt);
    return item;
}
bool MenuItemDAO::insertMenuItem(int restaurantId, std::shared_ptr<MenuItem> item)
{
    sqlite3* db = dbManager.getDB();
    const char* sql =
        "INSERT OR IGNORE INTO MenuItems "
        "(id, restaurantId, name, description, price, type, available, cookTime, volume) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    int cookTime = 0;
    int volume = 0;

    if (item->getType() == ItemType::FOOD)
    {
        auto food = std::dynamic_pointer_cast<Food>(item);
        if (food)
            cookTime = food->getCookTime();
    }
    else if (item->getType() == ItemType::DRINK)
    {
        auto drink = std::dynamic_pointer_cast<Drink>(item);
        if (drink)
            volume = drink->getVolume();
    }

    sqlite3_bind_int(stmt, 1, item->getId());
    sqlite3_bind_int(stmt, 2, restaurantId);

    sqlite3_bind_text(stmt, 3,
        item->getName().c_str(), -1, SQLITE_TRANSIENT);

    sqlite3_bind_text(stmt, 4,
        item->getDescription().c_str(), -1, SQLITE_TRANSIENT);

    sqlite3_bind_double(stmt, 5, item->getPrice());

    sqlite3_bind_int(stmt, 6, (int)item->getType());

    sqlite3_bind_int(stmt, 7, item->isAvailable() ? 1 : 0);

    sqlite3_bind_int(stmt, 8, cookTime);

    sqlite3_bind_int(stmt, 9, volume);

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);

    return ok;
}

std::vector<std::shared_ptr<MenuItem>>
MenuItemDAO::getMenuItemsByRestaurant(int restaurantId)
{
    sqlite3* db = dbManager.getDB();
    std::vector<std::shared_ptr<MenuItem>> result;

    const char* sql =
        "SELECT * FROM MenuItems WHERE restaurantId=?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return result;

    sqlite3_bind_int(stmt, 1, restaurantId);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);

        const unsigned char* n = sqlite3_column_text(stmt, 2);
        std::string name = n ? (const char*)n : "";

        const unsigned char* d = sqlite3_column_text(stmt, 3);
        std::string desc = d ? (const char*)d : "";

        double price = sqlite3_column_double(stmt, 4);
        int type = sqlite3_column_int(stmt, 5);
        int available = sqlite3_column_int(stmt, 6);
        int cookTime = sqlite3_column_int(stmt, 7);
        int volume = sqlite3_column_int(stmt, 8);

        std::shared_ptr<MenuItem> item = nullptr;

        if (type == (int)ItemType::FOOD)
        {
            item = std::make_shared<Food>(
                id,
                name,
                desc,
                price,
                cookTime,
                available
            );
        }
        else if (type == (int)ItemType::DRINK)
        {
            item = std::make_shared<Drink>(
                id,
                name,
                desc,
                price,
                volume,
                available
            );
        }

        if (item)
            result.push_back(item);
    }

    sqlite3_finalize(stmt);
    return result;
}

bool MenuItemDAO::deleteMenuItem(int itemId)
{
    sqlite3* db = dbManager.getDB();
    const char* sql =
        "DELETE FROM MenuItems WHERE id=?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, itemId);

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);

    return ok;
}

bool MenuItemDAO::updateMenuItem(std::shared_ptr<MenuItem> item)
{
    sqlite3* db = dbManager.getDB();
    const char* sql =
        "UPDATE MenuItems SET "
        "name=?, description=?, price=?, available=? "
        "WHERE id=?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1,
        item->getName().c_str(), -1, SQLITE_TRANSIENT);

    sqlite3_bind_text(stmt, 2,
        item->getDescription().c_str(), -1, SQLITE_TRANSIENT);

    sqlite3_bind_double(stmt, 3,
        item->getPrice());

    sqlite3_bind_int(stmt, 4,
        item->isAvailable() ? 1 : 0);

    sqlite3_bind_int(stmt, 5,
        item->getId());

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);

    return ok;
}