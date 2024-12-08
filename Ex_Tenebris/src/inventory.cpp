/*******************************************
    Written by Robert Parry [RJP] - 2024
    Refer to main.cpp for the license
*******************************************/

#include "inventory.h"
#include <iostream>

Inventory::Inventory() 
{
    // RJP - Initialize all pointers to nullptr
    for (int i = 0; i < TABLE_SIZE; ++i) 
    {
        table[i] = nullptr;
    }
}

Inventory::~Inventory() 
{
    clearInventory();
}

int Inventory::hash(const std::string& key) const 
{
    int hashValue = 0;
    for (char ch : key) {
        hashValue += ch;
    }
    return hashValue % TABLE_SIZE;
}

void Inventory::addItem(const std::string& item, int quantity) 
{
    int index = hash(item);
    Item* newItem = new Item(item, quantity);

    // RJP - If the item already exists, add quantity to its count
    Item* curr = table[index];
    while (curr) {
        if (curr->name == item) {
            curr->quantity += quantity;
            return;
        }
        curr = curr->next;
    }

    // RJP - Insert the new item at the beginning of the list
    newItem->next = table[index];
    table[index] = newItem;
}

void Inventory::removeItem(const std::string& item, int quantity) 
{
    int index = hash(item);
    Item* curr = table[index];
    Item* prev = nullptr;

    while (curr) 
    {
        if (curr->name == item) 
        {
            curr->quantity -= quantity;
            if (curr->quantity <= 0) 
            {
                if (prev) 
                {
                    prev->next = curr->next;
                }
                else 
                {
                    table[index] = curr->next;
                }
                delete curr;
            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

int Inventory::getItemCount(const std::string& item) const 
{
    int index = hash(item);
    Item* curr = table[index];
    while (curr) 
    {
        if (curr->name == item) 
        {
            return curr->quantity;
        }
        curr = curr->next;
    }
    return 0;
}

void Inventory::clearInventory() 
{
    for (int i = 0; i < TABLE_SIZE; ++i) 
    {
        Item* curr = table[i];
        while (curr) 
        {
            Item* next = curr->next;
            delete curr;
            curr = next;
        }
        table[i] = nullptr;
    }
}

// RJP - Output to console for debugging.
void Inventory::displayInventory() const 
{
    std::cout << "Inventory:" << std::endl;
    for (int i = 0; i < TABLE_SIZE; ++i) 
    {
        Item* curr = table[i];
        while (curr) 
        {
            std::cout << curr->name << ": " << curr->quantity << std::endl;
            curr = curr->next;
        }
    }
}

std::string Inventory::returnInventory() const
{
    std::string sInventory = "NULL";

    return sInventory;
}