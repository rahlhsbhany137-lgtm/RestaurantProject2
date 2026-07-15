#pragma once

#include <vector>
#include <string>

#include "../RestuarantProject/RestuarantProject/DatabaseManager.h"

class BadgeDAO
{
private:
    DatabaseManager& dbManager;

public:
    BadgeDAO(DatabaseManager& db);

    bool insertBadge(
        int customerId,
        const std::string& badge
    );

    std::vector<std::string>
        getBadgesByCustomer(
            int customerId
        );

    bool deleteBadge(
        int customerId,
        const std::string& badge
    );
};