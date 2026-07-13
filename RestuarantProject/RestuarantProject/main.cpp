#include "DatabaseManager.h"
#include <iostream>
#include <memory>
#include <string>
#include <limits>

#include "System.h"
#include "Restaurant.h"
#include "User.h"
#include "Models.h"
#include "Admin.h"

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    System system;
    if (system.isDatabaseEmpty()) {
        auto sysAdmin = std::make_shared<SystemAdmin>(1, "admin", "admin");
        system.registerUser(sysAdmin);


        auto rAdmin1 = std::make_shared<RestaurantAdmin>(100, "manager1", "123", 1);
        auto rAdmin2 = std::make_shared<RestaurantAdmin>(101, "manager2", "123", 2);
        auto rAdmin3 = std::make_shared<RestaurantAdmin>(102, "manager3", "123", 3);
        system.registerUser(rAdmin1);
        system.registerUser(rAdmin2);
        system.registerUser(rAdmin3);


        auto aliCust = std::make_shared<Customer>(300, "ali", "123");
        system.registerUser(aliCust);


        auto r1 =
            std::make_shared<Restaurant>(
                1,
                "Pizza House",
                "Valiasr St",
                25,
                "021111",
                "Best Pizza",
                100
            );
        r1->toggleStatus();

        system.addRestaurant(r1);
        auto pizza =
            std::make_shared<Food>(
                10,
                "Peperoni",
                "Spicy",
                12.5,
                20
            );

        system.addMenuItemToRestaurant(
            r1->getId(),
            pizza
        );
        auto pepsi =
            std::make_shared<Drink>(
                20,
                "Pepsi",
                "Cold",
                2.0,
                330
            );

        system.addMenuItemToRestaurant(
            r1->getId(),
            pepsi
        );

        auto r2 = std::make_shared<Restaurant>(2, "Burger Land", "Azadi st", 15, "021222", "Best Burgers", 101);
        r2->toggleStatus();
        system.addRestaurant(r2);
        auto burger =
            std::make_shared<Food>(
                30,
                "Cheese Burger",
                "Double Cheese",
                9.5,
                15
            );

        system.addMenuItemToRestaurant(
            r2->getId(),
            burger
        );

        auto fanta =
            std::make_shared<Drink>(
                40,
                "Fanta",
                "Orange",
                2.5,
                330
            );

        system.addMenuItemToRestaurant(
            r2->getId(),
            fanta
        );



        // Restaurant 

        auto r3 =
            std::make_shared<Restaurant>(
                3,
                "Kebab House",
                "Enghelab St",
                20,
                "021333",
                "Traditional Foods",
                102
            );
        r3->toggleStatus();
        system.addRestaurant(r3);
        auto koobideh =
            std::make_shared<Food>(
                50,
                "Koobideh",
                "Iranian Kebab",
                14.0,
                20
            );

        system.addMenuItemToRestaurant(
            r3->getId(),
            koobideh
        );

        auto doogh =
            std::make_shared<Drink>(
                60,
                "Doogh",
                "Traditional",
                1.5,
                250
            );

        system.addMenuItemToRestaurant(
            r3->getId(),
            doogh
        );

    }

    while (true) {
        std::cout << "\n========== WELCOME TO RESTAURANT SYSTEM ==========\n";
        std::cout << "1. Login\n2. Register (Customer)\n0. Exit\nChoice: ";
        int mainChoice;
        if (!(std::cin >> mainChoice)) { clearInput(); continue; }
        if (mainChoice == 0) break;

        if (mainChoice == 2) {
            std::string u, p;
            std::cout << "Enter new username: "; std::cin >> u;
            std::cout << "Enter new password: "; std::cin >> p;
            system.registerUser(std::make_shared<Customer>(rand() % 9000 + 1000, u, p));
            std::cout << "Registered successfully!\n";
            continue;
        }

        std::string username, password;
        std::cout << "Username: "; std::cin >> username;
        std::cout << "Password: "; std::cin >> password;

        auto user = system.login(username, password);
        if (!user) {
            std::cout << "Login failed! Incorrect credentials.\n";
            continue;
        }

        if (auto a = std::dynamic_pointer_cast<SystemAdmin>(user)) {
            int choice;
            do {
                std::cout << "\n--- SYSTEM ADMIN PANEL ---\n1. Register New Restaurant\n2. Toggle Status\n3. Reports\n4. Level Statistics\n5. Give Monthly Coupons\n0. Logout\nChoice: ";
                std::cin >> choice;
                if (choice == 1) a->addRestaurant(system);
                else if (choice == 2) a->toggleRestaurant(system);
                else if (choice == 3) a->viewReports(system);
                else if (choice == 4) system.showLevelStatistics();
                else if (choice == 5) system.giveMonthlyCoupons();
            } while (choice != 0);
        }
        else if (auto m = std::dynamic_pointer_cast<RestaurantAdmin>(user)) {
            m->runManagerMenu(system);
        }
        else if (auto c = std::dynamic_pointer_cast<Customer>(user)) {
            std::shared_ptr<Restaurant>selectedRestaurant = nullptr;
            int choice;
            do {
                std::cout << "\n--- CUSTOMER PANEL (" << c->getUsername() << ") ---\n";
                std::cout << "1. List Restaurants\n2. Select Restaurant\n3. View Menu\n4. Add to Cart\n5. Show Cart\n6. Checkout\n7. History\n8. show Badges\n9. Membership Information\n0. Logout\nChoice: ";
                if (!(std::cin >> choice)) { clearInput(); break; }
                switch (choice) {
                case 1: system.showRestaurants(); break;
                case 2: {
                    int restId;
                    std::cout << "Restaurant ID: ";
                    if (!(std::cin >> restId)) {
                        clearInput();
                        break;
                    }

                    selectedRestaurant = system.findRestaurantById(restId);
                    if (selectedRestaurant &&
                        selectedRestaurant->isActiveStatus()) {
                        std::cout << "Selected: "
                            << selectedRestaurant->getName()
                            << "\n";
                    }
                    else {
                        std::cout << "Restaurant not found or inactive.\n";
                        selectedRestaurant = nullptr;
                    }
                    break;
                }

                case 3: {
                    if (selectedRestaurant) {
                        selectedRestaurant->showMenu();
                    }
                    else {
                        std::cout << "Select a restaurant first.\n";
                    }
                    break;
                }
                case 4: {
                    if (!selectedRestaurant) {
                        std::cout << "Select a restaurant first.\n";
                        break;
                    }

                    int itemId;
                    int qty;

                    std::cout << "Item ID: ";
                    std::cin >> itemId;

                    std::cout << "Qty: ";
                    std::cin >> qty;

                    auto item =
                        selectedRestaurant->findMenuItemById(itemId);

                    if (item) {
                        c->getCart().addItem(item, qty);
                        std::cout << "Added to cart.\n";
                    }
                    else {
                        std::cout << "Item not found.\n";
                    }

                    break;
                }
                case 5: c->getCart().showCart(); break;

                case 6:
                {
                    if (!selectedRestaurant)
                    {
                        std::cout << "No restaurant selected.\n";
                        break;
                    }

                    c->checkout(system, selectedRestaurant->getId());

                    break;
                }

                case 7: c->showHistory(system); 
                    break;

                case 8: c->showBadges();
                  break;

                case 9: c->showMembershipInfo();
                    break;
              
                }
            } while (choice != 0);
        }
    }
    return 0;
}
