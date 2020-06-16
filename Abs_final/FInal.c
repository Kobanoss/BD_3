//Кондратив В.О. ИУ4-23Б
//Вариант 5 ( 3 для первого файла, 5 для второго)


//Задачи 1,2,3:

//Добавление книг +
//Удаление книг +\
//Редактирование книги
//Выдача книг на руки +
//Принятие книг +

//Добавить студента +
//удалить студента +
//Посмотреть информацию по студенту по номеру зачетки +

//Логин&&Пароль +
//Меню с доступом


//Коментарии к 3 сложности:

//Нельзя удалить студента, у которого есть книги +
//Нельзя удалить книгу, которая есть у студентов +
//Нельзя изменять ISBN книг и номер зачетки студентов 
//Каждый студент может посмотреть информацию по всем его книгам +
//Для каждой книги можно посмотреть информацию по студентам, которые ее взяли +
//При выдаче и сдаче книги студенту пользователь должен указать ISBN книги и номер зачетки студента +
//Если нет доступных книг для выдачи, необходимо выводить сообщение – когда будет сдана ближайшая книга +

//Логи +


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "./bookdb.h"
#include "./studdb.h"
#include "./studbookdb.h"
#include "./users.h"
#include "./logs.h"




//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------

int main() {
	int basemarkA = 0, basemarkB = 0, basemarkC = 0;
	
	int m = 0, q = 0, k = 0, l = 0; 					// количество элементов в массиве
	char log[20], pass[20]; 						// логин и пароль

	studbox* student = loadboxst("students.csv", &m); 			// массив студентов
	bookbox* book = loadboxbook("books.csv", &q); 				// массив книг
	userbox* user = loadboxuser("users.csv", &k); 				// массив пользователей
	studbookbox* studbk = loadstudb("student_books.csv", &l); 		// массив пользователей

	int lvl = 0; 								//уровень доступа к БД (1 - студенты, 2 - книги)
	int admlvl = 0; 							//уровень доступа к двум БД
	int pointstud, pointbook, pointadmin; 					// выбираемый пункт меню

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	printf("Введите логин: ");
		scanf("%[^\n]%*c", log); 					// считываем логин
	
	printf("Введите пароль: ");
		scanf("%[^\n]%*c", pass); 					// считываем пароль

	printf("\n");

	for (int i = 0; i < k; i++) {

		if ((strcmp(user[i].login, log) == 0) && (strcmp(user[i].password, pass) == 0) && (user[i].statusstud == 1) && (user[i].statusbook == 0)) { 
										// доступ только к БД со студентами
		
			lvl = 1;
			break;
		}

		else if ((strcmp(user[i].login, log) == 0) && (strcmp(user[i].password, pass) == 0) && (user[i].statusstud == 0) && (user[i].statusbook== 1)) {

			lvl = 2;
			break;
		}

		else if ((strcmp(user[i].login, log) == 0) && (strcmp(user[i].password, pass) == 0) && (user[i].statusstud == 1) && (user[i].statusbook == 1)) {

			admlvl = 1;
			break;
		}
	}

	if ((lvl == 0) && (admlvl == 0)) {

		printf("Такого пользователя не существует\n");
	       	free(user);
		free(student);
		free(book);
		free(studbk);	

		return 0;
	}	

	while (1) {

		if ((admlvl == 1) && (lvl == 0)) {

			printf("Меню доступа к БД\n");
			printf("1. База данных студентов библиотеки\n");
			printf("2. База данных книг библиотеки\n");
			printf("Выберите базу данных > ");
			scanf("%d%*c", &pointadmin); 				// считываем пункт меню

			while ((pointadmin < 1) || (pointadmin > 2)) { 		// проверка на верность введенного пункта меню

				printf("Неверный пункт меню, попробуйте еще раз > ");
				scanf("%d%*c", &pointadmin);
			}

			switch (pointadmin) {

				case 1: lvl = 1;					
					logging(log, "stud_menu_move");
					break;

				case 2: lvl = 2;
					logging(log, "book_menu_move");
					break;
			}
			
			printf("\n");
	
		}

		if (lvl == 1) {

			do {
				printf("Меню операций со студентами\n");
				printf("1. Вывод списка студентов\n");
				printf("2. Добавить нового студента\n");
				printf("3. Удалить студента\n");
				printf("4. Поиск по номеру зачетки студента\n");
				printf("5. Информация о книгах студента\n");
				printf("6. Сохранить и выйти\n");

				if (admlvl == 1) {

					printf("0. Выйти к меню с выбором БД\n");
				}

				printf("Введите пункт меню > ");
				scanf("%d%*c", &pointstud); 			// считываем пункт меню

				while (((pointstud < 1) || (pointstud > 6)) && (pointstud != 0)) { 
										// проверка на верность введенного пункта меню

					printf("Неверный пункт меню, попробуйте еще раз > ");
					scanf("%d%*c", &pointstud);
				}

				switch (pointstud) {


					case 1:
						printstudbox(student, m);
						logging(log, "print_students_table");
						break;

					case 2:
						student = addstudent(student, &m);
						logging(log, "add_student");
						basemarkA++;
						break;

					case 3:
						delstud(student, &m, studbk, &l);
						logging(log, "delete_student");
						basemarkA++;
						break;

					
					case 4:
						findstud(student, m);
						logging(log, "find_student");
						break;

					case 5:
						studinfo(student, m, book, q, studbk, l);
						logging(log, "student_info");
						break;

					case 6:
						if (basemarkA != 0){
							saveboxst("students.csv", student, m); // сохраняем студентов в файл
						}
						if (basemarkB != 0) {					
							savestudb("student_books.csv", studbk, l);
						}
						logging(log, "save_student_changes");

						free(user);
						free(student);
						free(book);
						free(studbk); 					// освобождаем динамическую память
						return 0;	
						
				}

				printf("\n");

			} while (pointstud != 0); 						// повторяем, пока не решим выйти

			logging(log, "back_to_main_menu");
			lvl = 0;
		}
		
		if (lvl == 2 ) {

			do {
		
				printf("Вы находитесь в базе данных книг библиотеки!\n");
				printf("1. Вывод всей библиотеки\n");
				printf("2. Добавить новую книгу в библиотеку\n");
				printf("3. Удалить книгу из библиотеки\n");
				printf("4. Редактировать книгу\n");
				printf("5. Получение книги\n");
				printf("6. Возврат книги\n");
				printf("7. Информация о владельцах книги\n");
				printf("8. Сохранить и выйти\n");

				if (admlvl == 1) {

					printf("0. Выйти к меню с выбором БД\n");
				}

				printf("Введите пункт меню > ");
					scanf("%d%*c", &pointbook); 		// считываем пункт меню
				
				while (((pointbook < 1) || (pointbook > 8)) && (pointbook != 0)) { 
										// проверка на верность введенного пункта меню
					
					printf("Неверный пункт меню, попробуйте еще раз: ");
				 		scanf("%d%*c", &pointbook);
				}

				switch (pointbook) {
				


					case 1: 
						printbookbox(book, q);
						logging(log, "print_book_table");
						break;
					
					case 2:
						book = createbook(book, &q);
						logging(log, "add_book");
						basemarkC++;
						break;

					case 3: 
						deletebook(book, &q, studbk, &l);
						logging(log, "delete_book");
						basemarkC++;
						break;
					case 4:
						book = remakebook(book, &q, studbk, &l);						
						logging(log, "rework_book");
						basemarkC++;	
						break;

					case 5:	
						givebook(book, q, student, &m, studbk, &l);
						logging(log, "giveout_book");
						basemarkB++;
						basemarkC++;
						break;

					case 6:
						returnbook(book, q, student, &m, studbk, &l);
						logging(log, "return_book");
						basemarkB++;
						basemarkC++;
						break;

					case 7:	
						bookinfo(student, m, book, q, studbk, l);
						logging(log, "book_info");
						break;

					case 8:
						if (basemarkC != 0) {
							savebox("books.csv", book, q); 	// сохраняем книги в файл
						} 
						if (basemarkB != 0) {	
							savestudb("student_books.csv", studbk, l);
						}
						logging(log, "save_book_changes");

						free(user);
						free(student);
						free(book);
						free(studbk); 				// освобождаем динамическую памят	
						return 0;	
						
				}

				printf("\n");

			} while (pointbook != 0);	 				// повторяем, пока не решим выйти

			logging(log, "back_to_main_menu");
			lvl = 0;
		}


	}


}
