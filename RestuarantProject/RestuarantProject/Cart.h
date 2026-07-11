#pragma once
#include <vector>
#include <memory>
#include "Models.h"

class CartItem {
private:
    std::shared_ptr<MenuItem> item;
    int quantity;

public:
    CartItem(std::shared_ptr<MenuItem> item, int quantity);
    std::shared_ptr<MenuItem> getItem() const;
    int getQuantity() const;
    void setQuantity(int q);

};

class Cart {
private:
    std::vector<std::shared_ptr<CartItem>> items;

public:
    void addItem(std::shared_ptr<MenuItem> item, int quantity);
    void removeItem(int id);
    double getTotal() const;
    void updateQuantity(int itemId, int newQty);
    void showCart() const;
    void clear();
    std::vector<std::shared_ptr<CartItem>>& getItems();

};
