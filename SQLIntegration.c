#include "SQLIntegration.h"

struct Personen personausSQL(int index)
{
	sqlite3 *db;
	char *err_msg = 0;
	sqlite3_stmt *res;
	sqlite3_stmt *res2;
	const char *ausgabe;
	
	struct Personen p;
	
	int rc = sqlite3_open("Test.db", &db); // Verbindung mit DB  !!!!!funktioniert nicht richitg
	
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        p.id = -1;
        return p;
	}
	
	char *sql = "Select Vorname, Nachname, date(Geburtsdatum), Ortsname, Strassenname, Hausnummer, PLZ, Vorwahl, Personen.PID from Personen, Adresse, Telefonnummer where Adresse.AID = Personen.AID and Personen.PID = ? LIMIT 1;";
	
	rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
	
	if (rc == SQLITE_OK) {
		sqlite3_bind_int(res, 1, index);
	} else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		
		p.id = -1;        
		return p;
	}
	
	int step = sqlite3_step(res);
	
	if (step == SQLITE_ROW) {   
		
		 strcpy(p.vorname, sqlite3_column_text(res, 0));
		 
		 strcpy(p.nachname, sqlite3_column_text(res, 1));
		 
		 strcpy(p.geburtsdatum, sqlite3_column_text(res, 2));
		
		 strcpy(p.adresse.ortsname, sqlite3_column_text(res, 3));
		 
		 strcpy(p.adresse.strassenname, sqlite3_column_text(res, 4));
		 
		 p.adresse.plz = sqlite3_column_int(res, 6);
		 
		 p.adresse.hausnummer = sqlite3_column_int(res, 5);
		 
		 p.tnummer.vorwahl = sqlite3_column_int(res, 7);
		 
		 p.id = sqlite3_column_int(res, 8);
		 //printf("%d	", p.id); 
		 
	} else {
		//printf("Fehlgeschlagen!\n");
		p.id = -1;
		return p;
	}
	
	char *sql2 = "select Nummer, Personen.PID from Telefonnummer, Personen where Personen.PID = Telefonnummer.PID and Telefonnummer.PID = ? limit 1;";
	
	rc = sqlite3_prepare_v2(db, sql2, -1, &res2, 0);
	
	if (rc == SQLITE_OK) {
		
		sqlite3_bind_int(res2, 1, index);
		
	} else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		
		p.tnummer.nummer = -1;        
		return p;
	}
	
	int step2 = sqlite3_step(res2);
	//step2 = sqlite3_step(res2);
	
	if (step2 == SQLITE_ROW) {   
	
		p.tnummer.nummer = sqlite3_column_int(res2, 0); 
		
	} else {
		p.tnummer.nummer = -1;        
		return p;
	}
	
	//printf("%d", p.id);
	
	
	
	sqlite3_finalize(res);
	sqlite3_finalize(res2);
	sqlite3_close(db);
	
	return p;
}

int getColumns() //gibt Zeilenanzahl der Personentabelle zur�ck
{
	sqlite3 *db;
	char *err_msg = 0;
	sqlite3_stmt *res;
	int counter;
	
	int rc = sqlite3_open("Test.db", &db);
	
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		
		return -1;
	}
	
	char *sql = ("select count(*) from Personen;");
	
	rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
	
	if (rc == SQLITE_OK) {
		sqlite3_bind_int(res, 1, 1);
	} else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}
	
	int step = sqlite3_step(res);
	
	if (step == SQLITE_ROW){
		counter = sqlite3_column_int(res, 0);
	}
	
	sqlite3_finalize(res);
	sqlite3_close(db);
	
	return counter;
}

void fillArraywithData(struct Personen *feld)
{
	for (int i = 0; i < 6; i++) {
		feld[i] = personausSQL(i+1);
	}
	
	
	//printf("%d\n", feld[1].id);
}

