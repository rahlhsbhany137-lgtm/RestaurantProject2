#include "Admin.h"
#include "System.h"
#include <iostream>
#include <string>
#include <algorithm>

// SystemAdmin
SystemAdmin::SystemAdmin(int id, const std::string& u, const std::string& p)
    : User(id, u, p, UserRole::SYSTEM_ADMIN) {
}

void SystemAdmin::addRestaurant(System& system) {
    int id, prep, adminId;
    std::string name, address, phone, extra;
    std::cout << "Restaurant ID: "; std::cin >> id;
    std::cout << "Name: "; std::cin >> std::ws; std::getline(std::cin, name);
    std::cout << "Address: "; std::getline(std::cin, address);
    std::cout << "Prep time: "; std::cin >> prep;
    std::cout << "Phone: "; std::cin >> std::ws; std::getline(std::cin, phone);
    std::cout << "Extra: "; std::getline(std::cin, extra);
    std::cout << "Admin ID: "; std::cin >> adminId;
    system.addRestaurant(std::make_shared<Restaurant>(id, name, address, prep, phone, extra, adminId));
}

void SystemAdmin::toggleRestaurant(System& system) {
    int id;
    std::cout << "Restaurant ID: "; std::cin >> id;
    system.toggleRestaurantStatus(id);
}

void SystemAdmin::viewReports(System& system) const { system.showReports(); }

//  RestaurantAdmin
RestaurantAdmin::RestaurantAdmin(int id, const std::string& u, const std::string& p, int restaurantId)
    : User(id, u, p, UserRole::RESTAURANT_ADMIN), restaurantId(restaurantId) {
}

int RestaurantAdmin::getRestaurantId() const { return restaurantId; }

void RestaurantAdmin::showOrders(System& system) const
{
    auto orders =
        system.getOrdersByRestaurantId(restaurantId);

    std::sort(
        orders.begin(),
        orders.end(),
        [&](const std::shared_ptr<Order>& a,
            const std::shared_ptr<Order>& b)
        {
            auto customerA =
                std::dynamic_pointer_cast<Customer>(
                    system.findUserById(a->getCustomerId()));

            auto customerB =
                std::dynamic_pointer_cast<Customer>(
                    system.findUserById(b->getCustomerId()));

            if (!customerA || !customerB)
                return false;

            bool vipA =
                customerA->getLevel()->getLevelName() == "VIP";

            bool vipB =
                customerB->getLevel()->getLevelName() == "VIP";

            return vipA > vipB;
        });

    if (orders.empty())
    {
        std::cout
            << "No orders found for your restaurant.\n";
        return;
    }
    for (const auto& o : orders)
    {
        if (!o) continue;
        auto customer =
            system.findUserById(
                o->getCustomerId()
            );
        if (customer)
        {
            std::cout
                << "\nCustomer Username: "
                << customer->getUsername()
                << "\n";
        }
        o->showOrder();
    }
}

void RestaurantAdmin::updateOrderStatus(System& system, int orderId, OrderStatus status) {
    if (system.setOrderStatus(orderId, restaurantId, status)) {
        std::cout << "Status of Order #" << orderId << "has been updated.\n";
    }

}

void RestaurantAdmin::runManagerMenu(System& system) {
    auto myRestaurant = system.findRestaurantById(this->restaurantId);
    if (!myRestaurant) {
        std::cout << "Error: Restaurant not found.\n";
        return;
    }
    int choice;
    while (true) {

        std::cout << "\nRestaurant Information\n"
            << "----------------------\n"
            << "Name: "
            << myRestaurant->getName()
            << "\n"

            << "Address: "
            << myRestaurant->getAddress()
            << "\n"

            << "Phone: "
            << myRestaurant->getPhone()
            << "\n"

            << "Status:"
            << (myRestaurant->isActiveStatus() ? "Active" : "Inactive")
            << "\n"

            << "Prep Time: "
            << myRestaurant->getPrepTimeMinutes()
            << " minutes\n"

            << "Description: "
            << myRestaurant->getExtraDesc()
            << "\n";

        std::cout << "\n--- Manager Panel (" << myRestaurant->getName() << ") ---\n";
        std::cout << "1. Edit Restaurant Info\n";
        std::cout << "2. View Menu\n";
        std::cout << "3. Add Menu Item\n";
        std::cout << "4. Edit Menu Item\n";
        std::cout << "5. Remove Menu Item\n";
        std::cout << "6. View Received Orders\n";
        std::cout << "7. Update Order Status\n";
        std::cout << "8. Toggle Item Availability\n";
        std::cout << "0. Logout\n";
        std::cout << "Choice: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
        case 1: { // ویرایش اطلاعات رستوران
            std::string n, a, p, d;
            std::cout << "New Name: "; std::cin >> std::ws; std::getline(std::cin, n);
            std::cout << "New Address: "; std::getline(std::cin, a);
            std::cout << "New Phone: "; std::getline(std::cin, p);
            std::cout << "New Description: "; std::getline(std::cin, d);
            myRestaurant->editDetails(n, a, p, d);
            system.updateRestaurantInfo(myRestaurant);
            break;
        }
        case 2:
            myRestaurant->showMenu();
            break;
        case 3: {
            int typeChoice;
            std::cout << "Type (1: Food, 2: Drink):";
            std::cin >> typeChoice;

            int id; double price;
            std::string name, desc;
            std::cout << "ID:"; std::cin >> id;
            std::cout << "Name:"; std::cin >> std::ws; std::getline(std::cin, name);
            std::cout << "Desc:"; std::getline(std::cin, desc);
            std::cout << "Price:"; std::cin >> price;

            if (typeChoice == 1) {
                int cookTime;
                std::cout << "CookTime (min):"; std::cin >> cookTime;
                auto food = std::make_shared<Food>(
                        id,
                        name,
                        desc,
                        price,
                        cookTime
                    );

                system.addMenuItemToRestaurant(myRestaurant->getId(), food);
            }
            else {
                int volume;
                std::cout << "Volume (ml):"; std::cin >> volume;
                auto drink = std::make_shared<Drink>(
                        id,
                        name,
                        desc,
                        price,
                        volume
                    );

                system.addMenuItemToRestaurant(myRestaurant->getId(), drink);
            }
            break;
        }
        case 4: { // ویرایش کامل آیتم
            int id; double price; std::string name, desc;
            std::cout << "Item ID to edit: "; std::cin >> id;
            std::cout << "New Name: "; std::cin >> std::ws; std::getline(std::cin, name);
            std::cout << "New Description: "; std::getline(std::cin, desc);
            std::cout << "New Price: "; std::cin >> price;
            system.updateMenuItemInRestaurant(
                myRestaurant->getId(),
                id,
                name,
                desc,
                price
            );
            break;
        }
        case 5: {
            int id;
            std::cout << "Enter Item ID to remove:"; std::cin >> id;
            system.removeMenuItemFromRestaurant(myRestaurant->getId(), id);
            break;
        }
        case 6:
            showOrders(system);
            break;
        case 7: { 
            int oId, s;
            std::cout << "Order ID: "; std::cin >> oId;
            std::cout << "New Status (0: Preparing, 1: Ready To Send, 2: Delivered, 3:Cancelied): "; std::cin >> s;
            updateOrderStatus(system, oId, static_cast<OrderStatus>(s));
            break;
        }

        case 8: {
            int itemId;

            std::cout << "Item ID: ";
            std::cin >> itemId;

            system.toggleMenuItemAvailability(myRestaurant->getId(),itemId);

            break;
        }

        default:
            std::cout << "Invalid choice.\n";
            break;
        }
    }
}