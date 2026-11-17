#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <queue>
#include <stack>
#include <algorithm>
#include <cctype>
#include <vector>
using namespace std;

struct Product {
    int id;
    string name;
    string category;
    string brand;
    double price;
    int stockQuantity;
};

class ProductCatalog {
private:
    Product* items;
    int count;
    int capacity;

    void resize(int newCapacity) {
        Product* newItems = new Product[newCapacity];

        for (int i = 0; i < count; ++i)
            newItems[i] = items[i];

        delete[] items;
        items = newItems;
        capacity = newCapacity;
    }

public:
    ProductCatalog() : items(new Product[4]), count(0), capacity(4) {}

    ~ProductCatalog() {
        delete[] items;
    }

    ProductCatalog(const ProductCatalog&) = delete;
    ProductCatalog& operator=(const ProductCatalog&) = delete;

    int size() const {
        return count;
    }

    Product& at(int index) {
        return items[index];
    }

    const Product& at(int index) const {
        return items[index];
    }

    int findIndex(int id) const {
        for (int i = 0; i < count; ++i) {
            if (items[i].id == id)
                return i;
        }
        return -1;
    }

    bool add(const Product& product) {
        if (findIndex(product.id) != -1)
            return false;

        if (count == capacity)
            resize(capacity * 2);

        items[count++] = product;
        return true;
    }

    bool removeById(int id, Product& removed) {
        int index = findIndex(id);

        if (index == -1)
            return false;

        removed = items[index];

        for (int i = index; i < count - 1; ++i)
            items[i] = items[i + 1];

        --count;

        if (capacity > 4 && count <= capacity / 4)
            resize(capacity / 2);

        return true;
    }

    void updateStock(int id, int newStock) {
        int index = findIndex(id);

        if (index != -1)
            items[index].stockQuantity = newStock;
    }

    vector<Product> toVector() const {
        vector<Product> result;

        for (int i = 0; i < count; ++i)
            result.push_back(items[i]);

        return result;
    }
};

struct CartNode {
    int productId;
    string productName;
    double unitPrice;
    int quantity;
    CartNode* prev;
    CartNode* next;

    CartNode(int id, const string& name, double price, int qty)
        : productId(id), productName(name), unitPrice(price),
          quantity(qty), prev(nullptr), next(nullptr) {}
};

class ShoppingCart {
private:
    CartNode* head;
    CartNode* tail;
    int itemCount;

public:
    ShoppingCart() : head(nullptr), tail(nullptr), itemCount(0) {}

    ~ShoppingCart() {
        clear();
    }

    bool empty() const {
        return head == nullptr;
    }

    int quantityFor(int id) const {
        for (CartNode* current = head; current; current = current->next) {
            if (current->productId == id)
                return current->quantity;
        }

        return 0;
    }

    void add(int id, const string& name, double price, int quantity) {
        for (CartNode* current = head; current; current = current->next) {
            if (current->productId == id) {
                current->quantity += quantity;
                return;
            }
        }

        CartNode* node = new CartNode(id, name, price, quantity);

        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }

        ++itemCount;
    }

    bool remove(int id) {
        CartNode* current = head;

        while (current && current->productId != id)
            current = current->next;

        if (!current)
            return false;

        if (current->prev)
            current->prev->next = current->next;
        else
            head = current->next;

        if (current->next)
            current->next->prev = current->prev;
        else
            tail = current->prev;

        delete current;
        --itemCount;

        return true;
    }

    void displayForward() const {
        if (!head) {
            cout << "\nYour cart is empty.\n";
            return;
        }

        cout << "\n--- SHOPPING CART (FORWARD) ---\n";

        double total = 0;

        for (CartNode* current = head; current; current = current->next) {
            double subtotal = current->unitPrice * current->quantity;

            cout << "ID: " << current->productId
                 << " | " << current->productName
                 << " | Qty: " << current->quantity
                 << " | Subtotal: P"
                 << fixed << setprecision(2) << subtotal << '\n';

            total += subtotal;
        }

        cout << "Cart total: P"
             << fixed << setprecision(2) << total << '\n';
    }

    void displayBackward() const {
        if (!tail) {
            cout << "\nYour cart is empty.\n";
            return;
        }

        cout << "\n--- SHOPPING CART (BACKWARD) ---\n";

        for (CartNode* current = tail; current; current = current->prev)
            cout << current->productName
                 << " (ID " << current->productId << ")\n";
    }

    vector<pair<int, int>> getItems() const {
        vector<pair<int, int>> result;

        for (CartNode* current = head; current; current = current->next)
            result.push_back({current->productId, current->quantity});

        return result;
    }

    double total() const {
        double sum = 0;

        for (CartNode* current = head; current; current = current->next)
            sum += current->unitPrice * current->quantity;

        return sum;
    }

    int size() const {
        return itemCount;
    }

    void clear() {
        CartNode* current = head;

        while (current) {
            CartNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }

        head = tail = nullptr;
        itemCount = 0;
    }
};

