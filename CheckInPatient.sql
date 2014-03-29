--This checks in a patient
CREATE PROCEDURE dbo.CheckIn (@firstName AS VARCHAR(50), @lastName AS VARCHAR(50), @dateOfBirth AS DATETIME, @SSN AS CHAR(9), 
	@middleInitial AS VARCHAR(1), @spouse AS INT, @addressId AS VARCHAR(50), @street1 VARCHAR(50), @street2 VARCHAR(50), 
	@city AS VARCHAR(30), @zip INT, @state AS VARCHAR(2), @phoneNumber AS VARCHAR(30), @doctorId AS INT, 
	@visitReason AS VARCHAR(200), @dateTime AS DATETIME, @treatment AS INT, @refferal AS INT, @related AS INT,
	@patientHeight AS VARCHAR(7), @patientWeight AS DECIMAL(5,2), @bloodPressure AS VARCHAR(20), 
	@oxygenLevel AS DECIMAL(5,2), @heartRate AS DECIMAL(5,2), @dateTimeTaken AS DATETIME, @roomId AS INT) AS
	DECLARE @patientId INT
	DECLARE @statisticsId INT
	DECLARE @amount INT
	IF(NOT EXISTS (SELECT 1 FROM Address WHERE AddressId = @addressId))
		BEGIN
			INSERT INTO Address (AddressId, Street1, Street2, City, State, Zip)
				VALUES (@addressId, @street1, @street2, @city, @state, @zip)
		END
	IF(NOT EXISTS (SELECT 1 FROM PhoneNumber WHERE PhoneNumber = @phoneNumber))
		BEGIN
			INSERT INTO PhoneNumber (PhoneNumber, PersonId)
				VALUES (@phoneNumber, NULL)
		END
	IF(NOT EXISTS(SELECT 1 FROM Person WHERE FirstName = @firstName AND LastName = @lastName AND DateOfBirth = @dateOfBirth))
		BEGIN
			INSERT INTO Person (FirstName, LastName, MiddleInitial, SSN, DateOfBirth, SpouseId)
				VALUES (@firstName, @lastName, @middleInitial, @SSN, @dateOfBirth, @spouse)
			SELECT @patientId = (SELECT PersonId FROM Person WHERE FirstName = @firstName AND LastName = @lastName AND DateOfBirth = @dateOfBirth)
			INSERT INTO AddressAndPeople (PersonId, AddressId)
				VALUES (@patientId, @addressId)
			UPDATE PhoneNumber
				SET PersonId = @patientId
				WHERE PhoneNumber = @phoneNumber
		END
	ELSE 
		BEGIN
			SELECT @patientId = (SELECT PersonId FROM Person WHERE FirstName = @firstName AND LastName = @lastName AND DateOfBirth = @dateOfBirth)
			INSERT INTO AddressAndPeople (PersonId, AddressId)
				VALUES (@patientId, @addressId)
			UPDATE PhoneNumber
				SET PersonId = @patientId
				WHERE PhoneNumber = @phoneNumber
		END