#pragma once
#include <vector>
#include <memory>
#include <string>

#include "User.h"
#include "Restaurant.h"
#include "Order.h"
#include "Cart.h"
#include "DatabaseManager.h"
#include "UserDAO.h"
#include "RestaurantDAO.h"
#include "MenuItemDAO.h"
#include "OrderDAO.h"
#include "../LevelHistoryDAO.h"
#include "../CouponDAO.h"
#include "../BadgeDAO.h"


class System {
private:
    std::vector<std::shared_ptr<User>> users;
    std::vector<std::shared_ptr<Restaurant>> restaurants;
    

    DatabaseManager db;
    UserDAO* userDAO;
    RestaurantDAO* restaurantDAO;
    MenuItemDAO* menuItemDAO;
    OrderDAO* orderDAO;

    int nextOrderId = 1;
    LevelHistoryDAO* levelHistoryDAO;
    CouponDAO* couponDAO;
    BadgeDAO* badgeDAO;

public:
    System();
    ~System();
    void registerUser(std::shared_ptr<User> user);
    std::shared_ptr<User> login(const std::string& u, const std::string& p);

    void addRestaurant(std::shared_ptr<Restaurant> r);

    
    void showRestaurants() const;

    std::vector<std::shared_ptr<Restaurant>>& getRestaurants();
    const std::vector<std::shared_ptr<Restaurant>>& getRestaurants() const;

    std::shared_ptr<Restaurant> findRestaurantById(int id);
    std::shared_ptr<User> findUserById(int id);

    std::shared_ptr<Order> createOrder(
        int customerId,
        int restaurantId,
        const std::vector<std::shared_ptr<CartItem>>& cartItems,
        double discount,
        double shipping,
        double finalPrice,
        int earnedPoints
    );

  
    std::vector<std::shared_ptr<Order>> getOrdersByCustomerId(int customerId) const;
    std::vector<std::shared_ptr<Order>> getOrdersByRestaurantId(int restaurantId) const;

    bool setOrderStatus(int orderId, int restaurantId, OrderStatus status);

    bool isDatabaseEmpty();
    bool addMenuItemToRestaurant(
        int restaurantId,
        std::shared_ptr<MenuItem> item
    );
    bool updateMenuItemInRestaurant(
        int restaurantId,
        int itemId,
        const std::string& name,
        const std::string& desc,
        double price

    );

    bool removeMenuItemFromRestaurant(
        int restaurantId,
        int itemId
    );

    bool toggleMenuItemAvailability(
        int restaurantId,
        int itemId
    );
    bool toggleRestaurantStatus(int restaurantId);
    bool updateRestaurantInfo(std::shared_ptr<Restaurant> restaurant);
    void showReports() const;
    void showLevelStatistics() const;
    void loadFromDatabase();
    bool updateCustomer(Customer* customer);
    bool addLevelHistory(int customerId, const std::string& oldLevel, const std::string& newLevel);
    void giveMonthlyCoupons();
    void giveBadge(Customer* customer, const std::string& badge);
    void showLevelHistory(int customerId);
    void checkInactiveCustomers();
    bool upgradeCustomer(int customerId);
    bool downgradeCustomer(int customerId);
    
};