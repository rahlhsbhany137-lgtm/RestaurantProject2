#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <string>
#include <vector>
#include <memory>
#include "Models.h"

class Restaurant {
private:
    int id;
    std::string name;
    std::string address;
    RestaurantStatus status;
    int prepTimeMinutes;
    std::string phone;
    std::string extraDesc;

    int adminUserId; 

    std::vector<std::shared_ptr<MenuItem>> menu;

public:
    Restaurant(int id,
        const std::string& name,
        const std::string& address,
        int prepTimeMinutes,
        const std::string& phone,
        const std::string& extraDesc,
        int adminUserId);

    int getId() const;
    std::string getName() const;

    std::string getAddress() const;
    int getPrepTimeMinutes() const;
    std::string getPhone() const;
    std::string getExtraDesc() const;

    int getAdminUserId() const;

    bool isActiveStatus() const;
    void toggleStatus();
    void setStatus(RestaurantStatus s);
    void setMenu(const std::vector<std::shared_ptr<MenuItem>>& m);
    bool updateMenuItem(int itemId, const std::string& newName, const std::string& newDesc, double newPrice);
    void editDetails(const std::string& n,
        const std::string& addr,
        const std::string& ph,
        const std::string& desc);

    void addMenuItem(std::shared_ptr<MenuItem> item);
    void removeMenuItem(int itemId);
    void updateMenuItemPrice(int itemId, double newPrice);
    void toggleItemAvailability(int itemId);
    void showMenu() const;

    std::shared_ptr<MenuItem>findMenuItemById(int itemId);
    std::vector<std::shared_ptr<MenuItem>>& getMenu();
    const std::vector<std::shared_ptr<MenuItem>>& getMenu() const;
};

#endif