// ---------------- LAB 3: CIRCULAR LINKED LIST ----------------
struct BrandNode {
    string brand;
    BrandNode* next;

    explicit BrandNode(const string& name)
        : brand(name), next(nullptr) {}
};

class BrandCircle {
private:
    BrandNode* tail;

public:
    BrandCircle() : tail(nullptr) {}

    ~BrandCircle() {
        if (!tail)
            return;

        BrandNode* start = tail->next;
        tail->next = nullptr;

        while (start) {
            BrandNode* nextNode = start->next;
            delete start;
            start = nextNode;
        }
    }

    void add(const string& brand) {
        BrandNode* node = new BrandNode(brand);

        if (!tail) {
            tail = node;
            tail->next = tail;
        } else {
            node->next = tail->next;
            tail->next = node;
            tail = node;
        }
    }

    void displayCycle() const {
        if (!tail) {
            cout << "No partner brands available.\n";
            return;
        }

        cout << "\n--- OFFICIAL PARTNER BRAND SHOWCASE ---\n";

        BrandNode* current = tail->next;

        do {
            cout << "-> " << current->brand << '\n';
            current = current->next;
        } while (current != tail->next);

        cout << "-> Back to " << tail->next->brand
             << " (circular link)\n";
    }
};

struct UndoAction {
    string action;
    Product product;
};

void rebuildProductIndex(class ProductBST& tree,
                         const ProductCatalog& catalog);

struct TreeNode {
    Product product;
    TreeNode* left;
    TreeNode* right;

    explicit TreeNode(const Product& p)
        : product(p), left(nullptr), right(nullptr) {}
};

class ProductBST {
private:
    TreeNode* root;

    TreeNode* insertNode(TreeNode* node, const Product& product) {
        if (!node)
            return new TreeNode(product);

        if (product.id < node->product.id)
            node->left = insertNode(node->left, product);
        else if (product.id > node->product.id)
            node->right = insertNode(node->right, product);

        return node;
    }

    const Product* searchNode(TreeNode* node, int id) const {
        if (!node)
            return nullptr;

        if (id == node->product.id)
            return &node->product;

        if (id < node->product.id)
            return searchNode(node->left, id);

        return searchNode(node->right, id);
    }

    void destroy(TreeNode* node) {
        if (!node)
            return;

        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    ProductBST() : root(nullptr) {}

    ~ProductBST() {
        destroy(root);
    }

    void clear() {
        destroy(root);
        root = nullptr;
    }

    void insert(const Product& product) {
        root = insertNode(root, product);
    }

    const Product* search(int id) const {
        return searchNode(root, id);
    }
};

void rebuildProductIndex(ProductBST& tree,
                         const ProductCatalog& catalog) {
    tree.clear();

    for (int i = 0; i < catalog.size(); ++i)
        tree.insert(catalog.at(i));
}

struct Order {
    int orderNumber;
    vector<pair<int, int>> items;
    double total;
};

class CheckoutSystem {
private:
    queue<Order> orders;
    int nextOrderNumber;

public:
    CheckoutSystem() : nextOrderNumber(1001) {}

