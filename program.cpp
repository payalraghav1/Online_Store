#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <ctime>

using namespace std;

class Product {
private:
    int id;
    string name;
    string category;
    double price;

public:
    Product(int prodID, const string& prodName, const string& prodCategory, double prodPrice)
        : id(prodID), name(prodName), category(prodCategory), price(prodPrice) {}

    int getID() const { 
        return id;
    }

    string getName() const {
        return name;
    }

    string getCategory() const { 
        return category; 
    }

    double getPrice() const { 
        return price; 
    }

    void displayProduct() const {
        cout << "ProductID: " << id
             << ", Name: " << name
             << ", Category: " << category
             << ", Price: $" << price << endl;
    }
};

class Order {
private:
    int orderID;
    string customerID;
    time_t orderDate;
    vector<pair<int, int>> orderItems;  // Store pairs of productID and quantity

public:
    Order(int ordID, const string& custID, time_t date)
        : orderID(ordID), customerID(custID), orderDate(date) {}

    void addItem(int productID, int quantity) {
        orderItems.push_back({productID, quantity});
    }

    // Getter method to access orderItems
    const vector<pair<int, int>>& getOrderItems() const {
        return orderItems;
    }

    double calculateTotal(const map<int, Product>& productCatalog) const {
        double total = 0;
        for (const auto& item : orderItems) {
            auto it = productCatalog.find(item.first);  // Find product by productID
            if (it != productCatalog.end()) {
                total += it->second.getPrice() * item.second;  // Calculate price * quantity
            }
        }
        return total;
    }

    void displayOrder(const map<int, Product>& productCatalog) const {
        cout << "OrderID: " << orderID
             << ", CustomerID: " << customerID
             << ", OrderDate: " << ctime(&orderDate);
        cout << "Items in Order:" << endl;
        for (const auto& item : orderItems) {
            auto it = productCatalog.find(item.first);
            if (it != productCatalog.end()) {
                cout << "ProductID: " << item.first
                     << ", Name: " << it->second.getName()
                     << ", Quantity: " << item.second
                     << ", Price: $" << it->second.getPrice()
                     << ", Subtotal: $" << it->second.getPrice() * item.second << endl;
            }
        }
        cout << "Total Bill: $" << calculateTotal(productCatalog) << endl;
    }
};

int main() {
    // Create a product catalog
    map<int, Product> productCatalog;
    productCatalog.emplace(101, Product(101, "Laptop", "Electronics", 1000.0));
    productCatalog.emplace(102, Product(102, "SmartPhone", "Electronics", 800.0));
    productCatalog.emplace(103, Product(103, "Coffee Maker", "Kitchen", 150.0));
    productCatalog.emplace(104, Product(104, "Blender", "Kitchen", 200.0));
    productCatalog.emplace(105, Product(105, "Desk Lamp", "Home", 50.0));

    // Initialize product stock
    map<int, int> productStock = {
        {101, 10},
        {102, 20},
        {103, 15},
        {104, 5},
        {105, 7},
    };

    // Create an order for a customer
    string customerID = "C001";
    int orderID = 1;
    time_t now = time(0);
    Order order(orderID, customerID, now);

    // Customer adds multiple products to the order
    order.addItem(101, 2);  // 2 Laptops
    order.addItem(103, 1);  // 1 Coffee Maker

    // Display all products in the catalog
    cout << "\nAvailable Products:" << endl;
    for (const auto& [id, product] : productCatalog) {
        product.displayProduct();
    }

    // Display the order and calculate the total bill
    cout << "\nOrder Summary:" << endl;
    order.displayOrder(productCatalog);

    // Update stock after the order is placed
    for (const auto& item : order.getOrderItems()) {  // Using the getter method
        if (productStock[item.first] >= item.second) {
            productStock[item.first] -= item.second;
        } else {
            cout << "Insufficient stock for ProductID: " << item.first << endl;
        }
    }

    // Display the updated stock levels
    cout << "\nUpdated Stock Levels:" << endl;
    for (const auto& [id, stock] : productStock) {
        cout << "ProductID: " << id << ", Stock: " << stock << endl;
    }

    return 0;
}
