#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <vector>
#include <memory>
#include "User.h"
#include "Admin.h"
#include "sqlite3.h"

class DatabaseManager {
private:
    sqlite3* db;

public:
    DatabaseManager();
    ~DatabaseManager();

    bool openDatabase(const std::string& dbName);
    void closeDatabase();

    bool execute(const std::string& sql);


    void createTables();

    sqlite3* getDB();
};

#endif
