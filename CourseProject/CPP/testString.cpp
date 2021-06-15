#include <iostream>
#include <string.h>

using namespace std;

int main ()
{
  string testString, testString2;
  char testChar[32] = {"test2"};
  char testOther[32];

  testString = "Test1";
  testString2 = "test2";

  cout << "testChar: " << testChar << endl;
  strcpy (testOther, testChar);
  cout << "testOther: " << testOther << endl;

  testString2 = testString2 + testString;

  cout << "testString2: " << testString2 << endl;

}
