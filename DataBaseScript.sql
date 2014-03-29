DROP TABLE TransactionAndVisit; --
DROP TABLE Transactions; --
DROP TABLE PaymentType; --
DROP TABLE TreatmentsAndVisits; --
DROP TABLE TreatmentAndInsurance; --
DROP TABLE Treatment; --
DROP TABLE InsuranceInformation; --
DROP TABLE PatientsAndRooms; --
DROP TABLE RoomStatus; --
DROP TABLE RoomAndLocation; --
DROP TABLE Room; --
DROP TABLE Location; --
DROP TABLE Section; --
DROP TABLE Area; --
DROP TABLE PatientVisit; --
DROP TABLE PatientStatistics; --
DROP TABLE PhoneNumber; --
DROP TABLE DoctorsAndPatients; --
DROP TABLE AddressAndPeople; --
DROP TABLE Address; --
DROP TABLE JobsAndEmployees; --
DROP TABLE Review; --
DROP TABLE Schedule; --
DROP TABLE Salary; --
DROP TABLE JobInformation; --
DROP TABLE VisitorInstance; --
DROP TABLE Person; --
DROP TABLE DepartmentsAndPeople; --
DROP TABLE Departments; --

CREATE TABLE Departments (
	DepartmentId		INT					PRIMARY KEY IDENTITY(1,1),
	Description			VARCHAR(40)			NOT NULL
);
CREATE TABLE Person (
	PersonId			INT				PRIMARY KEY IDENTITY(1,1),
	FirstName			VARCHAR(50)		NOT NULL,
	MiddleInitial		VARCHAR(1),
	LastName			VARCHAR(50)		NOT NULL,
	SSN					CHAR(9),
	DateOfBirth			DATETIME		NOT NULL,
	SpouseId			INT				REFERENCES Person(PersonId),
);
CREATE TABLE DepartmentsAndPeople (
	DepartmentId		INT			NOT NULL REFERENCES Departments(DepartmentId),
	PersonId			INT			NOT NULL REFERENCES Person(PersonId),
	PRIMARY KEY(DepartmentId, PersonId)
);
CREATE TABLE VisitorInstance (
	VisitorId			INT				PRIMARY KEY REFERENCES Person(PersonId),
	PatientVisited		INT				REFERENCES Person(PersonId),
	TimeIn				DATETIME		NOT NULL,
	TimeOut				DATETIME
);
CREATE TABLE JobInformation (
	JobId			VARCHAR(20)		PRIMARY KEY,
	JobTitle		VARCHAR(60)		NOT NULL,
	StartDate		DATE			NOT NULL,
	EndDate			DATE,
	SupervisorId	INT				REFERENCES Person(PersonId)
);
CREATE TABLE Salary (
	JobId			VARCHAR(20)		PRIMARY KEY REFERENCES JobInformation(JobId),
	CurrentSalary	DECIMAL(10, 2)	NOT NULL,
	IsHourly		CHAR(1)			NOT NULL,
	Raise			DECIMAL(9, 2)
);
CREATE TABLE Schedule (
	ScheduleId		VARCHAR(20)			PRIMARY KEY,
	EmployeeId		INT					NOT NULL REFERENCES Person(PersonId),
	TimeIn			DATETIME			NOT NULL,
	TimeOut			DATETIME			NOT NULL
);
CREATE TABLE Review (
	ReviewId		VARCHAR(20)			PRIMARY KEY,
	ReviewerId		INT					NOT NULL REFERENCES Person(PersonId),
	EmployeeId		INT					NOT NULL REFERENCES Person(PersonId),
	DateAndTime		DATETIME			NOT NULL,
	Notes			VARCHAR(1400)		NOT NULL
);
CREATE TABLE JobsAndEmployees (
	EmployeeId		INT				NOT NULL REFERENCES Person(PersonId),
	JobId			VARCHAR(20)		NOT NULL REFERENCES JobInformation(JobId),
	PRIMARY KEY(EmployeeId, JobId)
);
CREATE TABLE Address (
	AddressId		VARCHAR(50)		PRIMARY KEY,
	Street1			VARCHAR(50)		NOT NULL,
	Street2			VARCHAR(30),
	City			VARCHAR(30)		NOT NULL,
	State			CHAR(2)			NOT NULL,
	Zip				INT				NOT NULL
);
CREATE TABLE AddressAndPeople (
	AddressId		VARCHAR(50)		NOT NULL REFERENCES Address(AddressId),
	PersonId		INT				NOT NULL REFERENCES Person(PersonId),
	PRIMARY KEY(AddressId, PersonId)
);
CREATE TABLE DoctorsAndPatients (
	PersonId		INT		NOT NULL REFERENCES Person(PersonId),
	PatientId		INT		NOT NULL REFERENCES Person(PersonId),
	PRIMARY KEY(PersonId, PatientId)
);
CREATE TABLE PhoneNumber (
	PhoneNumberId		INT				PRIMARY KEY IDENTITY(1,1),
	PersonId			INT				REFERENCES Person(PersonId),
	PhoneNumber			VARCHAR(30)		NOT NULL
);
CREATE TABLE PatientStatistics (
	StatisticsId		INT				PRIMARY KEY IDENTITY(1,1),
	PatientWeight		DECIMAL(5,2)	NOT NULL,
	PatientHeight		VARCHAR(7)		NOT NULL, --in feet, inches
	HeartRate			DECIMAL(5,2)	NOT NULL,
	BloodPressure		VARCHAR(20)		NOT NULL,
	OxygenLevel			DECIMAL(5,2)	NOT NULL,
	DateAndTimeTaken	DATETIME		NOT NULL
);
CREATE TABLE PatientVisit (
	VisitId			INT					PRIMARY KEY IDENTITY(1,1),
	DateTimeIn		DATETIME			NOT NULL,
	DatetImeOut		DATETIME, 
	PatientId		INT					NOT NULL REFERENCES Person(PersonId),
	ReasonOfVisit	VARCHAR(200)		NOT NULL,
	RelatedVisit	INT					REFERENCES PatientVisit(VisitId),
	ReferredBy		INT					REFERENCES Person(PersonId),
	StatisticsId	INT					REFERENCES PatientStatistics(StatisticsId)		
);
CREATE TABLE Area (
	AreaId		INT			PRIMARY KEY IDENTITY(1,1),
	AreaName	VARCHAR(30)	NOT NULL
);
CREATE TABLE Section(
	SectionId	INT			PRIMARY KEY IDENTITY(1,1),
	SectionName	VARCHAR(30)	NOT NULL
);
CREATE TABLE Location (
	LocationId		INT					PRIMARY KEY IDENTITY(1,1),
	AreaId			INT					NOT NULL REFERENCES Area(AreaId),
	SectionId		INT					NOT NULL REFERENCES Section(SectionId),
	Floor			VARCHAR(20)			NOT NULL,
);
CREATE TABLE Room (
	RoomId			INT					PRIMARY KEY,
	PatientLimit	INT					NOT NULL,
	StartOccupy		DATETIME			NOT NULL,
	EndOccupy		DATETIME,
	PatientAmount	INT					NOT NULL,
	Notes			VARCHAR(1500)
);
CREATE TABLE RoomAndLocation (
	LocationId		INT			NOT NULL REFERENCES Location(LocationId),
	RoomId			INT			NOT NULL REFERENCES Room(RoomId),
	PRIMARY KEY(LocationId, RoomId)
);
CREATE TABLE RoomStatus (
	RoomId		INT				PRIMARY KEY REFERENCES Room(RoomId),
	Status		VARCHAR(20)		NOT NULL
);
CREATE TABLE PatientsAndRooms (
	RoomId		INT		NOT NULL REFERENCES Room(RoomId),
	PatientId	INT		NOT NULL REFERENCES Person(PersonId),
	PRIMARY KEY(RoomId, PatientId)
);
CREATE TABLE InsuranceInformation (
	InsuranceId			INT				PRIMARY KEY IDENTITY(1,1),
	ContactPersonId		INT				NOT NULL REFERENCES Person(PersonId),
	CompanyName			VARCHAR(40)		NOT NULL,
	AddressId			VARCHAR(50)		NOT NULL REFERENCES Address(AddressId), --??
	PhoneNumberId		INT				NOT NULL REFERENCES PhoneNumber(PhoneNumberId) --???
);
CREATE TABLE Treatment (
	TreatmentId			INT					PRIMARY KEY IDENTITY(1,1),
	TreatmentName		VARCHAR(30)			NOT NULL,
	TreatmentDose		DECIMAL(7,3)		NOT NULL,
	TreatmentDuration	TIME			NOT NULL
);
CREATE TABLE TreatmentAndInsurance (
	TreatmentId			INT			NOT NULL REFERENCES Treatment(TreatmentId),
	InsuranceId			INT			NOT NULL REFERENCES InsuranceInformation(InsuranceId),
	PRIMARY KEY(TreatmentId, InsuranceId),
	InsuranceCost		DECIMAL(12,2)		NOT NULL,
	PatientCost			DECIMAL(12,2)		NOT NULL
);
CREATE TABLE TreatmentsAndVisits (
	VisitId				INT			NOT NULL REFERENCES PatientVisit(VisitId),
	TreatmentId			INT			NOT NULL REFERENCES Treatment(TreatmentId),
	PRIMARY KEY(VisitId, TreatmentId),
	EmployeeGiving		INT			NOT NULL REFERENCES Person(PersonId),
	DateAndTime			DATETIME
);
CREATE TABLE PaymentType (
	PaymentId		INT			PRIMARY KEY IDENTITY(1,1),
	TypeName		VARCHAR(20)	NOT NULL
);
CREATE TABLE Transactions (
	TransactionId			INT				PRIMARY KEY IDENTITY(1,1),
	TotalCost				DECIMAL(8,2)	NOT NULL,
	InsurancePaidAmount		DECIMAL(8,2)	NOT NULL,
	PatientPaidAmount		DECIMAL(8,2)	NOT NULL,
	PaymentId				INT				NOT NULL REFERENCES PaymentType(PaymentId)
);
CREATE TABLE TransactionAndVisit (
	TransactionId			INT				NOT NULL REFERENCES Transactions(TransactionId),
	VisitId					INT				NOT NULL REFERENCES PatientVisit(VisitId),
	PRIMARY KEY(TransactionId, VisitId)
);
INSERT INTO Departments (Description)
	VALUES ('Emergency Department'),
		   ('Medicine'),
		   ('Psychiatry'),
		   ('Pediatrics'),
		   ('Intensive Care Unit'),
		   ('Pharmacy'); 
