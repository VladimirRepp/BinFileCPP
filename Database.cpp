#include "Database.h"

Database::Database()
{
	fname = "data.bin"; // файл по умолчанию
	count = 0;
}

Database::~Database()
{
}

bool Database::IsEmpty()
{
	Count();
	return count == 0;
}

int Database::Count()
{
	// Поток для чтения файла
	ifstream load(fname, ios::binary);

	if (!load) {
		cout << "Файл не найден!" << endl;
		return 0;
	}

	// Подсчет количества данных
	count = 0;
	Data buf;

	while (!load.eof()) {
		load.read((char*)&buf, sizeof(Data));
		count++;
	}
	load.close();

	count--; // так как в файле 1 пустая строка в конце, подсчет данных укажет на 1 больше, чем в действительности
	if (count < 0)
		count = 0;

	return count;
}

void Database::Add()
{
	Data buf;
	cout << "Строка: ";
	cin >> buf.str;
	cout << "Число: ";
	cin >> buf.i;
	cout << "Дробь: ";
	cin >> buf.f;

	system("cls");

	// Запись в конец файла 
	ofstream save(fname, ios::binary | ios::app);
	save.write((char*)&buf, sizeof(buf));
	save.close();

	cout << "Данные добавлены!" << endl;
}

void Database::Edit()
{
	// Проверка наличия данных
	if (IsEmpty()) {
		cout << "Данные пусты!" << endl;
		return;
	}

	// Выбор позиции
	int pos;
	cout << "Выберите (от 1 до " << count << "): ";
	cin >> pos;
	pos--;

	// Проверка выбранной позиции
	if (pos < 0 || pos > count - 1) {
		cout << "Позиция выбрана неверно!" << endl;
		return;
	}

	Data buf;
	cout << "Строка: ";
	cin >> buf.str;
	cout << "Число: ";
	cin >> buf.i;
	cout << "Дробь: ";
	cin >> buf.f;

	// Запись в указанное место в файле
	ofstream save(fname, ios::binary | ios::in);
	save.seekp(sizeof(buf) * pos, ios::beg);
	save.write((char*)&buf, sizeof(buf));
	save.close();

	cout << "Данные изменены!" << endl;
}

void Database::Del()
{
	// Проверка наличия данных
	if (IsEmpty()) {
		cout << "Данные пусты!" << endl;
		return;
	}

	// Выбор позиции
	int pos;
	cout << "Выберите (от 1 до " << count << "): ";
	cin >> pos;

	// Проверка выбранной позиции
	if (pos < 1 || pos > count) {
		cout << "Позиция выбрана неверно!" << endl;
		return;
	}

	// Читаем файл с места следующего после удаления 
	Data buf;
	ifstream load(fname, ios::binary | ios::in);
	load.seekg(sizeof(buf) * pos, ios::beg);

	// Записываем на место удаляемого 
	ofstream save(fname, ios::binary | ios::in);
	save.seekp(sizeof(buf) * (pos - 1), ios::beg);

	int biteSize = 0;
	for (int i = pos; i < count; i++) {
		load.read((char*)&buf, sizeof(Data));
		save.write((char*)&buf, sizeof(buf));
	}

	load.close();
	save.close();

	// Открываем файл для обрезания не нужной части
	int fh, result;
	if (_sopen_s(&fh, fname.c_str(), _O_RDWR | _O_CREAT, _SH_DENYNO,
		_S_IREAD | _S_IWRITE) == 0)
	{
		cout << "Размер файла до удалени: " << _filelength(fh) << endl;

		if ((result = _chsize(fh, sizeof(Data) * (count - 1))) == 0)
			cout << "Размер файла изменен..." << endl;
		else
			cout << "Ошибка изменения размера файла!" << endl;

		cout << "Текущий размер файла: " << _filelength(fh) << endl;
		_close(fh);
	}

	cout << "Данные удалены!" << endl;
}

void Database::Print()
{
	// Проверка наличия данных
	if (IsEmpty()) {
		cout << "Данные пусты!" << endl;
		return;
	}

	Data buf;
	ifstream load(fname, ios::binary | ios::in);

	// Если файл не открылся
	if (!load) {
		cout << "Файл не найден!" << endl;
		return;
	}

	count = Count();

	// Поэлементное чтение и вывод
	for (int i = 0; i < count; i++)
	{
		load.read((char*)&buf, sizeof(Data));
		Print(buf);
		cout << "-------------------------" << endl;
	}
	load.close();
}

void Database::Print(Data d)
{
	cout << "Строка: " << d.str << endl;
	cout << "Число: " << d.i << endl;
	cout << "Дробь: " << d.f << endl;
}

void Database::Sort()
{
	// Проверка наличия данных
	if (IsEmpty()) {
		cout << "Данные пусты!" << endl;
		return;
	}

	Data buf;
	Data bufNext;
	ifstream load(fname, ios::binary | ios::in);
	ifstream loadNexts(fname, ios::binary | ios::in);
	ofstream save(fname, ios::binary | ios::in);
	ofstream saveNext(fname, ios::binary | ios::in);

	// Если файл не открылся
	if (!load) {
		cout << "Файл не найден!" << endl;
		return;
	}

	count = Count();

	for (int i = 0; i < count; i++) {
		load.read((char*)&buf, sizeof(Data));

		for (int j = i + 1; j < count; j++) {
			loadNexts.seekg(sizeof(Data) * j, ios::beg);
			loadNexts.read((char*)&bufNext, sizeof(Data));

			if (buf.i < bufNext.i) {
				saveNext.seekp(sizeof(Data) * i, ios::beg);
				saveNext.write((char*)&bufNext, sizeof(Data));

				save.seekp(sizeof(Data) * j, ios::beg);
				save.write((char*)&buf, sizeof(Data));
			}
		}

		Print();
		cout << endl;
	}

	save.close();
	saveNext.close();
	load.close();
	loadNexts.close();

	cout << "Данные отсортированы!" << endl;
}

void Database::SetFileName()
{
	cout << "Файл: ";
	cin >> fname;
}

void Database::Menu(int& act)
{
	cout << "Выберите действие: " << endl
		<< "(0) Выход" << endl
		<< "(1) Вывод" << endl
		<< "(2) Добавить" << endl
		<< "(3) Изменить" << endl
		<< "(4) Удалить" << endl
		<< "(5) Сортировка" << endl
		<< "(6) Изменить файл" << endl
		<< "Ваш выбор: ";
	cin >> act;

	system("cls");
}
