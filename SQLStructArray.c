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
		 printf("%d	", p.id); 
		 
	} else {
		printf("Fehlgeschlagen!\n");
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
		printf("%s \n", feld[i].adresse.ortsname);
	}
	
	
	//feld[2] = personausSQL(2);
	//printf("%d", feld[2].id);
}

int main()
{
	
	struct Personen array[6];
	//struct Personen jalla;
	//jalla = personausSQL(12);
	
	//struct Personen parray;
	//parray = array;
	
	fillArraywithData(array);
	
	
	//printf("%d", jalla.tnummer.vorwahl);
	//printf("	%d", getColumns());

	//printf("%s\n", pers.vorname);

	
	return 0;
}
