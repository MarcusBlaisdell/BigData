// listTest.cpp

#include <iostream>
#include <string.h>
#include <list>

using namespace std;

list <int> addToList (list <int> myList, int myInt)
{
  for (list <int>::iterator it = myList.begin (); it != myList.end (); it++)
  {
    if (it->myString == myRec.myString)
    {
      myList.push_back (myRec);
    } // end if exists, add

  } // end print list

  return myList;

} // end addToList function

int main ()
{
  list <int> myList;
  int myInt = 0;

  myList = addToList (myList, myInt);

  myInt = 5;
  myList = addToList (myList, myInt);

  myInt = 3;
  myList = addToList (myList, myInt);

  myInt = 7;
  myList = addToList (myList, myInt);

  myInt = 2;
  myList = addToList (myList, myInt);

  for (list <MyStruct>::iterator it = myList.begin (); it != myList.end (); it++)
  {
    cout << it << ", ";
  } // end print list

  cout << endl;
}
