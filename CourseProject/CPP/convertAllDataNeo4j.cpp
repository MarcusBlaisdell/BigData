#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
//#include <mysql_connection.h>
//#include <driver.h>
//#include <exception.h>
//#include <resultset.h>
//#include <statement.h>
#include <time.h>

//using namespace sql;
using namespace std;

  // convertData function:

int convertData (string fileName, string fileName2)
{
  int i = 0, j = 0, k = 0, a = 0;
  string line;
  char writeLine[1024];
  char header[1024] = {"videoID,uploader,age,category,length,views,rate,ratings,comments,relatedID1,relatedID2,relatedID3,relatedID4,relatedID5,relatedID6,relatedID7,relatedID8,relatedID9,relatedID10,relatedID11,relatedID12,relatedID13,relatedID14,relatedID15,relatedID16,relatedID17,relatedID18,relatedID19,relatedID20"};
  char newInput[64], newOutput[64];

  ifstream inputFile;
  ofstream outputFile;

    // open the file for reading:
    k = 0;

    while (fileName[k] != 0)
    {
      newInput[k] = fileName[k];
      k++;
    }
    newInput[k] = 0;

  inputFile.open (newInput, ios::in);

  // open output file for writing:

  k = 0;

  while (fileName2[k] != 0)
  {
    newOutput[k] = fileName2[k];
    k++;
  }
  newOutput[k] = 0;

  outputFile.open (newOutput, ios::out);
  //inputFile.open ("Data/080512/0.txt", ios::in);

  i = 0;
  j = 0;

  if (inputFile.is_open())
  {
      // start each file with the header:

    outputFile << header << endl;

      // get one line at a time from the read file:

    getline (inputFile, line);

    while (line[0] != 0)
    {
      while (line[i] != '\0')
      {
        if (line[i] == '\t')
        {
          //writeLine[j++] = ',';
          //writeLine[j++] = ' ';
          if (a == 1 || a == 3)
          {
            outputFile << ",toInt(";
          }
          else if (a == 2 || a == 8)
          {
            outputFile << "),";
          }
          else if (a == 5)
          {
            outputFile << "),toFloat(";
          }
          else if (a == 4 || a == 6 || a == 7)
          {
            outputFile << "),toInt(";
          }
          else
          {
            outputFile << ",";
          }


          i++;
          a++;

        } // end replace tabs with comma's (necessary) and spaces (maybe not necessary)
        // if importing into neo4j, must exclude the space!!!
        else
        {
          outputFile << line[i];
          //writeLine[j] = line[i];
          //cout << "j: " << writeLine[j] << " i: " << line[i] << endl;
          i++;
          j++;
        } // end while to parse input

      } // end while loop for one line
      i = 0;
      j = 0;
      a = 0;

      //outputFile << writeLine << endl;
      outputFile << endl;

      getline (inputFile, line);
    } // end while


  } // end if file is open

    return 0;

} // end convertData function

int main(void)
{
  string theTable;
  string fileName;
  string folderLine;
  string folderLine2;
  string fileLine;
  string prefix = "Data/";
  time_t startTime;
  time_t endTime;
  string convertedFile;
  string convPrefix = "/home/marcus/CS415/Project/Converted/";
  string convSuffix = ".csv";
  string outFile;

  int i;

  ifstream folderFile;
  ifstream fileFile;
  ofstream timeFile;

  timeFile.open ("times.csv", ios::out);

  folderFile.open ("folders", ios::in);

  getline (folderFile, folderLine);

    // read every folder:

  while (folderLine[0] != 0)
  {
    i = 0;

      // folderLine will contain the folder name from "folders"

    folderLine2 = prefix + folderLine;
    outFile = convPrefix + folderLine + convSuffix;

    time (&startTime);
    fileName = (prefix + folderLine + "/3.txt");
    cout << "fileName: " << fileName << endl;

    // do conversion here:
    /////
    convertData (fileName, outFile);
    /////

    time (&endTime);

    cout << fileName << ", " << endTime - startTime << endl;
    timeFile << fileName << ", " << endTime - startTime << endl;

    getline (folderFile, folderLine);

  } // end read all folders

  folderFile.close ();
  timeFile.close ();

  return 0;
}
