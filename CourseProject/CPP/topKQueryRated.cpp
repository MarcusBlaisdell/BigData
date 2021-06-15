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


// insert data into table:

void insertData (string line)
{
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;

  driver = get_driver_instance();
  con = driver->connect("tcp://127.0.0.1:3306","root","$aturn5Ring$");

  con->setSchema("CS415");

  stmt = con->createStatement();

  stmt->execute("insert into ratesTable select distinct videoID, rate from " + line);

  delete stmt;

} // end insertData function

// getData function
void getData ()
{
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;

  string theVideoID;
  string theRate;
  int theRateInt;

  driver = get_driver_instance();
  con = driver->connect("tcp://127.0.0.1:3306","root","$aturn5Ring$");

  con->setSchema("CS415");

  stmt = con->createStatement();

  res = stmt->executeQuery("select distinct videoID, rates from ratesTable order by rates desc limit 20;");

  while (res->next())
  {
    // Access column data by numeric offset, 1 is the first column

    theVideoID = res->getString (1);
    theRate = res->getString (2);
    //myRates.theRate = strtoint (theRate);
    //cout << "theRate: " << theRate << " - myRates.theRate: " << myRates.theRate << endl;
    cout << "theVideoID: " << theVideoID << " - theRate: " << theRate << endl;

  } // end while loop

  stmt = con->createStatement();

  stmt->execute("delete from ratesTable where rates >= 0");

  delete stmt;

} // end getData function

// delete data from table:

void deleteData ()
{
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;

  driver = get_driver_instance();
  con = driver->connect("tcp://127.0.0.1:3306","root","$aturn5Ring$");

  con->setSchema("CS415");

  stmt = con->createStatement();

  stmt->execute("delete from ratesTable where rates >= 0");

  delete stmt;

} // end deleteData function

int main(void)
{
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
  int theRateInt;

  list <int> k_rates;
  list <theRateStruct> ratesList;
  theRateStruct myRates;

  int i = 0;
  int K = 20;
  int theKth;

  totalStart = clock ();

  //inFile.open ("readFolders.txt", ios::in);
  inFile.open ("readFolders1.txt", ios::in); // test on single table
  //outFile.open ("topK.txt", ios::out);
  outFile.open ("topK_rates_0.txt", ios::out);
  timeFile.open ("topK_rates_0_times.txt");

  getline (inFile, line);
  line += suffix;
  //cout << "Line:" << line << ":" << endl;

  while (line[0] != 0)
  {
    startTime = clock ();

    insertData (line);

    //cout << "Table: " << line << endl;
    //outFile << "Table: " << line << endl;
    //cout << "Count:  Category" << endl;
    //outFile << "Count:  Category" << endl;
    endTime = clock ();

    timeFile << line << ": " << endTime - startTime << endl;

    getline (inFile, line);
    if (line[0] == 0)
    {
      break;
    }
    line += suffix;
    //cout << "Line:" << line << ":" << endl;

  } // end while we have tables to read

  getData ();

  //deleteData ();


  inFile.close ();
  outFile.close ();
  timeFile.close ();

  totalEnd = clock ();

  cout << "totalTime: " << totalEnd - totalStart << endl;

  return 0;
}
