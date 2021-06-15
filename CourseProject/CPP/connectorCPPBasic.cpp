#include <stdlib.h>
#include <iostream>
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

  //string theName = "Bobby";
  string theName;
  char theCopy[255] = {"Gina"};
  string theAge = "28";

  theName = theCopy;

  driver = get_driver_instance();
  con = driver->connect("tcp://127.0.0.1:3306","root","$aturn5Ring$");

  con->setSchema("CS415");

  stmt = con->createStatement();
  //stmt->execute("insert into test1 (name, age) values (\"DouToan\", 44)");
  //stmt->execute("insert into test1 (name, age) values (\"" + theName + "\", \""  + theAge + "\")");
  stmt->execute("insert into test1 (name, age) values (\"" + theName + "\", "  + theAge + ")");


  //stmt->execute("delete from test1 where name = \"Joseph\"");
  delete stmt;
  //res = stmt->executeQuery("SELECT * from test1");
  //res = stmt->executeQuery("insert into test1 (name, age) values (\"Joseph\", \"22\")");

  /*
  while (res->next())
  {
    cout << "\t... MySQL replies: ";
    // Access column data by alias or column name

    cout << "\t... MySQL says it again: ";
    // Access column data by numeric offset, 1 is the first column

    cout << res->getString(1) << endl;
    cout << res->getString(2) << endl;
  } // end while loop
  */

  return 0;
}