INSERT INTO Person (FirstName, MiddleInitial, LastName, SSN, DateOfBirth, SpouseId)
	VALUES ('An', NULL, 'Shoji', 160132001, '1993-09-11', NULL), --Tsukasa, patient
		   ('Ryo', NULL, 'Sakuma', 460132002, '1963-10-10', NULL), --Bear, works in pediatrics, doctor 4
		   ('Machiko', NULL, 'Hiramitsu', 281302003, '1981-01-02', NULL), --BT, works in medicine, doctor 2
		   ('Yumi', NULL, 'Touma', NULL, '1979-12-24', NULL), --Helba, works in icu, doctor 5
		   ('Emma', 'A', 'Wielant', NULL, '1972-03-03', 6), --works as nurse 0
		   ('Harald', NULL, 'Hoerwick', NULL, '1960-05-05', 5), --doctor, emergency 1
		   ('Ryou', NULL, 'Misaki', 101212007, '1999-02-14', NULL), --Sora, patient 7
		   ('Shinchiro', NULL, 'Miki', NULL, '1971-07-05', NULL), --Crim, doctor, psychiatry 3
		   ('Mark', NULL, 'Shoji', NULL, '1953-11-11', NULL); --Tsukasa's father, insurance contact (CCCorp) 9 
INSERT INTO DepartmentsAndPeople (DepartmentId, PersonId)
	VALUES (4, 2), --Bear, pediatrics
		   (2, 3), --BT, medicine
		   (5, 4), --Helba, icu
		   (4, 5), --Emma, pediatrics
		   (1, 6), --Harald, emergency
		   (3, 8); --Crim, psychiatry
