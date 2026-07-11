#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Cart.h"
#include "Order.h"
#include "../../miniProject2/miniProject2/MembershipLevel.h"

class System; 

class User {
protected:
    int id;
    std::string username;
    std::string password;
    UserRole role;

public:
    User(int id, const std::string& u, const std::string& p, UserRole r);
    virtual ~User() = default;

    int getId() const;
    std::string getUsername() const;
    std::string getPassword() const;
    UserRole getRole() const;
};


class Customer : public User {
private:
    Cart cart;

    int points;
    std::unique_ptr<MembershipLevel> level;

public:
    Customer(int id, const std::string& u, const std::string& p);

    Cart& getCart();

  
    std::shared_ptr<Order> checkout(System& system, int restaurantId);

    void showHistory(System& system) const;

    int getPoints() const;
    void setPoints(int p);
    void addPoints(int amount);
    void removePoints(int amount);
    MembershipLevel* getLevel() const;
    void setLevel(std::unique_ptr<MembershipLevel> newLevel);
    void updateLevel();

};