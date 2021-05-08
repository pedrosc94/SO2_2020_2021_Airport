#include "pch.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "DLL.h"

Request a;
Aviao airplane;
Movimento* DataCliente, aux;

int terminar = 0;

int  Boot() { // check & create the Regystrkeys for the maximum number of planes and airports

	HKEY   hKey, h_second_Key;
	DWORD regFlag;
	LPTSTR Planes_amount, Aipor_amount;
	int queAconteceu, response_1, response_2;
	boot_struct tmp;

	tmp._max_aeroports = max_aeroports;
	tmp._max_planes = max_planes;

	if (max_aeroports <= 0 || max_planes <= 0) {

		_tprintf(TEXT("Please change the max aumount of aeroplanes/aeroportos)\n"));
		return 1;

	}

	if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Test"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &regFlag) != ERROR_SUCCESS)
	{

		_tprintf(TEXT("[REGISTRY] Error Creating Registry Key (%d)\n"), GetLastError());
		return 1;

	}

	_tprintf(TEXT("[REGISTRY] Registry Created Software\\Test\n"));

	if (regFlag == REG_CREATED_NEW_KEY)
	{

		response_1 = RegSetValueEx(hKey, L"AVIAO", NULL, REG_DWORD, (LPBYTE)&tmp._max_planes, sizeof(tmp._max_planes));
		response_2 = RegSetValueEx(hKey, L"AEROPORTO", NULL, REG_DWORD, (LPBYTE)&tmp._max_aeroports, sizeof(tmp._max_planes));

		_tprintf(TEXT("[REGISTRY] NEW KEY CREATED\n"));

		if (response_1 != ERROR_SUCCESS || response_2 != ERROR_SUCCESS) {

			_tprintf(TEXT("Registry creation failed with error 	. (% d)\n"), GetLastError());
			return 1;

		}

	}

	for (int i = 0; i < tmp._max_planes; i++)
	{
		aviao[i].aviao_Id = 0;
		aviao[i].capacidade = 0;
	}



	RegCloseKey(hKey);

	return 0;
}

int num_de_avioes() {

	int _max_planes = 0;
	HKEY hKey;
	int  numero_de_avioes, numero_de_aeroportos, resposta;
	int auxiliar1 = sizeof(numero_de_aeroportos);

	resposta = Boot();
	LONG lResult;
	TCHAR chave_nome[200] = TEXT("Software\\Test");
	HANDLE hMapFile;				//Guardar o Handle retornado pelo CreateFileMapping


	lResult = RegOpenKeyEx(HKEY_CURRENT_USER, chave_nome, 0, KEY_ALL_ACCESS, &hKey);

	if (lResult != ERROR_SUCCESS) {

		if (lResult == ERROR_FILE_NOT_FOUND) {
			printf("Key not found.\n");
			return 0;

		}
		else {

			printf("Error opening key.\n");
			return 0;

		}
	}

	if (RegQueryValueEx(hKey, TEXT("AVIAO"), 0, NULL, &numero_de_avioes, &auxiliar1) != ERROR_SUCCESS) {

		_tprintf(TEXT("Atributo  numero de avioes m?ximo n?o foi encontrado! ERRRO!\n"));
		RegCloseKey(hKey);
		return 0;

	}

	else {

		RegCloseKey(hKey);
		return numero_de_avioes;

	}



}





int num_aeroporto() {

	HKEY hKey;
	int  numero_de_avioes, numero_de_aeroportos, resposta, _max_aeroports = 0;
	int auxiliar1 = sizeof(numero_de_aeroportos);

	resposta = Boot();
	LONG lResult;
	TCHAR chave_nome[200] = TEXT("Software\\Test");
	HANDLE hMapFile;				//Guardar o Handle retornado pelo CreateFileMapping


	lResult = RegOpenKeyEx(HKEY_CURRENT_USER, chave_nome, 0, KEY_ALL_ACCESS, &hKey);

	if (lResult != ERROR_SUCCESS) {

		if (lResult == ERROR_FILE_NOT_FOUND) {
			printf("Key not found.\n");
			return 0;

		}
		else {

			printf("Error opening key.\n");
			return 0;

		}
	}

	if (RegQueryValueEx(hKey, TEXT("AEROPORTO"), 0, NULL, &numero_de_aeroportos, &auxiliar1) != ERROR_SUCCESS) {

		_tprintf(TEXT("Atributo numero de aeroportos m?ximo n?o foi encontrado! ERRRO!\n"));
		RegCloseKey(hKey);
		return 0;

	}

	else {

		_tprintf(TEXT("Atributo encontrado com valor:%d e tamanho:%d\n"), numero_de_aeroportos, auxiliar1);
		RegCloseKey(hKey);
		return numero_de_aeroportos;

	}

}

