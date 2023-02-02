// #ifndef LinkListID_cpp
// #define LinkListID_cpp

// #include "LinkedListID.h"


// template <class T>
// void LinkListID<T>::insertAfterID(uint16_t ID, T d)
// {
//     Node *new_node = new Node(d, 0);

//     if (!head)
//     {
//         head = new_node;
//         return;
//     }

//     Node *cur = head;
//     while (cur->next)
//     {
//         T element = cur->data;
//         if (element.getIdentity() == ID)    
//         //if (cur->id == ID)
//         {
//             new_node->next = cur->next;
//             cur->next = new_node;
//             return;
//         }

//         cur = cur->next;
//     }
//     // Not found, add to end of list
//     insertBottom(d);
// }

// template <class T>
// void LinkListID<T>::insertAfterExternID(uint16_t externID, T d)
// {
//     Node *new_node = new Node(d, 0);

//     if (!head)
//     {
//         head = new_node;
//         return;
//     }

//     Node *cur = head;
//     while (cur->next)
//     {
//         T element = cur->data;
//         if (element.getExternalID() == externID)
//         {
//             new_node->next = cur->next;
//             cur->next = new_node;
//             return;
//         }

//         cur = cur->next;
//     }
//     // Not found, add to end of list
//     insertBottom(d);
// }

// template <class T>
// void LinkListID<T>::insertBottom(T d)
// {
//     Node *new_node = new Node(d, 0);

//     if (!head)
//     {
//         head = new_node;
//         return;
//     }
//     Node *cur = head;
//     while (cur)
//     {
//         if (!cur->next)
//         {
//             cur->next = new_node;
//             return;
//         }
//         cur = cur->next;
//     }
// }

// template <class T>
// void LinkListID<T>::insertTop(T d)
// {
//     Node *new_node = new Node(d, 0);

//     if (!head)
//     {
//         head = new_node;
//         return;
//     }
//     new_node->next = head;
//     head = new_node;
//     return;
// }

// template <class T>
// T *LinkListID<T>::searchID(uint16_t ID)
// {
//     if (!head)
//         return NULL; // Empty list
//     Node *cur = head;
//     while (cur)
//     {
//         T element = cur->data;
//         if (element.getIdentity() == ID)    
// //        if (cur->id == ID)
//             return &cur->data;
//         cur = cur->next;
//     }
//     return NULL; // notFound;
// }

// template <class T>
// T *LinkListID<T>::searchExternID(uint16_t externID)
// {
//     if (!head)
//         return NULL; // Empty list
//     Node *cur = head;
//     while (cur)
//     {
//         T element = cur->data;
//         if (element.getExternalID() == externID)
//             return &cur->data;
//         cur = cur->next;
//     }
//     return NULL; // notFound;
// }

// template <class T>
// T *LinkListID<T>::searchName(String name)
// {
//     if (!head)
//         return NULL; // Empty list
//     Node *cur = head;
//     while (cur)
//     {
//         T element = cur->data;
//         if (element.getName() == name)
//             return &cur->data;
//         cur = cur->next;
//     }
//     return NULL; // notFound;
// }

// template <class T>
// uint16_t LinkListID<T>::sizeOfList()
// {
//     uint16_t size = 0;
//     if (!head)
//         return size;

//     Node *cur = head;
//     while (cur)
//     {
//         size++;
//         cur = cur->next;
//     }

//     return size;
// }

// template <class T>
// bool LinkListID<T>::removeID(uint16_t ID)
// {
//     if (!head)
//         return NULL; // Empty list
//     Node *cur, *prior;
//     cur = head;
//     while (cur)
//     {
//         T element = cur->data;
//         if (element.getIdentity() == ID)
//         { // Remove this ID
//             if (cur == head)
//             {
//                 head = head->next;
//             }
//             else
//             {
//                 prior->next = cur->next;
//             }
//             delete cur;
//             return true;
//         }
//         prior = cur;
//         cur = cur->next;
//     }
//     return false;
// }

// template <class T>
// bool LinkListID<T>::removeExternID(uint16_t externID)
// {
//     if (!head)
//         return NULL; // Empty list
//     Node *cur, *prior;
//     cur = head;
//     while (cur)
//     {
//         T element = cur->data;
//         if (element.getExternalID() == externID)
//         { // Remove this ID
//             if (cur == head)
//             {
//                 head = head->next;
//             }
//             else
//             {
//                 prior->next = cur->next;
//             }
//             delete cur;
//             return true;
//         }
//         prior = cur;
//         cur = cur->next;
//     }
//     return false;
// }

// template <class T>
// void LinkListID<T>::clear()
// {
//     Node *tmp;
//     while (head)
//     {
//         tmp = head;
//         head = head->next;
//         delete tmp;
//     }
// }


// template <class T>
// void LinkListID<T>::display()
// {
//     if (!head)
//         return;

//     Node *cur = head;

//     while (cur)
//     {
//         T element = cur->data;

//         Serial.print(" &addr: ");
//         Serial.print((unsigned int)&cur->data, HEX);

//         element.toString(" display: ");

//         cur = cur->next;
//     }
// }

// // destructor
// template <class T>
// LinkListID<T>::~LinkListID()
// {
//     clear();
// }

// #endif