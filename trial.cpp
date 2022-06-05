// create database testdb_751435C;
// use testdb_751435C;
// create table Users ( id int NOT NULL auto_increment primary key, first_name varchar(50) NOT NULL, last_name varchar(50), zipcode int);
// INSERT INTO Users (first_name, last_name, zipcode) VALUES ("terry","pratchett",92124), ("ian","tregellis",92123);


//create database db_9286;
//use db_9286;
//create table Users ( id int NOT NULL auto_increment primary key, first_name varchar(50) NOT NULL, last_name varchar(50), age int, zipcode int);
//INSERT INTO Users (first_name, last_name, age, zipcode) VALUES ("Aurthur C.","Clarke", 105, 92127), ("Terry",     "Pratchett", 70,  92124), ("Ian", "Tregellis", 48,  92123), ("Jody", "Taylor", 50, 92120), ("Stephen",   "King", 74, 92125), ("Ted", "Chiang", 56, 92120) ("Anthony", "Doerr", 52, 92122), ("J.R.R.", "Tolkien", 130, 92126), ("Seldon", "Edwards",   81,92128), ("Neal","Stephenson",62,92121);
//SELECT * from Users ;
//SELECT * from Users  limit 3 where zipcode>92122 order by zipcode;
//select first_name, last_name from Users order by last_name where age>40;
//SELECT * from Users where zipcode>92122 AND zipcode<92127;
//show tables;
//dump database db_9286;
//drop database db_9286;

//create database testdb_46261E;
//create database testdb_24725E;
//use testdb_24725E;
//create table Users ( id int NOT NULL auto_increment primary key, first_name varchar(50) NOT NULL, last_name varchar(50), age int, zipcode int);
//INSERT INTO Users (first_name, last_name, age, zipcode) VALUES ("Terry", "Pratchett", 70,  92124), ("Ian",       "Tregellis", 48,  92123), ("Jody",      "Taylor",    50,  92120), ("Stephen",   "King",      74,  92125), ("Ted",       "Chiang",    56,  92120);
//use testdb_46261E;
//drop database testdb_46261E;
//use testdb_24725E;
//select * from Users;
//update Users set zipcode=30057 where id=5;
//select * from Users where zipcode=30057;
//drop database testdb_24725E;\nquit;\n"



//create database testdb_63332F;
//create database testdb_50390F;
//use testdb_50390F;
//create table Users ( id int NOT NULL auto_increment primary key, first_name varchar(50) NOT NULL, last_name varchar(50), age int, zipcode int);
//INSERT INTO Users (first_name, last_name, age, zipcode) VALUES ("Terry",     "Pratchett", 70,  92124), ("Ian",       "Tregellis", 48,  92123), ("Jody",     "Taylor",    50,  92120), ("Stephen",   "King",      74,  92125), ("Ted",       "Chiang",    56,  92120);
//use testdb_63332F;
//drop database testdb_63332F;
//use testdb_50390F;
//select * from Users;
//DELETE from Users where zipcode=92120;
//select * from Users
//DELETE from Users where zipcode<92124;
//select * from Users
//DELETE from Users where zipcode>92124;
//select * from Users
//drop database testdb_50390F;
//quit;

//create database testdb_97497G;
//use testdb_97497G;
//create table Accounts ( id int NOT NULL auto_increment primary key, account_type varchar(25) NOT NULL, amount int);
//create table Users ( id int NOT NULL auto_increment primary key, first_name varchar(50) NOT NULL, last_name varchar(50), age int, zipcode int);
//INSERT INTO Users (first_name, last_name, age, zipcode) VALUES ("Terry",     "Pratchett", 70,  92124), ("Ian", "Tregellis", 48,  92123), ("Jody",      "Taylor",    50,  92120), ("Stephen", "King",      74,  92125), ("Ted",       "Chiang",    56,  92120);
//show tables;
//drop table Users;
//show tables;
//drop database testdb_97497G;