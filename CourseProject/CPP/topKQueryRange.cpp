#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <mysql_connection.h>
#include <driver.h>
#include <exception.h>
#include <resultset.h>
#include <statement.h>
#include <list>
#include <math.h>
#include <ctime>

using namespace sql;
using namespace std;

typedef struct viewsStruct
{
  string theVideoID;
  float theViews;
} theViewsStruct;

// string to integer:

int strtoint (string theString)
{
  int i = 0, theInt = 0, theConv = 0;
  //cout << "theString: " << theString << endl;

  while (theString[i] != 0)
  {
    //cout << "theString[" << i << "]: " << theString[i] << endl;
    theConv = theString[i] - '0';
    //cout << "theConv: " << theConv << endl;
    theInt = theInt * 10;
    //cout << "theInt after * 10: " << theInt << endl;
    theInt += theConv;
    //cout << "theInt-Final: " << theInt << endl;
    i++;
  } // end while
  //cout << "theInt: " << theInt << endl;
  return theInt;

} // end strtoint function

// string to float:

float strtofloat (string theString)
{
int i = 0, theConv = 0;
float theFloat = 0.0, theDec = 1.0;
int decimalFlag = 0;
//cout << "theString: " << theString << endl;

while (theString[i] != 0)
{
  //cout << "theString[" << i << "]: " << theString[i] << endl;
    // subtract the char by '0' to get the ascii value of the
    // number:

    // if we encounter a decimal, set the decimal flag and increment past it

  if (theString[i] == '.')
  {
    decimalFlag = 1;
    i++;
  } // end if we encounter a decimal, set decimal flag

  theConv = theString[i] - '0';
  //cout << "theConv: " << theConv << endl;
  if (decimalFlag == 0)
  {
      // move by tens to the left:
    theFloat = theFloat * 10;

    //cout << "theFloat after * 10: " << theFloat << endl;
      // running sum:
    theFloat += theConv;
  } // end if before decimal:
  else
  {
    // move by tens to the right:
    theDec = theDec * 0.1;
    theFloat += (theConv * theDec);
  } // end after decimal:

  //cout << "theFloat-Final: " << theFloat << endl;
  i++;
} // end while
//cout << "theFloat: " << theFloat << endl;
return theFloat;

} // end strtofloat function


// insertSortViewsList function

list <theViewsStruct> insertSortViewsList (theViewsStruct myViews, list <theViewsStruct> viewsList)
{
    // if list is empty, add first record:
  if (viewsList.size () == 0)
  {
    viewsList.push_back (myViews);
    return viewsList;
  } // end if list is empty:

      // if we need to insert at the end, handle that:
    list <theViewsStruct>::iterator itr = viewsList.end ();
    itr--;
    if (itr->theViews > myViews.theViews)
    {
      viewsList.push_back (myViews);
      return viewsList;
    } // end insert at end;

    // insert into list, descending:
  for (list <theViewsStruct>::iterator it = viewsList.begin (); it != viewsList.end (); it++)
  {
      if (it->theViews <= myViews.theViews)
      {
        //cout << "adding amid the records: " << myRates.theRate << endl;
        viewsList.insert (it, myViews);
        return viewsList;
      } // end if we inserted amid the list, return the list

  } // end iterate through to find right spot

  // if we get here, we did not find an element in the list in the top K
  // lower than us so we are too small to insert in the list

  return viewsList;

} // end insertSortViewsList function

// insertSortViews function

list <int> insertSortViews (list <int> myList, int theInt)
{
    // if list is empty, insert first record
  if (myList.size () == 0)
  {
    myList.push_back (theInt);
    return myList;
  }

    // if value is smaller than the smallest value in the list, add at the end
  list <int>::iterator itr = myList.end ();
  itr--;
  if (*itr > theInt)
  {
    myList.push_back (theInt);
    return myList;
  } // end if value is smaller than the smallest value in the list

    // otherwise, insert in the appropriate mid location
  for (list <int>::iterator it = myList.begin (); it!= myList.end (); it++)
  {
    if (*it < theInt)
    {
      myList.insert (it, theInt);
    } // end if we are smaller than the next number

    return myList;

  } // end for loop

    // if we get here we were too small to insert

  return myList;

} // end insertSortViews function

int main(void)
{
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;

  ifstream inFile;
  ofstream outFile;
  ofstream timeFile;

  string line;
  string suffix = "_0";
  //string suffix = "_3";

  clock_t totalStart;
  clock_t totalEnd;
  clock_t startTime;
  clock_t endTime;

  string theVideoID;
  string theViews;

  list <int> k_views;
  list <theViewsStruct> viewsList;
  theViewsStruct myViews;

  int i = 0;
  int K = 20;

  totalStart = clock ();

  //inFile.open ("readFolders.txt", ios::in);
  inFile.open ("readFolders1.txt", ios::in); // test on single table
  //outFile.open ("topK.txt", ios::out);
  outFile.open ("topK_range_0.txt", ios::out);
  timeFile.open ("topK_range_0_times.txt");

  getline (inFile, line);
  line += suffix;

  while (line[0] != 0)
  {
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306","root","$aturn5Ring$");

    con->setSchema("CS415");

    stmt = con->createStatement();

    startTime = clock ();

    res = stmt->executeQuery("select distinct videoID, length from " + line + " where length > 10 and length < 20;");

    endTime = clock ();

    timeFile << line << ": " << endTime - startTime << endl;

    while (res->next())
    {
      // Access column data by numeric offset, 1 is the first column

      myViews.theVideoID = res->getString (1);
      theViews = res->getString (2);
      myViews.theViews = strtoint (theViews);

      viewsList = insertSortViewsList (myViews, viewsList);

    } // end while loop

    getline (inFile, line);
    if (line[0] == 0)
    {
      break;
    }
    line += suffix;

  } // end while we have tables to read

    // print out the list:

  cout << "videos of length greater than 10 minutes and less than 20 minutes:" << endl;
  cout << "videoID:  Length" << endl;

  i = 0;

  for (list<theViewsStruct>::iterator it = viewsList.begin (); it != viewsList.end (); it++)
  //list <theViewsStruct>::iterator it = viewsList.begin ();
  {
    cout << it->theVideoID << " : " << it->theViews << endl;
    outFile << it->theVideoID << " : " << it->theViews << endl;

  } // end print out the list

  inFile.close ();
  outFile.close ();
  timeFile.close ();

  totalEnd = clock ();

  cout << "totalTime: " << totalEnd - totalStart << endl;

  return 0;
}
