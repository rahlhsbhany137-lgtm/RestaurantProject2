#include "LevelHistoryDAO.h"

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