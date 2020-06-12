//Кондратив В.О. ИУ4-23Б
//Вариант 5 ( 3 для первого файла, 5 для второго)


//Задачи 1,2,3:

//Добавление книг +
//Удаление книг +
//Выдача книг на руки +
//Принятие книг +

//Добавить студента +
//удалить студента +
//Редактировать студента
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


//----------------------------------------------Зададим структурки для хранения данных---------------------------------------------

typedef struct userbox {
	char login[20];		 // Nu зачетки
	char password[20]; 	 // Фамилия
	 int statusstud; 	 // Доступ к управлению студентами
	 int statusbook;	 // Доступ к управлению книгами
} userbox;

typedef struct bookbox {
	char isbn[15]; 		 // ISBN
	char author[20]; 	 // Автор
	char title[30]; 	 // Название
	 int all; 		 // Всего книг
	 int available; 	 // Из них доступно
} bookbox;

typedef struct studbox {

	char nu[8];		 // Nu зачетки  
	char surname[20];        // Фамилия
	char name[20];		 // Имя
	char fathername[20];	 // Отчество
	char faculty[4];	 // Факультет
	char spec[60];		 // Спеца
} studbox;

typedef struct studbookbox{
	char isbn[15];		 // ISBN
	char nu[8]; 		 // Nu зачетки
	 int day; 		 // Дата сдачи
	 int month; 		 // Месяц сдачи
	 int year; 		 // Год сдачи
} studbookbox;

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

	for (int i = 0; i < *q; i++) {		 		//Проверка на существование ISBN
		
		if (strcmp(book[i].isbn, nbook.isbn) == 0) {
		
			printf("ISBN '%s' уже существует\n", nbook.isbn);
			return book;
		}
	}

	book = (bookbox*)realloc(book, (*q + 1) * sizeof(bookbox));
	book[*q] = nbook;	
	(*q)++;
	
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
				studbk[*l].month = 05;
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
//---------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------Логин/Пароль-------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------


userbox* loadboxuser(char* boxuser, int* k) { 						//загрузка пользователей из файла
	
	FILE* fileuser = fopen(boxuser, "r");

	if (fileuser == 0) { 								// если не удалось открыть файл

		printf("Не удалось открыть файл '%s'\n", boxuser);
		*k = 0;
		return NULL; 								// возвращаем пустой указатель
	}

	userbox* user = (userbox*)malloc(sizeof(userbox)); 				// выделяем память под один элемент
	*k = 0; 									// изначально элементов нет

	while (feof(fileuser) == 0) {

		fscanf(fileuser, "%[^;]%*c", user[*k].login);
		fscanf(fileuser, "%[^;]%*c;", user[*k].password);
		fscanf(fileuser, "%d%*c", &user[*k].statusstud);
		fscanf(fileuser, "%d%*c", &user[*k].statusbook);

		*k = *k + 1; 								// увеличиваем размер

		user = (userbox*)realloc(user, (*k + 1) * sizeof(userbox)); 		// перераспределяем память
	}

	fclose(fileuser);
	return user; 									// возвращаем считанный массив

}


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

//---------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------Логирование-----------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------


void logging(char* log, char* func) {

	FILE* logging = fopen("library.log", "a");
	time_t s_time;
	struct tm* m_time;
	char timer[100];

	s_time = time(NULL);
	m_time = localtime(&s_time);
	strftime(timer, 128, "%d.%m.%y %H:%M:%S", m_time);

	fprintf(logging, "%s;%s;%s", timer, log, func);
	fprintf(logging, "\n");

	fclose(logging);
}

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
				printf("4. Редактировать студента\n");
				printf("5. Поиск по номеру зачетки студента\n");
				printf("6. Информация о книгах студента\n");
				printf("7. Сохранить и выйти\n");

				if (admlvl == 1) {

					printf("0. Выйти к меню с выбором БД\n");
				}

				printf("Введите пункт меню > ");
				scanf("%d%*c", &pointstud); 			// считываем пункт меню

				while (((pointstud < 1) || (pointstud > 8)) && (pointstud != 0)) { 
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
						
						logging(log, "rework_student_profile ");
						basemarkB++;	
						break;

					case 5:
						findstud(student, m);
						logging(log, "find_student");
						break;

					case 6:
						studinfo(student, m, book, q, studbk, l);
						logging(log, "student_info");
						break;

					case 7:
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
				printf("4. Получение книги\n");
				printf("5. Возврат книги\n");
				printf("6. Информация о владельцах книги\n");
				printf("7. Сохранить и выйти\n");

				if (admlvl == 1) {

					printf("0. Выйти к меню с выбором БД\n");
				}

				printf("Введите пункт меню > ");
					scanf("%d%*c", &pointbook); 		// считываем пункт меню
				
				while (((pointbook < 1) || (pointbook > 7)) && (pointbook != 0)) { 
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
						givebook(book, q, student, &m, studbk, &l);
						logging(log, "giveout_book");
						basemarkB++;
						basemarkC++;
						break;

					case 5:
						returnbook(book, q, student, &m, studbk, &l);
						logging(log, "return_book");
						basemarkB++;
						basemarkC++;
						break;

					case 6:	
						bookinfo(student, m, book, q, studbk, l);
						logging(log, "book_info");
						break;

					case 7:
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

			} while (pointbook != 0);	 						// повторяем, пока не решим выйти

			logging(log, "back_to_main_menu");
			lvl = 0;
		}


	}


}
