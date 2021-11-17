#pragma once

#include <fstream>
#include <string>
#include <Windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
using namespace std;

#define _CRT_SECURE_NO_WARNINGS

// Структура данных
struct Data
{
	char str[50];
	int i;
	float f;
};

class Database
{
	// Поля класса
private: // закрытая часть
	Data data;
	string fname;
	size_t count;

	// Методы класса
public: // открытая часть
	Database();
	~Database();

	bool IsEmpty();
	int Count();
	void Add();
	void Edit();
	void Del();
	void Print();
	void Print(Data d);
	void Sort();
	void SetFileName();
	void Menu(int& act);
};

