#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <mysql_connection.h>
#include <driver.h>
#include <exception.h>
#include <resultset.h>
#include <statement.h>

using namespace sql;
using namespace std;
int main(void)
{
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;
  ifstream inputFile;
    // variables for reading from file:
  int i, j, k = 0;
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

    // create an sql connector instance:
  driver = get_driver_instance();
  con = driver->connect("tcp://127.0.0.1:3306","root","$aturn5Ring$");

  con->setSchema("CS415");

    // open a file for reading:

  inputFile.open ("0301/0.txt", ios::in);

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

        /*
        cout << "videoID: " << videoID << endl;
        cout << "uploader: " << uploader << endl;
        cout << "age: " << age << endl;
        cout << "category: " << category << endl;
        cout << "length: " << length << endl;
        cout << "views: " << views << endl;
        cout << "rate: " << rate << endl;
        cout << "ratings: " << ratings << endl;
        cout << "comments: " << comments << endl;
        cout << "relatedIDs: " << relatedIDs << endl;
        cout << "--" << endl;

        cout << "thevideoID: " << thevideoID << endl;
        cout << "theuploader: " << theuploader << endl;
        cout << "theage: " << theage << endl;
        cout << "thecategory: " << thecategory << endl;
        cout << "thelength: " << thelength << endl;
        cout << "theviews: " << theviews << endl;
        cout << "therate: " << therate << endl;
        cout << "theratings: " << theratings << endl;
        cout << "thecomments: " << thecomments << endl;
        cout << "therelatedIDs: " << therelatedIDs << endl;
        cout << endl;
        */


        stmt = con->createStatement();
        //stmt->execute("insert into test1 (name, age) values (\"" + theName + "\", \""  + theAge + "\")");
        //stmt->execute("insert into youtube (videoID) values (\"" + thevideoID + "\")");


        stmt->execute("insert into youtube (videoID, uploader, age, \
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


  return 0;
}
