#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <windows.h>

static int callback(void *, int, char **, char **);
struct Personen personausSQL(int index);

int columns;
int *pcolumns = &columns;

struct Adresse{
	char ortsname[40];
	char strassenname[40];
	int plz;
	int hausnummer;
} ;

struct Telefonnummer{
	int vorwahl;
	int nummer;
};

struct Personen{
	int id;
	char vorname[20];
    char nachname[20];
   	struct Adresse adresse;
    struct Telefonnummer tnummer;
    char geburtsdatum[10];
};


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
	
	char *sql = "Select Vorname, Nachname, Geburtsdatum, Ortsname, Strassenname, Hausnummer, PLZ, Vorwahl, Personen.PID from Personen, Adresse, Telefonnummer where Adresse.AID = Personen.AID and Personen.PID = ? LIMIT 1;";
	
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
	
	char *sql2 = "select Personen.PID, Nummer from Telefonnummer, Personen where Personen.PID = Telefonnummer.PID and Personen.PID = ? limit 1;";
	
	rc = sqlite3_prepare_v2(db, sql2, -1, &res2, 0);
	
	if (rc == SQLITE_OK) {
		sqlite3_bind_int(res2, 1, index);
	} else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		
		p.tnummer.nummer = -1;        
		return p;
	}
	
	int step2 = sqlite3_step(res2);
	
	if (step2 == SQLITE_ROW) {   
		
		p.tnummer.nummer = sqlite3_column_int(res, 0);
		
	} else {
		//printf("Laden der Daten fehlgeschlagen!\n");
		
		p.tnummer.nummer = -1;        
		return p;
	}
	
	//printf("%d", p.id);
	
	
	
	sqlite3_finalize(res);
	sqlite3_finalize(res2);
	sqlite3_close(db);
	
	return p;
}

int getColumns() //gibt Zeilenanzahl der Personentabelle zurück
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

int scrollArray(int richtung, struct Personen *feld)  //richtung: 1 nach 'unten'/ -1 nach open
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

int main()
{
	
	struct Personen array[6];
	
	
	
	fillArraywithData(array);
	
	//printf("%d", array[1].id);
	
	
	for (int i = 0; i < 100; i ++) {
		scrollArray(1, array);
		printf(("%d\n"),array[5].id);
	}
	
	for (int i = 0; i < 100; i ++) {
		scrollArray(-1, array);
		printf(("%d\n"),array[5].id);
	}
	

	
	return 0;
}
