/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#pragma once

#include <string>

    const int TABLE_SIZE = 100;

class Inventory 
{
    public:
        Inventory();
        ~Inventory();

        void addItem(const std::string& item, int quantity = 1);
        void removeItem(const std::string& item, int quantity = 1);
        int getItemCount(const std::string& item) const;
        void clearInventory();
        void displayInventory() const;
        std::string returnInventory() const;

    private:
        struct Item {
            std::string name;
            int quantity;
            Item* next;

            Item(const std::string& itemName, int itemQuantity) : name(itemName), quantity(itemQuantity), next(nullptr) {}
        };

        Item* table[TABLE_SIZE];

        int hash(const std::string& key) const;
};