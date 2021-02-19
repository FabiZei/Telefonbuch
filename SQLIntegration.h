#ifndef sqlintegration_NEVER_USE
#define sqlintegration_NEVER_USE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <windows.h>

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

struct Personen personausSQL(int index);

int getColumns();

void fillArraywithData(struct Personen *feld);

int scrollArray(int richtung, struct Personen *feld);

int getIDArrayLength(int suchanfrage, char suchanfrage_char[20]);

void getIDArray(int suchanfrage, char suchanfrage_char[20], int* feld);

void fillArraywithDatav2(struct Personen *feld, int idArrayLength, int *idArray);

void scrollThroughIDArray(int richtung, struct Personen *feld, int idArrayLength, int *idArray);

int findIndex(int *feld, int size, int target);

#endif