int scrollArray(int richtung, struct Personen *feld)  //richtung: 1 nach unten/ -1 nach oben
{
	int scrollLock = getColumns();
	int currentIndex = feld[0].id;
	
	if (richtung == -1) {
		if ((feld[0].id + richtung * 6) < 0) {
			
			feld[0] = personausSQL(1);
			feld[1] = personausSQL(2);
			feld[2] = personausSQL(3);
			feld[3] = personausSQL(4);
			feld[4] = personausSQL(5);
			feld[5] = personausSQL(6);
			
		} else {
			for (int i = 0; i < 6; i++) {
				feld[i] = personausSQL(currentIndex - 6 + i);
			}
		}
	}
	
	if (richtung == 1) {
		if ((feld[5].id + 6) >= scrollLock) {
			
			feld[0] = personausSQL(scrollLock - 5);
			feld[1] = personausSQL(scrollLock - 4);
			feld[2] = personausSQL(scrollLock - 3);
			feld[3] = personausSQL(scrollLock - 2);
			feld[4] = personausSQL(scrollLock - 1);
			feld[5] = personausSQL(scrollLock);
			
		} else {
			for (int i = 0; i < 6; i++) {
				feld[i] = personausSQL(currentIndex + 6 + i);
			}
		}
	}
	
	if ((richtung != 1)&&(richtung != -1)) {
		printf("Bitte als Richtung nur -1 oder 1 angeben -.-");
	}
	

	
	return 0;
}

int getIDArrayLength(int suchanfrage, char suchanfrage_char[20]) //gibt die Zeilenzahl der Ergebnisse einer Suche an
{
	
	sqlite3 *db;
	char *err_msg = 0;
	sqlite3_stmt *res;
	int counterReturns;
	
	int rc = sqlite3_open("Test.db", &db);
	
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	
		return 0;
	} 
	
	if (suchanfrage == 1) {
		
		char *sql = "select count(distinct Personen.PID) from Personen, Adresse, Telefonnummer where Personen.AID = Adresse.AID and Personen.PID = Telefonnummer.PID and Personen.Vorname Like ?;";
		
		rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
		
		if (rc == SQLITE_OK) {
			rc = sqlite3_bind_text(res, 1, suchanfrage_char, -1, 0);
		} else {
			fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		}
		
		if (sqlite3_step(res) == SQLITE_ROW) {
			counterReturns = sqlite3_column_int(res, 0);
		}
		
		sqlite3_finalize(res);
		//sqlite3_close(db);
		
		return counterReturns;
	}
	
	if (suchanfrage == 2) {
		
		char *sql = "select count(distinct Personen.PID) from Personen, Adresse, Telefonnummer where Personen.AID = Adresse.AID and Personen.PID = Telefonnummer.PID and Personen.Nachname Like ?;";
		
		rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
		
		if (rc == SQLITE_OK) {
			rc = sqlite3_bind_text(res, 1, suchanfrage_char, -1, 0);
		} else {
			fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		}
		
		if (sqlite3_step(res) == SQLITE_ROW) {
			counterReturns = sqlite3_column_int(res, 0);
		}
		
		sqlite3_finalize(res);
		//sqlite3_close(db);
		
		return counterReturns;
	}
	
	if (suchanfrage == 3) {
		
		char *sql = "select count(distinct Telefonnummer.TID) from Personen, Adresse, Telefonnummer where Personen.AID = Adresse.AID and Personen.PID = Telefonnummer.PID and Telefonnummer.nummer like ?;";
		
		rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
		
		if (rc == SQLITE_OK) {
			rc = sqlite3_bind_text(res, 1, suchanfrage_char, -1, 0);
		} else {
			fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		}
		
		if (sqlite3_step(res) == SQLITE_ROW) {
			counterReturns = sqlite3_column_int(res, 0);
		}
		
		sqlite3_finalize(res);
		//sqlite3_close(db);
		
		return counterReturns;
	}
}

