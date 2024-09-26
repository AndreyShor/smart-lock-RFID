CREATE DATABASE IF NOT EXISTS smartlock;
USE smartlock;

CREATE TABLE IF NOT EXISTS Users (
    UserID int NOT NULL AUTO_INCREMENT,
    FirstName varchar(255) NOT NULL,
    LastName varchar(255) NOT NULL,
    PRIMARY KEY (UserID)
);

CREATE TABLE IF NOT EXISTS UsersToKey (
    UserID int NOT NULL,
    UIDKey varchar(255) NOT NULL,
    PRIMARY KEY (UIDKey),
    FOREIGN KEY (UserID) REFERENCES Users(UserID)
);

INSERT INTO Users (FirstName, LastName) VALUES ('Andrejs', 'Sorstkisn');
INSERT INTO Users (FirstName, LastName) VALUES ('Ian', 'Dukes');
INSERT INTO Users (FirstName, LastName) VALUES ('Jon ', 'Whitby');

INSERT INTO UsersToKey (UserID, UIDKey) VALUES ('1', '5A C1 3F 16');
INSERT INTO UsersToKey (UserID, UIDKey) VALUES ('2', '39 3F AF 87');
INSERT INTO UsersToKey (UserID, UIDKey) VALUES ('3', '39 4F AF 55');