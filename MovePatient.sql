--This moves a patient to a different room
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