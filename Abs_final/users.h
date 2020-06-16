#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "./bookdb.h"
#include "./studdb.h"
#include "./studbookdb.h"
#include "./logs.h"














typedef struct userbox {
	char login[20];		 // Nu зачетки
	char password[20]; 	 // Фамилия
	 int statusstud; 	 // Доступ к управлению студентами
	 int statusbook;	 // Доступ к управлению книгами
} userbox;

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