INSERT INTO VisitorInstance (VisitorId, PatientVisited, TimeIn, TimeOut)
	VALUES (9, 1, '2009/04/10 10:00:00', '2009/04/10 11:00:30'), --Mark visited Tsukasa, 1
		   (2, 1, '2009/12/17 05:30:00', '2009/12/17 06:45:30'), --Bear visited Tsukasa, 2
		   (4, 7, '2009/12/20 03:15:00', '2009/12/20 03:55:00'), --Bear visited Sora
		   (8, 7, '2010/01/01 02:33:40', '2010/01/01 03:01:50'), --Crim visited Sora
		   (1, 7, '2010/01/01 12:10:00', '2010/01/01 01:30:00'), --Tsukasa visited Sora
		   (3, 1, '2009/11/10 10:45:10', '2009/11/10 05:10:50'); --Mark visited Tsukasa, 3
INSERT INTO JobInformation (JobId, JobTitle, StartDate, EndDate, SupervisorId)
	VALUES ('Nurse-1', 'Nurse', '2006/03/15', NULL, 4), --Emma
		   ('Pediatrician-2', 'Pediatrician', '2000/02/05', '2008/09/20', NULL), --Bear
		   ('Surgeon-3', 'Surgeon', '1999/09/09', NULL, NULL), --Helba
		   ('Janitor-4', 'Janitor', '2000/12/13', '2009/02/12', 2), --Bear
		   ('Receptionist-5', 'Receptionist', '1995/05/10', '2000/02/10', 5), --Emma, BT
		   ('Psychiatrist-6', 'Psychiatrist', '1990/12/19', NULL, NULL), --Crim
		   ('Pharmicist-7', 'Pharmicist', '1999/12/12', NULL, NULL); --BT
INSERT INTO Salary (JobId, CurrentSalary, IsHourly, Raise)
	VALUES ('Nurse-1', 65000.00, 'N', NULL),
		   ('Pediatrician-2', 77.60, 'Y', 1.2),
		   ('Surgeon-3', 260000.00, 'N', NULL),
		   ('Janitor-4', 23000, 'N', NULL),
		   ('Receptionist-5', 31000, 'N', 2000),
		   ('Psychiatrist-6', 165000, 'Y', 3000);
INSERT INTO Schedule (ScheduleId, EmployeeId, TimeIn, TimeOut)
	VALUES ('Shift 2-1', 2, '2009/12/16 01:00:00', '2009/12/16 10:00:00'), --Bear 1
		   ('Shift 2-2', 2, '2009/12/17 10:30:00', '2009/12/18 06:15:00'), --Bear 2
		   ('Shift 6-1', 6, '2009/12/17 02:00:00', '2009/12/17 10:00:00'), --Emma 1
		   ('Shift 5-1', 5, '2009/12/17 09:00:00', '2009/12/17 18:00:00'), --Harald 1
		   ('Shift 4-1', 4, '2009/12/17 08:00:00', '2009/12/17 20:00:00'), --Helba 1
		   ('Shift 3-1', 3, '2009/12/17 06:00:00', '2009/12/17 18:00:00'), --BT 1
		   ('Shift 6-2', 6, '2009/12/18 08:00:00', '2009/12/18 12:00:00'); --Emma 1
