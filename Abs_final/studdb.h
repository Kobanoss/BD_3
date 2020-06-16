#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "./bookdb.h"
#include "./studbookdb.h"
#include "./users.h"
#include "./logs.h"


typedef struct studbox {

	char nu[8];		 // Nu зачетки  
	char surname[20];        // Фамилия
	char name[20];		 // Имя
	char fathername[20];	 // Отчество
	char faculty[4];	 // Факультет
	char spec[60];		 // Спеца
} studbox;

//---------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------Функционал для студенческой БД-----------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------Ввод студента------------------------------------------------------------


void printstud(studbox student) { 					 	// вывод студента
	
	printf("| %-9s ", student.nu);
	printf("| %-31s ", student.surname);
	printf("| %-15s ", student.name);
	printf("| %-24s ", student.fathername);
	printf("| %-9s ", student.faculty);
	printf("| %-30s ", student.spec);
	printf("|\n");
}

void printstudbox(studbox* student, int m) {				 	// вывод студентов

	if (m == 0) { 								// если ещё нет студентов, сообщаем об этом

		printf("Ни одного студента еще не добавлено\n");
		return;
	}

	printf("|   Номер   |             Фамилия             |       Имя       |         Отчество         | Факультет |         Специальность          |\n");
	printf("|-----------|---------------------------------|-----------------|--------------------------|-----------|--------------------------------|\n");

	for (int i = 0; i < m; i++) {

		 printstud(student[i]);						//вывод каждого студента
	}
}

studbox read_stud() { 								// чтение новой информации о студенте

	studbox student; 							// введение нового студента
	
	printf("Введите номер зачетной книжки: "); 
		scanf("%[^\n]%*c", student.nu);

	printf("Введите фамилию студента: "); 
		scanf("%[^\n]%*c", student.surname);

	printf("Введите имя студента: "); 
		scanf("%[^\n]%*c", student.name);

	printf("Введите отчество студента: "); 
		scanf("%[^\n]%*c", student.fathername);
	
	printf("Введите факультет студента: "); 
		scanf("%[^\n]%*c", student.faculty);

	printf("Введите специальность студента: "); 
		scanf("%[^\n]%*c", student.spec);
	
	return student; 							// возвращаем введенного студента
}


studbox* addstudent(studbox* student, int* m) { 				// добавление новой книги

	printf("Введите информацию о новом студенте\n");
	studbox studentA = read_stud(); 					// считываем запись

	for (int i = 0; i < *m; i++) {     					// проверка на наличие копии по номеру зачетки
        
		if (!strcmp(student[i].nu, studentA.nu)) {
            		
			printf("Студент с номером '%s' уже имеется\n", studentA.nu);
			return student;
        }
    }

    student = (studbox*)realloc(student, (*m + 1) * sizeof(studbox));
    student[*m] = studentA;
    (*m)++;

    return student; 								// возвращаем обновлённый массив
}


//--------------------------------------------------Удаление студента--------------------------------------------------------------


void delstud(studbox* student, int* m, studbookbox* studbk, int* l) { 		// удаление студента
    
	if (m == 0) {
		
		printf("Ни одного студента еще не добавлено\n");
		return;
	}

	char nu[8];
	printf("Введите номер зачетной книжки студента, которого хотите удалить: ");
	scanf("%[^\n]%*c", &nu); 						// считываем поле для поиска

	for (int i = 0; i < *l; i++) {
        
		if (strcmp(studbk[i].nu, nu) == 0) {
            
			printf("Данный студент имеет книгу, его невозможно удалить\n");
			return;
		}
	}

	int markerstud = 0; 							// маркер для поиска совпадений
	int j = 0; 								// новый индекс для элементов после удаления

	for (int i = 0; i < *m; i++) {
        
		if (strcmp(student[i].nu, nu) != 0) {
            
			student[j++] = student[i];
		}
        
		else {
            
			markerstud = 1;
		}
	}
	
	if (markerstud == 0) {
	
		printf("Студента с номером '%s' не существует\n", nu);
	}

	*m = j; 								// обновляем размер
}


//---------------------------------------------Поиск студента по номеру зачетки----------------------------------------------------


void findstud(studbox* student, int m) { 					// поиск студента

	char nu[8];
	int markerstud = 0;
    
	printf("Введите номера зачетки студента, которого хотите найти: ");
		scanf("%[^\n]%*c", nu);


	printf("|   Номер   |             Фамилия             |       Имя       |         Отчество         | Факультет |         Специальность          |\n");
	printf("|-----------|---------------------------------|-----------------|--------------------------|-----------|--------------------------------|\n");

    for (int i = 0; i < m; i++) {
        
	if (strcmp(student[i].nu, nu) == 0) { 							// если нашли студента
        
		printstud(student[i]);
		markerstud++;
	}
    }

	if (markerstud == 0) {
		
		printf("Студента с таким номером зачетки '%s' не существует\n", nu);
	}    
}








//---------------------------------------------Работа с файлом студентов----------------------------------------------------------


void saveboxst(char* boxst, studbox* student, int m) { 					// сохранение студентов в файл

	if (m == 0) {
		
		printf("Ни одного студента еще не добавлено\n");
		return 0;
	}

	FILE* filestud = fopen(boxst, "w");

	for (int i = 0; i < m; i++) {

		fprintf(filestud, "%s;", student[i].nu);
		fprintf(filestud, "%s;", student[i].surname);
		fprintf(filestud, "%s;", student[i].name);
		fprintf(filestud, "%s;", student[i].fathername);
		fprintf(filestud, "%s;", student[i].faculty);
		fprintf(filestud, "%s", student[i].spec);

		if (i < m - 1) {
			
			fprintf(filestud, "\n");
		}
	}

	fclose(filestud);
}

studbox* loadboxst(char* boxst, int* m) { 						// загрузка студентов из файла

	FILE* filestud = fopen(boxst, "r");

	if (filestud == 0) { 								// если не удалось открыть файл

		printf("Не удалось открыть файл '%s'\n", boxst);
		*m = 0;
		return NULL; 								// возвращаем пустой указатель
	}

	studbox* student = (studbox*)malloc(sizeof(studbox)); 				// выделяем память под один элемент

	*m = 0; 									// изначально элементов нет

	while (feof(filestud) == 0) {

		fscanf(filestud, "%[^;]%*c", student[*m].nu);
		fscanf(filestud, "%[^;]%*c", student[*m].surname);
		fscanf(filestud, "%[^;]%*c", student[*m].name);
		fscanf(filestud, "%[^;]%*c", student[*m].fathername);
		fscanf(filestud, "%[^;]%*c", student[*m].faculty);
		fscanf(filestud, "%[^\n]%*c", student[*m].spec);

		*m = *m + 1; 								// увеличиваем размер
		student = (studbox*)realloc(student, (*m + 1) * sizeof(studbox)); 	// перераспределяем память
	}

	fclose(filestud);
	return student; 								// возвращаем считанный массив
}

