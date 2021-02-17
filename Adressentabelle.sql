INSERT INTO Adresse ('Ortsname', 'Strassenname', 'Hausnummer', 'PLZ', 'Vorwahl') VALUES ('Leisnig', 'Zollschwitz', 12, 04703, 034321);
INSERT INTO Adresse ('Ortsname', 'Strassenname', 'Hausnummer', 'PLZ', 'Vorwahl') VALUES ('Neustadt am Rübenberge', 'Landwehr', 68, 31535, 05032);
INSERT INTO Adresse ('Ortsname', 'Strassenname', 'Hausnummer', 'PLZ', 'Vorwahl') VALUES ('Aachen', 'Straße der Pariser Kommune', 45, 52078, 0241);
INSERT INTO Adresse ('Ortsname', 'Strassenname', 'Hausnummer', 'PLZ', 'Vorwahl') VALUES ('Karby', 'Hochstrasse', 82, 24396, 04644);
INSERT INTO Adresse ('Ortsname', 'Strassenname', 'Hausnummer', 'PLZ', 'Vorwahl') VALUES ('Camburg', 'Koenigstrasse', 91, 07771, 036421);
INSERT INTO Adresse ('Ortsname', 'Strassenname', 'Hausnummer', 'PLZ', 'Vorwahl') VALUES ('Einöllen', 'Grosse Praesidenten Str.', 14, 67753, 06304);
INSERT INTO Adresse ('Ortsname', 'Strassenname', 'Hausnummer', 'PLZ', 'Vorwahl') VALUES ('Stuttgart Flughafen', 'Ollenhauer Str.', 7, 70629, 0711);
INSERT INTO Adresse ('Ortsname', 'Strassenname', 'Hausnummer', 'PLZ', 'Vorwahl') VALUES ('Hamburg Barmbek-Süd', 'Waßmannsdorfer Chaussee', 63, 22081, 040);
INSERT INTO Adresse ('Ortsname', 'Strassenname', 'Hausnummer', 'PLZ', 'Vorwahl') VALUES ('Willich', 'Fischerinsel', 66, 24396, 04644);
INSERT INTO Adresse ('Ortsname', 'Strassenname', 'Hausnummer', 'PLZ', 'Vorwahl') VALUES ('Bad Wilsnack', 'An der Alster', 75, 19334, 038791);
INSERT INTO Adresse ('Ortsname', 'Strassenname', 'Hausnummer', 'PLZ', 'Vorwahl') VALUES ('Hausach', 'Bleibtreustraße', 20, 24396, 04644);

UPDATE Personen SET AID = ABS(RANDOM() %(select count(*) from Adresse))+ 1;


