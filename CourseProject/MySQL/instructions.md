### Setting up mysql
(Set up on Debian 9)
** Install and setup mysql server **
1. sudo apt-get update
1. sudo apt-get install mysql-server
1. Launch mysql server `systemctl start mysql`

### Load in tables
1. Simply run the script by `./loadTables.sh` to load all tables into the server

### Setup vscode (Visual Studio Code) for easy queries
** Set up User **
(You may not have to do this, I had to make another user for it to work)
1. Login to mysql server `mysql -u root -p`
1. Command `USE mysql;`
1. `GRANT ALL PRIVILEGES ON *.* TO 'username'@'localhost' IDENTIFIED BY 'password';` Makes user with root privileges.
** Vscode setup **
1. Install vscode
1. In VScode install "mysql" extension
1. Lower left corner it should have "mysql" after you reload, add new mysql server/user and follow instructions
1. Type queries in file and right click and run a query.
