#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>

enum class UserRole {
    CUSTOMER,
    RESTAURANT_ADMIN,
    SYSTEM_ADMIN
};

enum class RestaurantStatus {
    ACTIVE,
    INACTIVE
};

enum class ItemType {
    FOOD,
    DRINK,
    OTHER
};

enum class OrderStatus {
    PREPARING,
    READY_TO_SEND,
    DELIVERED,
    CANCELLED
};

class MenuItem {
protected:
    int id;
    std::string name;
    std::string description;
    double basePrice;
    ItemType type;
    bool available;

public:
    MenuItem(int id,
        const std::string& name,
        const std::string& description,
        double basePrice,
        ItemType type,
        bool available = true)
        : id(id), name(name), description(description),
        basePrice(basePrice), type(type), available(available) {
    }

    virtual ~MenuItem() = default;

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    double getBasePrice() const { return basePrice; }
    ItemType getType() const { return type; }
    bool isAvailable() const { return available; }

    void setAvailable(bool a) { available = a; }
    void setPrice(double p) { basePrice = p; }
    void setName(const std::string& newName) { name = newName; }
    void setDescription(const std::string& newDesc) { description = newDesc; }
   
    double getPrice() const { return basePrice; }

    virtual std::string getSpecialDetails() const = 0;
    virtual void showDetails() const = 0;
};

class Food : public MenuItem {
private:
    int cookTime;
public:
    Food(int id,
        const std::string& name,
        const std::string& description,
        double basePrice,
        int cookTime,
        bool available = true)
        : MenuItem(id, name, description, basePrice, ItemType::FOOD, available),
        cookTime(cookTime) {
    }

    int getCookTime() const { return cookTime; }

    
    std::string getSpecialDetails() const override {
        return "Cook Time: " + std::to_string(cookTime) + " mins";
    }
    void showDetails() const override {
        std::cout << id << ". [Food] " << name << " - $" << basePrice
            << "\n   Desc: " << description
            << "\n   " << getSpecialDetails()
            << (available ? " (Available)" : " (Out of Stock)") << "\n";
    }
};

class Drink : public MenuItem {
private:
    int volume; 

public:
    Drink(int id,
        const std::string& name,
        const std::string& description,
        double basePrice,
        int volume,
        bool available = true)
        : MenuItem(id, name, description, basePrice, ItemType::DRINK, available),
        volume(volume) {
    }

    int getVolume() const { return volume; }

    
    std::string getSpecialDetails() const override {
        return "Volume: " + std::to_string(volume) + " ml";
    }

   
    void showDetails() const override {
        std::cout << id << ". [Drink] " << name << " - $" << basePrice
            << "\n   Desc: " << description
            << "\n   " << getSpecialDetails()
            << (available ? " (Available)" : " (Out of Stock)") << "\n";
    }
};
#endif
