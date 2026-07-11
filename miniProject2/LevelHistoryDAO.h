#pragma once

#include <string>
#include "../../RestuarantProject/RestuarantProject/DatabaseManager.h"

class LevelHistoryDAO
{
private:
    DatabaseManager& dbManager;

public:
    LevelHistoryDAO(DatabaseManager& databaseManager);

    bool insertHistory(
        int customerId,
        const std::string& oldLevel,
        const std::string& newLevel
    );
};