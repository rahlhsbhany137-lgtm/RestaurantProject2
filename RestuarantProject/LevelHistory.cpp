#include "LevelHistory.h"
#include <iostream>

LevelHistory::LevelHistory(
int id,
int customerId,
const std::string& oldLevel,
const std::string& newLevel,
const std::string& changeDate)
    : id(id),
    customerId(customerId),
    oldLevel(oldLevel),
    newLevel(newLevel),
    changeDate(changeDate)
{
}

int LevelHistory::getCustomerId() const
{
    return customerId;
}

std::string LevelHistory::getOldLevel() const
{
    return oldLevel;
}

std::string LevelHistory::getNewLevel() const
{
    return newLevel;
}

std::string LevelHistory::getChangeDate() const
{
    return changeDate;
}

void LevelHistory::showHistory() const
{
    std::cout << oldLevel
        << " -> "
        << newLevel
        << std::endl;

    std::cout << changeDate
        << std::endl;
}