INSERT INTO Review (ReviewId, ReviewerId, EmployeeId, DateAndTime, Notes)
	VALUES ('Review-1', 2, 5, '2008/10/10 01:20:00', 'Has good beside manner'), --Bear reviewing Emma
		   ('Review-2', 2, 3, '2009/12/21 10:30:00', 'Effcient'), --Bear reviewing BT
		   ('Review-3', 8, 3, '2006/05/05 9:00:00', 'Efficient, but cold'), --Crim reviewing BT
		   ('Review-4', 4, 2, '2009/04/20', 'Overall, does good work.'), --Harald reviewing Bear
		   ('Review-5', 4, 8, '2009/10/10', 'Efficient'), -- Harald reviewing Crim
		   ('Review-6', 4, 2, '2009/12/18 08:00:00', 'Mandatory review'); -- Harald reviewing Bear 2
INSERT INTO JobsAndEmployees (EmployeeId, JobId)
	VALUES (2, 'Pediatrician-2'), --Bear, pediatrician
		   (5, 'Surgeon-3'), --Harald, surgeon
		   (8, 'Psychiatrist-6'), --Crim, Psychiatrist
		   (6, 'Nurse-1'), --Emma, nurse
		   (6, 'Receptionist-5'), --Emma, Receptionist
		   (3, 'Pharmicist-7'), --BT, pharmicist
		   (4, 'Surgeon-3'); --Helba, surgeon
INSERT INTO Address (AddressId, Street1, Street2, City, State, Zip)
	VALUES ('The World Hospital', '500 First Street', NULL, 'San Diego', 'CA', 91914),
		   ('Ryo-01', '14 Godot Way', 'Box 1', 'San Diego', 'CA', 91915),
		   ('Harald-02', '23 Fragment Road', NULL, 'San Diego', 'CA', 91915),
		   ('World Insurance Company', '212 Second Street', NULL, 'New York City', 'NY', 10032), --212
		   ('Altimit Corp', '100 Nassau Street', NULL, 'San Diego', 'CA', 91914), --619
		   ('CCCorp', '48 Main Street', NULL,'San Diego', 'CA', 91914),
		   ('Crim-01', '45 Speed Street', NULL, 'San Diego', 'CA', 91915),
		   ('Shoji-01', '666 Old School Way', NULL, 'San Diego', 'CA', 91915),
		   ('New Enterprises', '7000 Main Street', NULL, 'Syracuse', 'NY', 13244), --315
		   ('Aperture Insurance', '1100 New Main Street', 'PO Box 1010', 'Chicago', 'IL', 60018); --773
INSERT INTO AddressAndPeople (AddressId, PersonId)
	VALUES ('Harald-02', 4), --it's temporary...
		   ('Shoji-01', 1),
		   ('Crim-01', 8),
		   ('Harald-02', 6),
		   ('Harald-02', 5),
		   ('Ryo-01', 2),
		   ('Shoji-01', 9);
INSERT INTO DoctorsAndPatients (PatientId, PersonId)
	VALUES (1, 2), --Tsukasa
		   (1, 6),
		   (1, 3),
		   (7, 2), --Sora
		   (7, 3),
		   (7, 6);
INSERT INTO PhoneNumber (PersonId, PhoneNumber)
	VALUES (9, '(619) 234-7789'), --Father
		   (2, '(619) 199-6675'), --Bear
		   (3, '(619) 332-4521'), --BT
		   (4, '(619) 683-4477'), --Helba
		   (5, '(619) 687-2376'), --Emma
		   (6, '(619) 120-0543'), --Harald
		   (8, '(619) 344-2023'); --Crim
INSERT INTO PatientStatistics (PatientWeight, PatientHeight, HeartRate,	BloodPressure, OxygenLevel, DateAndTimeTaken)
	VALUES (145, '5ft5in', 70, '100/80', 95, '2009/04/10 13:00:00'), --normal is 60-100 bpm, above 92% oxygen, pref less than 120/80
		   (145, '5ft5in', 67, '100/75', 94.5, '2009/04/10 19:00:00'),
		   (143, '5ft5in', 70, '99/75', 94, '2009/04/11 01:00:00'),
		   (143, '5ft5in', 75, '97/77', 97, '2009/04/11 07:00:00'),
		   (141, '5ft5in', 72, '98/75', 93, '2009/04/11 13:00:00'),
		   (135, '5ft5in', 100.1, '98/76', 90, '2009/7/04 09:16:00');
INSERT INTO PatientVisit (DateTimeIn, DateTimeOut, PatientId, ReasonOfVisit, RelatedVisit, ReferredBy, StatisticsId) 
	VALUES ('2009/04/10 13:00:00', '2009/04/10 13:05:00', 1,'check up', NULL, 2, 1), --check Tsukasa's vitals
		   ('2009/04/10 19:00:00', '2009/04/10 19:05:00', 1, 'check up', 1, 2, 2), --check An 2
		   ('2009/04/11 01:00:00', '2009/04/11 01:05:00', 1, 'routine check', 2, 2, 3), --check An 3
		   ('2009/04/11 07:00:00', '2009/04/11 07:05:00', 1, 'routine check', 3, 2, 4), --check An 4
		   ('2009/04/11 13:00:00', '2009/04/11 13:05:00', 1, 'routine check', NULL, 2, 5), --check An 5
		   ('2009/04/10 10:00:00', '2009/04/10 11:00:30', 1,'family visit', NULL, NULL, NULL), --family visit 1
		   ('2009/7/04 09:16:00', '2009/7/04 09:36:00',1, 'family visit', 6,NULL, 6); --family 2
