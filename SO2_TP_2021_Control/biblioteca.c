#define MAXPLAYERS 20
#define TAM 200
#define NameSize 15
#include "biblioteca.h"

int convertTimeToSec(SYSTEMTIME time) {
	int tempo = time.wSecond + (time.wMinute * 60) + (time.wHour * 60 * 60);
	return tempo;
}
int get_time() {
	SYSTEMTIME st;
	GetSystemTime(&st);
	_tprintf(TEXT("\nThe system time is: %02d:%02d:%02d\n"), st.wHour, st.wMinute, st.wSecond);
	return convertTimeToSec(st);
}


int Boot() { // check & create the Regystrkeys for the maximum number of planes and airports

	HKEY   hKey, h_second_Key;
	DWORD regFlag;
	LPTSTR Planes_amount, Aipor_amount;
	int queAconteceu, response_1, response_2;
	boot_struct tmp;

	tmp._max_aeroports = max_aeroports;
	tmp._max_planes = max_planes;

	if (tmp._max_aeroports <= 0 || tmp._max_planes <= 0) {
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
			printf("[ERROR] Key not found.\n");
			return 0;
		}
		else {

			printf("[ERROR] Failed to open Key.\n");
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
			printf("[ERROR] Key not found.\n");
			return 0;
		}
		else {
			printf("[ERROR] Failed to open key.\n");
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


