#ifndef TBA_ListPlus_h
#define TBA_ListPlus_h

#include <Arduino.h>
#include <iostream>
#include "VirtualBase.h"

/*

 LinkedListID will only work with a Derived class of VirtualBase.h or VirtualBase.h itself.

*/

template <class T>
class TBA_ListPlus
{
struct Node
{
    T data;
    Node *next;

    Node(T d, Node *n = 0) : data(d), next(n) {}
};
Node *head;
Node *iterator;

public:
    void iteratorHead()
    {
    iterator = head;
    }

    T *iteratorNext()
    {
    if (!head)
        return NULL; // Empty list

    if (iterator)
    {
        Node *tmp = iterator;
        iterator = iterator->next;
        return &tmp->data;
    }
    else
    {
        return NULL;
    }
    }

    // void insertAfterID(uint16_t ID, T d)
    //template <class T>
    void insertAfterID(uint16_t ID, T d)
    {
        Node *new_node = new Node(d, 0);

        if (!head)
        {
            head = new_node;
            return;
        }

        Node *cur = head;
        while (cur->next)
        {
            T element = cur->data;
            if (element.getIdentity() == ID)
            // if (cur->id == ID)
            {
                new_node->next = cur->next;
                cur->next = new_node;
                return;
            }

            cur = cur->next;
        }
        // Not found, add to end of list
        insertBottom(d);
    }

    //template <class T>
    void insertAfterExternID(uint16_t externID, T d)
    {
        Node *new_node = new Node(d, 0);

        if (!head)
        {
            head = new_node;
            return;
        }

        Node *cur = head;
        while (cur->next)
        {
            T element = cur->data;
            if (element.getExternalID() == externID)
            {
                new_node->next = cur->next;
                cur->next = new_node;
                return;
            }

            cur = cur->next;
        }
        // Not found, add to end of list
        insertBottom(d);
    }

    //template <class T>
    void insertBottom(T d)
    {
        Node *new_node = new Node(d, 0);

        if (!head)
        {
            head = new_node;
            return;
        }
        Node *cur = head;
        while (cur)
        {
            if (!cur->next)
            {
                cur->next = new_node;
                return;
            }
            cur = cur->next;
        }
    }

    //template <class T>
    void insertTop(T d)
    {
        Node *new_node = new Node(d, 0);

        if (!head)
        {
            head = new_node;
            return;
        }
        new_node->next = head;
        head = new_node;
        return;
    }

    //template <class T>
    T *searchID(uint16_t ID)
    {
        if (!head)
            return NULL; // Empty list
        Node *cur = head;
        while (cur)
        {
            T element = cur->data;
            if (element.getIdentity() == ID)
    //        if (cur->id == ID)
                return &cur->data;
            cur = cur->next;
        }
        return NULL; // notFound;
    }

    //template <class T>
    T *searchExternID(uint16_t externID)
    {
        if (!head)
            return NULL; // Empty list
        Node *cur = head;
        while (cur)
        {
            T element = cur->data;
            if (element.getExternalID() == externID)
                return &cur->data;
            cur = cur->next;
        }
        return NULL; // notFound;
    }

    //template <class T>
    T *searchName(String name)
    {
        if (!head)
            return NULL; // Empty list
        Node *cur = head;
        while (cur)
        {
            T element = cur->data;
            if (element.getName() == name)
                return &cur->data;
            cur = cur->next;
        }
        return NULL; // notFound;
    }

    //template <class T>
    uint16_t sizeOfList()
    {
        uint16_t size = 0;
        if (!head)
            return size;

        Node *cur = head;
        while (cur)
        {
            size++;
            cur = cur->next;
        }

        return size;
    }

    //template <class T>
    bool removeID(uint16_t ID)
    {
        if (!head)
            return NULL; // Empty list
        Node *cur, *prior;
        cur = head;
        while (cur)
        {
            T element = cur->data;
            if (element.getIdentity() == ID)
            { // Remove this ID
                if (cur == head)
                {
                    head = head->next;
                }
                else
                {
                    prior->next = cur->next;
                }
                delete cur;
                return true;
            }
            prior = cur;
            cur = cur->next;
        }
        return false;
    }

    //template <class T>
    bool removeExternID(uint16_t externID)
    {
        if (!head)
            return NULL; // Empty list
        Node *cur, *prior;
        cur = head;
        while (cur)
        {
            T element = cur->data;
            if (element.getExternalID() == externID)
            { // Remove this ID
                if (cur == head)
                {
                    head = head->next;
                }
                else
                {
                    prior->next = cur->next;
                }
                delete cur;
                return true;
            }
            prior = cur;
            cur = cur->next;
        }
        return false;
    }

    //template <class T>
    void clear()
    {
        Node *tmp;
        while (head)
        {
            tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    //template <class T>
    void display()
    {
        if (!head)
            return;

        Node *cur = head;

        while (cur)
        {
            T element = cur->data;

            Serial.print(" &addr: ");
            Serial.print((unsigned int)&cur->data, HEX);

            element.toString(" display:");

            cur = cur->next;
        }
    }

    // destructor
    //template <class T>
    ~TBA_ListPlus()
    {
        clear();
    }
};

#endif

/*
The below line solves the linker error requiring the 'DerivedElement' to be defined.

This linke shows options to solve: https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp

Example one of many errors: (one per method)
c:/users/{user}/appdata/local/arduino15/packages/esp32/tools/xtensa-esp32-elf-gcc/gcc8_4_0-esp-2021r2-patch3/bin/../lib/gcc/xtensa-esp32-elf/8.4.0/../../../../xtensa-esp32-elf/bin/ld.exe:
c:\Users\{user}\Documents\Arduino\libraries\Build\TBA_ListID_test\sketch\TBA_ListID_test.ino.cpp.o:(.literal._Z14TBA_ListPlusTestv+0x98): undefined reference to `TBA_ListPlus<DerivedElement>::insertBottom(DerivedElement)'

*/





