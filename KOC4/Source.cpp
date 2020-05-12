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

	DWORD bytesread; // ��������������� ������
	HANDLE hFile; // ������������� ���������� ��� ��������� �����
	int	AmountOfByte = 0; // ����� ��������� ������ 

	cout << "�������������� " << MSIZE << " �����" << endl;
	PBYTE buff = (PBYTE)VirtualAlloc(NULL, // ����� �� �������� ���������� �������� ������
		MSIZE *MEGABYTE,  // ���������� ����, ������� ���������� ��������
		MEM_RESERVE,	  // �������������� ��������� �������
		PAGE_READWRITE); // ���������� ������, ��� ��� ������, ��� � ��� ������.

	if (buff) cout << "���� ������ @" << hex << (size_t)buff << "\n";
	else { // ���� �������� ������ ��� ��������� ������
		cout << "������ ��������� ������!\n";
		system("pause");
		return;
	}

	cout << "��������� ������" << endl << endl;
	for (int i = 1; i < 21; ++i) {
		string path = "�������\\�������" + to_string(i) + ".bin";  // ���� � �����
		wstring drive_name = L"�������\\�������" + to_wstring(i) + L".bin"; // ���� � �����
		
		hFile = CreateFile( // �������� ����� ��� ������
			drive_name.data(), // ��� �����
			GENERIC_READ, // ����� �������
			FILE_SHARE_READ, // ���������� ������
			NULL, // ���������� ������
			OPEN_EXISTING, // ��������� �������� 
			0, // �������� �����
			NULL); // ������������� ����� � ����������
				   
		int FileSize = GetFileSize( // ����������� ������� ����� � ������
			hFile, // ���������� �����
			NULL); // ������� ����� ������� �����. NULL �.�. �� �����

		if (hFile == INVALID_HANDLE_VALUE) { // ���� �������� ��� ������ 
			VirtualFree( // ������������ ������
				buff,	   // ����� ������� ������
				MSIZE *MEGABYTE, // ��� ������ (� ������)
				NULL);			 // ��� ������������ ������
			MessageBox(0, L"������� ���������� �� �������", NULL, MB_OK | MB_ICONSTOP);
			return;
		}
		
		SetFilePointer( // ���������� ��������� ����� ������ � ����� �� ������ �����
			hFile, // ���������� �����
			0, 0, // ����� ����������� ���������
			FILE_BEGIN); // ����� �������

		// �������� ������ ��� ���������� ������
		cout << "���� ������ @" << hex <<
			(size_t)VirtualAlloc(buff + AmountOfByte, FileSize, MEM_COMMIT, PAGE_READWRITE)
			<< "\n"; 
		
		if (!ReadFile(hFile, buff, FileSize, &bytesread, NULL)) { // ������ ������ � ����������
			VirtualFree(buff, MSIZE * MEGABYTE, NULL); // ������������ ������
			MessageBox(0, L"������ ������ �� �������", NULL, MB_OK | MB_ICONSTOP);
			return;
		}
		
		CloseHandle(hFile); // ������� ���� (����������) 
		wstring newFile_name = L"���������������\\��������������" + to_wstring(i) + L".bin"; // ���� � �����

		// �������� ����� ��� ������
		hFile = CreateFile(newFile_name.data(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);

		if (hFile == INVALID_HANDLE_VALUE) {// ���� �������� ��� ������
			VirtualFree(buff, MSIZE * MEGABYTE, NULL); // ������������ ������
			MessageBox(0, L"������� ���������� �� �������", NULL, MB_OK | MB_ICONSTOP);
			return;
		}
		
		if (!WriteFile(hFile, buff, FileSize, &bytesread, NULL)) { // �������� ������ �� ����������
			VirtualFree(buff, MSIZE * MEGABYTE, NULL); // ������������ ������
			MessageBox(0, L"������ ������ �� �������", NULL, MB_OK | MB_ICONSTOP);
			return;
		}

		CloseHandle(hFile);
		AmountOfByte += FileSize;
	}

	cout << "\n����� ��������� ������: " << dec << AmountOfByte << endl;
	VirtualFree(buff, MSIZE * MEGABYTE, NULL); // ������������ ������
	system("pause");
}