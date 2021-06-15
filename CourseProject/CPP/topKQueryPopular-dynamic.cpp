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

list <theViewsStruct> insertSortViewsList (theViewsStruct myViews, list <theViewsStruct> viewsList, int K)
{
  int i = 0;

  // if list is empty, add first record:
  if (viewsList.size () == 0)
  {
    //cout << "adding first record: " << myRates.theRate << endl;
    viewsList.push_back (myViews);
    return viewsList;
  } // end if list is empty:


  if (viewsList.size () < K)
  {
    // if we need to insert at the end, handle that:
    list <theViewsStruct>::iterator itr = viewsList.end ();
    if (itr->theViews > myViews.theViews)
    {
      //cout << "adding record to end: " << myRates.theRate << endl;
      viewsList.push_back (myViews);
      return viewsList;
    } // end insert at end;

  } // end only insert at end if fewer than K elements

    // insert into list, descending:
  //for (list <theViewsStruct>::iterator it = viewsList.begin (); it != viewsList.end (); it++)
  list <theViewsStruct>::iterator it = viewsList.begin ();
  {
    while (i < K)
    {
      if (it->theViews <= myViews.theViews)
      {
        //cout << "adding amid the records: " << myRates.theRate << endl;
        viewsList.insert (it, myViews);
        return viewsList;
      } // end if we inserted amid the list, return the list

      i++;
      it++;

    } // end only check top K

  } // end iterate through to find right spot

  // if we get here, we did not find an element in the list lower than us
  // so we are too small to insert in the list

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
  if (*itr > theInt)
  {
    myList.push_back (theInt);
    return myList;
  } // end if value is smaller than the smallest value in the list

    // otherwise, insert in the appropriate mid location
  for (list <int>::iterator it = myList.begin (); it!= myList.end (); it++)
  {
    if (*it <= theInt)
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
  int theViewsInt;

  list <int> k_views;
  list <theViewsStruct> viewsList;
  theViewsStruct myViews;

  int i = 0, j = 0;
  int K = 20;

  totalStart = clock ();

  //inFile.open ("readFolders.txt", ios::in);
  inFile.open ("readFolders1.txt", ios::in); // test on single table
  //outFile.open ("topK.txt", ios::out);
  outFile.open ("topK_views_3.txt", ios::out);
  timeFile.open ("topK_views_3_times.txt");

  getline (inFile, line);
  line += suffix;
  //cout << "Line:" << line << ":" << endl;

  while (line[0] != 0)
  {
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306","root","$aturn5Ring$");

    con->setSchema("CS415");

    stmt = con->createStatement();

    startTime = clock ();

    res = stmt->executeQuery("select distinct videoID, views from " + line + ";");

    //cout << "Table: " << line << endl;
    //outFile << "Table: " << line << endl;
    //cout << "Count:  Category" << endl;
    //outFile << "Count:  Category" << endl;
    endTime = clock ();
    timeFile << line << ": " << endTime - startTime << endl;

    while (res->next())
    {
      // Access column data by numeric offset, 1 is the first column

      myViews.theVideoID = res->getString (1);
      theViews = res->getString (2);
      myViews.theViews = strtoint (theViews);

      //k_views = insertSortViews (k_views, myViews.theViews);
      viewsList = insertSortViewsList (myViews, viewsList, K);

      //cout << "k_rate: " << myRates.theRate << endl;
      /*
      cout << theCountInt << " : ";
      outFile << theCount << " : ";
      cout << theCategory << endl;
      outFile << theCategory << endl;
      */
      //cout << res->getString (2) << " : ";
      //outFile << res->getString (2) << " : ";
      //cout << res->getString (1) << endl;
      //outFile << res->getString (1) << endl;

    } // end while loop

    getline (inFile, line);
    if (line[0] == 0)
    {
      break;
    }
    line += suffix;
    //cout << "Line:" << line << ":" << endl;

  } // end while we have tables to read

    // print out the list:

  cout << "top" << K << ": " << endl;
  cout << "videoID:  Views" << endl;

  j = viewsList.size ();

  if (j < K)
  {
    K = j;
  } // end if fewer than K, only print what we have

  //for (list <theViewsStruct>::iterator it = viewsList.begin (); it != viewsList.end (); it++)

  list <theViewsStruct>::iterator it = viewsList.begin ();

  while (i < K)
  {
    cout << it->theVideoID << " : " << it->theViews << endl;
    outFile << it->theVideoID << " : " << it->theViews << endl;

    i++;
    it++;

  } // end print out the list

  inFile.close ();
  outFile.close ();
  timeFile.close ();

  totalEnd = clock ();

  cout << "totalTime: " << totalEnd - totalStart << endl;

  return 0;
}
