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
#include <unordered_map>

using namespace sql;
using namespace std;

typedef struct theSums
{
  string category;
  int total;
} TheSUMs;

typedef struct rateStruct
{
  string theVideoID;
  float theRate;
} theRateStruct;

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


  // insertSortSums function

list <TheSUMs> insertSortSums (TheSUMs theElement, list <TheSUMs> theList, int K)
{
  int i = 0;

    // if the list is empty, just add it:

  if (theList.size () == 0)
  {
    theList.push_back (theElement);
    return theList;
  } // end if empty list

    // only add to the end of the list if the size is smaller than K:

  if (theList.size () < K)
  {
      // if we need to add to the end:

    list <TheSUMs>::iterator itr = theList.end ();
    itr--;
    if (itr->total > theElement.total)
    {
      theList.push_back (theElement);
      return theList;
    } // end if we need to insert at end
  } // end only add to end if smaller than K

  // insert somewhere amid but only up to K elements

  //for (list <TheSUMs>::iterator it = theList.begin (); it != theList.end (); it++)
  list <TheSUMs>::iterator it = theList.begin ();
  while (i < K)
  {
    if (it->total <= theElement.total)
    {
      theList.insert (it, theElement);
      return theList;
    } // end if we're in the right spot, add the value

    it++;
    i++;

  } // end iterate to the correct spot

  // we shouldn't get here but if we do, for some reason, we didn't
  // add the element, just return the list as-is

  return theList;

} // end insertSortSums function

  // insertSortRateList function

list <theRateStruct> insertSortRateList (theRateStruct myRates, list <theRateStruct> ratesList, int K)
{
  int i = 0;

  // if list is empty, add first record:
  if (ratesList.size () == 0)
  {
    //cout << "adding first record: " << myRates.theRate << endl;
    ratesList.push_back (myRates);
    return ratesList;
  } // end if list is empty:

  // if size is less than K, check insert at end :
  if (ratesList.size () < K)
  {
    // if we need to insert at the end, handle that:
    list <theRateStruct>::iterator itr = ratesList.end ();
    itr--;
    if (itr->theRate > myRates.theRate)
    {
      //cout << "adding record to end: " << myRates.theRate << endl;
      ratesList.push_back (myRates);
      return ratesList;
    } // end insert at end;

  } // end only insert at end if less than K

    // insert into list, descending:
  //for (list <theRateStruct>::iterator it = ratesList.begin (); it != ratesList.end (); it++)
  list <theRateStruct>::iterator it = ratesList.begin ();
  {
    while (i < K)
    {
      if (it->theRate <= myRates.theRate)
      {
        //cout << "adding amid the records: " << myRates.theRate << endl;
        ratesList.insert (it, myRates);
        return ratesList;
      } // end if we inserted amid the list, return the list

      i++;
      it++;

    } // end only check the first K entries:

  } // end iterate through to find right spot

  // if we get here, we did not find an element in the list lower than us
  // so we are too small to insert in the list

  return ratesList;

} // end insertSortRateList function

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

// addToSums function

list <TheSUMs> addToSums (list <TheSUMs> runningSums, TheSUMs mySum)
{
  // check if category is already in runningSums:

  for (list <TheSUMs> ::iterator it = runningSums.begin (); it != runningSums.end (); it++)
  {
    if (it->category == mySum.category)
    {
      it->total += mySum.total;
      return runningSums;
    } // end if category is in list, add our count to it

  } // end iterate through list

  // if the category wasn't in the list, add it:
  //cout << "adding category: " << mySum.category << endl;
  runningSums.push_back (mySum);

  return runningSums;

} // end addToSUMs function

  // sortSums, sorts the sums from a list in descenging order
  // by total

list <TheSUMs> sortSums (list <TheSUMs> runningSums)
{
  int addFlag = 0;
  list <TheSUMs> sortedSums;
  TheSUMs mySum;

    // read each line from runningSums:

  for (list <TheSUMs>::iterator it = runningSums.begin (); it != runningSums.end (); it++)
  {
    mySum.category = it->category;
    mySum.total = it->total;

    // insert in order into a sorted list:
    if (sortedSums.size () == 0)
    {
      sortedSums.push_back (mySum);
    } // end if list is empty, just add the element
    else
    {
      for (list <TheSUMs>::iterator its = sortedSums.begin (); its != sortedSums.end (); its++)
      {
        if (its->total <= it->total)
        {
          sortedSums.insert (its, mySum);
          addFlag = 1;
          break;
        } // end if we are bigger, insert us in front

      } // end find correct location to insert
      // if we get to the end and nothing was smaller, then we are smallest,
      // add to end:
      if (addFlag == 0)
      {
        sortedSums.push_back (mySum);
      }
      else
      {
        addFlag = 0;
      }

    } // end if list is not empty, insert in descending order of total

  } // end iterate through unsorted list, runningSums

  return sortedSums;

} // end sortSums function

  // insertSort function

