#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "./users.h"
#include "./logs.h"



typedef struct bookbox {
	char isbn[15]; 		 // ISBN
	char author[20]; 	 // Автор
	char title[30]; 	 // Название
	 int all; 		 // Всего книг
	 int available; 	 // Из них доступно
} bookbox;

//---------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------Функционал для книг-------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------Добавление книги----------------------------------------------------------------


bookbox new_book() {				 		//Сканим новую книгу
	bookbox book;
	
	printf("Введите ISBN (До 15 цифр): ");
		scanf("%[^\n]%*c", book.isbn);
	
	printf("Введите ФИО автора (С соблюдением формата: Фамилия И.О.): ");
		scanf("%[^\n]%*c", book.author);

	printf("Введите название книги: ");
		scanf("%[^\n]%*c", book.title);
	
	printf("Введите общее количество книг: ");
		scanf("%d%*c", &book.all);	

	printf("Введите количество книг в наличии (Обязательно меньше общего числа книг): ");
		scanf("%d%*c", &book.available);

       	

		return book;	
	
}

void printbook(bookbox book) { 			 		//Вывод книги 

	printf("| %-10s ", book.isbn);
	printf("| %-30s ", book.author);
	printf("| %-25s ", book.title);
	printf("| %-5d ", book.all);
	printf("| %-7d ", book.available);
	printf("|\n");

}

void printbookbox(bookbox* book, int q) {	 		//Вывод всей библиотеки 
						 		//Если книг нет - дропаем предупреждение
	if (q == 0) {
	
		printf("Еще нет ни одной книги\n");
		return;
}

	printf("|    ISBN    |             Авторы             |       Название книг       | Всего | Имеется |\n");
       	printf("|------------|--------------------------------|---------------------------|-------|---------|\n");
	
	for (int i = 0; i < q; i++) {
	
		printbook(book[i]);
	}
}

bookbox* createbook(bookbox* book, int* q) {	 		//Добавление книги

	printf("Введите информацию о книге\n");
	bookbox nbook = new_book();
	if ((nbook.all > 0) && (nbook.available > -1)){

		for (int i = 0; i < *q; i++) {		 		//Проверка на существование ISBN
		
			if (strcmp(book[i].isbn, nbook.isbn) == 0) {
		
				printf("ISBN '%s' уже существует\n", nbook.isbn);
				return book;
			}
		}
	} 
	else {

		printf ("Число книг должно быть не отрицательным\n");
		return book;
	}

	book = (bookbox*)realloc(book, (*q + 1) * sizeof(bookbox));
	book[*q] = nbook;	
	(*q)++;
	
	return book;
}

//---------------------------------------------------Редактирование книги----------------------------------------------------------



bookbox* remakebook(bookbox* book, int* q, studbookbox* studbk, int* l) {	 		//Редактирование  книги

	printf("Введите существующий ISBN книги, которую вы хотите отредактировать, а так же всю новую информацию о книге,\n");
	bookbox rbook = new_book();
	for (int i = 0; i < *l; i++) {
        	
		if (strcmp(studbk[i].isbn, rbook.isbn) == 0) {					//Проверка на БД студентов
            	
			printf("Данную книгу невозможно отредактировать, она имеется в наличии у кого-то из студентов\n");
            		return;
        	}
    	}
	for (int i = 0; i < *q; i++) {		 		
		
		if (strcmp(book[i].isbn, rbook.isbn) == 0) {
			
			book[i] = rbook;
			return book;
		}
	}
	
	printf(" Такой ISBN '%s' отсуствует\n", rbook.isbn);
	return book;
}




//----------------------------------------------------Удаление книги---------------------------------------------------------------


void deletebook(bookbox* book, int* q, studbookbox* studbk, int* l) {
	
	if (q == 0) {
		
		printf("Ни одной книги еще не добавлено\n");
		return;

	}

	char isbn[15];
	printf("Введите ISBN книги, которую хотите удалить: ");
	scanf("%[^\n]", &isbn); 		 			// считываем поле для поиска

	for (int i = 0; i < *l; i++) {
        	
		if (strcmp(studbk[i].isbn, isbn) == 0) {
            	
			printf("Данную книгу невозможно удалить, она имеется в наличии у кого-то из студентов\n");
            		return;
        	}
    	}

	int j = 0; 		 		 			// новый индекс для после удаления
	
	for (int i = 0; i < *q; i++) {
		
		if (strcmp(book[i].isbn, isbn) != 0) {
	       		
			book[j++] = book[i];
		}	
		else {
			printf("Книги с ISBN '%s' не существует\n", isbn); ;
		}
	}
	
	*q = j; 				  			//обновляем 
}

