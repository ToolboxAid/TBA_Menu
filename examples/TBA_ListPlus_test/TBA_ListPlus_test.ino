
#include "Arduino.h"

#include "TBA_ListPlus.h"
#include "VirtualBase.h"

#include "../examples/TBA_ListPlus_test/DerivedElement.h"
#include "../examples/TBA_ListPlus_test/DerivedElement2.h"

/*

 TBA_ListPlus will only work with a Derived class of VirtualBase.h
 In this case, DerivedElements.h extend VirtualBase.h

*/  

void LinkListPlusTest()
{
  TBA_ListPlus<DerivedElement> *myLinkList = new TBA_ListPlus<DerivedElement>();

  // uint16_t max value 65535
  DerivedElement de = DerivedElement("1st element    ", 1111);
  myLinkList->insertBottom(de);


// Insert Top
  de = DerivedElement("Insert Top     ", 500);
  myLinkList->insertTop(de);

// Insert Bottom
  de = DerivedElement("2nd element    ", 2222);
  myLinkList->insertBottom(de);

  de = DerivedElement("3rd element    ", 3333);
  myLinkList->insertBottom(de);

// Insert After ID 
  de = DerivedElement("AfterID-1      ", 4444);
  myLinkList->insertAfterID(1, de);

// Search ID and Insert After ExternID
  DerivedElement *found = myLinkList->searchID(2);
  if (found == NULL)
  {
    Serial.println("ID is NULL");
  }
  else
  {
    found->toString("ID Found: ");
    de = DerivedElement("afterSearch #2 ", 550);
    myLinkList->insertAfterID(found->getIdentity(), de);
  }

// Search ID and Insert After ExternID
  found = myLinkList->searchExternID(2222);
  if (found == NULL)
  {
    Serial.println("ExternID is NULL");
  }
  else
  {
    found->toString("ExternID Found: ");
    de = DerivedElement("afterSearch #2222 ", 2323);
    myLinkList->insertAfterExternID(found->getExternalID(), de);
  }

// Search Name
  found = myLinkList->searchName("2nd element    ");
  if (found == NULL)
  {
    Serial.println("Name is NULL");
  }
  else
  {
    found->toString("Name Found: ");
  }

// all done, clean up
  Serial.print  ("Post Search/InsertAfters - size: ");
  Serial.println(myLinkList->sizeOfList());
  myLinkList->display();

////////////////////////////////////////////////

// Insert Top for Delete
  de = DerivedElement("Top for del    ", 100);
  myLinkList->insertTop(de);

// Insert Middle for Delete
  de = DerivedElement("Middle for del ", 5151);
  myLinkList->insertAfterID(5, de);

// Insert Bottom for Delete
  de = DerivedElement("Bottom for del ", 10000);
  myLinkList->insertBottom(de);

  Serial.print  ("Post insert for delete - Size: ");
  Serial.println(myLinkList->sizeOfList());
  myLinkList->display();

  bool deleted;
  deleted = myLinkList->removeExternID(100);
  Serial.println(deleted);

  deleted = myLinkList->removeExternID(5151);
  Serial.println(deleted);

  deleted = myLinkList->removeExternID(10000);
  Serial.println(deleted);

  // Not found
  deleted = myLinkList->removeExternID(10000);
  Serial.println(deleted);

  Serial.print  ("Post deletes - Size: ");
  Serial.println(myLinkList->sizeOfList());

  myLinkList->display();

/////////////////////////////////////////////////
  // Check overloads
  de = DerivedElement("overload 0");  // externalID defaults to 0
  myLinkList->insertBottom(de);

  de = DerivedElement("overload 1",9000,"more data", 9001);
  myLinkList->insertBottom(de);

  myLinkList->display();

/////////////////////////////////////////////////

// Insert After ID 
  de = DerivedElement("AfterID-2      ", 5555);
  myLinkList->insertAfterID(2, de);

// Insert After ID 
  de = DerivedElement("AfterID-99>last", 6666);
  myLinkList->insertAfterID(99, de);

// Insert After externID 
  de = DerivedElement("AfterExternID^1", 7777);
  myLinkList->insertAfterExternID(1111, de);

// Insert After externID 
  de = DerivedElement("AfterExternID^3", 8888);
  myLinkList->insertAfterExternID(3333, de);

// Insert Bottom
  de = DerivedElement("insert last    ", 9999);
  myLinkList->insertBottom(de);

  // iterator
  Serial.println("Iterator");
  myLinkList->iteratorHead();
  DerivedElement *iterator = myLinkList->iteratorNext();
  while (iterator)
  {
    iterator->toString("iterator");
    iterator = myLinkList->iteratorNext();
  }

  // ----------------------------------------------------
  Serial.println("All methods executed");
  myLinkList->display();
  Serial.println("Ready to clear");
  myLinkList->clear();
  Serial.println("List emptied");
  myLinkList->display();

}

void setup()
{
  Serial.begin(115200); // Use serial port
  while (!Serial && (millis() <= 1000))
    ;

  Serial.println("*** Double link list test ***");
  LinkListPlusTest();
}

void loop()
{
  Serial.print(".");
  delay(10000); // Slow down print
  yield();      // Allow WDT to reset
}
