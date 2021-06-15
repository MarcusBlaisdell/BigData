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

typedef struct rateStruct
{
  string theVideoID;
  float theRate;
} theRateStruct;

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

// insertSortRate function

list <float> insertSortRate (list <float> myList, float theFloat)
{
    // if list is empty, insert first record
  if (myList.size () == 0)
  {
    myList.push_back (theFloat);
    return myList;
  }

    // if value is smaller than the smallest value in the list, add at the end
  list <float>::iterator itr = myList.end ();
  if (*itr > theFloat)
  {
    myList.push_back (theFloat);
    return myList;
  } // end if value is smaller than the smallest value in the list

    // otherwise, insert in the appropriate mid location
  for (list <float>::iterator it = myList.begin (); it!= myList.end (); it++)
  {
    if (*it <= theFloat)
    {
      myList.insert (it, theFloat);
    } // end if we are smaller than the next number

    return myList;

  } // end for loop

    // if we get here we were too small to insert

  return myList;

} // end insertSortRate function

// printList function

void printList (list<theRateStruct> ratesList, ofstream &outFile, int K)
{
  int i = 0, j = 0;

  j = ratesList.size ();

  if (j < K)
  {
    K = j;
  } // end if fewer than K

  cout << "top" << K << ": " << endl;
  cout << "videoID:  Rate" << endl;


  list <theRateStruct>::iterator it = ratesList.begin ();
  while (i < K)
  {
    cout << it->theVideoID << " : " << it->theRate << endl;
    outFile << it->theVideoID << " : " << it->theRate << endl;

    i++;
    it++;

  } // end print out the list

} // end printList

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
  string theRate;
  int theRateFloat;

  list <float> k_rate;
  list <theRateStruct> ratesList;
  theRateStruct myRates;

  int i = 0;
  int K = 20;
  float trackRate;
  int rateCount;

  totalStart = clock ();

  //inFile.open ("readFolders.txt", ios::in);
  inFile.open ("readFolders1.txt", ios::in);
  //outFile.open ("topK.txt", ios::out);
  outFile.open ("topK_rates_3.txt", ios::out);
  timeFile.open ("topK_rates_3_times.txt");

  getline (inFile, line);
  line += suffix;
  //cout << "Line:" << line << ":" << endl;

  while (line[0] != 0)
  {
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306","root","$aturn5Ring$");

    con->setSchema("CS415");

    stmt = con->createStatement();

    //time (&startTime);
    startTime = clock();

    res = stmt->executeQuery("select distinct videoID, rate from " + line + ";");

    //cout << "Table: " << line << endl;
    //outFile << "Table: " << line << endl;
    //cout << "Count:  Category" << endl;
    //outFile << "Count:  Category" << endl;
    //time (&endTime);
    endTime = clock();
    //cout << "Time: " << endTime << " - " << startTime << " = " << endTime - startTime << endl;
    timeFile << line << ": " << endTime - startTime << endl;

    while (res->next())
    {
      // Access column data by numeric offset, 1 is the first column

      myRates.theVideoID = res->getString (1);
      theRate = res->getString (2);
      myRates.theRate = strtofloat (theRate);

      //k_rate = insertSortRate (k_rate, myRates.theRate);
      //cout << "myRates.theVideoID: " << myRates.theVideoID << " - myRates.theRate: " << myRates.theRate << endl;

      ratesList = insertSortRateList (myRates, ratesList, K);
      //ratesList.push_back (myRates);

      //cout << "ratesList.size (): " << ratesList.size () << endl;
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

  totalEnd = clock ();

    // print out the list:

  printList (ratesList, outFile, K);

  cout << "totalTime: " << totalEnd - totalStart << endl;
  timeFile << "totalTime: " << totalEnd - totalStart << endl;

  inFile.close ();
  outFile.close ();
  timeFile.close ();



  return 0;
}
