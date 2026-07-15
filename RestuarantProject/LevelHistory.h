#pragma once

#include <string>

class LevelHistory
{
private:
    int id;
    int customerId;
    std::string oldLevel;
    std::string newLevel;
    std::string changeDate;

public:
    LevelHistory(
        int id,
        int customerId,
        const std::string& oldLevel,
        const std::string& newLevel,
        const std::string& changeDate
    );
    int getCustomerId() const;
    std::string getOldLevel() const;
    std::string getNewLevel() const;
    std::string getChangeDate() const;

    void showHistory() const;
};