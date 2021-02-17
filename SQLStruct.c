#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <windows.h>

static int callback(void *, int, char **, char **);
int structFromSQL();


struct Adresse{
	char ortsname[40];
	char strassenname[40];
	char plz[6];
	char hausnummer[3];
} ;

struct Telefonnummer{
	char vorwahl[7];
	char nummer[7];
};

struct Personen{
	char vorname[20];
    char nachname[20];
   	struct Adresse adresse;
    struct Telefonnummer tnummer;
    char geburtsdatum[10];
};

struct Personen p1;


int structFromSQL()
{
	sqlite3 *db;
	char *err_msg = 0;
	sqlite3_stmt *res;
	const char *ausgabe;
	
	int rc = sqlite3_open("Test.db", &db); // Verbindung mit DB
	
	if (rc != SQLITE_OK) { // Try Catch :)
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
		return 1;
	}
	
	char *sql = "Select Vorname, Nachname, Geburtsdatum, Ortsname, Strassenname, Hausnummer, PLZ, Vorwahl, Nummer from Personen, Adresse, Telefonnummer where Adresse.AID = Personen.AID and Personen.PID = Telefonnummer.PID and Personen.PID = ?;";
	
	rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
	
	if (rc == SQLITE_OK) {
		sqlite3_bind_int(res, 1, 1);
	} else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}
	
	int step = sqlite3_step(res);
	
	if (step == SQLITE_ROW) {
		
		 strcpy(p1.vorname, sqlite3_column_text(res, 0));
		 strcpy(p1.nachname, sqlite3_column_text(res, 1));
		 strcpy(p1.geburtsdatum, sqlite3_column_text(res, 2));
		 strcpy(p1.adresse.ortsname, sqlite3_column_text(res, 3));
		 strcpy(p1.adresse.strassenname, sqlite3_column_text(res, 4));
		 strcpy(p1.adresse.plz, sqlite3_column_text(res, 6));
		 strcpy(p1.adresse.hausnummer, sqlite3_column_text(res, 5));
		 strcpy(p1.tnummer.nummer, sqlite3_column_text(res, 7));
		 strcpy(p1.tnummer.vorwahl, sqlite3_column_text(res, 8));
		 
		 
		//p1.Nachname = sqlite3_column_text(res, 1);
		//printf("%s\n", sqlite3_column_text(res, 1));
	}
	
	sqlite3_finalize(res);
	sqlite3_close(db);
	
	return 0;
}



int main()
{
	structFromSQL();
	
	printf("%s", p1.vorname);
	printf("%s", p1.adresse.ortsname);
	printf("%s", p1.tnummer.vorwahl);
	
	return 0;
}
