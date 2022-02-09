#include <iostream>
#include <pqxx/pqxx>

int main(int, char *argv[])
{
  pqxx::connection c("host=127.0.0.1 port=5433 dbname=yugabyte user=yugabyte password=yugabyte");
  pqxx::work txn(c);
  pqxx::result r;

  /* Create table */
  try
  {
    r = txn.exec("CREATE TABLE employee (id int PRIMARY KEY, \
                  name varchar, age int, \
                  language varchar)");
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  std::cout << "Created table employee\n";

  /* Insert a row */
  try
  {
    r = txn.exec("INSERT INTO employee (id, name, age, language) \
                  VALUES (1, 'John', 35, 'C++')");
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  std::cout << "Inserted data (1, 'John', 35, 'C++')\n";

  /* Query the row */
  try
  {
    r = txn.exec("SELECT name, age, language FROM employee WHERE id = 1");

    for (auto row: r)
      std::cout << "Query returned: "
          << row["name"].c_str() << ", "
          << row["age"].as<int>() << ", "
          << row["language"].c_str() << std::endl;
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  txn.commit();
  return 0;
}
