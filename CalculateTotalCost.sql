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