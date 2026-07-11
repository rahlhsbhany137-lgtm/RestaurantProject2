#pragma once

#include <vector>
#include <memory>

#include "DatabaseManager.h"
#include "Order.h"
#include "MenuItemDAO.h"

class OrderDAO
{
private:
    DatabaseManager& dbManager;
   
public:
    OrderDAO(DatabaseManager& databaseManager);

    bool insertOrder(
        std::shared_ptr<Order> order
    );

    std::vector<
        std::shared_ptr<Order>
    > getAllOrders();

    std::shared_ptr<Order>
        getOrderById(int orderId);

    std::vector<std::shared_ptr<Order>> getOrdersByCustomerId(int customerId);
    std::vector<std::shared_ptr<Order>> getOrdersByRestaurantId(int restaurantId);

    bool updateOrderStatus(
        int orderId,
        OrderStatus status
    );

    bool deleteOrder(
        int orderId
    );
};