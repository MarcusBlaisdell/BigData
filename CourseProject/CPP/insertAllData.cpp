#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <mysql_connection.h>
#include <driver.h>
#include <exception.h>
#include <resultset.h>
#include <statement.h>
#include <time.h>

using namespace sql;
using namespace std;

// createTable function:

string createTable (string fileName)
{
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;
  ifstream inputFile;

  char tableName[64];
  string theTable;

  //cout << "fileName: " << fileName << endl;

  int i, j, n;
  n = fileName.size () - 4;

  j = 0;

  for (i = 5; i < n; i++)
  {
    if (fileName[i] == '/')
    {
      tableName[j] = '_';
    } // end if we find a '/', change to '-'
    else
    {
      tableName[j] = fileName[i];
    } // end, otherwise, just copy character

    j++;

  } // end capture
  tableName[j] = 0;

  cout << "tableName: " << tableName << endl;

  theTable = tableName;

    // create an sql connector instance:
  driver = get_driver_instance();
  con = driver->connect("tcp://127.0.0.1:3306","root","$aturn5Ring$");

  con->setSchema("CS415");

  stmt = con->createStatement();

  stmt->execute("create table " + theTable + " (videoID char(255), \
                 uploader char(255), age int, category char(255), \
                  length int, views int, rate float, ratings int, \
                   comments int, relatedIDs char(255) )");

/*
insert into youtube (videoID, uploader, age, \
               category, length, views, rate, ratings, comments, \
               relatedIDs) \
               values (\"" + thevideoID + "\", \"" + theuploader + "\", \"" + theage + "\", \
               \"" + thecategory + "\", \"" + thelength + "\", \"" + theviews + "\", \
               \"" + therate + "\", \"" + theratings + "\", \"" + thecomments + "\", \
               \"" + therelatedIDs + "\")
*/

  delete stmt;

  /*
  create table youtube (videoID char(255), uploader char(255), age int, category c
har(255), length int, views int, rate float, ratings int, comments int, relatedI
Ds char(255) );
*/

  return theTable;

} // end createTable function

  // insertData function

