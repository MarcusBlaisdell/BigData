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

list <TheSUMs> insertSort (TheSUMs theElement, list <TheSUMs> theList, int K)
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
} // end insertSort function

// createList function

list <TheSUMs> createList (unordered_map <string, int> theMap, list <TheSUMs> sortedSums, int K)
{
  TheSUMs myElement;

    // iterate through the map and insertSort each element into sortedSums

  for (auto x : theMap)
  {
    myElement.category = x.first;
    myElement.total = x.second;

    sortedSums = insertSort (myElement, sortedSums, K);
  } // end iterate through map

  return sortedSums;

} // end createList function

  // printList function

int printList (list <TheSUMs> sortedSums, int K)
{
  int i = 0;

  cout << "category    :  Total" << endl;

  list <TheSUMs>::iterator it = sortedSums.begin ();
  list <TheSUMs>::iterator ite = sortedSums.end ();

  while (i < K)
  {
    if (it == ite)
    {
      return 0;
    } // end if we reach the end of the list before K

    cout << it->category << " : " << it->total << endl;
    it++;
    i++;

  } // end while loop to print K element

} // end printList function

  // printMap function
int printMap (unordered_map <string, int> runMap)
{
  cout << "printMap: " << endl;
  for (auto x : runMap)
  {
    cout << x.first << " : " << x.second << endl;
  }
} // end printMap

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

  TheSUMs mySum;

  int i = 0;
  int K = 20;

  totalStart = clock ();

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

    /*
    res = stmt->executeQuery("select category, count(distinct videoID) as myList from " \
     + line + " where category in (select distinct category from " + line + \
      ") group by category order by myList desc");
    */

    res = stmt->executeQuery("select distinct videoID, category from " \
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

      theCategory = res->getString (2);
      //theCount = res->getString (2);
      //theCountInt = strtoint (theCount);

      //mySum.category = theCategory;
      //mySum.total = theCountInt;

      //runningSums = addToSums (runningSums, mySum);
      runMap[theCategory]++;

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

  //printMap (runMap);

  sortedSums = createList (runMap, sortedSums, K);

  printList (sortedSums, K);

  /*
    // print out the list:

  cout << "Total:  Category" << endl;

  for (list <TheSUMs>::iterator it = runningSums.begin (); it != runningSums.end (); it++)
  {
    cout << it->total << " : " << it->category << endl;
    outFile << it->total << " : " << it->category << endl;
  } // end print out the list
  */

  inFile.close ();
  outFile.close ();
  timeFile.close ();

  /*
  sortedSums = sortSums (runningSums);

  cout << "sorted sums: " << endl;
  for (list <TheSUMs>::iterator it = sortedSums.begin (); it != sortedSums.end (); it++)
  {
    cout << it->total << " : " << it->category << endl;
    outFile << it->total << " : " << it->category << endl;
  } // end print out the list

  cout << "top" << K << ": " << endl;

  list <TheSUMs>::iterator it = sortedSums.begin ();
  while (i < K && it != sortedSums.end ())
  {
    cout << it->total << " : " << it->category << endl;
    outFile << it->total << " : " << it->category << endl;
    it++;
    i++;
  } // end only print top K
  */

  totalEnd = clock ();

  cout << "totalTime: " << totalEnd - totalStart << endl;

  return 0;
}
