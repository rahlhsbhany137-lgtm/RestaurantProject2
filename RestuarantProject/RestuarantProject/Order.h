#pragma once
#include <vector>
#include <memory>
#include "Models.h"

class CartItem;

class Order {
private:
    int id;
    int customerId;
    int restaurantId;

    std::vector<std::shared_ptr<MenuItem>> items;
    std::vector<int> quantities;

    OrderStatus status;

    double discount;
    double shippingCost;
    double finalPrice;
    int earnedPoints;

public:
    Order(int id, int customerId, int restaurantId);
    Order(int id, int customerId, int restaurantId,
        const std::vector<std::shared_ptr<CartItem>>& cartItems);

    int getOrderId() const;
    int getCustomerId() const;
    int getRestaurantId() const;
    const std::vector<std::shared_ptr<MenuItem>>& getItems() const;
    const std::vector<int>& getQuantities() const;
    void addItem(std::shared_ptr<MenuItem> item, int quantity);

    void setStatus(OrderStatus s);
    OrderStatus getStatus() const;

    double getTotalPrice() const;
    void showOrder() const;
    void setInvoice(double discount, double shipping, double finalPrice);

    void setEarnedPoints(int points);

    double getDiscount() const;
    double getShippingCost() const;
    double getFinalPrice() const;
    int getEarnedPoints() const;
   

};
