//Кондратив В.О. ИУ4-23Б
//Вариант 5 ( 3 для первого файла, 5 для второго)
//Задачи
//Добавление книг
//Удаление книг
//Выдача книг на руки
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>


//----------------------------------------------Зададим структурки для хранения данных---------------------------------------------

typedef struct userbox {
	char login[20];		 // Nu зачетки
	char password[20]; 	 // Фамилия
	int statusstud; 	 // Доступ к управлению студентами
	int statusbook;		 // Доступ к управлению книгами
}

typedef struct bookbox {
	char isbn[15]; 		 // номер ISBN
	char author[20]; 	 // фио автора
	char title[40]; 	 // название
	int all; 		 // всего книг
	int available; 		 // доступно книг
}

typedef struct studbox {

	char nu[8];		 // Nu зачетки  
	char surname[20];        // Фамилия
	char name[20];		 // Имя
	char fathername[20];	 // Отчество
	char faculty[4];	 // Факультет
	char spec[60];		 // Спеца
}

typedef struct stbbox {
	char isbn[15];		 // номер ISBN
	char nu[8]; 		 // номер зачетной книжки
	int day; 		 // дата сдачи
	int mon; 		 // месяц сдачи
	int year; 		 // год сдачи
}




















int main() {
	
	system("chcp 1251");
	system("cls");
	printf("Now do nothing\n");
	
	return 0;	
}


