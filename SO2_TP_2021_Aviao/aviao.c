#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include "../SO2_TP_2021_Control/biblioteca.h"
#include "../SO2_TP_DLL_2021/SO2_TP_DLL_2021.h"
Movimento* ptr_movimento, * ptr_movimento_ejetar;
//int terminar = 0;
//  Iniciar viagem (pode iniciar viagem sem embarcar ninguém).
// mandar outro destino quando chega ?
//  o piloto sai no aeroporto 

HANDLE hMutex;

//int estado_piloto = 0;
int tmpx = 1, flag = 1;

typedef int(__stdcall* doFunc)(int);

DWORD envia_movimento(void* aux)
{
	Autentication* auxs = (Autentication*)aux;

	//VARS PARA USAR NA DLL
	int cur_x, cur_y, flag = 0;
	int final_dest_x, final_dest_y;
	int next_x, next_y;
	int* pnext_x = &next_x;
	int* pnext_y = &next_y;
	int tmp = auxs->error;
	cur_x = auxs->AEROPORTO_ORIGEM_X; //AeroportoX 
	cur_y = auxs->AEROPORTO_ORIGEM_Y;; //AeroportoY
	final_dest_x = auxs->AEROPORTO_DESTINO_X; //AeroportoFinalX
	final_dest_y = auxs->AEROPORTO_DESTINO_Y; //AeroportoY
	DWORD  dwExitCode = 0;

	//Sleep(2000);


	while (flag == 0) {
		WaitForSingleObject(hMutex, INFINITE);
		if (auxs->terminar == 1 || auxs->estado_piloto == 1) {
			_tprintf(TEXT("11111m \n"));

			flag = 1;
		}
		ReleaseMutex(hMutex);

	}
	_tprintf(TEXT("[a iniciar a viagem \n"));

	while (flag == 1)
	{

		HANDLE PodeEscrever = CreateSemaphore(NULL, BUFFERS, BUFFERS, TEXT("Semáforo Pode Escrever Info"));
		HANDLE PodeLer = CreateSemaphore(NULL, 0, BUFFERS, TEXT("Semáforo Pode Ler Info"));
		WaitForSingleObject(hMutex, INFINITE);

		if (PodeEscrever == NULL || PodeLer == NULL)
		{
			_tprintf(TEXT("[Erro] Criação de objectos do Windows(%d)\n"), GetLastError());
			return -1;
		}

		HANDLE hT = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Movimento), TEXT("MovementMemory"));
		if (hT == NULL)
		{

			_tprintf(TEXT("[Erro] Criação FileMappingMemory do Windows(%d)\n"), GetLastError());
			return -1;

		}

		ptr_movimento = (Movimento*)MapViewOfFile(hT, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, sizeof(Movimento));
		if (ptr_movimento == NULL)
		{
			_tprintf(TEXT("[Erro] Mapeamento da memória partilhada(%d)\n"), GetLastError());
			return -1;
		}

		WaitForSingleObject(PodeEscrever, INFINITE);

		if (auxs->estado_piloto == 0) // a voar
		{
			ptr_movimento->sair = false;
			ptr_movimento->id = tmp;
			_tprintf(TEXT("[Erro] Mptr_movimento->id da(%d)\n"), ptr_movimento->id);

			int f = move(cur_x, cur_y, final_dest_x, final_dest_y, pnext_x, pnext_y);

			if (f == 0) {
				_tprintf(TEXT("\n[DEBUG] Avião chegou ao destino!\n"));
				ptr_movimento->chegou_ao_destino = true;
				flag = 0;

				TerminateThread(auxs->id_of_other_thread, dwExitCode);
				_tprintf(TEXT("\n[DEBUG]dwExitCode %d!\n"), dwExitCode);

			}

			if (f == 1) {
				_tprintf(TEXT("\n[DEBUG] Next position is X:%d Y:%d"), next_x, next_y);
				cur_x = next_x;
				cur_y = next_y;
				//	Sleep(10000);
			}

			if (f == 2) {
				_tprintf(TEXT("[DEBUG] move() returned 2"));
			}

			ptr_movimento->x = cur_x;
			ptr_movimento->y = cur_y;
		}
		else
		{
			_tprintf(TEXT("OPERACAO DE ptr_movimento)\n"));

			ptr_movimento->sair = true;
			flag = 0;
		}
		ReleaseSemaphore(PodeLer, 1, NULL);
		//		ReleaseMutex(hMutex);


		//		if (flag == 0) // CASO AVIAO CHEGUE AO DESTINO SAI
		//		{
				//	tmpx = 0;
				//	_tprintf(TEXT(" ESTADO DO estado_piloto E DE 0 !!!!!! \n"));
				//	auxs->estado_piloto = 1;
				//	}


		ReleaseMutex(hMutex);
		UnmapViewOfFile(ptr_movimento);
		CloseHandle(PodeEscrever);
		CloseHandle(PodeLer);
		CloseHandle(hT);

	}

	_tprintf(TEXT("A BAZAR DO MOVIMENTO! \n"));
	return 0;
}

