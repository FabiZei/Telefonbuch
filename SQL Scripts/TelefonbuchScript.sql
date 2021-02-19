CREATE TABLE Personen (
	PID INTEGER PRIMARY KEY AUTOINCREMENT,
	Vorname varchar(20),
	Nachname varchar(20),
	Geburtsdatum DATE,
	AID int
	);
	
CREATE TABLE Adresse(
	AID INTEGER PRIMARY KEY AUTOINCREMENT,
	Ortsname varchar(40),
	Strassenname varchar(40),
	Hausnummer  int,
	PLZ int,
	Vorwahl int
	);

CREATE TABLE Telefonnummer(
	TID INTEGER PRIMARY KEY AUTOINCREMENT,
	Nummer int,
	PID int
	);

