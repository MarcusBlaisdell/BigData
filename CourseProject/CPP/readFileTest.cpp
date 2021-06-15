#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

int main ()
{
  ifstream inputFile;
  string line;

  inputFile.open ("readTest.txt", ios::in);

  getline (inputFile, line);

  while (line[0] != 0)
  {
    cout << "line: " << line << endl;
    getline (inputFile, line);
  }

  cout << "Done!" << endl;

  inputFile.close ();

}
