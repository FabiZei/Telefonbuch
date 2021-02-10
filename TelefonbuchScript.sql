CREATE TABLE Personen (
	PID INTEGER PRIMARY KEY AUTOINCREMENT,
	Vorname varchar(255),
	Nachname varchar(255),
	Geburtsdatum DATE,
	AID int
	);
	
CREATE TABLE Adresse(
	AID INTEGER PRIMARY KEY AUTOINCREMENT,
	Ortsname varchar(255),
	Strassenname varchar(255),
	Hausnummer  int,
	PLZ int,
	Vorwahl int
	);

CREATE TABLE Telefonnummer(
	TID INTEGER PRIMARY KEY AUTOINCREMENT,
	Nummer int,
	PID int
	);

INSERT INTO Personen ('Vorname', 'Nachname', 'Geburtsdatum', 'AID') VALUES ('Amy', 'Forrest', date('1984-12-09'), 1);
INSERT INTO Personen ('Vorname', 'Nachname', 'Geburtsdatum', 'AID') VALUES ('Amanda', 'Lowe', date('1997-04-28'), 2);
INSERT INTO Personen ('Vorname', 'Nachname', 'Geburtsdatum', 'AID') VALUES ('Heather', 'Terry', date('1965-06-03'), 1);

INSERT INTO Adresse ('Ortsname', 'Strassenname', 'Hausnummer', 'PLZ', 'Vorwahl') VALUES ('Leisnig', 'Zollschwitz', 12, 04703, 034321);
INSERT INTO Adresse ('Ortsname', 'Strassenname', 'Hausnummer', 'PLZ', 'Vorwahl') VALUES ('Neustadt am Rübenberge', 'Landwehr', 68, 31535, 05032);

INSERT INTO Telefonnummer ('Nummer', 'PID') VALUES (678901, 1);
INSERT INTO Telefonnummer ('Nummer', 'PID') VALUES (1337420, 2);
INSERT INTO Telefonnummer ('Nummer', 'PID') VALUES (3141, 2);	
	