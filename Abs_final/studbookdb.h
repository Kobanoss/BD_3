#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "./bookdb.h"
#include "./studdb.h"
#include "./users.h"
#include "./logs.h"











typedef struct studbookbox{
	char isbn[15];		 // ISBN
	char nu[8]; 		 // Nu зачетки
	 int day; 		 // Дата сдачи
	 int month; 		 // Месяц сдачи
	 int year; 		 // Год сдачи
} studbookbox;

//---------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------Работа с students_books.csv-------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------

studbookbox* loadstudb(char* studb, int* l) { 						// загрузка взятых книг из файла

	FILE* filestudb = fopen(studb, "r");
	
	if (filestudb == 0) { 								// если не удалось открыть файл

		printf("Не удалось открыть файл '%s'\n", studb);
		*l = 0;
		return NULL; 								// возвращаем пустой указатель
	}

	studbookbox* studbk = (studbookbox*)malloc(sizeof(studbookbox)); 			// выделяем память под один элемент
	*l = 0; 									// изначально элементов нет
	
	while (feof(filestudb) == 0) {

		fscanf(filestudb, "%[^;]%*c", studbk[*l].isbn);
		fscanf(filestudb, "%[^;]%*c;", studbk[*l].nu);
		fscanf(filestudb, "%d%*c", &studbk[*l].day);
		fscanf(filestudb, "%d%*c", &studbk[*l].month);
		fscanf(filestudb, "%d%*c", &studbk[*l].year);

		*l = *l + 1; 								// увеличиваем размер
		studbk = (studbookbox*)realloc(studbk, (*l + 1) * sizeof(studbookbox)); 	// перераспределяем память
	}

	fclose(filestudb);
	return studbk; 									// возвращаем считанный массив
}

void savestudb(char* studb, studbookbox* studbk, int l) { 					// сохранение взятых книг в файл

	FILE* filestudb = fopen(studb, "w");

	for (int i = 0; i < l; i++) {

		fprintf(filestudb, "%s;", studbk[i].isbn);
		fprintf(filestudb, "%s;", studbk[i].nu);
		fprintf(filestudb, "%d.", studbk[i].day);
		fprintf(filestudb, "%d.", studbk[i].month);
		fprintf(filestudb, "%d", studbk[i].year);

		if (i < l - 1) {

			fprintf(filestudb, "\n");
		}
	}

	fclose(filestudb);
}

