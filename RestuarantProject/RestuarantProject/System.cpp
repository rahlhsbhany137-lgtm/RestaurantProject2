#include <iostream>
#include <algorithm>

#include "System.h"

System::System() {
    db.openDatabase("restaurant_project2.db");
    db.createTables();
    userDAO = new UserDAO(db);
    orderDAO = new OrderDAO(db);
    restaurantDAO =new RestaurantDAO(db);
    menuItemDAO = new MenuItemDAO(db);
    levelHistoryDAO = new LevelHistoryDAO(db);

    loadFromDatabase();

    std::cout << "Loaded users" << users.size() << std::endl;
}

System::~System()
{
    delete userDAO;
    delete restaurantDAO;
    delete menuItemDAO;
    delete orderDAO;
    delete levelHistoryDAO;
}

void System::registerUser(std::shared_ptr<User> user)
{
    if (!user) return;

    
    users.push_back(user);

    userDAO->insertUser(user);

   
}

std::shared_ptr<User> System::login(const std::string& u, const std::string& p) {
    return userDAO->getUserByCredentials(u, p);
}

void System::addRestaurant(std::shared_ptr<Restaurant> r) {
    restaurants.push_back(r);
    restaurantDAO->insertRestaurant(r);
}

bool System::isDatabaseEmpty()
{
    auto users =
        userDAO->getAllUsers();

    return users.empty();
}

bool System::addMenuItemToRestaurant(
    int restaurantId,
    std::shared_ptr<MenuItem> item
)
{
    auto restaurant =
        findRestaurantById(
            restaurantId
        );

    if (!restaurant)
    {
        return false;
    }

    if (!item)
    {
        return false;
    }

    restaurant->addMenuItem(item);

    return
        menuItemDAO->insertMenuItem(
            restaurantId,
            item
        );
}

bool System::removeMenuItemFromRestaurant(
    int restaurantId,
    int itemId
)
{
    auto restaurant =
        findRestaurantById(
            restaurantId
        );

    if (!restaurant)
    {
        return false;
    }

    restaurant->removeMenuItem(
        itemId
    );

    return
        menuItemDAO->deleteMenuItem(
            itemId
        );
}

bool System::toggleMenuItemAvailability(
    int restaurantId,
    int itemId
)
{
    auto restaurant =
        findRestaurantById(
            restaurantId
        );

    if (!restaurant)
    {
        return false;
    }

    restaurant->toggleItemAvailability(
        itemId
    );

    auto menu =
        restaurant->getMenu();

    for (auto& item : menu)
    {
        if (item->getId() == itemId)
        {
            return
                menuItemDAO->updateMenuItem(
                    item
                );
        }
    }

    return false;
}

bool System::updateMenuItemInRestaurant(
    int restaurantId,
    int itemId,
    const std::string& name,
    const std::string& desc,
    double price
)
{
    auto restaurant =
        findRestaurantById(
            restaurantId
        );

    if (!restaurant)
    {
        return false;
    }

    bool ok =
        restaurant->updateMenuItem(
            itemId,
            name,
            desc,
            price
        );

    if (!ok)
    {
        return false;
    }

    auto item =
        restaurant->findMenuItemById(
            itemId
        );

    if (!item)
    {
        return false;
    }

    return
        menuItemDAO->updateMenuItem(
            item
        );
}
void System::showRestaurants() const {
    std::cout << "\n---List of active restaurants---\n";
    int idx = 1;
    auto restaurants = restaurantDAO->getAllRestaurants();

    for (auto& r : restaurants) {
        if (!r) continue;
        if (!r->isActiveStatus()) continue;
        std::cout << idx++ << ". Name: " << r->getName() << "\n"
            << "About:" << r->getExtraDesc() << "\n"
            << "| Address:" << r->getAddress()
            << "|phone:" << r->getPhone() << "\n"
            << "|Est. Prep Time:" << r->getPrepTimeMinutes() << "mins\n"
            << " (ID=" << r->getId() << ")\n";
    }
    if (idx == 1) std::cout << "No active restaurants.\n";
}

std::vector<std::shared_ptr<Restaurant>>& System::getRestaurants() { return restaurants; }
const std::vector<std::shared_ptr<Restaurant>>& System::getRestaurants() const { return restaurants; }

std::shared_ptr<Restaurant> System::findRestaurantById(int id) {
    return  restaurantDAO->getRestaurantById(id);
}

std::shared_ptr<User> System::findUserById(int id){
    return userDAO->getUserById(id);
}