void gotoxy(int x, int y)
{
	static HANDLE hStdout = NULL;
	COORD coord;

	coord.X = x;
	coord.Y = y;

	if (!hStdout)
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(hStdout, coord);
}

DWORD WINAPI recebeDados()
{
	int i = 1;
	TCHAR* token[4];
	TCHAR* seps = TEXT(" ,\t\n");
	HANDLE PodeEscrever = CreateSemaphore(NULL, 0, BUFFERS, TEXT("Sem?foro Pode Escrever Data"));
	HANDLE PodeLer = CreateSemaphore(NULL, 0, BUFFERS, TEXT("Sem?foro Pode Ler Data"));

	HANDLE hT = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Movimento), TEXT("SharedMemoryData"));
	if (hT == NULL)
	{
		_tprintf(TEXT("[Erro]Cria??o FileMappingMemory do Windows(%d)\n"), GetLastError());
		return;
	}

	DataCliente = (Movimento*)MapViewOfFile(hT, FILE_MAP_READ, 0, 0, sizeof(Movimento));
	if (DataCliente == NULL)
	{
		_tprintf(TEXT("[Erro]Mapeamento da mem?ria partilhada(%d)\n"), GetLastError());
		return;
	}
	Sleep(1000);

	while (terminar == 0)
	{
		//WaitForSingleObject(PodeEscrever, INFINITE);
		aux.direction = DataCliente->direction;
		aux.speed = DataCliente->speed;
		aux.x = DataCliente->x;
		aux.y = DataCliente->y;

		system("cls");
		gotoxy(aux.x, aux.y);
		_tprintf(TEXT("O"));
		Sleep(1000);
		//ReleaseSemaphore(PodeLer, 1, NULL);
	}

	UnmapViewOfFile(DataCliente);
	CloseHandle(hT);
	return 0;
}

DWORD login(Request avioneta, int maximo_avioes) //Novo aviao
{


	_tprintf(TEXT("jjjjjjjjjj %s %d"), avioneta.aviao->aviao_name, avioneta.Id);
	HANDLE PodeEscrever = CreateSemaphore(NULL, BUFFERS, BUFFERS, TEXT("Sem?foro Pode Escrever Info"));
	HANDLE PodeLer = CreateSemaphore(NULL, 0, BUFFERS, TEXT("Sem?foro Pode Ler Info"));
	//	_AviaoMax = CreateSemaphore(NULL, 6, 6, TEXT("TaxiMax"));


	if (PodeEscrever == NULL || PodeLer == NULL)
	{
		_tprintf(TEXT("[Erro]Cria??o de objectos do Windows(%d)\n"), GetLastError());
		return -1;
	}

	TCHAR(*PtrMemoria)[BUFFERS][BUFFERSIZE];

	HANDLE hT = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(TCHAR[BUFFERS][BUFFERSIZE]), TEXT("SharedMemory"));
	if (hT == NULL)
	{
		_tprintf(TEXT("[Erro]Cria??o FileMappingMemory do Windows(%d)\n"), GetLastError());
		return -1;
	}

	PtrMemoria = (TCHAR(*)[BUFFERS][BUFFERSIZE])MapViewOfFile(hT, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, sizeof(TCHAR[BUFFERS][BUFFERSIZE]));
	if (PtrMemoria == NULL)
	{
		_tprintf(TEXT("[Erro]Mapeamento da mem?ria partilhada(%d)\n"), GetLastError());
		return -1;
	}

	TCHAR string[BUFFERSIZE];
	_stprintf_s(string, BUFFERSIZE, TEXT("%s %d"), &avioneta.aviao->aviao_name, &avioneta.Id);

	WaitForSingleObject(PodeEscrever, INFINITE);
	_stprintf_s((*PtrMemoria)[0], BUFFERSIZE, &string);

	ReleaseSemaphore(PodeLer, 1, NULL);
	Sleep(1000);

	//_tprintf(TEXT("Cliente(%s)\n"), (*PtrMemoria)[0]);


	Sleep(1000);

	UnmapViewOfFile(PtrMemoria);
	CloseHandle(PodeEscrever);
	CloseHandle(PodeLer);
	CloseHandle(hT);
}

DWORD recebeTecla()
{
	HANDLE htRecebeDados = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recebeDados, NULL, 0, NULL);

	while (1)
	{
		if (_gettch() > 0)
		{
			terminar = 1;
			CloseHandle(htRecebeDados);
			return 1;
		}
	}
	//	CloseHandle(_AviaoMax);
	return 0;
}
