#include "Database.h"

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);	

	Database db;
	int act = 0;

    do{
        db.Menu(act);

        switch (act)
        {
        case 0:
            break;

        case 1:
            db.Print();
            break;

        case 2:
            db.Add();
            break;

        case 3:
            db.Edit();
            break;

        case 4:
            db.Del();
            break;

        case 5:
            db.Sort();
            break;

        case 6:
            db.SetFileName();
            break;

        default:
            cout << "Неверный выбор!" << endl;
            break;
        }

        system("pause");
        system("cls");
    } while (act != 0);
}