#include "Restaurant.h"
#include <iostream>
#include <algorithm>

Restaurant::Restaurant(int id,
    const std::string& name,
    const std::string& address,
    int prepTimeMinutes,
    const std::string& phone,
    const std::string& extraDesc,
    int adminUserId)
    : id(id),
    name(name),
    address(address),
    status(RestaurantStatus::INACTIVE),
    prepTimeMinutes(prepTimeMinutes),
    phone(phone),
    extraDesc(extraDesc),
    adminUserId(adminUserId) {
}



int Restaurant::getId() const { return id; }
std::string Restaurant::getName() const { return name; }
std::string Restaurant::getAddress() const { return address; }
int Restaurant::getPrepTimeMinutes() const { return prepTimeMinutes; }
std::string Restaurant::getPhone() const { return phone; }
std::string Restaurant::getExtraDesc() const { return extraDesc; }
int Restaurant::getAdminUserId() const { return adminUserId; }

bool Restaurant::isActiveStatus() const {
    return status == RestaurantStatus::ACTIVE;
}

void Restaurant::toggleStatus() {
    status = (status == RestaurantStatus::ACTIVE)
        ? RestaurantStatus::INACTIVE
        : RestaurantStatus::ACTIVE;
}

void Restaurant::setStatus(RestaurantStatus s)
{
    status = s;
}

void Restaurant::setMenu(const std::vector<std::shared_ptr<MenuItem>>& m)
{
    menu = m;
}

void Restaurant::editDetails(const std::string& n, const std::string& addr, const std::string& ph, const std::string& desc) {
    name = n;
    address = addr;
    phone = ph;
    extraDesc = desc;
    std::cout << "Restaurant details updated successfully.\n";
}

void Restaurant::addMenuItem(std::shared_ptr<MenuItem> item) {
    menu.push_back(item);
}
bool Restaurant::updateMenuItem(int itemId, const std::string& newName, const std::string& newDesc, double newPrice) {
    for (auto& item : menu) {
        if (item && item->getId() == itemId) {
            item->setName(newName);         
            item->setDescription(newDesc); 
            item->setPrice(newPrice);
            std::cout << "Item updated successfully.\n";
            return true;
        }
    }
    std::cout << "Item not found.\n";
    return false;
}

void Restaurant::showMenu() const {
    if (!isActiveStatus()) {
        std::cout << "Restaurant\"" << name << "\"is currently INACTIVE.\n";
        return;
    }
    std::cout << "\n---Menu for" << name << "---\n";
    std::cout << "Description:" << extraDesc << "\n";
    std::cout << "Contact:" << phone << "\n";
    std::cout << "Address:" << address << "\n";

    for (const auto& item : menu) {
        if (!item) continue;
        std::cout << item->getId() << "." << item->getName()
            << " [" << item->getDescription() << "|" << item->getSpecialDetails() << "]"
            << " - $" << item->getPrice()
            << (item->isAvailable() ? " (Available)" : " (Out of Stock)") << "\n";
    }
}
void Restaurant::removeMenuItem(int itemId)
{
    auto it = std::remove_if(
        menu.begin(),
        menu.end(),
        [itemId](const auto& item)
        {
            return item &&
                item->getId() == itemId;
        }
    );

    if (it != menu.end())
    {
        menu.erase(it, menu.end());
        std::cout << "Item ID " << itemId << " removed from menu.\n";
    }
    else
    {
        std::cout << "Item not found.\n";
    }
}

void Restaurant::updateMenuItemPrice(int itemId, double newPrice) {
    for (auto& item : menu) {
        if (item && item->getId() == itemId) {
            item->setPrice(newPrice); 
            std::cout << "Price updated for" << item->getName() << ".\n";
            return;
        }
    }
    std::cout << "Item not found.\n";
}

void Restaurant::toggleItemAvailability(int itemId) {
    for (auto& item : menu) {
        if (item && item->getId() == itemId) {
            item->setAvailable(!item->isAvailable());
            std::cout << "Availability of " << item->getName() << " changed.\n";
            return;
        }
    }
    std::cout << "Item not found.\n";
}

std::shared_ptr<MenuItem>Restaurant::findMenuItemById(int itemId) {
    for (auto& item : menu) {
        if (item && item->getId() == itemId) {
            return item;
        }
    }
    return nullptr;
}
std::vector<std::shared_ptr<MenuItem>>& Restaurant::getMenu() { return menu; }
const std::vector<std::shared_ptr<MenuItem>>& Restaurant::getMenu() const { return menu; }