INSERT INTO Area (AreaName)
	VALUES ('Psychiatric'),
		   ('General'),
		   ('Emergency Room'),
		   ('Long Term Ward'),
		   ('Short Term Ward');
INSERT INTO Section (SectionName)
	VALUES ('Physical Therapy'),
		   ('Pediatrics'),
		   ('Adults'),
		   ('Senior Citizens'),
		   ('Reception');
INSERT INTO Location (AreaId, SectionId, Floor)
	VALUES (4, 2, 2), --1 short term
		   (4, 2, 3), --2 
		   (5, 3, 2), --3 longterm
		   (5, 3, 3), --4 
		   (2, 5, 1), --5 reception
		   (1, 1, 4), --6 physical therapy
		   (3, 5, 1); --7 emergency room
INSERT INTO Room (RoomId, PatientLimit, StartOccupy, EndOccupy, PatientAmount, Notes)
	VALUES (201, 5, '2009/04/05 13:00:00', '2009/04/12 09:00:00', 1, 'New patient in coma'), --short term patients, child
		   (342, 5, '2009/12/05 09:32:00', NULL, 4, NULL), --short term patients, adult
		   (222, 3, '2009/04/12 09:00:00', NULL, 1, NULL), --long term patients, child
		   (321, 3, '2009/12/12 10:00:00', NULL, 2, NULL), --long term patiens, adult
		   (445, 25, '2009/12/17 09:15:00', '2009/12/17 18:00:00', 20, NULL), --physical therapy room
		   (100, 50, '2009/04/10 09:00:00', '2009/04/10 09:10:00', 3, NULL), --reception/lobby
		   (101, 10, '2009/04/10 12:00:00', '2009/04/10 14:00:00', 1, NULL); --emergency room
INSERT INTO RoomAndLocation (RoomId, LocationId)
	VALUES (201, 1),
		   (342, 2),
		   (222, 3),
		   (321, 4),
		   (445, 6),
		   (100, 5),
		   (101, 7);
INSERT INTO RoomStatus (RoomId, Status)
	VALUES (201, 'Occupied'),
		   (342, 'Vacant'),
		   (222, 'Cleaning'),
		   (321, 'Cleaning'),
		   (445, 'Occupied'),
		   (100, 'Occupied'),
		   (101, 'Occupied');
INSERT INTO PatientsAndRooms (PatientId, RoomId)
	VALUES (6, 201),
		   (5, 222),
		   (1, 101),
		   (2, 445),
		   (7, 201),
		   (7, 222);
INSERT INTO InsuranceInformation (ContactPersonId, CompanyName, AddressId, PhoneNumberId)
	VALUES (9, 'CCCorp', 'CCCorp', 1),
		   (8, 'Altimit Corp', 'Altimit Corp', 7),
		   (6, 'World Insurance Company', 'World Insurance Company', 6),
		   (5, 'New Enterprises', 'New Enterprises', 5),
		   (3, 'Aperture Insurance', 'Aperture Insurance', 3);
INSERT INTO Treatment (TreatmentName, TreatmentDose, TreatmentDuration)
	VALUES ('General check up', 0, '00:30:00'), --350 dollars 
		   ('X ray', 0, '00:20:00'), --70 dollars to 200
		   ('Bone setting', 0, '00:30:00'), --850
		   ('Blood test', 0, '00:05:00'), --50
		   ('Life support', 0, '23:59:59'), --2500/day
		   ('Dialysis', 0, '05:00:00'), --8500
		   ('MRI', 0, '00:20:00'),
		   ('Vicodin', .003, '05:00:00'), --grams
		   ('Physical Therapy', 0, '01:00:00'); --300
INSERT INTO TreatmentAndInsurance (TreatmentId, InsuranceId, InsuranceCost, PatientCost)
	VALUES (1, 1, 250.00, 350.00),
		   (2, 2, 100.00, 120.00),
		   (3, 1, 500.00, 850.00),
		   (4, 2, 45.00, 50.00),
		   (5, 3, 2000.00, 2500.00),
		   (6, 4, 7000.00, 8500.00),
		   (7, 4, 800.00, 1100.00),
		   (8, 5, 2.00, 2.00),
		   (9, 1, 300.00, 450.00);
INSERT INTO TreatmentsAndVisits (VisitId, TreatmentId, EmployeeGiving, DateAndTime)
	VALUES (1, 1, 5, '2009/04/10 13:00:00'), --1, emma
		   (2, 1, 2, '2009/04/10 19:00:00'), --2, bear
		   (3, 1, 2, '2009/04/11 01:00:00'), --3, bear
		   (4, 4, 5, '2009/04/11 07:00:00'), --4, emma
		   (5, 1, 2, '2009/04/11 13:00:00'), --5, bear
		   (7, 5, 2, '2009/7/04 09:16:00'); --7 life support, bear
