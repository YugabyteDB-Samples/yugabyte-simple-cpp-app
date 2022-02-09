# Simple C++ Application for YugabyteDB

This application connects to your YugabyteDB instance via the 
[libpqxx](http://pqxx.org/development/libpqxx/) driver for PostgreSQL and performs basic SQL 
operations. The instructions below are provided for [Yugabyte Cloud](https://cloud.yugabyte.com/) deployments. 
If you use a different type of deployment, then update the `sample-app.cpp` file with proper connection parameters.

## Prerequisite

* A 32-bit (x86) or 64-bit (x64) architecture machine. Use [Rosetta](https://support.apple.com/en-us/HT211861) to build and run on Apple silicon.
* gcc 4.1.2 or later, clang 3.4 or later installed.
* OpenSSL 1.1.1 or later (used by libpqxx to establish secure SSL connections).
* [libpqxx](http://pqxx.org/development/libpqxx/) driver.
* Command line tool or your favourite IDE, such as Visual Studio Code.

## Start Yugabyte Cloud Cluster

* [Start YugabyteDB Cloud](https://docs.yugabyte.com/latest/yugabyte-cloud/cloud-quickstart/qs-add/) instance. You can use
the free tier at no cost.
* Add an IP address of your machine/laptop to the [IP allow list](https://docs.yugabyte.com/latest/yugabyte-cloud/cloud-secure-clusters/add-connections/#manage-ip-allow-lists)

## Clone Application Repository

Clone the repository and change dirs into it:

```bash
git clone https://github.com/yugabyte/yugabyte-simple-cpp-app && cd yugabyte-simple-cpp-app
```

## Provide Yugabyte Cloud Connection Parameters

Locate and define the following configuration-related macros in the `sample-app.cpp` file:
* `HOST` - the hostname of your Yugabyte Cloud instance.
* `USER` - the username for your instance.
* `PASSWORD` - the database password.
* `SSL_ROOT_CERT` - a full path to your CA root cert (for example, `/Users/dmagda/certificates/root.crt`). 

Note, you can easily find all the settings on the Yugabyte Cloud dashboard:

![image](resources/cloud_app_settings.png)

## Run the Application

1. Install the libpqxx driver:
    * Homebrew users can install using the [brew install libpqxx](https://formulae.brew.sh/formula/libpqxx) command.
    * Others can build the driver using one of the options listed in the [Building libpqxx](https://github.com/jtv/libpqxx#building-libpqxx) section.
    
2. Replace the `{libpq-install-dir}` placeholder with your libpq installation root dir and build the application with `gcc` or `clang`:
    ```bash
    gcc sample-app.c -o sample-app -I{libpq-install-dir}/libpq/include -L{libpq-install-dir}/libpq/lib -lpq
    ```
3. Run the application:
    ```bash
    ./sample-app
    ```

Upon successful execution, you will see output similar to the following:

```bash
>>>> Connecting to YugabyteDB!
>>>> Connected to YugabyteDB!
>>>> Successfully created table DemoAccount.
>>>> Selecting accounts:
name = Jessica, age = 28, country = USA, balance = 10000
name = John, age = 28, country = Canada, balance = 9000
>>>> Transferred 800 between accounts.
>>>> Selecting accounts:
name = Jessica, age = 28, country = USA, balance = 9200
name = John, age = 28, country = Canada, balance = 9800
```

## Explore Application Logic

Congrats! You've successfully executed a simple C app that works with Yugabyte Cloud.

Now, explore the source code of `sample-app.c`:
1. `connect` function - establishes a connection with your cloud instance via the libpq driver.
3. `createDatabase` function - creates a table and populates it with sample data.
4. `selectAccounts` function - queries the data with SQL `SELECT` statements.
5. `transferMoneyBetweenAccounts` function - updates records consistently with distributed transactions.

## Questions or Issues?

Having issues running this application or want to learn more from Yugabyte experts?

Join [our Slack channel](https://communityinviter.com/apps/yugabyte-db/register),
or raise a question on StackOverflow and tag the question with `yugabytedb`!