DWORD timer(void* param)
{

	Autentication* dados = (Autentication*)param;

	_tprintf(TEXT("\n \n \n WDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD tempo de espera para voar e de %d: "), dados->tempo_de_aguardar);
	WaitForSingleObject(hMutex, INFINITE);

	if (dados->terminar == 1) // verifico se o piloto escreveu antes de sequer a contagem 
	{
		return 0;
	}

	int num = dados->tempo_de_aguardar;
	ReleaseMutex(hMutex);
	_tprintf(TEXT("\n \n |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\AC DORMIR E ||||| "));

	Sleep(num);
	/*	int x = 0, antes =0, depois, reach;
		antes = get_time();
		while (x==0)
		{
			depois =  get_time();
			reach = depois - antes;
			if (reach >= dados->tempo_de_aguardar) {
				x = 1;
			}
		}
	*/
	//	WaitForSingleObject(hMutexTimer, INFINITE);
	WaitForSingleObject(hMutex, INFINITE);
	if (dados->terminar == 1)
	{
		_tprintf(TEXT("\n \n +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ \ACORDARAM ME ||||| "));

		return 0;
	}
	dados->estado_piloto = 0;
	dados->terminar = 1;
	ReleaseMutex(hMutex);
	_tprintf(TEXT("\n \n \nACORDEI d: "));

	return 0;

	//	ReleaseMutex(hMutexTimer);

}