std::shared_ptr<Order> System::createOrder(
    int customerId,
    int restaurantId,
    const std::vector<std::shared_ptr<CartItem>>& cartItems,
    double discount,
    double shipping,
    double finalPrice,
    int earnedPoints)
{
    auto r = findRestaurantById(restaurantId);

    if (!r)
    {
        std::cout << "Restaurant not found.\n";
        return nullptr;
    }

    if (!r->isActiveStatus())
    {
        std::cout << "Restaurant is inactive.\n";
        return nullptr;
    }

    auto order =
        std::make_shared<Order>(
            nextOrderId++,
            customerId,
            restaurantId,
            cartItems
        );

       order->setInvoice(
        discount,
        shipping,
        finalPrice
    );

    order->setEarnedPoints(
        earnedPoints
    );

    if (!orderDAO->insertOrder(order))
    {
        std::cout << "Failed to save order.\n";
        return nullptr;
    }

    return order;
}

std::vector<std::shared_ptr<Order>> System::getOrdersByCustomerId(int customerId) const {
    return
        orderDAO->getOrdersByCustomerId(customerId);
}

std::vector<std::shared_ptr<Order>> System::getOrdersByRestaurantId(int restaurantId) const {
    return orderDAO->getOrdersByRestaurantId(restaurantId);
}

bool System::setOrderStatus(int orderId, int restaurantId, OrderStatus status)
{
    auto order = orderDAO->getOrderById(orderId);

    if (!order)
        return false;

    if (status == OrderStatus::CANCELLED)
    {
        auto customer =
            std::dynamic_pointer_cast<Customer>(
                findUserById(order->getCustomerId())
            );

        if (customer)
        {
            std::string oldLevel =
                customer->getLevel()->getLevelName();

            customer->removePoints(order->getEarnedPoints());

            std::string newLevel =
                customer->getLevel()->getLevelName();

            if (oldLevel != newLevel)
            {
                addLevelHistory(
                    customer->getId(),
                    oldLevel,
                    newLevel
                );
            }

            updateCustomer(customer.get());
        }
    }

    return orderDAO->updateOrderStatus(orderId, status);
}

bool System::toggleRestaurantStatus(int restaurantId) {
    auto r = findRestaurantById(restaurantId);
    if (!r) return false;
    r->toggleStatus();

    restaurantDAO->updateRestaurantStatus(restaurantId, r->isActiveStatus());

    return true;
}
bool System::updateRestaurantInfo(
    std::shared_ptr<Restaurant> restaurant
)
{
    if (!restaurant)
    {
        return false;
    }

    return
        restaurantDAO->updateRestaurant(
            restaurant
        );
}
void System::showReports() const {
    std::cout << "\n--- SYSTEM REPORTS ---\n";
    std::cout << "Total Registered Users: " << users.size() << "\n";
    std::cout << "Total Restaurants: " << restaurants.size() << "\n";

   
    size_t activeCount = std::count_if(restaurants.begin(), restaurants.end(),
        [](const std::shared_ptr<Restaurant>& r) {
            return r && r->isActiveStatus();
        });

    std::cout << "Total Active: " << activeCount << "\n";
    auto allOrders = orderDAO->getAllOrders();

    std::cout << "Total Orders: " << allOrders.size() << "\n";

    double totalSales = 0;
    for (auto& o : allOrders) {
        if (o) { 
            totalSales += o->getFinalPrice();
        }
    }
    std::cout << "Total Sales: $" << totalSales << "\n";
    std::cout << "----------------------\n";
}

void System::loadFromDatabase()
{
    users =
        userDAO->getAllUsers();

    restaurants =
        restaurantDAO->getAllRestaurants();

    for (auto& r : restaurants)
    {
        if (!r) continue;

        auto menu =
            menuItemDAO->getMenuItemsByRestaurant(
                r->getId()
            );

        r->setMenu(menu);
    }

   auto allOrders =
        orderDAO->getAllOrders();
   nextOrderId = 1;

    for (auto& o : allOrders)
    {
        if (o &&
            o->getOrderId() >= nextOrderId)
        {
            nextOrderId =
                o->getOrderId() + 1;
        }
    }
}

bool System::updateCustomer(Customer* customer)
{
    return userDAO->updateCustomerLevelAndPoints(customer);
}

bool System::addLevelHistory(int customerId, const std::string& oldLevel, const std::string& newLevel)
{
    return levelHistoryDAO->insertHistory(
        customerId,
        oldLevel,
        newLevel
    );
}