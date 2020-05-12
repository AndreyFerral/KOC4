#include <iostream>
#include <Windows.h>
#include <fstream>
#include <sstream>
using namespace std;

#define MEGABYTE 1048576
#define MSIZE 30

void main(int argc, char* argv[]) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	DWORD bytesread; // вспомогательная ячейка
	HANDLE hFile; // идентификатор созданного или открытого файла
	int	AmountOfByte = 0; // сумма считанных байтов 

	cout << "Резервирование " << MSIZE << " Мбайт" << endl;
	PBYTE buff = (PBYTE)VirtualAlloc(NULL, // адрес по которому необходимо выделить память
		MSIZE *MEGABYTE,  // количество байт, которое необходимо выделить
		MEM_RESERVE,	  // резервировании диапазона адресов
		PAGE_READWRITE); // Выделяется память, как для чтения, так и для записи.

	if (buff) cout << "Блок памяти @" << hex << (size_t)buff << "\n";
	else { // если возникла ошибка при выделении памяти
		cout << "Ошибка выделения памяти!\n";
		system("pause");
		return;
	}

	cout << "Выделение памяти" << endl << endl;
	for (int i = 1; i < 21; ++i) {
		string path = "Дискеты\\Дискета" + to_string(i) + ".bin";  // путь к файлу
		wstring drive_name = L"Дискеты\\Дискета" + to_wstring(i) + L".bin"; // путь к файлу
		
		hFile = CreateFile( // создание файла для чтения
			drive_name.data(), // имя файла
			GENERIC_READ, // режим доступа
			FILE_SHARE_READ, // совместный доступ
			NULL, // дескриптор защиты
			OPEN_EXISTING, // параметры создания 
			0, // атрибуты файла
			NULL); // идентификатор файла с атрибутами
				   
		int FileSize = GetFileSize( // определение размера файла в байтах
			hFile, // дескриптор файла
			NULL); // старшее слово размера файла. NULL т.к. не нужно

		if (hFile == INVALID_HANDLE_VALUE) { // если вернулся код ошибки 
			VirtualFree( // освобождение памяти
				buff,	   // адрес участка памяти
				MSIZE *MEGABYTE, // его размер (в байтах)
				NULL);			 // тип освобождения памяти
			MessageBox(0, L"Открыть устройство не удалось", NULL, MB_OK | MB_ICONSTOP);
			return;
		}
		
		SetFilePointer( // установить начальную точку чтения в файле на начало файла
			hFile, // дескриптор файла
			0, 0, // байты перемещения указателя
			FILE_BEGIN); // точка отсчета

		// выделить память для результата чтения
		cout << "Блок памяти @" << hex <<
			(size_t)VirtualAlloc(buff + AmountOfByte, FileSize, MEM_COMMIT, PAGE_READWRITE)
			<< "\n"; 
		
		if (!ReadFile(hFile, buff, FileSize, &bytesread, NULL)) { // читать данные с устройства
			VirtualFree(buff, MSIZE * MEGABYTE, NULL); // освобождение памяти
			MessageBox(0, L"Чтение данных НЕ удалось", NULL, MB_OK | MB_ICONSTOP);
			return;
		}
		
		CloseHandle(hFile); // закрыть файл (устройство) 
		wstring newFile_name = L"ОтдельныеОбразы\\ОтдельныйОбраз" + to_wstring(i) + L".bin"; // путь к файлу

		// создание файла для записи
		hFile = CreateFile(newFile_name.data(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);

		if (hFile == INVALID_HANDLE_VALUE) {// если вернулся код ошибки
			VirtualFree(buff, MSIZE * MEGABYTE, NULL); // освобождение памяти
			MessageBox(0, L"Создать устройство не удалось", NULL, MB_OK | MB_ICONSTOP);
			return;
		}
		
		if (!WriteFile(hFile, buff, FileSize, &bytesread, NULL)) { // записать данные на устройство
			VirtualFree(buff, MSIZE * MEGABYTE, NULL); // освобождение памяти
			MessageBox(0, L"Чтение данных НЕ удалось", NULL, MB_OK | MB_ICONSTOP);
			return;
		}

		CloseHandle(hFile);
		AmountOfByte += FileSize;
	}

	cout << "\nСумма считанных байтов: " << dec << AmountOfByte << endl;
	VirtualFree(buff, MSIZE * MEGABYTE, NULL); // освобождение памяти
	system("pause");
}