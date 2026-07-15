#pragma once


#include <vector>
#include <string>
#include "../RestuarantProject/LevelHistory.h"
#include "../RestuarantProject/RestuarantProject/DatabaseManager.h"



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

    std::vector<LevelHistory>getHistoryByCustomer(int customerId);
};