#include "SQLIntegration.h"
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

void resultmenue();
void showgreeter();
int greetermenu();
int changemenue(int);
int searchmenue();
void titleprint(char *);
void writerow(int, char *, char *, int, int);
void printlistui();

int nextmenue = 0; // 0= greeter, 1 = search 2 = add 3 = list -1 = exit
char eingabe[10];
int counter = 1;

struct Personen persons[6];
struct Personen workingperson;

void showgreeter()
{
    int eintraege = getColumns();
    system("cls");
    titleprint("Telefonbuch");
    printf("\n");
    printf_s("Willkommen im Telefonbuch. Sie haben bereits %i Eintr\x84ge\n", eintraege);
    printf_s("(s)earch | (l)ist | (e)xit:");
}
int greetermenu()
{
    while (1)
    {
        showgreeter();
        scanf("%s", eingabe);
        switch (eingabe[0])
        {
        case 's':
            return 1;
        case 'a':
            return 2;
        case 'l':
            return 3;
        case 'e':
            return -1;
        }
    }
}
void showsearch()
{
    system("cls");
    titleprint("Suche");
    printf("\n");
    printf_s("Nach was soll gesucht werden?\n");
    printf_s("(v)orname | (n)achname| (t)elefonnummer | (b)ack:");
}

int searchmenue()
{
    char suchterm[20] = "%"; // prevent segfault und
    char temp[99];

    while (1)
    {
        strcpy(suchterm, "%");
        strcpy(temp, "");
        showsearch();
        scanf("%s", eingabe);
        switch (eingabe[0])
        {
        case 'b':
            return 0;
        case 'v':
            printf_s("Nach welchen Vorname soll gesucht werden:");
            scanf("%s", temp);
            strcpy(suchterm, strcat(suchterm, temp));
            strcpy(suchterm, strcat(suchterm, "%"));
            int vlen = getIDArrayLength(1, suchterm);
            int *varrayids = (int *)malloc(vlen);
            getIDArray(1, suchterm, varrayids);
            fillArraywithDatav2(persons, vlen, varrayids);
            resultmenue(temp, 1, varrayids, suchterm);
            break;
        case 'n':
            printf_s("Nach welchen Nachnamen soll gesucht werden:");
            scanf("%s", temp);
            strcpy(suchterm, strcat(suchterm, temp));
            strcpy(suchterm, strcat(suchterm, "%"));
            int nlen = getIDArrayLength(2, suchterm);
            int *narrayids = (int *)malloc(nlen);
            getIDArray(2, suchterm, narrayids);
            fillArraywithDatav2(persons, nlen, narrayids);
            resultmenue(temp, 2, narrayids, suchterm);
            break;
        case 't':
            printf_s("Nach welcher Telefonnummer soll gesucht werden:");
            scanf("%s", temp);
            strcpy(suchterm, strcat(suchterm, temp));
            strcpy(suchterm, strcat(suchterm, "%"));
            int tlen = getIDArrayLength(3, suchterm);
            int *tarrayids = (int *)malloc(tlen);
            getIDArray(3, suchterm, tarrayids);
            fillArraywithDatav2(persons, tlen, tarrayids);
            resultmenue(temp, 3, tarrayids, suchterm);
            break;
        case 'e':
            return -1;
        }
    }
}
void showsearchresult(char *suche, int suchtyp, char *fullsearch)
{

    system("cls");
    titleprint("Suchergebnis");
    printf("\n");
    printf_s("Sie haben nach %s gesucht\n", suche);
    printf("\n");
    printf("Zeige Eintrag %d - %d von %d\n", counter, counter + 5, getIDArrayLength(suchtyp, fullsearch));
    printf_s("\n");
    if (persons[0].id == -1)
    {
        printf("Es wurden keine Ergebnisse gefunden\n");
    }
    else
    {
        printlistui();
    }
    printf("\n");
    printf("(u)p | (d)own | 1-6 = view entry | (b)ack:");
}
void resultmenue(char *suche, int suchtyp, int *arrayids, char *fullsearch)
{
    workingperson = persons[0];
    counter = 1;
    while (1)
    {
        showsearchresult(suche, suchtyp, fullsearch);
        scanf("%s", eingabe);
        switch (eingabe[0])
        {
        case 'b':
            return;
        case 'd':
            scrollThroughIDArray(1, persons, getIDArrayLength(suchtyp, fullsearch), arrayids);
            counter = counter + 6;
            if (counter > getIDArrayLength(suchtyp, fullsearch), arrayids)
            {
                counter = getIDArrayLength(suchtyp, fullsearch) - 5;
                if (counter < 1)
                {
                    counter = 1;
                }
            }
            break;
        case 'u':
            scrollThroughIDArray(-1, persons, getIDArrayLength(suchtyp, fullsearch), arrayids);
            counter = counter - 6;
            if (counter < 1)
            {
                counter = 1;
            }
            break;
        case '1':
            workingperson = persons[0];
            break;
        case '2':
            if (persons[1].id == -1)
            {
                break;
            }
            workingperson = persons[1];
            break;
        case '3':
            if (persons[2].id == -1)
            {
                break;
            }
            workingperson = persons[2];
            break;
        case '4':
            if (persons[3].id == -1)
            {
                break;
            }
            workingperson = persons[3];
            break;
        case '5':
            if (persons[4].id == -1)
            {
                break;
            }
            workingperson = persons[4];
            break;
        case '6':
            if (persons[5].id == -1)
            {
                break;
            }
            workingperson = persons[5];
            break;
        }
    }
}
void titleprint(char *titletext) // Titel auf 120 zeichen normieren und centern
{
    int lentitle = strlen(titletext);
    int divtitle = lentitle / 2;
    int modulotitle = lentitle % 2;
    int spaceleft = 59 - divtitle;
    int spaceright = 59 - modulotitle - divtitle;
    int i;
    for (i = 0; i < spaceleft; i++)
    {
        printf("#");
    }
    printf(" ");
    printf(titletext);
    printf(" ");
    for (i = 0; i < spaceright; i++)
    {
        printf("#");
    }
    printf("\n");
}
void showlist()
{
    system("cls");
    titleprint("Liste");
    printf("\n");
    printf("Zeige Eintrag %d - %d von %d", counter, counter + 5, getColumns());
    printf("\n");
    printlistui();
    printf("\n");
    printf("(u)p | (d)own | 1-6 = view entry | (b)ack | (e)xit: ");
}
int listmenue()
{
    fillArraywithData(persons);
    workingperson = persons[0];
    counter = 1;
    while (1)
    {
        showlist();
        scanf("%s", eingabe);
        switch (eingabe[0])
        {
        case 'b':
            return 0;
        case 'd':
            scrollArray(1, persons);
            counter = counter + 6;
            if (counter > getColumns() - 6)
            {
                counter = getColumns() - 5;
            }
            break;
        case 'u':
            scrollArray(-1, persons);
            counter = counter - 6;
            if (counter < 1)
            {
                counter = 1;
            }
            break;
        case '1':
            workingperson = persons[0];
            break;
        case '2':
            workingperson = persons[1];
            break;
        case '3':
            workingperson = persons[2];
            break;
        case '4':
            workingperson = persons[3];
            break;
        case '5':
            workingperson = persons[4];
            break;
        case '6':
            workingperson = persons[5];
            break;
        }
    }
}
void printlistui()
{
    //header
    int i;
    int headerpaddingleft = 8;
    int headerpaddingmid = 46;
    for (i = 0; i <= headerpaddingleft; i++)
    {
        printf(" ");
    }
    printf("Adressbuch");
    for (i = 0; i <= headerpaddingmid; i++)
    {
        printf(" ");
    }
    printf(" Aktuell ausgew\x84hlte Person\n");
    printf("\n");
    writerow(1, "Vorname", workingperson.vorname, -2, -2);
    writerow(2, "Nachname", workingperson.nachname, -2, -2);
    writerow(3, "Adresse", workingperson.adresse.strassenname, workingperson.adresse.hausnummer, -2);
    writerow(4, "Wohnort", workingperson.adresse.ortsname, workingperson.adresse.plz, -2);
    writerow(5, "Telefonnummer", "NONE", workingperson.tnummer.vorwahl, workingperson.tnummer.nummer);
    writerow(6, "Geburtsdatum", workingperson.geburtsdatum, -2, -2);
}
void writerow(int num, char *pafterpipe, char *pafterval, int intafter1, int intafter2)
// overloading/ safe typecasting isn't a thing so more parameters
//120 Zeichen pipe bei 60 links Vorname Nachname rechts Details working set
{
    int i;
    int paddingleft = 5;
    int written = 0;
    int paddingtomid; //60 - written
    int paddingafterpipe = 3;
    int namelen; // len vorname + len nachname + 4 für "i. v n"
    //kein padding rechts
    if (persons[num - 1].id != -1)
    {
        namelen = strlen(persons[num - 1].vorname) + strlen(persons[num - 1].nachname) + 4; //
        written = paddingleft + namelen;
        paddingtomid = 60 - written;
        for (i = 0; i <= paddingleft; i++)
        {
            printf(" ");
        }
        // padding left
        printf("%i. %s %s", num, persons[num - 1].vorname, persons[num - 1].nachname);
        // num name
        for (i = 0; i <= paddingtomid; i++)
        {
            printf(" ");
        }
    }
    else
    {
        for (i = 0; i < 62; i++)
        {
            printf(" ");
        }
    }

    printf("|");
    for (i = 0; i <= paddingafterpipe; i++)
    {
        printf(" ");
    }
    //padding after pipe
    printf(pafterpipe);
    printf(": ");
    if (pafterval != "NONE")
    {
        //print char *
        printf("%s ", pafterval);
    }
    if (intafter2 == -1)
    {
        printf("keine\n");
        return;
    }
    if (intafter1 > 0 && intafter2 > 0)
    {
        printf("0%d ", intafter1);
    }
    else
    {
        if (intafter1 != -2)
        {
            printf("%d ", intafter1);
        }
    }
    if (intafter2 != -2)
    {
        printf("%d", intafter2);
    }
    printf("\n");
}

int changemenue(int menue)
{
    int next = 0;
    switch (menue)
    {
    case 0:
        next = greetermenu();
        break;
    case 1:
        next = searchmenue();
        break;
    case 3:
        next = listmenue();
        counter = 0;
        break;
    }
    return next;
}
int main()
{

    while (nextmenue != -1)
    {
        nextmenue = changemenue(nextmenue);
    }
    getchar();
    return 0;
}