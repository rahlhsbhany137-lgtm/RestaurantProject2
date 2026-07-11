#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "Models.h"

class System;

class SystemAdmin : public User {
public:
    SystemAdmin(int id, const std::string& u, const std::string& p);
    void addRestaurant(System& system);
    void toggleRestaurant(System& system);
    void viewReports(System& system) const;
};


class RestaurantAdmin : public User {
private:
    int restaurantId;
public:
    RestaurantAdmin(int id, const std::string& u, const std::string& p, int restaurantId);
    int getRestaurantId() const;
    void showOrders(System& system) const;
    void updateOrderStatus(System& system, int orderId, OrderStatus status);
    void runManagerMenu(System& system);
};

#endif