Autentication login(Request avioneta, int maximo_avioes) //Novo aviao
{
	Autentication tmp;
	DWORD result = 1;


	HANDLE PodeEscrever = CreateSemaphore(NULL, maximo_avioes, maximo_avioes, TEXT("SemáforoEscrita"));
	HANDLE PodeLer = CreateSemaphore(NULL, 0, maximo_avioes, TEXT("SemáforoLeitura"));

	//	_tprintf(TEXT("[INFO]  Process ID: %d (avioneta.Id)\n"), avioneta.Id);
//	_tprintf(TEXT("[INFO]   Avião ID: %d\n"), avioneta.aviao.aviao_Id);
//	_tprintf(TEXT("[INFO] Nome Avião: %s\n"), avioneta.aviao.aviao_name);

	//	_AviaoMax = CreateSemaphore(NULL, 6, 6, TEXT("TaxiMax"));

	if (PodeEscrever == NULL || PodeLer == NULL)
	{
		_tprintf(TEXT("[Erro] Criação de objectos do Windows(%d)\n"), GetLastError());
		tmp.error = 1;
	}

	HANDLE hT = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Request), TEXT("SharedMemory"));
	if (hT == NULL)
	{
		_tprintf(TEXT("[Erro] Criação FileMappingMemory do Windows(%d)\n"), GetLastError());
		tmp.error = 1;
	}

	PtrMemoria = (Request*)MapViewOfFile(hT, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, sizeof(Request));
	if (PtrMemoria == NULL)
	{
		_tprintf(TEXT("[Erro] Mapeamento da memória partilhada(%d)\n"), GetLastError());
		tmp.error = 1;
	}

	//	_tprintf(TEXT("[SENDING] A registar o Avião em Controlador: %s\n"), avioneta.aviao.aviao_name);

	WaitForSingleObject(PodeEscrever, INFINITE);

	//	wcscpy(PtrMemoria->aviao->aviao_name, avioneta.aviao->aviao_name);
	PtrMemoria->aviao.aviao_Id = avioneta.aviao.aviao_Id;
	PtrMemoria->aviao.capacidade = avioneta.aviao.capacidade;
	PtrMemoria->aviao.tempo_de_espera = avioneta.aviao.tempo_de_espera;

	PtrMemoria->autentication.AEROPORTO_DESTINO_X = avioneta.autentication.AEROPORTO_DESTINO_X;
	PtrMemoria->autentication.AEROPORTO_DESTINO_Y = avioneta.autentication.AEROPORTO_DESTINO_Y;
	PtrMemoria->autentication.AEROPORTO_ORIGEM_X = avioneta.autentication.AEROPORTO_ORIGEM_X;
	PtrMemoria->autentication.AEROPORTO_ORIGEM_Y = avioneta.autentication.AEROPORTO_ORIGEM_Y;

	_tcscpy_s(PtrMemoria->aviao.aviao_name, BUFFERSIZE, avioneta.aviao.aviao_name);
	//	_tprintf(TEXT("[A COPIA DO Avião: %s\n"), PtrMemoria->aviao.aviao_name);
	_tcscpy_s(PtrMemoria->aviao.original_aeorporto, BUFFERSIZE, avioneta.aviao.original_aeorporto);
	_tcscpy_s(PtrMemoria->aviao.next_destionation, BUFFERSIZE, avioneta.aviao.next_destionation);

	//operacoes de leitura

	ReleaseSemaphore(PodeLer, 1, NULL);
	Sleep(1000);

	_tprintf(TEXT("OPERACAO LEITURA   PtrMemoria->autentication.AEROPORTO_DESTINO_X; %d \n"), PtrMemoria->autentication.AEROPORTO_DESTINO_X);
	_tprintf(TEXT(" \n PtrMemoria->tipo == %d \n"), PtrMemoria->tipo);
	//Sleep(90000);

	if (PtrMemoria->tipo == 2) {
		_tprintf(TEXT(" \n Verifique valores do aviao ! Valores nao aceites	\n"));

		tmp.autenticado = false;
		tmp.server_disable_comms = false;
		return tmp;

	}

	if (PtrMemoria->tipo == 3) {

		_tprintf(TEXT(" \n SERVIDOR NAO ESTA ACEITAR MAIS CONEXOES \n"));
		// este qwe
		//	Sleep(50000);
		tmp.autenticado = false;
		tmp.server_disable_comms = true;
		return tmp;

	}

	tmp.AEROPORTO_DESTINO_X = PtrMemoria->autentication.AEROPORTO_DESTINO_X;
	tmp.AEROPORTO_DESTINO_Y = PtrMemoria->autentication.AEROPORTO_DESTINO_Y;

	tmp.AEROPORTO_ORIGEM_X = PtrMemoria->autentication.AEROPORTO_ORIGEM_X;
	tmp.AEROPORTO_ORIGEM_Y = PtrMemoria->autentication.AEROPORTO_ORIGEM_Y;

	tmp.autenticado = true;
	tmp.server_disable_comms = false;

	UnmapViewOfFile(PtrMemoria);
	CloseHandle(PodeEscrever);
	CloseHandle(PodeLer);
	CloseHandle(hT);
	return tmp;
}



