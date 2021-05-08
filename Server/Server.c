#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include "Server.h"


HANDLE pedido;

TCHAR((*ptrMemoria)[BUFFERS][BUFFERSIZE]);
Movimento* DataCliente;



DWORD verificaJogador(TCHAR* strJogador)
{

	TCHAR auxJogador[15];
	DWORD  i;
	TCHAR* token[2], * nextToken;
	TCHAR* seps = TEXT(" ,\t\n");
	if (strlen(strJogador) == 0) {
		return;
	}

	i = 0;

	//	while (aviao[i].aviao_Id != 0 && i < max_planes)
	//		i++;

	//	_tcscpy_s(aviao[i].aviao_name, NameSize, *token);
	//	aviao[i].aviao_Id = _tstoi(*(token + 1));
	return 1;

	//		return 0;

}

DWORD WINAPI enviaDadosAoCliente()
{
	HANDLE PodeEscreverData = CreateSemaphore(NULL, BUFFERS, BUFFERS, TEXT("Sem?foro Pode Escrever Data"));
	HANDLE PodeLerData = CreateSemaphore(NULL, 0, BUFFERS, TEXT("Sem?foro Pode Ler Data"));
	HANDLE hT = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Movimento), TEXT("SharedMemoryData"));

	if (hT == NULL)
	{
		_tprintf(TEXT("[Erro]Cria??o FileMappingMemory do Windows(%d)\n"), GetLastError());
		return -1;
	}

	DataCliente = (Movimento*)MapViewOfFile(hT, FILE_MAP_WRITE, 0, 0, sizeof(Movimento));
	if (DataCliente == NULL)
	{
		_tprintf(TEXT("[Erro]Mapeamento da mem?ria partilhada(%d)\n"), GetLastError());
		return -1;
	}


	for (int i = 0;; i++)
	{
		//WaitForSingleObject(PodeEscreverData, INFINITE);

		Sleep(1000);
		//ReleaseSemaphore(PodeLerData, 1, NULL);
	}

	CloseHandle(DataCliente);
	CloseHandle(hT);
}

DWORD WINAPI receiveMessage(int numero_maximo_de_avioes)//SERVIDOR
{

	HANDLE PodeEscreverInfo = CreateSemaphore(NULL, BUFFERS, BUFFERS, TEXT("Sem?foro Pode Escrever Info"));
	HANDLE PodeLerInfo = CreateSemaphore(NULL, 0, BUFFERS, TEXT("Sem?foro Pode Ler Info"));
	// _AviaoMax = CreateSemaphore(NULL, numero_maximo_de_avioes, numero_maximo_de_avioes, TEXT("_AviaoMax"));

	if (PodeEscreverInfo == NULL || PodeLerInfo == NULL)
	{
		_tprintf(TEXT("[Erro]Cria??o de objectos do Windows(%d)\n"), GetLastError());
		return -1;
	}


	pedido = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(TCHAR[BUFFERS][BUFFERSIZE]), TEXT("SharedMemory"));
	if (pedido == NULL)
	{
		_tprintf(TEXT("[Erro]Cria??o FileMappingMemory do Windows(%d)\n"), GetLastError());
		return -1;
	}

	ptrMemoria = (TCHAR(*)[BUFFERS][BUFFERSIZE])MapViewOfFile(pedido, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(TCHAR[BUFFERS][BUFFERSIZE]));
	if (ptrMemoria == NULL)
	{
		_tprintf(TEXT("[Erro]Mapeamento da mem?ria partilhada(%d)\n"), GetLastError());
		return -1;
	}

	for (int i = 0;; ++i)
	{
		int flag;
		WaitForSingleObject(PodeLerInfo, INFINITE);
		//LER E ESCREVER
		if (flag = verificaJogador((*ptrMemoria)[i % 10]) == 1)
		{
			//CLIENTE VERIFICADO, ESCREVER NO BUFFER UM BOOLEAN PARA O CLIENTE SABER
			TCHAR string[BUFFERSIZE];
			_stprintf_s(string, BUFFERSIZE, TEXT("%s %d"), ((*ptrMemoria)[i % 10]), flag);
			_stprintf_s((*ptrMemoria)[i % BUFFERS], BUFFERSIZE, &string, i);
			_tprintf(TEXT("PASSOU\n"));
		}
		else
		{
			TCHAR string[BUFFERSIZE];
			_stprintf_s(string, BUFFERSIZE, TEXT("%s %d"), ((*ptrMemoria)[i % 10]), 0);
			_stprintf_s((*ptrMemoria)[i % BUFFERS], BUFFERSIZE, &string, i);
			_tprintf(TEXT("N?O PASSOU\n"));
		}

		ReleaseSemaphore(PodeEscreverInfo, 1, NULL);
	}

	UnmapViewOfFile(pedido);
	CloseHandle(PodeEscreverInfo);
	CloseHandle(PodeLerInfo);
	CloseHandle(ptrMemoria);
}

