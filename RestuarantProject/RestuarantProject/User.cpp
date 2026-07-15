#include "User.h"
#include "System.h"
#include <iostream>
#include "../../miniProject2/MembershipLevel.h"

// ---------- User ----------
User::User(int id, const std::string& u, const std::string& p, UserRole r)
    : id(id), username(u), password(p), role(r) {
}

int User::getId() const { return id; }
std::string User::getUsername() const { return username; }
std::string User::getPassword() const { return password; }
UserRole User::getRole() const { return role; }

// ---------- Customer ----------
Customer::Customer(int id, const std::string& u, const std::string& p)
    : User(id, u, p, UserRole::CUSTOMER), points(0),
level(std::make_unique<NormalLevel>()){
}

Cart& Customer::getCart() { return cart; }

void Customer::addBadge(const std::string& badge)
{
    badges.push_back(badge);
}

std::vector<std::string> Customer::getBadges() const
{
    return badges;
}

bool Customer::hasBadge(const std::string& badge) const
{
    for (const auto& b : badges)
    {
        if (b == badge)
        {
            return true;
        }
    }

    return false;
}

int Customer::getPoints() const
{
    return points;
}

void Customer::setPoints(int p)
{
    points = p;
    updateLevel();

}

void Customer::addPoints(int amount)
{
    points += amount;
    updateLevel();
}

void Customer::removePoints(int amount)
{
    points -= amount;
    if (points < 0)
        points = 0;
    updateLevel();
}

MembershipLevel* Customer::getLevel() const
{
    return level.get();
}

void Customer::setLevel(std::unique_ptr<MembershipLevel> newLevel)
{
    level = std::move(newLevel);
}

void Customer::updateLevel()
{
    std::string oldLevel = level->getLevelName();

    if (points >= 700)
    {
        level = std::make_unique<VIPLevel>();
    }
    else if (points >= 300)
    {
        level = std::make_unique<GoldLevel>();
    }
    else if (points >= 100)
    {
        level = std::make_unique<SilverLevel>();
    }
    else
    {
        level = std::make_unique<NormalLevel>();
    }

    if (oldLevel != level->getLevelName())
    {
        std::cout << "\n================================\n";
        std::cout << "Congratulations!\n";
        std::cout << "Your membership level changed.\n";
        std::cout << oldLevel << " --> " << level->getLevelName() << "\n";
        std::cout << "================================\n";
    }
}

std::shared_ptr<Order> Customer::checkout(System& system, int restaurantId) {
    if (cart.getItems().empty()) {
        std::cout << "Cart is empty!\n";
        return nullptr;
    }

    cart.showCart();
    std::cout << "Confirm order? (1 = yes, 0 = no): ";
    int choice;
    std::cin >> choice;
    if (choice != 1) {
        std::cout << "Checkout cancelled.\n";
        return nullptr;
    }
    double total = cart.getTotal();
    double discount = total * level->getDiscount();
    double shipping = level->getShippingCost(20);
    double finalPrice = total - discount + shipping;

    std::cout << "\n========== Invoice ==========\n";

    std::cout << "Base Price : "
        << total
        << "\n";

    std::cout << "Discount : -"
        << discount
        << "\n";

    std::cout << "Shipping : "
        << shipping
        << "\n";

    std::cout << "-------------------------\n";

    std::cout << "Final Price : "
        << finalPrice
        << "\n";

    std::cout << "Earned Points : "
        << earnedPoints
        << std::endl;

    std::cout << "=========================\n";

    int earnedPoints = static_cast<int>((finalPrice / 10.0) * level->getPointMultiplier());
    auto order = system.createOrder(
        id,
        restaurantId,
        cart.getItems(),
        discount,
        shipping,
        finalPrice,
        earnedPoints
    );

    if (order) {
        std::string oldLevel = level->getLevelName();
        addPoints(earnedPoints);
        std::string newLevel = level->getLevelName();
        if (oldLevel != newLevel)
        {
            system.addLevelHistory(
                getId(),
                oldLevel,
                newLevel
            );
        }
        
        system.updateCustomer(this);

        std::cout << "Earned Points" << earnedPoints << std::endl;
        std::cout << "Current Points" << getPoints() << std::endl;
        std::cout << "Current Level" << getLevel()->getLevelName() << std::endl;

        cart.clear();
        std::cout << "Order placed successfully! Order ID: " << order->getOrderId() << "\n";
        return order;

    }
    return nullptr;
}

void Customer::showHistory(System& system) const {
    auto orders = system.getOrdersByCustomerId(id);
    std::cout << "\n---ORDER HISTORY---\n";
    if (orders.empty()) {
        std::cout << "No orders yet.\n";
        return;
    }
    for (const auto& o : orders) {
        if (o) o->showOrder();
    }
}

void Customer::showBadges() const
{
    std::cout << "\n----- Badges -----\n";

    if (badges.empty())
    {
        std::cout << "No badges.\n";
        return;
    }

    for (const auto& badge : badges)
    {
        std::cout << "- " << badge << std::endl;
    }
}

void Customer::showMembershipInfo() const
{
    std::cout << "\n========== MEMBERSHIP ==========\n";

    std::cout << "Username : "
        << getUsername()
        << std::endl;

    std::cout << "Level : "
        << level->getLevelName()
        << std::endl;

    std::cout << "Points : "
        << points
        << std::endl;

    std::cout << "Next Level : ";

    if (level->getLevelName() == "VIP")
    {
        std::cout << "MAX";
    }
    else
    {
        std::cout << level->getRequiredPoints();
    }

    std::cout << std::endl;

    std::cout << "Discount : "
        << level->getDiscount() * 100
        << "%"
        << std::endl;

    std::cout << "Shipping : ";

    if (level->getLevelName() == "Normal")
        std::cout << "Normal";

    else if (level->getLevelName() == "Silver")
        std::cout << "Half Price";

    else if (level->getLevelName() == "Gold")
        std::cout << "Half Price";

    else if (level->getLevelName() == "VIP")
        std::cout << "Free";

    std::cout << std::endl;
}

