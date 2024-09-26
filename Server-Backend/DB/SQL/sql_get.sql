
--  Select UID of user by name
SELECT userstokey.UIDKey FROM userstokey, users WHERE (users.UserID = userstokey.UserID) AND users.FirstName = 'Andrejs';

--  Select all UID
SELECT userstokey.UIDKey FROM userstokey, users WHERE users.UserID = userstokey.UserID;

--  ADD user (You need UID, Name, Surname)

USE smartlock;

start transaction;

	INSERT INTO Users (FirstName, LastName) VALUES ('Joshua', 'Raistric');

	SELECT @id:=LAST_INSERT_ID();

	INSERT INTO UsersToKey (UserID, UIDKey) VALUES (@id, 'AA AC 81 16');

commit;


--  Delete User by UID

USE smartlock;

start transaction;
	select @id:=UserID from userstokey where UIDKey = "AA AC 81 16";

	DELETE FROM userstokey where UserID=@id;

	DELETE FROM Users where UserID=@id;
commit;

--  Delete User by name and surname

USE smartlock;

start transaction;
	select @id:=UserID from Users where FirstName = "Joshua" and LastName = "Raistric";

	DELETE FROM userstokey where UserID=@id;

	DELETE FROM Users where UserID=@id;
commit;