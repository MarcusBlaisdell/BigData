#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

int main()
{
  string line;
  int i, j;
  char command3[32] = {"ls Data/"};
  char command2[32] = {" > Lists/"};
  char command1[32];
  char theFile[32];

  ifstream inputFile;
  inputFile.open ("folders", ios::in);

  getline (inputFile, line);

  while (line[0] != 0)
  {
    i = 0;
    j = 8;

      // add filename to "ls Data/"
    theFile[0] = 0;
    strcpy (command1, command3);
    //cout << "new command1: " << command1 << endl;

    while (line[i] != 0)
    {
      command1[j] = line[i];
      theFile[i] = line[i];
      i++;
      j++;
    }
    command1[j] = 0;
    theFile[i] = 0;

      // concatenate command 2 onto command 1:
    strcat (command1, command2);
    //cout << "cat'ed command1: " << command1 << endl;

    i = 0;
    j += 9;

    while (theFile[i] != 0)
    {
      command1[j] = theFile[i];
      i++;
      j++;
    }
    command1[j] = 0;

    cout << "command1: " << command1 << endl;
    system (command1);

    getline (inputFile, line);

  } // end read all folders



}