DWORD WINAPI PilotoCMD(void* param)
{
	Autentication* dados = (Autentication*)param;

	TCHAR input[100];
	int tempo, id_taxi, flag = 0;
	DWORD  dwExitCode = 0;

	do {

		//  	WaitForSingleObject(hMutex, INFINITE);

		// system("cls");
	// 	if (dados->estado_piloto == 1) {

	// 		flag = 1;
	// 		ReleaseMutex(hMutex);

	// 	}
		_tprintf(TEXT("\n[PILOTO ESCREVA exit PARA EJETAR: "));
		_tscanf_s(TEXT("%99s"), input, 100);

		if (_tcscmp(input, TEXT("voar")) == 0) {
			WaitForSingleObject(hMutex, INFINITE);

			_tprintf(TEXT("\n[PILOTO ESCREVA voar voar voar: "));

			if (dados->terminar == 1)
			{
				_tprintf(TEXT("\n \n \ja a voar !!!!! "));

			}
			else
			{
				_tprintf(TEXT("\n \n \reparanda xxxxxxx "));

				dados->terminar = 1; // PARA VOAR
				dados->estado_piloto = 0;

			}

			//	WaitForSingleObject(hMutexTimer, INFINITE);
			//	ReleaseMutex(hMutexTimer);
			_tprintf(TEXT("\n[PILOTO ESCREVA 2 2 2: "));
			ReleaseMutex(hMutex);

		}
		else if (_tcscmp(input, TEXT("exit")) == 0) {

			WaitForSingleObject(hMutex, INFINITE);
			flag = 1;
			dados->terminar = 1;
			dados->estado_piloto = 1;
			TerminateThread(dados->paraterminar_o_sleep, dwExitCode);
			_tprintf(TEXT("\n[DEBUG]dwExitCode %d!\n"), dwExitCode);
			ReleaseMutex(hMutex);

		}
		_tprintf(TEXT("\n[.]"));
		_tprintf(TEXT("\n[.:]"));
		_tprintf(TEXT("\n[.:.]"));
		_tprintf(TEXT("\n[ :.]"));
		_tprintf(TEXT("\n[ .]"));
		//	_tprintf(TEXT("\n »»»»»ptrMemoria->permite_entrada %d »»»»» \n"), flag);

		//	ReleaseMutex(hMutex);

	} while (flag == 0);

	_tprintf(TEXT("\n \\\\\\\\\\\\\ aaaaaaaaIT \\\\\\)\n"));

	//	WaitForSingleObject(hMutex, INFINITE);
	//	estado_piloto = 1;

//		ReleaseMutex(hMutex);
	_tprintf(TEXT("\n \\\\\\\\\\\\\ Obbbbbbbbbbbbbbbbbbbbbbbff \\\\\\)\n"));

	//	system("cls");
	return 0;
}