INSERT INTO PaymentType (TypeName)
	VALUES ('Check'),
		   ('Credit Card'),
		   ('Debit Card'),
		   ('Cash'),
		   ('Direct Deposit');
INSERT INTO Transactions (TotalCost, InsurancePaidAmount, PatientPaidAmount, PaymentId)
	VALUES (350.00, 300.00, 50.00, 2), --check ups for 1
		   (2500.00, 2500.00, 0.0, 1), --life support 1 2500
		   (2500.00, 2000.00, 500.00, 5), --life support for sora 2500
		   (300.00, 250.00, 50.00, 4), --physical therapy for 1 
		   (300.00, 250.00, 50.00, 3), --pt for sora
		   (3000.00, 3000.00, 0.0, 5); --life support 2
INSERT INTO TransactionAndVisit (TransactionId, VisitId)
	VALUES (1, 1),
		   (1, 2),
		   (1, 3),
		   (1, 4),
		   (1, 5),
		   (2, 7);
SELECT * FROM Departments;
SELECT * FROM Person;
SELECT * FROM DepartmentsAndPeople;
SELECT * FROM VisitorInstance;
SELECT * FROM JobInformation;
SELECT * FROM Salary;
SELECT * FROM Review;
SELECT * FROM JobsAndEmployees;
SELECT * FROM Address;
SELECT * FROM AddressAndPeople;
SELECT * FROM DoctorsAndPatients;
SELECT * FROM PhoneNumber;
SELECT * FROM PatientStatistics;
SELECT * FROM PatientVisit;
SELECT * FROM Area;
SELECT * FROM Section;
SELECT * FROM Location;
SELECT * FROM Room;
SELECT * FROM RoomAndLocation;
SELECT * FROM RoomStatus;
SELECT * FROM PatientsAndRooms;
SELECT * FROM InsuranceInformation;
SELECT * FROM Treatment;
SELECT * FROM TreatmentAndInsurance;
SELECT * FROM TreatmentsAndVisits;
SELECT * FROM PaymentType;
SELECT * FROM Transactions;
SELECT * FROM TransactionAndVisit;

--VIEWS
DROP VIEW dbo.DoctorsAndJobs;
DROP VIEW dbo.DetailedPatientVisit;
DROP VIEW dbo.EmployeeContactInformation;
--Displays employees and their jobs
CREATE VIEW dbo.DoctorsAndJobs AS
	SELECT I.JobTitle, P.FirstName + ' ' + P.LastName AS 'Name'
		FROM JobsAndEmployees J
		INNER JOIN Person P
		ON J.EmployeeId = P.PersonId
		INNER JOIN JobInformation I
		ON J.JobId = I.JobId;
