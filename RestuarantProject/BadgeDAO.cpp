#include "BadgeDAO.h"

BadgeDAO::BadgeDAO(DatabaseManager& databaseManager)
    : dbManager(databaseManager)
{
}

bool BadgeDAO::insertBadge(
    int customerId,
    const std::string& badge
)
{

    sqlite3* db = dbManager.getDB();

    const char* sql =
        "INSERT INTO Badges "
        "(customerId, badge) "
        "VALUES (?, ?);";

    sqlite3_stmt* stmt = nullptr;

    if (
        sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &stmt,
            nullptr
        ) != SQLITE_OK
        )
    {
        return false;
    }

    sqlite3_bind_int(
        stmt,
        1,
        customerId
    );

    sqlite3_bind_text(
        stmt,
        2,
        badge.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    bool success =
        (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);

    return success;
}

std::vector<std::string>
BadgeDAO::getBadgesByCustomer(
    int customerId
)
{
    sqlite3* db = dbManager.getDB();

    std::vector<std::string> badges;

    const char* sql =
        "SELECT badge "
        "FROM Badges "
        "WHERE customerId=?;";

    sqlite3_stmt* stmt = nullptr;

    if (
        sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &stmt,
            nullptr
        ) != SQLITE_OK
        )
    {
        return badges;
    }

    sqlite3_bind_int(
        stmt,
        1,
        customerId
    );

    while (
        sqlite3_step(stmt)
        == SQLITE_ROW
        )
    {
        std::string badge =
            (const char*)
            sqlite3_column_text(
                stmt,
                0
            );

        badges.push_back(
            badge
        );
    }

    sqlite3_finalize(stmt);

    return badges;
}

bool BadgeDAO::deleteBadge(
    int customerId,
    const std::string& badge
)
{
    sqlite3* db = dbManager.getDB();

    const char* sql =
        "DELETE FROM Badges "
        "WHERE customerId=? "
        "AND badge=?;";

    sqlite3_stmt* stmt = nullptr;

    if (
        sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &stmt,
            nullptr
        ) != SQLITE_OK
        )
    {
        return false;
    }

    sqlite3_bind_int(
        stmt,
        1,
        customerId
    );

    sqlite3_bind_text(
        stmt,
        2,
        badge.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    bool success =
        (sqlite3_step(stmt)
            == SQLITE_DONE);

    sqlite3_finalize(stmt);

    return success;
}