//---------------------------------------------------Выдача книги------------------------------------------------------------------


void givebook(bookbox* book, int q, studbox* student, int* m, studbookbox* studbk, int* l) {

	int existence = 0;			  			// проверка на существование студента
	int daymin = 31, monthmin = 12, yearmin = 2025;
	int studsum, min;
	char isbn[15], nu[8];

	printf("Введите ISBN необходимой книги: ");
		scanf("%[^\n]%*c", isbn);

	printf("Введите номер зачетки студента, который получает книжку: ");
		scanf("%[^\n]%*c", nu);
	
	for (int i = 0; i < q; i++) {

		if (strcmp(student[i].nu, nu) == 0) {
			existence++;
		}
	}
	
	if (existence == 0) {

		printf("Студента с таким номером зачетки не существует\n");
		return 0;
	}
	
	for (int i = 0; i < q; i++) {

		if (strcmp(book[i].isbn, isbn) == 0) { 			// если нашли книгу

			if (book[i].available == 0) {  			// доступна ли
			
				for (int j = 0; j < *l; j++) {

					if (strcmp(studbk[j].isbn, isbn) == 0) {

						studsum = studbk[j].year * 10000 + studbk[j].month * 100 + studbk[j].day;
						min = yearmin * 10000 + monthmin * 100 + daymin;

						if (studsum < min) {

							daymin = studbk[j].day;
							monthmin = studbk[j].month;
							yearmin = studbk[j].year;
						}
					}
				}
				
				printf("К сожалению, данной книги нет в наличииБлижайшая книга будет сдана %d.%d.%d\n", daymin, monthmin, yearmin);
			}	
			
			else {
				book[i].available--;

				studbk = (studbookbox*)realloc(studbk, (*l + 1) * sizeof(studbookbox));
				strcpy(studbk[*l].isbn, isbn);
				strcpy(studbk[*l].nu, nu);

				studbk[*l].day = 31;
				studbk[*l].month = 06;
				studbk[*l].year = 2020;

				(*l)++;

				printf("Книга выдана\n");
			}
			return 0;
		}
	}
	printf("Книги с ISBN '%s' не существует\n", isbn);
}

//---------------------------------------------------------Сдача книги-------------------------------------------------------------


void returnbook(bookbox* book, int q, studbox* student, int* m, studbookbox* studbk, int* l) {	 // возврат книги

	int existenceA = 0, existenceB = 0; 		 					 // проверка на существование
	char isbn[15], nu[8];

	printf("Введите ISBN книги, которую хотите вернуть: ");
		scanf("%[^\n]%*c", isbn);

	printf("Введите номер зачетки студента, который возвращает книжку: ");
		scanf("%[^\n]%*c", nu);

	for (int i = 0; i < q; i++) {

		if (strcmp(student[i].nu, nu) == 0) {

			existenceA++;
		}
	}
	
	if (existenceA == 0) {

		printf("Студента с таким номером зачетки не существует\n");
		return 0;
	}

	for (int i = 0; i < *l; i++) {

		if ((strcmp(studbk[i].nu, nu) == 0) && (strcmp(studbk[i].isbn, isbn) == 0)) {

			existenceB++;
		}
	}

	if (existenceB == 0) {
	
		printf("Студент с таким номером не имеет в наличии такой книги\n");
		return 0;
	}

	for (int i = 0; i < q; i++) {

		if (strcmp(book[i].isbn, isbn) == 0) {	 			// если нашли книгу

			if (book[i].all == book[i].available) {

				printf("Невозможно вернуть эту книгу - в библиотеке уже имеются все экземпляры\n");
			}

			else {
				book[i].available++;
				int j = 0; 		 			// новый индекс для элементов после удаления

				for (int i = 0; i < *l; i++) {

					if ((strcmp(studbk[i].isbn, isbn) != 0) || (strcmp(studbk[i].nu, nu) != 0)) {
						studbk[j++] = studbk[i];
					}
				}
				
				*l = j; 		 			// обновляем размер
				printf("Книга успешно возвращена\n");
			}

			return 0;
		}
	}
	
	printf("Книги с ISBN '%s' не существует\n", isbn);
}

//-----------------------------------------------------Данные о студентах, имеющих книгу-------------------------------------------


