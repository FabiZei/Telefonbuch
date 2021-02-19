install:
	gcc -g ./main.c ./SQLIntegration.c ./libs/sqlite3.c -o ./Telefonbuch.exe -I ./libs/