int _tmain(int argc, TCHAR* argv[]) {
#ifdef UNICODE
	_Check_return_ _setmode(_fileno(stdin), _O_WTEXT);
	_Check_return_ _setmode(_fileno(stdout), _O_WTEXT);
	_Check_return_ _setmode(_fileno(stderr), _O_WTEXT);
#endif

	int x, aviao_valor_registry = 0;
	//	WaitForSingleObject(Semaforo_PlanesMax, INFINITE);
	int  response = 0, volante_terminal;
	Autentication aux;
	boot_struct boot_values;
	boot_values._max_planes = num_de_avioes();
	boot_values._max_aeroports = num_aeroporto();
	HANDLE  handle_volante_terminal;

	if (response == 0 && boot_values._max_aeroports != 0 && boot_values._max_planes != 0) {

		Request novo_pedido;
		novo_pedido.Id = GetCurrentProcessId();
		novo_pedido.aviao.aviao_Id = novo_pedido.Id;
		_tprintf(TEXT("aux.error = novo_pedido.aviao.aviao_Id; %d.\n"), novo_pedido.aviao.aviao_Id);

		Semoforo_max_planes = CreateSemaphore(NULL, boot_values._max_planes, boot_values._max_planes, TEXT("SO2_SEM"));

		if (Semoforo_max_planes == NULL) {

			_tprintf(TEXT("Erro no CreateSemaphore.\n"));
			return 1;

		}

		_tprintf(TEXT("A aguardar por uma vaga	\n"));
		WaitForSingleObject(Semoforo_max_planes, INFINITE);
		_tprintf(TEXT("****** Bem vindo piloto ******\n"));

		//espera ate o haver vagas para a criacao de avioes e depois entr
		//WaitForSingleObject(_AviaoMax, INFINITE);

		_tprintf(TEXT("\nDigite a capacidade max:"));
		_tscanf_s(TEXT("%99d"), &novo_pedido.aviao.capacidade);

		_tprintf(TEXT("\nDigite o tempo de espera por passageiros:"));
		_tscanf_s(TEXT("%99d"), &novo_pedido.aviao.tempo_de_espera);
		novo_pedido.aviao.tempo_de_espera = novo_pedido.aviao.tempo_de_espera * 10 * 10 * 10;
		_tprintf(TEXT("[DEBUG] novo_pedido.aviao.tempo_de_espera %d )\n"), novo_pedido.aviao.tempo_de_espera);

		_tprintf(TEXT("\nDigite o nome do aviao:"));
		_tscanf_s(TEXT("%99s"), novo_pedido.aviao.aviao_name, TAM);

		_tprintf(TEXT("\nDigite o aeroporto destino:"));
		_tscanf_s(TEXT("%99s"), novo_pedido.aviao.next_destionation, TAM);

		_tprintf(TEXT("\nDigite o nome do aeroporto:"));
		_tscanf_s(TEXT("%99s"), novo_pedido.aviao.original_aeorporto, TAM);

		_tprintf(TEXT("[DEBUG] Process ID: %d (novo_pedido.Id)\n"), novo_pedido.Id);

		novo_pedido.aviao.ground = true;
		novo_pedido.aviao.exit = false;
		novo_pedido.autentication.AEROPORTO_DESTINO_X = 0;
		novo_pedido.autentication.AEROPORTO_DESTINO_Y = 0;
		novo_pedido.autentication.AEROPORTO_ORIGEM_X = 0;
		novo_pedido.autentication.AEROPORTO_ORIGEM_Y = 0;
		//aux.estado_piloto = 0;
		aux = login(novo_pedido, boot_values._max_planes);

		hMutex = CreateMutex(NULL, FALSE, TEXT("The_Access"));

		if (hMutex == NULL)
		{
			_tprintf(TEXT("\nERRO na criacao do mutex"));

			return 0;

		}
		//	hMutexTimer = CreateMutex(NULL, FALSE, TEXT("ParaTempo"));
		//	if (hMutexTimer == NULL)
		//	{
		//		_tprintf(TEXT("\ERRO na criacao do mutex"));
		//		return 0;
		//	}

		if (aux.autenticado == true)
		{
			aux.error = novo_pedido.aviao.aviao_Id;

			aux.tempo_de_aguardar = novo_pedido.aviao.tempo_de_espera;
			aux.terminar = 0;
			_tprintf(TEXT("[RECEIVED LOGIN] Avião foi registado  %d \n"), aux.tempo_de_aguardar);

			aux.paraterminar_o_sleep = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)timer, &aux, 0, NULL);
			aux.id_of_other_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PilotoCMD, &aux, 0, NULL);


			_tprintf(TEXT("[SENDING] Reporting my Info to Control!\n"));


			WaitForSingleObject(aux.paraterminar_o_sleep, INFINITE);
			_tprintf(TEXT("[DEBUG] a ir para os  envia_movimento()!\n"));

			HANDLE h_movimento = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)envia_movimento, &aux, 0, NULL);
			WaitForSingleObject(h_movimento, INFINITE);
			ReleaseSemaphore(Semoforo_max_planes, 1, NULL);
			Sleep(9000);

			_tprintf(TEXT("[DEBUG] Terminated envia_movimento()!\n"));
			// este qwe
			CloseHandle(hMutex);
			//	CloseHandle(hMutexTimer);

		}
		Sleep(9000);
	}
	return 0;
}