--view to display who visited who and when (visitorInstance
CREATE VIEW DetailedPatientVisit AS
	SELECT N.FirstName + ' ' + N.LastName AS 'VisitorName', P.FirstName + ' ' + P.LastName AS 'PatientName', V.TimeIn, V.TimeOut
		FROM Person P
		INNER JOIN VisitorInstance V
		ON P.PersonId = V.PatientVisited
		INNER JOIN Person N
		ON N.PersonId = V.VisitorId
--view to display employees and addresses
CREATE VIEW dbo.EmployeeContactInformation AS
	SELECT DISTINCT (P.FirstName + ' ' + P.LastName) AS 'EmployeeName', D.Street1, D.Street2, D.City, D.State, D.Zip, N.PhoneNumber
		FROM Person P
		INNER JOIN JobsAndEmployees J
		ON J.EmployeeId = P.PersonId
		INNER JOIN PhoneNumber N
		ON P.PersonId = N.PersonId
		INNER JOIN AddressAndPeople A
		ON P.PersonId = A.PersonId
		INNER JOIN Address D
		ON A.AddressId = D.AddressId;
SELECT * FROM dbo.DoctorsAndJobs;
SELECT * FROM dbo.DetailedPatientVisit;
SELECT * FROM dbo.EmployeeContactInformation;
--SP to update (done), SP to insert (done), SP to delete (done), SP to return table (done)
--extra SP (check in patient)
--SP to swap shifts (update)
DROP PROCEDURE dbo.SwapShifts;
SELECT * FROM Schedule;
EXEC dbo.SwapShifts 5, 6, 'Shift 5-1', 'Shift 6-1';
CREATE PROCEDURE dbo.SwapShifts (@employee1 AS INT, @employee2 AS INT, @shift1 AS VARCHAR(20), @shift2 AS VARCHAR(20)) AS
	DECLARE @message VARCHAR(40)
	DECLARE @hasShift1 BIT
	DECLARE @hasShift2 BIT
	IF (EXISTS (SELECT 1 FROM Schedule WHERE EmployeeId = @employee1 AND ScheduleId = @shift1))
		BEGIN
			IF (EXISTS (SELECT 1 FROM Schedule WHERE EmployeeId = @employee2 AND ScheduleId = @shift2))
				BEGIN
					UPDATE Schedule
					SET EmployeeId = @employee2
					WHERE EmployeeId = @employee1
						AND ScheduleId = @shift1
					UPDATE Schedule
					SET EmployeeId = @employee1
					WHERE EmployeeId = @employee2
						AND ScheduleId = @shift2
					SELECT @message = 'Swap successful!'
				END
			ELSE
				BEGIN
					SELECT @message = 'Employee 2 does not have this shift'
				END
		END
	ELSE
		BEGIN
			SELECT @message = 'Employee 1 does not have this shift'
		END
	PRINT @message;
--SP to move patient to another room (insert)--need to test...do on Monday
DROP PROCEDURE dbo.MovePatient;
EXEC dbo.MovePatient 1, 111; --works
EXEC dbo.MovePatient 1, 101; --works
EXEC dbo.MovePatient 1, 222; --works
CREATE PROCEDURE dbo.MovePatient (@patientId AS INT, @newRoomId AS INT) AS
	DECLARE @oldRoomId INT
	DECLARE @message VARCHAR(30)
	DECLARE @oldAmount INT
	DECLARE @newAmount INT
	SET @oldRoomId = (SELECT RoomId FROM PatientsAndRooms WHERE PatientId = @patientId)
	SELECT @oldAmount = (SELECT DISTINCT(PatientAmount) FROM Room WHERE RoomId = @oldRoomId)
	SELECT @newAmount = (SELECT DISTINCT(PatientAmount) FROM Room WHERE RoomId = @newRoomId)
	IF (NOT EXISTS (SELECT 1 FROM Room WHERE RoomId = @newRoomId))--doesn't exist
		BEGIN
			SELECT @message = 'This room does not exist.'
		END
	ELSE IF (@newAmount >= (SELECT PatientLimit FROM Room WHERE RoomId = @newRoomId))
		BEGIN
			SELECT @message = 'The room is full.'
		END
	ELSE IF (@oldRoomId = @newRoomId)
		BEGIN
			SELECT @message = 'Patient is in this room.'
		END
	ELSE 
		BEGIN
			SELECT @message = 'Patient moved.'
			IF (@newAmount <= 0)
				BEGIN
					UPDATE RoomStatus
					SET Status = 'Occupied'
					WHERE RoomId = @newRoomId
					SET @newAmount = 0
				END
			IF(@oldAmount <=0)
				BEGIN
					SET @oldAmount = 0
				END
			UPDATE Room
			SET PatientAmount = @oldAmount - 1
			WHERE RoomId = @oldRoomId
			UPDATE Room
			SET PatientAmount = @newAmount + 1
			WHERE RoomId = @newRoomId
			DELETE FROM PatientsAndRooms
			WHERE PatientId = @patientId
				AND RoomId = @oldRoomId
			INSERT INTO PatientsAndRooms (PatientId, RoomId)
				VALUES (@patientId, @newRoomId)
		END
	PRINT @message;
	INSERT INTO PatientsAndRooms (PatientId, RoomId) VALUES (1, 101)
--take 1 out of all the rooms except 101, move to 101 (case 1), done
--move 5 to 321
EXEC dbo.MovePatient 5, 321;
--try to put someone in rm 111 (dne), done
--put someone in an empty room 5, done
--move someone to PT room
SELECT * FROM Room
SELECT * FROM RoomStatus
SELECT * FROM PatientsAndRooms
UPDATE Room SET PatientAmount = 0 WHERE RoomId = 321
UPDATE PatientsAndRooms SET RoomId = 222 WHERE PatientId = 6
DROP PROCEDURE dbo.ShowAllTransactions;
--SP to display all transactions made (returns table)
CREATE PROCEDURE dbo.ShowAllTransactions (@patientId AS INT) AS
	DECLARE @message VARCHAR(30)
	IF (NOT EXISTS (SELECT PersonId FROM Person WHERE PersonId = @patientId))
		BEGIN
			SELECT @message = 'Patient does not exist.'
		END
	ELSE
		BEGIN
			SELECT P.PatientId, P.DateTimeIn, P.DatetImeOut, R.TreatmentName, N.TotalCost
				FROM PatientVisit P
				INNER JOIN TreatmentsAndVisits T
				ON T.VisitId = P.VisitId
				INNER JOIN Treatment R
				ON T.TreatmentId = R.TreatmentId
				INNER JOIN TransactionAndVisit A
				ON P.VisitId = A.VisitId
				INNER JOIN Transactions N
				ON A.TransactionId = N.TransactionId
				WHERE P.PatientId = @patientId
		END
	PRINT @message;
EXEC dbo.ShowAllTransactions 12;
EXEC dbo.ShowAllTransactions 1;
--SP to remove a treatment (delete), works
DROP PROCEDURE dbo.RemoveTreatment;
CREATE PROCEDURE dbo.RemoveTreatment (@treatmentId AS INT) AS
	DECLARE @message VARCHAR(30)
	IF (NOT EXISTS (SELECT 1 FROM Treatment WHERE TreatmentId = @treatmentId))
		BEGIN
			SELECT @message = 'Treatment does not exist.'
		END
	ELSE
		BEGIN
			SELECT @message = 'Removing treatment.'
			DELETE FROM TreatmentAndInsurance
			WHERE TreatmentId = @treatmentId
			DELETE FROM Treatment
			WHERE TreatmentId = @treatmentId
		END
	PRINT @message;
EXEC dbo.RemoveTreatment 12;
SELECT * FROM Treatment;
SELECT * FROM TreatmentAndInsurance;
SELECT * FROM TreatmentsAndVisits;
INSERT INTO Treatment (TreatmentName, TreatmentDose, TreatmentDuration)
	VALUES ('X ray', 0.0, '01:00:00');
--SP to check in patient (insert)
EXEC dbo.RemoveTreatment 3;
DROP PROCEDURE dbo.CheckIn;
--SP to check in patient
SELECT * FROM PatientsAndRooms
EXEC dbo.CheckIn 'An', 'Shoji', '1993-09-11', 160132001, NULL, NULL, 'An-01', '20 Day Road', NULL, 'San Diego', 91915, 'CA',
	'(619) 239-6698', 2, 'three month follow up', '2010/03/12 09:00:00', 1, 2, 7, '5ft5in', 147, '97/77', 97, 70, '2010/03/12 09:10:00', 201;
EXEC dbo.CheckIn 'Mariko', 'Misono', '1987-08-12', 220212332, NULL, NULL, 'Mariko-01', '20 Day Road', NULL, 'San Diego', 91915, 'CA',
	'(619) 239-6698', 6, 'General check up', '2010/03/12 10:00:00', 1, 2, NULL, '5ft4in', 143, '98/78', 97, 72, '2010/03/12 10:10:00', 342;
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
	INSERT INTO PatientStatistics (PatientWeight, PatientHeight, BloodPressure, OxygenLevel, DateAndTimeTaken, HeartRate)
		VALUES (@patientWeight, @patientHeight, @bloodPressure, @oxygenLevel, @dateTimeTaken, @heartRate)
	SELECT @statisticsId = (SELECT StatisticsId FROM PatientStatistics WHERE @dateTimeTaken = DateAndTimeTaken)
	INSERT INTO PatientVisit (DateTimeIn, PatientId, ReasonOfVisit, ReferredBy, RelatedVisit, StatisticsId)
		VALUES (@dateTime, @patientId, @visitReason, @refferal, @related, @statisticsId)
	INSERT INTO PatientsAndRooms (PatientId, RoomId)
		VALUES (@patientId, 100) --this is temporary
	EXEC dbo.MovePatient @patientId, @roomId;
SELECT * FROM Person;
SELECT * FROM Address;
SELECT * FROM PhoneNumber;
SELECT * FROM AddressAndPeople;
SELECT * FROM PatientStatistics;
SELECT * FROM PatientsAndRooms;
SELECT * FROM Room;
--Function to calculate total cost for a given period of time for a patient
CREATE FUNCTION dbo.CalculateTotalCost (@dateStart AS DATETIME, @dateEnd AS DATETIME, @patientId AS INT) 
RETURNS DECIMAL(10, 2) AS 
	BEGIN
	DECLARE @totalCost DECIMAL(10, 2)
	DECLARE @dateSt DATETIME
	DECLARE @dateEd DATETIME
	DECLARE @tempTable TABLE (VisitId INT, DateTimeIn DATETIME)
	SELECT @totalCost = 0
	INSERT INTO @tempTable
		SELECT VisitId, DateTimeIn 
			FROM PatientVisit
			WHERE PatientId = @patientId
				AND MONTH(DateTimeIn) >= MONTH(@dateStart)
				AND MONTH(DateTimeOut) <= MONTH(@dateEnd)
				AND YEAR(DateTimeIn) >= YEAR(@dateStart)
				AND YEAR(DateTimeOut) <= YEAR(@dateEnd)
				AND DAY(DateTimeIn) >= DAY(@dateStart)
				AND DAY(DateTimeOut) <= DaY(@dateEnd)
		SELECT @dateSt = (SELECT TOP 1 DateTimeIn FROM @tempTable ORDER BY DateTimeIn ASC)
		SELECT @dateEd = (SELECT TOP 1 DateTimeIn FROM @tempTable ORDER BY DateTimeIn DESC)
		IF ((@dateSt <= @dateEnd) AND (@dateEd >= @dateStart))
			BEGIN
				SELECT	@totalCost = (SELECT SUM(R.TotalCost)
					FROM @tempTable A
					INNER JOIN TransactionAndVisit T
					ON T.VisitId = A.VisitId
					INNER JOIN Transactions R
					ON T.TransactionId = R.TransactionId)
			END
		RETURN @totalCost
	END;
DROP FUNCTION dbo.CalculateTotalCost;
DECLARE @cost DECIMAL(10, 2);
DECLARE @id INTEGER;
DECLARE @start DATETIME;
DECLARE @end DATETIME;
SELECT @id = 1;
SELECT @start = '2009/04/10 00:00:00';
SELECT @end = '2009/04/13 06:00:00';
SELECT @cost = dbo.CalculateTotalCost(@start, @end, @id);
PRINT @cost;
