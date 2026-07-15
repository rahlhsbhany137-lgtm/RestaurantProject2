#pragma once

#include <vector>
#include <memory>

#include "DatabaseManager.h"
#include "User.h"
#include "Admin.h"

class UserDAO
{
private:
    DatabaseManager& dbManager;

public:
    UserDAO(DatabaseManager& databaseManager);

    bool insertUser(std::shared_ptr<User> user);

    std::vector<std::shared_ptr<User>>
        getAllUsers();

    std::shared_ptr<User> getUserById(int id);
    std::shared_ptr<User> getUserByCredentials(const std::string& username,
        const std::string& password);
    bool updateUser(std::shared_ptr<User> user);
    bool updateCustomerLevelAndPoints(Customer* customer);
    bool deleteUser(int id);
    bool updateLastOrderDate(int customerId);
    std::string getLastOrderDate(int customerId);
};
