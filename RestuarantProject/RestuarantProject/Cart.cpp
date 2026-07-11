#include "Cart.h"
#include <algorithm>
#include <iostream>
#include <vector>

CartItem::CartItem(std::shared_ptr<MenuItem> item, int quantity)
    : item(item), quantity(quantity) {
}

std::shared_ptr<MenuItem> CartItem::getItem() const {
    return item;
}

int CartItem::getQuantity() const {
    return quantity;
}

void CartItem::setQuantity(int q) {
    quantity = q;
}


void Cart::addItem(std::shared_ptr<MenuItem> item, int quantity) {
    if (!item || quantity <= 0) return;
    for (auto& ci : items) {
        if (ci && ci->getItem() && ci->getItem()->getId() == item->getId()) {
            ci->setQuantity(ci->getQuantity() + quantity);
            return;
        }
    }

    items.push_back(std::make_shared<CartItem>(item, quantity));
}

void Cart::removeItem(int id) {
    auto originalSize = items.size();
    items.erase(
        std::remove_if(items.begin(), items.end(),
            [id](const std::shared_ptr<CartItem>& ci) {
                return ci && ci->getItem() && ci->getItem()->getId() == id;
            }),
        items.end());
    if (items.size() < originalSize) {
        std::cout << "Item removed from cart.\n";
    }
    else {
        std::cout << "Item not found in cart.\n";
    }
}

double Cart::getTotal() const {
    double total = 0.0;
    for (const auto& ci : items) {
        if (ci && ci->getItem()) {
            total += ci->getItem()->getPrice() * ci->getQuantity();
        }

    }

    return total;
}

void Cart::updateQuantity(int itemId, int newQty) {
    if (newQty <= 0) {
        removeItem(itemId);
        return;
    }
    for (auto& ci : items) {
        if (ci && ci->getItem() && ci->getItem()->getId() == itemId) {
            ci->setQuantity(newQty);
            std::cout << "Quantity updated for " << ci->getItem()->getName() << ".\n";
            return;
        }
    }
    std::cout << "Item not found in cart.\n";
}

void Cart::showCart() const {
    if (items.empty()) {
        std::cout << "Cart is empty\n";
        return;
    }

    std::cout << "---YOUR CART---\n";
    for (const auto& ci : items) {
        if (!ci || !ci->getItem()) continue;
        double subtotal = ci->getItem()->getPrice() * ci->getQuantity();
        std::cout << "ID: " << ci->getItem()->getId() << "|"
            << ci->getItem()->getName()
            << " x " << ci->getQuantity()
            << " = $" << subtotal
            << "\n";
    }

    std::cout << "Total Amount: $" << getTotal() << "\n";
}

std::vector<std::shared_ptr<CartItem>>& Cart::getItems() {
    return items;
}

void Cart::clear() {
    items.clear();
}