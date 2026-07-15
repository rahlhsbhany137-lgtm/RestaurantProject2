#include "LevelHistoryDAO.h"
#include "../RestuarantProject/RestuarantProject/sqlite3.h"
#include <vector>

LevelHistoryDAO::LevelHistoryDAO(DatabaseManager& databaseManager)
    : dbManager(databaseManager)
{
}

bool LevelHistoryDAO::insertHistory(
    int customerId,
    const std::string& oldLevel,
    const std::string& newLevel
)
{
    sqlite3* db = dbManager.getDB();

    const char* sql =
        "INSERT INTO LevelHistory "
        "(customerId, oldLevel, newLevel) "
        "VALUES (?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_int(stmt, 1, customerId);

    sqlite3_bind_text(
        stmt,
        2,
        oldLevel.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        stmt,
        3,
        newLevel.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);

    return ok;
}

std::vector<LevelHistory>
LevelHistoryDAO::getHistoryByCustomer(int customerId)
{
    std::vector<LevelHistory> historyList;

    sqlite3* db = dbManager.getDB();

    const char* sql =
        "SELECT id, customerId, oldLevel, newLevel, changeDate "
        "FROM LevelHistory "
        "WHERE customerId = ? "
        "ORDER BY id ASC;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return historyList;
    }

    sqlite3_bind_int(stmt, 1, customerId);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id =
            sqlite3_column_int(stmt, 0);

        int customerId =
            sqlite3_column_int(stmt, 1);


        std::string oldLevel =
            reinterpret_cast<const char*>(
                sqlite3_column_text(stmt, 2)
                );

        std::string newLevel =
            reinterpret_cast<const char*>(
                sqlite3_column_text(stmt, 3)
                );

        std::string changeDate =
            reinterpret_cast<const char*>(
                sqlite3_column_text(stmt, 4)
                );

        LevelHistory history(
            id,
            customerId,
            oldLevel,
            newLevel,
            changeDate
        );

        historyList.push_back(history);
    }

    sqlite3_finalize(stmt);

    return historyList;
}