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
#include "./users.h"










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