/// <summary>
///  to
/// </summary>
/// <param name="id"></param>
/// <param name="numero_max_aeroporto"></param>
/// <returns></returns>
DWORD aviaoDesapareceu(int id, int numero_max_aeroporto)
{
	int flag = 0;
	_tprintf(TEXT("[requestLogOff] O aviao Saiu!\n"));
	for (int i = 0; i < numero_max_aeroporto; i++)
	{
		if (aviao[i].aviao_Id == id)
		{

			_tprintf(TEXT("aviao %s com Id %d acabou de SAIR!\n"), aviao[i].aviao_name, aviao[i].aviao_Id);
			aviao[i].aviao_Id = -1;
			flag = 1;

		}

	}
	return flag;
}






// verifica se existe as RegestryKeys


DWORD __stdcall adminCommands(LPVOID p) {
	TCHAR input[100];
	int tempo, id_taxi;

	do {

		// system("cls");
		_tprintf(TEXT("\n[Controller] - Commands: "));
		_tscanf_s(TEXT("%99s"), input, 100);

		if (_tcscmp(input, TEXT("listpassegers")) == 0)
			_tprintf(TEXT("Por terminar \n"));
		else if (_tcscmp(input, TEXT("new_aeroporto")) == 0)
			_tprintf(TEXT("Por terminar \n"));
		else if (_tcscmp(input, TEXT("disable_or_enable_new_planes")) == 0)
			_tprintf(TEXT("Por terminar \n"));
		else if (_tcscmp(input, TEXT("disable_or_enable_new_passagers")) == 0)
			_tprintf(TEXT("Por terminar \n"));
		else if (_tcscmp(input, TEXT("listplanes")) == 0)
			_tprintf(TEXT("Por terminar \n"));
		else if (_tcscmp(input, TEXT("listaeroplanes")) == 0)
			_tprintf(TEXT("Por terminar \n"));


	} while (_tcscmp(input, TEXT("exit")) != 0);
	system("cls");
	return 0;
}

int _tmain(int argc, TCHAR* argv[])
{
#ifdef UNICODE
	_Check_return_ _setmode(_fileno(stdin), _O_WTEXT);
	_Check_return_ _setmode(_fileno(stdout), _O_WTEXT);
	_Check_return_ _setmode(_fileno(stderr), _O_WTEXT);
#endif

	DWORD i, ThreadId, valor, response;

	HANDLE hMutexHandle = CreateMutex(NULL, TRUE, L"my.mutex.name");
	HANDLE hTHREAD, hTHREAD_2;
	HANDLE hFileMap;
	boot_struct boot_values;

	response = Boot();
	boot_values._max_planes = num_de_avioes();
	boot_values._max_aeroports = num_aeroporto();

	if (response == 0 && boot_values._max_aeroports != 0 && boot_values._max_planes != 0) {

		//	hTHREAD = CreateThread(NULL, 0, adminCommands, NULL, 0, &valor);
		//	hTerminar = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)desligar, NULL, 0, NULL);

		// thread para ver se ha embate de avioes ?

	//	WaitForSingleObject(hTHREAD, INFINITE);  //Comandos Admin


	}


	HANDLE htDadosJogo = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)enviaDadosAoCliente, NULL, 0, NULL);
	receiveMessage(boot_values._max_planes);


	_tprintf(TEXT("Leaving controller\n"));
	ReleaseMutex(hMutexHandle);
	CloseHandle(hMutexHandle);

	//	CloseHandle(_AviaoMax);

	return 0;
}