    int placeOrder(const vector<pair<int, int>>& items, double total) {
        Order order;

        order.orderNumber = nextOrderNumber++;
        order.items = items;
        order.total = total;

        orders.push(order);

        return order.orderNumber;
    }

    bool empty() const {
        return orders.empty();
    }

    int size() const {
        return static_cast<int>(orders.size());
    }

    void display() const {
        if (orders.empty()) {
            cout << "\nThere are no pending orders.\n";
            return;
        }

        queue<Order> copy = orders;

        cout << "\n--- PENDING CHECKOUT ORDERS (FIFO) ---\n";

        while (!copy.empty()) {
            const Order& order = copy.front();

            cout << "Order #" << order.orderNumber
                 << " | Items: " << order.items.size()
                 << " | Total: P"
                 << fixed << setprecision(2)
                 << order.total << '\n';

            copy.pop();
        }
    }

    bool processNext(Order& processed) {
        if (orders.empty())
            return false;

        processed = orders.front();
        orders.pop();

        return true;
    }
};

int readInt(const string& prompt) {
    int value;

    while (true) {
        cout << prompt;

        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Invalid input. Please enter a whole number.\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double readDouble(const string& prompt) {
    double value;

    while (true) {
        cout << prompt;

        if (cin >> value && value >= 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Invalid amount. Enter zero or a positive number.\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readText(const string& prompt) {
    string value;

    cout << prompt;
    getline(cin, value);

    return value;
}

string toLowerCase(string text) {
    transform(text.begin(), text.end(), text.begin(),
        [](unsigned char c) {
            return static_cast<char>(tolower(c));
        });

    return text;
}

void displayProduct(const Product& p) {
    cout << left << setw(7) << p.id
         << setw(27) << p.name.substr(0, 26)
         << setw(19) << p.category.substr(0, 18)
         << setw(15) << p.brand.substr(0, 14)
         << "P" << right << setw(10)
         << fixed << setprecision(2) << p.price
         << "   " << setw(5) << p.stockQuantity << '\n';
}

void displayProductHeader() {
    cout << left << setw(7) << "ID"
         << setw(27) << "Product Name"
         << setw(19) << "Category"
         << setw(15) << "Brand"
         << right << setw(11) << "Price"
         << "   " << setw(5) << "Stock" << '\n';

    cout << string(85, '-') << '\n';
}

void displayProducts(const vector<Product>& products) {
    if (products.empty()) {
        cout << "\nNo matching products found.\n";
        return;
    }

    displayProductHeader();

    for (const Product& product : products)
        displayProduct(product);
}

void displayCatalog(const ProductCatalog& catalog) {
    cout << "\n========== GLOW & CARE PRODUCT CATALOG ==========\n";

    if (catalog.size() == 0) {
        cout << "The catalog is empty.\n";
        return;
    }

    displayProductHeader();

    for (int i = 0; i < catalog.size(); ++i)
        displayProduct(catalog.at(i));
}

void loadSampleProducts(ProductCatalog& catalog) {
    catalog.add({2001, "Hydrating Cleanser", "Facial Cleanser",
                 "CeraVe", 599.00, 20});

    catalog.add({2002, "Moisturizing Cream", "Moisturizer",
                 "CeraVe", 899.00, 15});

    catalog.add({2003, "Niacinamide 10%", "Serum",
                 "The Ordinary", 650.00, 18});

    catalog.add({2004, "Hyaluronic Acid 2%", "Serum",
                 "The Ordinary", 720.00, 12});

    catalog.add({2005, "Micellar Water", "Cleanser",
                 "Garnier", 299.00, 30});

    catalog.add({2006, "Vitamin C Serum", "Serum",
                 "Garnier", 499.00, 16});

    catalog.add({2007, "Soft Moisturizing Cream", "Moisturizer",
                 "Nivea", 250.00, 25});

    catalog.add({2008, "Sun Protect SPF 50", "Sunscreen",
                 "Nivea", 450.00, 22});

    catalog.add({2009, "Foaming Facial Cleanser", "Facial Cleanser",
                 "CeraVe", 699.00, 10});

    catalog.add({2010, "Natural Glow Sunscreen", "Sunscreen",
                 "Garnier", 399.00, 14});
}

void filterByBrand(const ProductCatalog& catalog) {
    string brand = toLowerCase(readText("Enter brand name: "));
    vector<Product> matches;

    for (int i = 0; i < catalog.size(); ++i) {
        if (toLowerCase(catalog.at(i).brand) == brand)
            matches.push_back(catalog.at(i));
    }

    displayProducts(matches);
}

void searchByKeyword(const ProductCatalog& catalog) {
    string keyword = toLowerCase(readText("Enter product keyword: "));
    vector<Product> matches;

    // LAB 7: LINEAR SEARCH
    for (int i = 0; i < catalog.size(); ++i) {
        const Product& p = catalog.at(i);

        if (toLowerCase(p.name).find(keyword) != string::npos ||
            toLowerCase(p.category).find(keyword) != string::npos ||
            toLowerCase(p.brand).find(keyword) != string::npos) {

            matches.push_back(p);
        }
    }

    cout << "\nLinear Search Results:\n";
    displayProducts(matches);
}

void filterByPrice(const ProductCatalog& catalog) {
    double minimum = readDouble("Minimum price (P): ");
    double maximum = readDouble("Maximum price (P): ");

    if (minimum > maximum) {
        cout << "Minimum price cannot be greater than maximum price.\n";
        return;
    }

    vector<Product> sorted = catalog.toVector();

    sort(sorted.begin(), sorted.end(),
        [](const Product& a, const Product& b) {
            return a.price < b.price;
        });

    // LAB 7: BINARY SEARCH
    auto first = lower_bound(
        sorted.begin(), sorted.end(), minimum,
        [](const Product& p, double price) {
            return p.price < price;
        });

    auto last = upper_bound(
        sorted.begin(), sorted.end(), maximum,
        [](double price, const Product& p) {
            return price < p.price;
        });

    vector<Product> matches(first, last);

    cout << "\nProducts within the selected price range:\n";
    displayProducts(matches);
}

void addProduct(ProductCatalog& catalog,
                stack<UndoAction>& undoStack,
                ProductBST& tree) {
    Product p;

    p.id = readInt("Enter new product ID: ");

    if (p.id <= 0 || catalog.findIndex(p.id) != -1) {
        cout << "Invalid or already-used product ID.\n";
        return;
    }

    p.name = readText("Enter product name: ");
    p.category = readText("Enter category: ");
    p.brand = readText("Enter brand: ");

    if (p.name.empty() || p.category.empty() || p.brand.empty()) {
        cout << "Product name, category, and brand cannot be empty.\n";
        return;
    }

    p.price = readDouble("Enter price (P): ");
    p.stockQuantity = readInt("Enter stock quantity: ");

    if (p.stockQuantity < 0) {
        cout << "Stock cannot be negative.\n";
        return;
    }

    if (catalog.add(p)) {
        undoStack.push({"ADD", p});
        rebuildProductIndex(tree, catalog);

        cout << "Product added successfully.\n";
    }
}

void removeProduct(ProductCatalog& catalog,
                   stack<UndoAction>& undoStack,
                   ProductBST& tree) {
    int id = readInt("Enter product ID to remove: ");
    Product removed{};

    if (catalog.removeById(id, removed)) {
        undoStack.push({"REMOVE", removed});
        rebuildProductIndex(tree, catalog);

        cout << "Product removed successfully. You can undo this action.\n";
    } else {
        cout << "Product ID not found.\n";
    }
}

void undoAdminAction(ProductCatalog& catalog,
                     stack<UndoAction>& undoStack,
                     ProductBST& tree) {
    if (undoStack.empty()) {
        cout << "There is no admin action to undo.\n";
        return;
    }

    UndoAction action = undoStack.top();
    undoStack.pop();

    if (action.action == "ADD") {
        Product removed{};
        catalog.removeById(action.product.id, removed);

        cout << "Undo complete: the added product was removed.\n";
    }
    else if (action.action == "REMOVE") {
        catalog.add(action.product);

        cout << "Undo complete: the removed product was restored.\n";
    }

    rebuildProductIndex(tree, catalog);
}

void insertionSortByPrice(vector<Product>& products) {
    for (size_t i = 1; i < products.size(); ++i) {
        Product key = products[i];
        int j = static_cast<int>(i) - 1;

        while (j >= 0 && products[j].price > key.price) {
            products[j + 1] = products[j];
            --j;
        }

        products[j + 1] = key;
    }
}

int partitionByStock(vector<Product>& products, int low, int high) {
    int pivot = products[high].stockQuantity;
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (products[j].stockQuantity <= pivot) {
            ++i;
            swap(products[i], products[j]);
        }
    }

    swap(products[i + 1], products[high]);

    return i + 1;
}

void quickSortByStock(vector<Product>& products, int low, int high) {
    if (low < high) {
        int pivotIndex = partitionByStock(products, low, high);

        quickSortByStock(products, low, pivotIndex - 1);
        quickSortByStock(products, pivotIndex + 1, high);
    }
}

void sortingMenu(const ProductCatalog& catalog) {
    vector<Product> products = catalog.toVector();

    cout << "\n--- SORT PRODUCTS ---\n";
    cout << "1. Insertion Sort by price (lowest to highest)\n";
    cout << "2. Quick Sort by stock (lowest to highest)\n";
    cout << "3. Sort by brand (A-Z)\n";

    int choice = readInt("Choose sorting method: ");

    if (choice == 1) {
        insertionSortByPrice(products);
        cout << "\nSorted using Insertion Sort:\n";
    }
    else if (choice == 2) {
        if (!products.empty()) {
            quickSortByStock(
                products, 0, static_cast<int>(products.size()) - 1);
        }

        cout << "\nSorted using Quick Sort:\n";
    }
    else if (choice == 3) {
        sort(products.begin(), products.end(),
            [](const Product& a, const Product& b) {
                return a.brand < b.brand;
            });

        cout << "\nProducts sorted by brand:\n";
    }
    else {
        cout << "Invalid sorting choice.\n";
        return;
    }

    displayProducts(products);
}

void addToCart(ProductCatalog& catalog, ShoppingCart& cart) {
    int id = readInt("Enter product ID: ");
    int index = catalog.findIndex(id);

    if (index == -1) {
        cout << "Product not found.\n";
        return;
    }

    Product& p = catalog.at(index);

    int quantity = readInt("Enter quantity: ");

    if (quantity <= 0) {
        cout << "Quantity must be greater than zero.\n";
        return;
    }

    int alreadyInCart = cart.quantityFor(id);

    if (quantity + alreadyInCart > p.stockQuantity) {
        cout << "Not enough available stock.\n";
        return;
    }

    cart.add(p.id, p.name, p.price, quantity);

    cout << "Product added to cart.\n";
}

void cartMenu(ProductCatalog& catalog, ShoppingCart& cart) {
    cout << "\n--- SHOPPING CART ---\n";
    cout << "1. Add product to cart\n";
    cout << "2. View cart (forward)\n";
    cout << "3. View cart (backward)\n";
    cout << "4. Remove product from cart\n";
    cout << "5. Clear cart\n";

    int choice = readInt("Choose an option: ");

    switch (choice) {
        case 1:
            addToCart(catalog, cart);
            break;

        case 2:
            cart.displayForward();
            break;

        case 3:
            cart.displayBackward();
            break;

        case 4: {
            int id = readInt("Enter product ID to remove from cart: ");

            cout << (cart.remove(id)
                ? "Item removed from cart.\n"
                : "Item not found in cart.\n");

            break;
        }

        case 5:
            cart.clear();
            cout << "Cart cleared.\n";
            break;

        default:
            cout << "Invalid choice.\n";
    }
}

void placeCheckoutOrder(ProductCatalog& catalog,
                        ShoppingCart& cart,
                        CheckoutSystem& checkout) {
    if (cart.empty()) {
        cout << "Your cart is empty. Add products first.\n";
        return;
    }

    vector<pair<int, int>> items = cart.getItems();

    // Check all stock levels before changing any stock.
    for (const auto& item : items) {
        int index = catalog.findIndex(item.first);

        if (index == -1 ||
            catalog.at(index).stockQuantity < item.second) {

            cout << "Checkout failed: stock is no longer available "
                 << "for product ID " << item.first << ".\n";

            return;
        }
    }

    double total = cart.total();

    // Reserve stock when order enters the FIFO checkout queue.
    for (const auto& item : items) {
        int index = catalog.findIndex(item.first);
        catalog.at(index).stockQuantity -= item.second;
    }

    int orderNumber = checkout.placeOrder(items, total);

    cart.clear();

    cout << "Order #" << orderNumber
         << " added to checkout queue.\n";

    cout << "Total: P"
         << fixed << setprecision(2) << total << '\n';

    cout << "Stock has been reserved.\n";
}

void processCheckout(CheckoutSystem& checkout) {
    Order order{};

    if (!checkout.processNext(order)) {
        cout << "No pending checkout orders.\n";
        return;
    }

    cout << "\nOrder #" << order.orderNumber
         << " processed successfully!\n";

    cout << "Order total: P"
         << fixed << setprecision(2) << order.total << '\n';

    cout << "This order was processed first because checkout uses FIFO.\n";
}

void initializeBrands(BrandCircle& brands) {
    brands.add("CeraVe");
    brands.add("The Ordinary");
    brands.add("Garnier");
    brands.add("Nivea");
}

void showMenu() {
    cout << "\n====================================================\n";
    cout << "       GLOW & CARE BEAUTY AND SKINCARE STORE\n";
    cout << "====================================================\n";

    cout << " 1. View all products\n";
    cout << " 2. Filter products by partner brand\n";
    cout << " 3. Search product by keyword (Linear Search)\n";
    cout << " 4. Filter products by price range (Binary Search)\n";
    cout << " 5. Shopping cart\n";
    cout << " 6. Place order / checkout queue\n";
    cout << " 7. View pending checkout orders\n";
    cout << " 8. Process next checkout order\n";
    cout << " 9. Undo last admin action (Stack)\n";
    cout << "10. Partner brand showcase (Circular List)\n";
    cout << "11. Sort products\n";
    cout << "12. Search product by ID (BST)\n";
    cout << "13. Add new product (Admin)\n";
    cout << "14. Remove product (Admin)\n";
    cout << " 0. Exit\n";

    cout << "====================================================\n";
}

int main() {
    ProductCatalog catalog;
    ShoppingCart cart;
    BrandCircle brands;
    ProductBST productIndex;

    stack<UndoAction> undoStack;
    CheckoutSystem checkout;

    loadSampleProducts(catalog);
    initializeBrands(brands);
    rebuildProductIndex(productIndex, catalog);

    int choice;

    do {
        showMenu();

        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1:
                displayCatalog(catalog);
                break;

            case 2:
                filterByBrand(catalog);
                break;

            case 3:
                searchByKeyword(catalog);
                break;

            case 4:
                filterByPrice(catalog);
                break;

            case 5:
                cartMenu(catalog, cart);
                break;

            case 6:
                placeCheckoutOrder(catalog, cart, checkout);
                break;

            case 7:
                checkout.display();
                break;

            case 8:
                processCheckout(checkout);
                break;

            case 9:
                undoAdminAction(catalog, undoStack, productIndex);
                break;

            case 10:
                brands.displayCycle();
                break;

            case 11:
                sortingMenu(catalog);
                break;

            case 12: {
                int id = readInt("Enter product ID to search: ");

                const Product* found = productIndex.search(id);

                if (found) {
                    cout << "\nProduct found using Binary Search Tree:\n";
                    displayProductHeader();
                    displayProduct(*found);
                } else {
                    cout << "Product ID not found.\n";
                }

                break;
            }

            case 13:
                addProduct(catalog, undoStack, productIndex);
                break;

            case 14:
                removeProduct(catalog, undoStack, productIndex);
                break;

            case 0:
                cout << "\nThank you for shopping at Glow & Care!\n";
                break;

            default:
                cout << "Invalid choice. Please select from 0 to 14.\n";
        }

    } while (choice != 0);

    return 0;
}
