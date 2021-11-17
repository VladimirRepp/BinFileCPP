#include "Database.h"

Database::Database()
{
	fname = "data.bin"; // ���� �� ���������
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
	// ����� ��� ������ �����
	ifstream load(fname, ios::binary);

	if (!load) {
		cout << "���� �� ������!" << endl;
		return 0;
	}

	// ������� ���������� ������
	count = 0;
	Data buf;

	while (!load.eof()) {
		load.read((char*)&buf, sizeof(Data));
		count++;
	}
	load.close();

	count--; // ��� ��� � ����� 1 ������ ������ � �����, ������� ������ ������ �� 1 ������, ��� � ����������������
	if (count < 0)
		count = 0;

	return count;
}

void Database::Add()
{
	Data buf;
	cout << "������: ";
	cin >> buf.str;
	cout << "�����: ";
	cin >> buf.i;
	cout << "�����: ";
	cin >> buf.f;

	system("cls");

	// ������ � ����� ����� 
	ofstream save(fname, ios::binary | ios::app);
	save.write((char*)&buf, sizeof(buf));
	save.close();

	cout << "������ ���������!" << endl;
}

void Database::Edit()
{
	// �������� ������� ������
	if (IsEmpty()) {
		cout << "������ �����!" << endl;
		return;
	}

	// ����� �������
	int pos;
	cout << "�������� (�� 1 �� " << count << "): ";
	cin >> pos;
	pos--;

	// �������� ��������� �������
	if (pos < 0 || pos > count - 1) {
		cout << "������� ������� �������!" << endl;
		return;
	}

	Data buf;
	cout << "������: ";
	cin >> buf.str;
	cout << "�����: ";
	cin >> buf.i;
	cout << "�����: ";
	cin >> buf.f;

	// ������ � ��������� ����� � �����
	ofstream save(fname, ios::binary | ios::in);
	save.seekp(sizeof(buf) * pos, ios::beg);
	save.write((char*)&buf, sizeof(buf));
	save.close();

	cout << "������ ��������!" << endl;
}

void Database::Del()
{
	// �������� ������� ������
	if (IsEmpty()) {
		cout << "������ �����!" << endl;
		return;
	}

	// ����� �������
	int pos;
	cout << "�������� (�� 1 �� " << count << "): ";
	cin >> pos;

	// �������� ��������� �������
	if (pos < 1 || pos > count) {
		cout << "������� ������� �������!" << endl;
		return;
	}

	// ������ ���� � ����� ���������� ����� �������� 
	Data buf;
	ifstream load(fname, ios::binary | ios::in);
	load.seekg(sizeof(buf) * pos, ios::beg);

	// ���������� �� ����� ���������� 
	ofstream save(fname, ios::binary | ios::in);
	save.seekp(sizeof(buf) * (pos - 1), ios::beg);

	int biteSize = 0;
	for (int i = pos; i < count; i++) {
		load.read((char*)&buf, sizeof(Data));
		save.write((char*)&buf, sizeof(buf));
	}

	load.close();
	save.close();

	// ��������� ���� ��� ��������� �� ������ �����
	int fh, result;
	if (_sopen_s(&fh, fname.c_str(), _O_RDWR | _O_CREAT, _SH_DENYNO,
		_S_IREAD | _S_IWRITE) == 0)
	{
		cout << "������ ����� �� �������: " << _filelength(fh) << endl;

		if ((result = _chsize(fh, sizeof(Data) * (count - 1))) == 0)
			cout << "������ ����� �������..." << endl;
		else
			cout << "������ ��������� ������� �����!" << endl;

		cout << "������� ������ �����: " << _filelength(fh) << endl;
		_close(fh);
	}

	cout << "������ �������!" << endl;
}

void Database::Print()
{
	// �������� ������� ������
	if (IsEmpty()) {
		cout << "������ �����!" << endl;
		return;
	}

	Data buf;
	ifstream load(fname, ios::binary | ios::in);

	// ���� ���� �� ��������
	if (!load) {
		cout << "���� �� ������!" << endl;
		return;
	}

	count = Count();

	// ������������ ������ � �����
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
	cout << "������: " << d.str << endl;
	cout << "�����: " << d.i << endl;
	cout << "�����: " << d.f << endl;
}

void Database::Sort()
{
	// �������� ������� ������
	if (IsEmpty()) {
		cout << "������ �����!" << endl;
		return;
	}

	Data buf;
	Data bufNext;
	ifstream load(fname, ios::binary | ios::in);
	ifstream loadNexts(fname, ios::binary | ios::in);
	ofstream save(fname, ios::binary | ios::in);
	ofstream saveNext(fname, ios::binary | ios::in);

	// ���� ���� �� ��������
	if (!load) {
		cout << "���� �� ������!" << endl;
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

	cout << "������ �������������!" << endl;
}

void Database::SetFileName()
{
	cout << "����: ";
	cin >> fname;
}

void Database::Menu(int& act)
{
	cout << "�������� ��������: " << endl
		<< "(0) �����" << endl
		<< "(1) �����" << endl
		<< "(2) ��������" << endl
		<< "(3) ��������" << endl
		<< "(4) �������" << endl
		<< "(5) ����������" << endl
		<< "(6) �������� ����" << endl
		<< "��� �����: ";
	cin >> act;

	system("cls");
}