void insertData (string fileName, string theTable)
{
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;
  ifstream inputFile;

    // create an sql connector instance:
  driver = get_driver_instance();
  con = driver->connect("tcp://127.0.0.1:3306","root","$aturn5Ring$");

  con->setSchema("CS415");

    // variables for reading from file:
  int i, j, k = 0, a;
  string line;          // for reading from file
  char word[255];       // for parsing
  int lineSize;

  char videoID[255];
  char uploader[255];
  char age[255];
  char category[255];
  char length[255];
  char views[255];
  char rate[255];
  char ratings[255];
  char comments[255];
  char relatedIDs[255];

  char newFileName[64];

  string thevideoID;
  string theuploader;
  string theage;
  string thecategory;
  string thelength;
  string theviews;
  string therate;
  string theratings;
  string thecomments;
  string therelatedIDs;

    // open a file for reading:

  a = 0;

  while (fileName[a] != 0)
  {
    newFileName[a] = fileName[a];
    a++;
  }
  newFileName[a] = 0;

  inputFile.open (newFileName, ios::in);
  //inputFile.open ("Data/080512/0.txt", ios::in);

  i = 0;
  j = 0;

  if (inputFile.is_open())
  {

    getline (inputFile, line);

    while (line[0] != 0)
    {
      lineSize = line.size ();
      //cout << "lineSize: " << lineSize << endl;
      // videoID

      while (line[i] != '\t')
      {
        word[j++] = line[i++];
      } // end while to parse input
      word[j] = 0;

      strcpy (videoID, word);

      word[0] = 0;
      j = 0;
      i++;

      // uploader:

      while (line[i] != '\t')
      {
        word[j++] = line[i++];
      } // end while to parse input
      word[j] = 0;
      strcpy (uploader, word);

      word[0] = 0;
      j = 0;
      i++;

      // age:

      while (line[i] != '\t')
      {
        word[j++] = line[i++];
      } // end while to parse input
      word[j] = 0;

      strcpy (age, word);

      word[0] = 0;
      j = 0;
      i++;

      // category:

      while (line[i] != '\t')
      {
        word[j++] = line[i++];
      } // end while to parse input
      word[j] = 0;

      strcpy (category, word);

      word[0] = 0;
      j = 0;
      i++;

      // length:

      while (line[i] != '\t')
      {
        word[j++] = line[i++];
      } // end while to parse input
      word[j] = 0;

      strcpy (length, word);

      word[0] = 0;
      j = 0;
      i++;

      // views:

      while (line[i] != '\t')
      {
        word[j++] = line[i++];
      } // end while to parse input
      word[j] = 0;

      strcpy (views, word);

      word[0] = 0;
      j = 0;
      i++;

      // rate:
      while (line[i] != '\t')
      {
        word[j++] = line[i++];
      } // end while to parse input
      word[j] = 0;

      strcpy (rate, word);

      word[0] = 0;
      j = 0;
      i++;

      // ratings:

      while (line[i] != '\t')
      {
        word[j++] = line[i++];
      } // end while to parse input
      word[j] = 0;

      strcpy (ratings, word);

      word[0] = 0;
      j = 0;
      i++;

      // comments:

      while (line[i] != '\t')
      {
        word[j++] = line[i++];
      } // end while to parse input
      word[j] = 0;

      strcpy (comments, word);

      word[0] = 0;
      j = 0;
      i++;

        // we need to repeat relatedIDs up to 20 times
        // so store all of our other variables to
        // remain constant while we update relatedIDs
        // to create multiple records to capture
        // the unique relatedIDs

      thevideoID = videoID;
      theuploader = uploader;
      theage = age;
      thecategory = category;
      thelength = length;
      theviews = views;
      therate = rate;
      theratings = ratings;
      thecomments = comments;

      // relatedIDs:

      while (i < lineSize)
      {
        while (line[i] != '\t')
        {
          word[j++] = line[i++];
        } // end while to parse input
        word[j] = 0;

        strcpy (relatedIDs, word);

        word[0] = 0;
        j = 0;
        i++;

        therelatedIDs = relatedIDs;

        stmt = con->createStatement();

        stmt->execute("insert into " + theTable + " (videoID, uploader, age, \
                       category, length, views, rate, ratings, comments, \
                       relatedIDs) \
                       values (\"" + thevideoID + "\", \"" + theuploader + "\", \"" + theage + "\", \
                       \"" + thecategory + "\", \"" + thelength + "\", \"" + theviews + "\", \
                       \"" + therate + "\", \"" + theratings + "\", \"" + thecomments + "\", \
                       \"" + therelatedIDs + "\")");

        delete stmt;

      } // end repeat until end of line

      i = 0;
      getline (inputFile, line);

    } // end while line is valid


} // end if inputfile is open

inputFile.close ();

} // end insertData function


int main(void)
{
  string theTable;
  string fileName;
  string folderLine;
  string folderLine2;
  string fileLine;
  string prefix = "Data/";
  string prefix2 = "Lists/";
  time_t startTime;
  time_t endTime;

  char test3[64];
  int i;

  ifstream folderFile;
  ifstream fileFile;
  ofstream outFile;

  outFile.open ("times.csv", ios::out);

  folderFile.open ("folders", ios::in);

  getline (folderFile, folderLine);

    // read every folder:

  while (folderLine[0] != 0)
  {
    i = 0;

      // folderLine will contain the folder name from "folders"

    folderLine2 = prefix2 + folderLine;
    //cout << "folderLine2: " << folderLine2 << endl;

    while (folderLine2[i] != 0)
    {
      test3[i] = folderLine2[i];
      i++;
    }
    test3[i] = 0;
    //cout << "test3: " << test3 << endl;
    //fileFile.open (folderLine, ios::in);
    fileFile.open (test3, ios::in);

    getline (fileFile, fileLine);

    while (fileLine[0] != 0)
    {
      time (&startTime);
      fileName = (prefix + folderLine + "/" + fileLine);
      cout << "fileName: " << fileName << endl;
      //"Data/080512/0.txt"
      theTable = createTable (fileName);
      insertData (fileName, theTable);

      getline (fileFile, fileLine);

      time (&endTime);

      cout << fileName << ", " << endTime - startTime << endl;
      outFile << fileName << ", " << endTime - startTime << endl;

    } // end get every file

    fileFile.close ();

    getline (folderFile, folderLine);

  } // end read all folders

  folderFile.close ();
  outFile.close ();

  return 0;
}