void getIDArray(int suchanfrage, char suchanfrage_char[20], int* feld) // 1 f�r Vornamen, 2 f�r Nachnamen, 3 f�r Telefonnummern
{ 
	sqlite3 *db;
	char *err_msg = 0;
	sqlite3_stmt *res;
	char *sql;
	
	
	int rc = sqlite3_open("Test.db", &db);
	
	if (rc != SQLITE_OK) {
		
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	
	}
	
		
	if (suchanfrage == 1) {
		
		sql = "select distinct Personen.PID from Personen, Adresse, Telefonnummer where Personen.AID = Adresse.AID and Personen.PID = Telefonnummer.PID and Personen.Vorname Like ?;";
		
	} 
	
	if (suchanfrage == 2) {
		
		sql = "select distinct Personen.PID from Personen, Adresse, Telefonnummer where Personen.AID = Adresse.AID and Personen.PID = Telefonnummer.PID and Personen.Nachname Like ?;";
		
	} 
	
	if (suchanfrage == 3) {
		
		sql = "select Personen.PID from Personen, Adresse, Telefonnummer where Personen.AID = Adresse.AID and Personen.PID = Telefonnummer.PID and Telefonnummer.Nummer Like ?;";
		
	} 
	
	
	rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
		
	if (rc == SQLITE_OK) {
		rc = sqlite3_bind_text(res, 1, suchanfrage_char, -1, 0);
		
	} else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}
	
	int step = sqlite3_step(res);
	
		
	int i = 0;
	while (step == SQLITE_ROW) {
		feld[i] = sqlite3_column_int(res, 0);
		i++;
		step = sqlite3_step(res);
	}	
		
	sqlite3_finalize(res);
	sqlite3_close(db);
}

void fillArraywithDatav2(struct Personen *feld, int idArrayLength, int *idArray)
{
	if (idArrayLength >= 6) {
		for (int i = 0; i < 6; i++){
			feld[i] = personausSQL(idArray[i]);
		}
	} else {
		for (int i = 0; i < idArrayLength; i++) {
			feld[i] = personausSQL(idArray[i]);
		}
		
		for (int i = 6; i >= idArrayLength; i--) {
			feld[i].id = -1;
		}
	}
	
}

void scrollThroughIDArray(int richtung, struct Personen *feld, int idArrayLength, int *idArray) 
{
	int currentidArrayIndex = findIndex(idArray, idArrayLength, idArray[0]);
	
	if ((richtung != 1)&&(richtung != -1)) {
		printf("Bitte als Richtung nur -1 oder 1 angeben -.-");
	}
	
	if (idArrayLength > 6) {
			if (richtung == -1) {
				if ((currentidArrayIndex + richtung * 6) < 0) {
					
					feld[0] = personausSQL(idArray[0]);
					feld[1] = personausSQL(idArray[1]);
					feld[2] = personausSQL(idArray[2]);
					feld[3] = personausSQL(idArray[3]);
					feld[4] = personausSQL(idArray[4]);
					feld[5] = personausSQL(idArray[5]);
					
				} else {
					for (int i = 0; i < 6; i++) {
						feld[i] = personausSQL(idArray[currentidArrayIndex - 6 + i]);
					}
				}
			}
			
			if (richtung == 1) {
				if ((findIndex(idArray, idArrayLength, feld[5].id) + 6) >= idArrayLength) {
					
					feld[0] = personausSQL(idArray[idArrayLength -6]);
					feld[1] = personausSQL(idArray[idArrayLength -5]);
					feld[2] = personausSQL(idArray[idArrayLength -4]);
					feld[3] = personausSQL(idArray[idArrayLength -3]);
					feld[4] = personausSQL(idArray[idArrayLength -2]);
					feld[5] = personausSQL(idArray[idArrayLength -1]);
					
				} else {
					for (int i = 0; i < 6; i++) {
						feld[i] = personausSQL(idArray[currentidArrayIndex + 6 + i]);
					}
				}
			}
	}

}




int findIndex(int *feld, int size, int target)
{
	int i = 0;
	
	while((i < size) && (feld[i] != target)){
		i++;
	}
	
	return i;
}
