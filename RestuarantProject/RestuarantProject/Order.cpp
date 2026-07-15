#include "Order.h"
#include "Cart.h"
#include <iostream>
#include <iomanip>

Order::Order(int id, int customerId, int restaurantId)
    : id(id),
    customerId(customerId),
    restaurantId(restaurantId),
    status(OrderStatus::PREPARING),
    discount(0),
    shippingCost(0),
    finalPrice(0),
   earnedPoints(0){
}

Order::Order(int id, int customerId, int restaurantId,
    const std::vector<std::shared_ptr<CartItem>>& cartItems)
    : id(id),
    customerId(customerId),
    restaurantId(restaurantId),
    status(OrderStatus::PREPARING),
    discount(0),
    shippingCost(0),
    finalPrice(0),
    earnedPoints(0) {
    for (const auto& ci : cartItems) {
        if (ci && ci->getItem()) {
            items.push_back(ci->getItem());
            quantities.push_back(ci->getQuantity());
        }

    }
}

int Order::getOrderId() const { return id; }
int Order::getCustomerId() const { return customerId; }
int Order::getRestaurantId() const { return restaurantId; }

const std::vector<std::shared_ptr<MenuItem>>& Order::getItems() const
{
    return items;
}

const std::vector<int>& Order::getQuantities() const
{
    return quantities;
}

void Order::addItem(std::shared_ptr<MenuItem> item, int quantity) {
    if (item && quantity > 0) {
        items.push_back(item);
        quantities.push_back(quantity);
    }

}

void Order::setStatus(OrderStatus s) { status = s; }
OrderStatus Order::getStatus() const { return status; }

double Order::getTotalPrice() const {
    double total = 0;
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i]) {
            total += items[i]->getPrice() * quantities[i];
        }
    }
    return total;
}

void Order::showOrder() const {
    std::cout << "Order ID: " << id
        << " | RestaurantID: " << restaurantId
        << " | CustomerID: " << customerId
        << "\n";

    for (size_t i = 0; i < items.size(); i++) {
        if (items[i]) {
            std::cout << "-" << std::left << std::setw(15) << items[i]->getName()
                << " x" << quantities[i]
                << "($" << (items[i]->getPrice() * quantities[i]) << ")\n";
        }

    }

    std::cout << "\n------ Invoice ------\n";

    std::cout << "Base Price : "
        << getTotalPrice()
        << "\n";

    std::cout << "Discount : "
        << discount
        << "\n";

    std::cout << "Shipping : "
        << shippingCost
        << "\n";

    std::cout << "Final Price : "
        << finalPrice
        << "\n";

    std::cout << "Earned Points : "
        << earnedPoints
        << "\n";
    std::cout << "-----------\n";

    std::cout << "Status: ";
    switch (status) {
    case OrderStatus::PREPARING:
        std::cout << "Preparing";
        break;
    case OrderStatus::READY_TO_SEND:
        std::cout << "Ready To Send";
        break;
    case OrderStatus::DELIVERED:
        std::cout << "Delivered";
        break;
    case OrderStatus::CANCELLED:
        std::cout << "Cancelled";
        break;
    default:
        std::cout << "UNKNOWN";
        break;
    }
    std::cout << "\n";
}

void Order::setInvoice(double discount, double shipping, double finalPrice)
{
    this->discount = discount;
    this->shippingCost = shipping;
    this->finalPrice = finalPrice;
}

void Order::setEarnedPoints(int points)
{
    earnedPoints = points;
}

double Order::getDiscount() const
{
    return discount;
}

double Order::getShippingCost() const
{
    return shippingCost;
}

double Order::getFinalPrice() const
{
    return finalPrice;
}

int Order::getEarnedPoints() const
{
    return earnedPoints;
}