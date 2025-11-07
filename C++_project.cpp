#include <iostream>
#include <fstream>
using namespace std;

// ----------------------------- TEMPLATE CLASS -----------------------------
template <class T>
class Item {
protected:
    T id;
    string name;
    float price;
public:
    Item() { id = 0; name = "Unknown"; price = 0.0; }
    Item(T i, string n, float p) : id(i), name(n), price(p) {}
    virtual void display() {
        cout << "Item ID: " << id << "\nName: " << name << "\nPrice: " << price << endl;
    }
    float getPrice() { return price; }
    string getName() { return name; }
};

// ----------------------------- DERIVED CLASS -----------------------------
class DiscountedItem : public Item<int> {
    float discount;
public:
    DiscountedItem(int id, string n, float p, float d) : Item(id, n, p), discount(d) {}
    void display() {
        cout << "Item ID: " << id << "\nName: " << name
             << "\nPrice: " << price << "\nDiscount: " << discount << "%" << endl;
    }
    float finalPrice() { return price - (price * discount / 100); }
};

// ----------------------------- SHOPPING CART CLASS -----------------------------
class Cart {
    DiscountedItem **items;   // dynamic array of pointers
    int count;
public:
    Cart() {
        items = nullptr;
        count = 0;
    }

    // Operator Overloading (+= to add item)
    void operator+=(DiscountedItem *item) {
        DiscountedItem **temp = new DiscountedItem*[count + 1];
        for (int i = 0; i < count; i++)
            temp[i] = items[i];
        temp[count] = item;
        delete[] items;
        items = temp;
        count++;
    }

    void showCart() {
        if (count == 0) {
            cout << "\nCart is empty!\n";
            return;
        }
        cout << "\n------ CART ITEMS ------\n";
        float total = 0;
        for (int i = 0; i < count; i++) {
            cout << "\nItem " << i + 1 << ":\n";
            items[i]->display();
            total += items[i]->finalPrice();
        }
        cout << "\nTotal Payable: " << total << endl;
    }

    // Save bill to file
    void saveBill() {
        ofstream fout("bill.txt", ios::out);
        if (!fout) {
            cout << "File cannot be opened!\n";
            return;
        }
        float total = 0;
        fout << "----- Smart Shopping Bill -----\n";
        for (int i = 0; i < count; i++) {
            fout << items[i]->getName() << "  Rs." << items[i]->finalPrice() << endl;
            total += items[i]->finalPrice();
        }
        fout << "\nTotal: Rs." << total << endl;
        fout.close();
        cout << "\nBill saved successfully to 'bill.txt'!\n";
    }

    // Destructor
    ~Cart() {
        for (int i = 0; i < count; i++)
            delete items[i];
        delete[] items;
    }
};

// ----------------------------- FUNCTION OVERLOADING -----------------------------
void menu() {
    cout << "\n===== SMART SHOPPING MENU =====\n";
    cout << "1. Add Item to Cart\n";
    cout << "2. Show Cart\n";
    cout << "3. Save Bill to File\n";
    cout << "4. Exit\n";
}
void menu(string user) {
    cout << "\nWelcome, " << user << "! Let's start shopping.\n";
    menu();
}

// ----------------------------- MAIN FUNCTION -----------------------------
int main() {
    try {
        string user;
        cout << "Enter Customer Name: ";
        getline(cin, user);
        menu(user);

        Cart cart;
        int choice;

        do {
            cout << "\nEnter your choice: ";
            cin >> choice;

            if (choice == 1) {
                int id;
                string name;
                float price, discount;
                cout << "Enter Item ID: ";
                cin >> id;
                cout << "Enter Item Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Price: ";
                cin >> price;
                cout << "Enter Discount (%): ";
                cin >> discount;

                // Dynamic Memory Allocation using new
                DiscountedItem *item = new DiscountedItem(id, name, price, discount);
                cart += item;  // operator overloading
                cout << "Item added to cart successfully!\n";
            }

            else if (choice == 2)
                cart.showCart();

            else if (choice == 3)
                cart.saveBill();

            else if (choice == 4)
                cout << "Thank you for shopping!\n";

            else
                throw runtime_error("Invalid menu choice!");

        } while (choice != 4);
    }

    // Exception Handling
    catch (const exception &e) {
        cout << "\nError: " << e.what() << endl;
    }

    return 0;
}