list <TheSUMs> insertSort (TheSUMs theElement, list <TheSUMs> theList)
{
    // if the list is empty, just add it:

  if (theList.size () == 0)
  {
    theList.push_back (theElement);
    return theList;
  } // end if empty list

    // if we need to add to the end:

  list <TheSUMs>::iterator itr = theList.end ();
  itr--;
  if (itr->total > theElement.total)
  {
    theList.push_back (theElement);
    return theList;
  } // end if we need to insert at end

  // insert somewhere amid
  for (list <TheSUMs>::iterator it = theList.begin (); it != theList.end (); it++)
  {
    if (it->total <= theElement.total)
    {
      theList.insert (it, theElement);
      return theList;
    } // end if we're in the right spot, add the value

  } // end iterate to the correct spot

  // we shouldn't get here but if we do, for some reason, we didn't
  // add the element, just return the list as-is

  return theList;
} // end insertSort function

  // printSums function

void printSums (list <TheSUMs> sortedSums, ofstream &outFile, int K)
{
  int i = 0;

  cout << "top " << K << " categories: " << endl;

  list <TheSUMs>::iterator it = sortedSums.begin ();

  while (i < K && it != sortedSums.end ())
  //while (i < K)
  {
    cout << it->total << " : " << it->category << endl;
    outFile << it->total << " : " << it->category << endl;
    it++;
    i++;
  } // end only print top K

} // end printSums function

// printRates function:

void printRates (list<theRateStruct> ratesList, ofstream &outFile, int K)
{
  int i = 0;

  cout << "top" << K << " Rated: " << endl;
  cout << "videoID:  Rate" << endl;

  list <theRateStruct>::iterator it = ratesList.begin ();
  //for (list <theRateStruct>::iterator it = ratesList.begin (); it != ratesList.end (); it++)
  //for (i = 0; i < K; i++)

  while (i < K)
  {
    cout << it->theVideoID << " : " << it->theRate << endl;
    outFile << it->theVideoID << " : " << it->theRate << endl;

    i++;
    it++;

  } // end print out the list

} // end printRates function

// printViews function

void printViews (list<theViewsStruct> viewsList, ofstream &outFile, int K)
{
  int i = 0, j = 0;

  cout << "top" << K << " Popular: " << endl;
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

} // end printViews function

// createList function

list <TheSUMs> createList (unordered_map <string, int> theMap, list <TheSUMs> sortedSums, int K)
{
  TheSUMs myElement;

    // iterate through the map and insertSort each element into sortedSums

  for (auto x : theMap)
  {
    myElement.category = x.first;
    myElement.total = x.second;
    //cout << "myElement.category: " << myElement.category << " : myElement.total: " << myElement.total << endl;
    sortedSums = insertSortSums (myElement, sortedSums, K);
  } // end iterate through map

  return sortedSums;

} // end createList function

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

  list <TheSUMs> runningSums;
  list <TheSUMs> sortedSums;
  unordered_map <string, int> runMap;

  string theCategory;
  string theCount;
  int theCountInt;

  string theVideoID;
  string theRate;
  int theRateFloat;

  list <float> k_rate;
  list <theRateStruct> ratesList;
  theRateStruct myRates;

  string theViews;
  int theViewsInt;

  list <int> k_views;
  list <theViewsStruct> viewsList;
  theViewsStruct myViews;

  TheSUMs mySum;

  int i = 0, j = 0;
  int K = 20;

  totalStart = clock ();

  //inFile.open ("readFolders.txt", ios::in);
  inFile.open ("readFolders1.txt", ios::in);
  //outFile.open ("topK.txt", ios::out);
  outFile.open ("topK_3.txt", ios::out);
  timeFile.open ("topK_3_times.txt");

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

    res = stmt->executeQuery("select distinct videoID, rate, views, category from " \
     + line);

    //cout << "Table: " << line << endl;
    //outFile << "Table: " << line << endl;
    //cout << "Count:  Category" << endl;
    //outFile << "Count:  Category" << endl;

    endTime = clock ();

    timeFile << line << ": " << endTime - startTime << endl;

    while (res->next())
    {
      // Access column data by numeric offset, 1 is the first column

      myRates.theVideoID = res->getString (1);
      theRate = res->getString (2);
      myRates.theRate = strtofloat (theRate);

      myViews.theVideoID = res->getString (1);
      theViews = res->getString (3);
      myViews.theViews = strtoint (theViews);

      theCategory = res->getString (4);
      //theCount = res->getString (5);
      //theCountInt = strtoint (theCount);

      //mySum.category = theCategory;
      //mySum.total = theCountInt;

      runMap[theCategory]++;
      //runningSums = addToSums (runningSums, mySum);
      ratesList = insertSortRateList (myRates, ratesList, K);
      viewsList = insertSortViewsList (myViews, viewsList, K);

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

  inFile.close ();
  outFile.close ();
  timeFile.close ();

  sortedSums = createList (runMap, sortedSums, K);

  printSums (sortedSums, outFile, K);
  printRates (ratesList, outFile, K);
  printViews (viewsList, outFile, K);

  totalEnd = clock ();

  cout << "totalTime: " << totalEnd - totalStart << endl;

  return 0;
}
