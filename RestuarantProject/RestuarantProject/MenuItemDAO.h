#pragma once
#include <vector>
#include <memory>
#include "DatabaseManager.h"
#include "Models.h"

class MenuItemDAO {
private:
    DatabaseManager& dbManager;

public:
    MenuItemDAO(DatabaseManager& databaseManager);

    std::shared_ptr<MenuItem>
        getMenuItemById(int itemId);

    bool insertMenuItem(int restaurantId, std::shared_ptr<MenuItem> item);

    std::vector<std::shared_ptr<MenuItem>>
        getMenuItemsByRestaurant(int restaurantId);

    bool updateMenuItem(std::shared_ptr<MenuItem> item);

    bool deleteMenuItem(int itemId);
};