void bookinfo(studbox* student, int m, bookbox* book, int q, studbookbox* studbk, int l) {
	
	char isbn[15];
	int marker= 0;

	printf("Введите номер ISBN, чтобы просмотреть всю информацию о владельцах книги: ");
		scanf("%[^\n]%*c", isbn);

	printf("|   Номер   |             Фамилия             |       Имя       |         Отчество         | Факультет |         Специальность          |  Дата возврата  \n");
	printf("|-----------|---------------------------------|-----------------|--------------------------|-----------|--------------------------------|-----------------\n");
		
	for (int i = 0; i < m; i++) {

		if (strcmp(book[i].isbn,isbn) == 0) {						 // если нашли книгу

			for (int j = 0; j < l; j++) {

				if (strcmp(studbk[j].isbn, isbn) == 0) {			 // если нашли студента с книгой

					for (int r = 0; r < q; r++) {

						if (strcmp(student[r].nu, studbk[j].nu) == 0) {	 // поиск студента в таблице

							printf("| %-9s ", student[r].nu);
							printf("| %-31s ", student[r].surname);
							printf("| %-15s ", student[r].name);
							printf("| %-24s ", student[r].fathername);
							printf("| %-9s ", student[r].faculty);
							printf("| %-30s ", student[r].spec);
							printf("| %d.%d.%d ", studbk[j].day, studbk[j].month, studbk[j].year);
							printf("\n");
						}
					}
				}
			}

			marker++;
		}
	}

	if (marker == 0) {

		printf("Книги с ISBN '%s' не существует\n", isbn);
	}
}

//-------------------------------------------------------Работа с файлом книг------------------------------------------------------


void savebox(char* boxbk, bookbox* book, int q) {	 			 // сохранение книг в файл 

	if (q == 0) {
		
		printf("Ни одной книги еще не добавлено\n");
       		return 0;
       	}

	FILE* filebook = fopen(boxbk, "w"); 		 			 // открываем файл на запись
	
	for (int i = 0; i < q; i++) {

		fprintf(filebook, "%s;", book[i].isbn);
		fprintf(filebook, "%s;", book[i].author);
		fprintf(filebook, "%s;", book[i].title);
		fprintf(filebook, "%d;", book[i].all);
		fprintf(filebook, "%d", book[i].available);

		if (i < q - 1) {
			
			fprintf(filebook, "\n");
		}
	}

	fclose(filebook); 				 			 // закрываем файл
}

bookbox* loadboxbook(char* boxbk, int* q) { 		 			 // загрузка книг из файла

	FILE* filebook = fopen(boxbk, "r"); 		 			 // открываем файл на чтение

	if (filebook == 0) { 				 			 // если не удалось открыть файл

		printf("Не удалось открыть файл '%s'\n", boxbk);
		*q = 0;
		return NULL; 				 			 // возвращаем пустой указатель
	}

	bookbox* book = (bookbox*)malloc(sizeof(bookbox)); 			 // выделяем память под один элемент
	
	*q = 0; 					 			 // изначально элементов нет


	while (feof(filebook) == 0) {

		fscanf(filebook, "%[^;]%*c", book[*q].isbn);
		fscanf(filebook, "%[^;]%*c", book[*q].author);
		fscanf(filebook, "%[^;]%*c;", book[*q].title);
		fscanf(filebook, "%d%*c", &book[*q].all);
		fscanf(filebook, "%d%*c", &book[*q].available);
		
		*q = *q + 1; 				 			 // увеличиваем размер
		
		book = (bookbox*)realloc(book, (*q + 1) * sizeof(bookbox)); 	 // перераспределяем память
	}
	
	fclose(filebook);

	return book; 								 // возвращаем считанный массив
}



//------------------------------------------------Информация по книгам студента----------------------------------------------------

void studinfo(studbox* student, int m, bookbox* book, int q, studbookbox* studbk, int l) {
	
	char nu[8];
	int marker = 0;

	printf("Введите номер зачетной книжки, чтобы просмотреть всю информацию о ваших книгах: ");
		scanf("%[^\n]%*c", nu);
		
	printf("|    ISBN    |             Авторы             |       Название книг       |  Дата возврата  \n");
	printf("|------------|--------------------------------|---------------------------|-----------------\n"); 

	for (int i = 0; i < m; i++) {
		
		if (strcmp(student[i].nu, nu) == 0) { 						// если нашли студента
			
			for (int j = 0; j < l; j++) {
				
				if (strcmp(studbk[j].nu, nu) == 0) { 				// если нашли книгу у студента
					
					for (int r = 0; r < q; r++) {
						
						if (strcmp(book[r].isbn, studbk[j].isbn) == 0) { // поиск книги в библиотеке
							
							printf("| %-10s ", book[r].isbn);
							printf("| %-30s ", book[r].author);
							printf("| %-25s ", book[r].title);
							printf("| %d.%d.%d ", studbk[j].day, studbk[j].month, studbk[j].year);
							printf("\n");
						}
					}
				}
			}

			marker++;
		}
	}

	if (marker == 0) {
		printf("Студента с номером зачетки '%s' не существует\n", nu);
	}
}