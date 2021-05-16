#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include "biblioteca.h"

int terminar = 0;

HANDLE pedido;
HANDLE hMutexControler;
Movimento* ptr_movimento;
Request* ptrMemoria;


void atualizaHorasUltimaCom(int id) {
	for (int i = 0; i < max_planes; i++) {
		if (aviao[i].aviao_Id == id) {
			aviao[i].tempo = get_time();
			_tprintf(TEXT("\n[DEBUG]Atualizei as horas da ultima comunicação com valor: %d"), aviao[i].tempo);
			Sleep(2000);
		}
	}

}

int initAviaoArr() { // depois mandaro numero de max_planes que ocorre pela leitura da regkey
	for (int i = 0; i < max_planes; i++) {
		aviao[i].aviao_Id = 0;
		aviao[i].capacidade = 0;

		_tcscpy_s(aviao[i].aviao_name, BUFFERSIZE, TEXT("NULL")); // mudei para BUFFERSIZE pq é o buff qe existe para o nome dos avioes
		_tcscpy_s(aviao[i].original_aeorporto, BUFFERSIZE, TEXT("NULL"));
		_tcscpy_s(aviao[i].next_destionation, BUFFERSIZE, TEXT("NULL"));

	}
	return 1;
}

int adicionaAviao(Request r) { // ver os ponteiros

	for (int i = 0; i < max_planes; i++) {
		if (aviao[i].aviao_Id == 0) {
			aviao[i].aviao_Id = r.aviao.aviao_Id;
			aviao[i].capacidade = r.aviao.capacidade;
			_tcscpy_s(aviao[i].aviao_name, BUFFERSIZE, r.aviao.aviao_name);
			_tcscpy_s(aviao[i].original_aeorporto, BUFFERSIZE, r.aviao.original_aeorporto);
			_tcscpy_s(aviao[i].next_destionation, BUFFERSIZE, r.aviao.next_destionation);
			return 1;
		}
	}
	return 0;
}

DWORD existeAviao(Request r) {
	for (int i = 0; i < max_planes; i++) {
		if (_tcscmp(aviao[i].aviao_name, r.aviao.aviao_name) == 0) { //acabar a condição, usar strcmp()
			return 1;
		}
	}
	return 0;
}

int apagaAviao(int ID) {
	_tprintf(TEXT("\n\t     ID: %d"), ID);

	for (int i = 0; i < max_planes; i++) {
		if (aviao[i].aviao_Id == ID) {
			aviao[i].aviao_Id = 0;
			aviao[i].capacidade = 0;
			_tcscpy_s(aviao[i].aviao_name, BUFFERSIZE, TEXT("NULL")); // mudei para BUFFERSIZE pq é o buff qe existe para o nome dos avioes
			_tcscpy_s(aviao[i].original_aeorporto, BUFFERSIZE, TEXT("NULL"));
			_tcscpy_s(aviao[i].next_destionation, BUFFERSIZE, TEXT("NULL"));
		}
	}
}

void listaAvioes() {

	for (int i = 0; i < max_planes; i++) {
		if (aviao[i].aviao_Id != 0) {

			_tprintf(TEXT("\nAvião: %s"), aviao[i].aviao_name);
			_tprintf(TEXT("\n\t     ID: %d"), aviao[i].aviao_Id);
			_tprintf(TEXT("\n\t  Capac: %d"), aviao[i].capacidade = 0);
			_tprintf(TEXT("\n\tInicial: %d"), aviao[i].original_aeorporto);
			_tprintf(TEXT("\n\t  Final: %d"), aviao[i].next_destionation);
		}
	}
}

void listaAeroportos() {
	_tprintf(TEXT("\n Lista de Aeroportos\n"));

	for (int i = 0; i < max_aeroports; i++) {
		if (aeroporto[i].aero_Id != 0) {
			_tprintf(TEXT("\nAeroporto ID: %d"), aeroporto[i].aero_Id);
			_tprintf(TEXT("\nNome Aeroporto: %s"), aeroporto[i].aero_Name);
			_tprintf(TEXT("\nAeroporto PosX: %d"), aeroporto[i].x);
			_tprintf(TEXT("\nAeroporto PosY: %d"), aeroporto[i].y);
		}
	}
}

int initAeroportoArr() {
	for (int i = 0; i < max_aeroports; i++) {
		aeroporto[i].aero_Id = 0;
		aeroporto[i].x = -1;
		aeroporto[i].y = -1;
		_tcscpy_s(aeroporto[i].aero_Name, BUFFERSIZE, TEXT("NULL"));
	}
	return 1;
}

Autentication enviaAeroportoCoords(TCHAR nome[BUFFERSIZE]) {
	Autentication a;
	for (int i = 0; i < max_aeroports; i++) {
		if (_tcscmp(aeroporto[i].aero_Name, nome) == 0) {
			a.AEROPORTO_ORIGEM_X = aeroporto[i].x;
			a.AEROPORTO_ORIGEM_Y = aeroporto[i].y;
			a.AEROPORTO_DESTINO_X = NULL;
			a.AEROPORTO_DESTINO_Y = NULL;
			a.autenticado = NULL;
			a.error = NULL;
			a.server_disable_comms = NULL;
			return a;
		}
	}
}


int adicionaAeroporto(Aeroporto a) {
	if (verificaAeroportoRange(a) == 1 && existeAeroporto(a) == 0) {
		for (int i = 0; i < max_aeroports; i++) {
			if (aeroporto[i].aero_Id == 0) {
				aeroporto[i].aero_Id = a.aero_Id;
				aeroporto[i].x = a.x;
				aeroporto[i].y = a.y;
				_tcscpy_s(aeroporto[i].aero_Name, BUFFERSIZE, a.aero_Name);
				return 1;
				_tprintf(TEXT("\nAeroporto adicionado com sucesso!\n"));
			}
		}
	}
	_tprintf(TEXT("\nAeroporto não foi criado!\n"));
	return 0;
}

int existeAeroporto(Aeroporto a) {
	for (int i = 0; i < max_aeroports; i++) {
		if (_tcscmp(aeroporto[i].aero_Name, a.aero_Name) == 0) {
			return 1;
		}
	}
	return 0;
}

int verificaAeroportoRange(Aeroporto a) {
	int n_aeroportos = 0;
	if (a.x >= 0 && a.x <= 1000 && a.y <= 1000 && a.y >= 0) {
		for (int i = 0; i < max_aeroports; i++) {
			if ((aeroporto[i].x + 10 <= a.x && aeroporto[i].x - 10 >= a.x) && (aeroporto[i].y + 10 <= a.y && aeroporto[i].y - 10 >= a.y)) {
				n_aeroportos++;
			}
		}
		if (n_aeroportos == 0) {
			return 1;
		}
	}
	return 0;
}


DWORD WINAPI recebeMovement()
{
	_tprintf(TEXT("[AQUI]\n"));

	HANDLE PodeEscreverInfo = CreateSemaphore(NULL, BUFFERS, BUFFERS, TEXT("Semáforo Pode Escrever Info"));
	HANDLE PodeLerInfo = CreateSemaphore(NULL, 0, BUFFERS, TEXT("Semáforo Pode Ler Info"));
	//	WaitForSingleObject(hMutexControler, INFINITE);


	if (PodeEscreverInfo == NULL || PodeLerInfo == NULL)
	{
		_tprintf(TEXT("[ERROR] Criação de objectos do Windows(%d)\n"), GetLastError());
		return -1;
	}

	hmap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Movimento), TEXT("MovementMemory"));
	if (hmap == NULL)
	{
		_tprintf(TEXT("[ERROR] Criação FileMappingMemory do Windows(%d)\n"), GetLastError());
		return -1;
	}

	while (terminar == 0) {


		ptr_movimento = (Movimento*)MapViewOfFile(hmap, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, sizeof(Movimento));
		if (ptr_movimento == NULL)
		{
			_tprintf(TEXT("[ERROR] Mapeamento da mem?ria partilhada(%d)\n"), GetLastError());
			return -1;
		}

		WaitForSingleObject(PodeLerInfo, INFINITE);

		if (ptr_movimento->sair == false) {
			_tprintf(TEXT("[RECEIVED MOVEMENT] ID:%d Dir:%d x:%d y:%d\n"), ptr_movimento->id,
				ptr_movimento->direction,
				ptr_movimento->x,
				ptr_movimento->y);


			ptr_movimento = (Movimento*)MapViewOfFile(hmap, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, sizeof(Movimento));
			if (ptr_movimento == NULL)
			{
				_tprintf(TEXT("[ERROR] Mapeamento da mem?ria partilhada(%d)\n"), GetLastError());
				return -1;
			}


			atualizaHorasUltimaCom(ptr_movimento->id);



			if (ptr_movimento->chegou_ao_destino == true) {


				atualizaHorasUltimaCom(ptr_movimento->id);


				if (ptr_movimento->chegou_ao_destino == true) {


					_tprintf(TEXT("\n \n [RECEIVED MOVEMENT] O AVIAO CHEGOU AO SEU DESTINO FINAL !\n"));
					apagaAviao(ptr_movimento->id);
					// aqui mudar no array o estado do aviao para a voar 
				}

			}
			else
			{
				// o piloto quer ejetar
				_tprintf(TEXT("\n\n[PILOTO SAIU] com id %d\n"), ptr_movimento->id);
				apagaAviao(ptr_movimento->id);

			}

			//	ReleaseMutex(hMutexControler);
			ReleaseSemaphore(PodeEscreverInfo, 1, NULL);
			UnmapViewOfFile(hmap);
			//	CloseHandle(PodeEscreverInfo);
			//	CloseHandle(PodeLerInfo);
			//	CloseHandle(ptr_movimento);

		}
		return 0;

	}
}

	DWORD WINAPI recebe_logins()//SERVIDOR
	{
		Request r_tmp;
		Aeroporto aer_temp;
		DWORD as;
		Autentication aut_temp;
		int response1, response2;
		SYSTEMTIME st;


		while (terminar == 0) {

			_tprintf(TEXT("\n[WAITING FOR LOGIN]\n"));

			HANDLE PodeEscreverInfo = CreateSemaphore(NULL, BUFFERS, BUFFERS, TEXT("SemáforoEscrita"));
			HANDLE PodeLerInfo = CreateSemaphore(NULL, 0, BUFFERS, TEXT("SemáforoLeitura"));

			if (PodeEscreverInfo == NULL || PodeLerInfo == NULL)
			{
				_tprintf(TEXT("[ERROR] Criação de objectos do Windows(%d)\n"), GetLastError());
				return -1;
			}

			pedido = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Request), TEXT("SharedMemory"));
			if (pedido == NULL)
			{
				_tprintf(TEXT("[ERROR] Criação FileMappingMemory do Windows(%d)\n"), GetLastError());
				return -1;
			}

			ptrMemoria = (Request*)MapViewOfFile(pedido, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(Request));
			if (ptrMemoria == NULL)
			{
				_tprintf(TEXT("[ERROR] Mapeamento da mem?ria partilhada(%d)\n"), GetLastError());
				return -1;
			}

			ptrMemoria->permite_entrada = true;

			WaitForSingleObject(PodeLerInfo, INFINITE);
			// aguarda um logins 

			r_tmp.aviao.tempo = get_time();

			r_tmp.aviao.aviao_Id = ptrMemoria->aviao.aviao_Id;
			r_tmp.aviao.capacidade = ptrMemoria->aviao.capacidade;

			_tcscpy_s(r_tmp.aviao.aviao_name, BUFFERSIZE, ptrMemoria->aviao.aviao_name);
			_tcscpy_s(r_tmp.aviao.original_aeorporto, BUFFERSIZE, ptrMemoria->aviao.original_aeorporto);
			_tcscpy_s(r_tmp.aviao.next_destionation, BUFFERSIZE, ptrMemoria->aviao.next_destionation);


			_tcscpy_s(aer_temp.aero_Name, BUFFERSIZE, ptrMemoria->aviao.original_aeorporto);
			response1 = existeAeroporto(aer_temp);

			_tcscpy_s(aer_temp.aero_Name, BUFFERSIZE, ptrMemoria->aviao.next_destionation);
			response2 = existeAeroporto(aer_temp);


			_tprintf(TEXT("[RECEIVED] Avião foi ***TENTA*** comunicar com o controlador  %s E O ptrMemoria->permite_entrada  ESTA %d\n"), ptrMemoria->aviao.aviao_name, ptrMemoria->permite_entrada);
			//	Sleep(1000);



			as = existeAviao(r_tmp);

			WaitForSingleObject(hMutexControler, INFINITE);
			//		WaitForSingleObject(hMutexControler, INFINITE);
			_tprintf(TEXT("\n[525425252452525 FOR LOGIN]\n"));

			if (ptrMemoria->permite_entrada == false) {

				_tprintf(TEXT("[DEBUG] MUDEI PARA NAO ACEITAR MAIS CONEXOES E ESTOU NO recebe_logins()\n"));
				ptrMemoria->tipo = 3; //  3- o controlador nao permite mais avioes por parte do admin
				ptrMemoria->autentication.AEROPORTO_DESTINO_X = 5;
			}
			else
			{


				if (as == 0 && response2 == 1 && response1 == 1) { // nao existe um aviao com aqueles parametros

					int creation = adicionaAviao(r_tmp);
					_tprintf(TEXT("\n[AVIAO FOI ADICIONADOO creation %d]\n", creation));

					_tprintf(TEXT("[RECEIVED LOGIN] Avião foi registado: %s com ID %d\n"), ptrMemoria->aviao.aviao_name, ptrMemoria->Id);
					ptrMemoria->tipo = 0;

					aut_temp = enviaAeroportoCoords(ptrMemoria->aviao.original_aeorporto);
					ptrMemoria->autentication.AEROPORTO_ORIGEM_X = aut_temp.AEROPORTO_ORIGEM_X;
					ptrMemoria->autentication.AEROPORTO_ORIGEM_Y = aut_temp.AEROPORTO_ORIGEM_Y;

					aut_temp = enviaAeroportoCoords(ptrMemoria->aviao.next_destionation);
					ptrMemoria->autentication.AEROPORTO_DESTINO_X = aut_temp.AEROPORTO_DESTINO_X;
					ptrMemoria->autentication.AEROPORTO_DESTINO_Y = aut_temp.AEROPORTO_DESTINO_Y;

				}

				else {

					_tprintf(TEXT("NÃO AUTENTICADO\n"));
					ptrMemoria->tipo = 2; //  2 - nao aceite por motivos de autenticacaoo

				}
			}
			ReleaseMutex(hMutexControler);
			ReleaseSemaphore(PodeEscreverInfo, 1, NULL);

			//ReleaseMutex(hMutexControler);

		}
		_tprintf(TEXT("NÃO adadadadadadadada\n"));

		UnmapViewOfFile(pedido);
		CloseHandle(PodeEscreverInfo);
		CloseHandle(PodeLerInfo);
		CloseHandle(ptrMemoria);
	}

	DWORD aviaoDesapareceu(int id, int numero_max_aeroporto)
	{
		int flag = 0;
		_tprintf(TEXT("[requestLogOff] O avião saiu!\n"));
		for (int i = 0; i < numero_max_aeroporto; i++)
		{
			if (aviao[i].aviao_Id == id)
			{
				_tprintf(TEXT("Avião %s com Id %d acabou de SAIR!\n"), aviao[i].aviao_name, aviao[i].aviao_Id);
				aviao[i].aviao_Id = -1;
				flag = 1;
			}
		}
		return flag;
	}


	DWORD __stdcall adminCommands(LPVOID p) {
		TCHAR input[100];
		int tempo, resposta = 0, resposta2 = 0;
		Aeroporto tmp_aero;
		tmp_aero.aero_Id = 1;
		int flag = 0;
		do {


			// system("cls");
			_tprintf(TEXT("\n[CONTROLLER] Command List: "));
			_tscanf_s(TEXT("%99s"), input, 100);

			if (_tcscmp(input, TEXT("listpassegers")) == 0)
				_tprintf(TEXT("Na atual iteracao do projeto, esta funcionalidade nao esta disponivel \n"));
			else if (_tcscmp(input, TEXT("new")) == 0) {

				// TODO ADICIONAR AS VARIAVEIS/ VALORES PARA O AEROPORTO
				_tprintf(TEXT("Digite o nome do aeroporto \n"));
				_tscanf_s(TEXT("%99s"), tmp_aero.aero_Name, TAM);

				_tprintf(TEXT("Digite a coordenado do X \n"));
				_tscanf_s(TEXT("%99d"), &tmp_aero.x);

				_tprintf(TEXT("Digite a coordenado do Y \n"));
				_tscanf_s(TEXT("%99d"), &tmp_aero.y);
				adicionaAeroporto(tmp_aero); // ADICIONAR A VAR

			}

			else if (_tcscmp(input, TEXT("enable")) == 0) {

				_tprintf(TEXT("\n »»»»» A permitir a entrada de novos avioes »»»»» \n"));
				WaitForSingleObject(hMutexControler, INFINITE);
				_tprintf(TEXT("\n »»»»»ptrMemoria->permite_entrada %d »»»»» \n"), ptrMemoria->permite_entrada);

				if (ptrMemoria->permite_entrada == true) {
					_tprintf(TEXT("\n««««« Condicao ja se encontrava previamente em vigor ««««« \n"));
				}
				else
				{
					ptrMemoria->permite_entrada = true;
					_tprintf(TEXT("\n««««« Operacacao sucedida ««««« \n"));

				}
				ReleaseMutex(hMutexControler);
			}

			else if (_tcscmp(input, TEXT("dis")) == 0) {


				_tprintf(TEXT("\n««««« A impedir a entrada de novos avioes %d ««««« \n"), ptrMemoria->permite_entrada);

				if (ptrMemoria->permite_entrada == false) {
					_tprintf(TEXT("\n««««« Condicao ja se encontrava previamente em vigor ««««« \n"));
				}
				else
				{
					_tprintf(TEXT("\n««««« fefefefefefefefef sucedida ««««« \n"));

					WaitForSingleObject(hMutexControler, INFINITE);
					ptrMemoria->permite_entrada = false;
					_tprintf(TEXT("\n««««« Operacacao sucedida ««««« \n"));

				}
				_tprintf(TEXT("\n««««« 3434344343434 sucedida ««««« \n"));

				ReleaseMutex(hMutexControler);

			}

			else if (_tcscmp(input, TEXT("planes")) == 0) {

				_tprintf(TEXT("\n««««« Operacacao listagem de avioes«««« \n"));
				listaAvioes();

			}
			else if (_tcscmp(input, TEXT("porto")) == 0) {

				_tprintf(TEXT("\n«««««  Operacacao listagem de Aeroportos ««««« \n"));
				listaAeroportos();

			}

			else if (_tcscmp(input, TEXT("exit")) == 0) {

				_tprintf(TEXT("\n   Operacacao  de saida   \n"));
				flag = 1;

			}

		} while (flag == 0);
		// ao sair o sistema vai permitir receber mais logins. 
		// depois ir ao vetor e os notificar mediante vai recebendo os seus movimentos  ( zera o id dos que foram ja avisados)
		// depois termina

		//system("cls");
		return 0;
	}

	int _tmain(int argc, TCHAR * argv[])
	{
#ifdef UNICODE
		_Check_return_ _setmode(_fileno(stdin), _O_WTEXT);
		_Check_return_ _setmode(_fileno(stdout), _O_WTEXT);
		_Check_return_ _setmode(_fileno(stderr), _O_WTEXT);
#endif

		DWORD i, ThreadId, valor, response;

		HANDLE hMutexHandle = CreateMutex(NULL, FALSE, L"my.mutex.name");
		HANDLE hTHREAD_admin;
		HANDLE hFileMap;
		boot_struct boot_values;
		int resposta = 0;

		resposta = Boot();
		_tprintf(TEXT("\n«««««  resposta ««««« %d \n"), resposta);

		boot_values._max_aeroports = num_aeroporto();
		boot_values._max_planes = num_de_avioes();

		// _tprintf(TEXT("Leaving controller %d %d\n"), boot_values._max_planes, boot_values._max_aeroports);

		if (resposta == 0 && boot_values._max_planes != 0 && boot_values._max_aeroports != 0) {
			initAviaoArr();
			initAeroportoArr();

			hMutexControler = CreateMutex(NULL, FALSE, NULL);
			HANDLE hThreadLogins = CreateThread(NULL, 0, recebe_logins, NULL, 0, NULL);
			hTHREAD_admin = CreateThread(NULL, 0, adminCommands, NULL, 0, &valor);
			HANDLE htDadosJogo = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recebeMovement, NULL, 0, NULL);

			//	HANDLE hthread_3seconds = CreateThread(NULL, 0, time_checker, NULL, 0, NULL);
			WaitForSingleObject(hThreadLogins, INFINITE);
			WaitForSingleObject(htDadosJogo, INFINITE);


			CloseHandle(PodeEscreverInfo);
			CloseHandle(PodeLerInfo);
			CloseHandle(hmap);
			ReleaseMutex(hMutexHandle);
			CloseHandle(hMutexHandle);
			_tprintf(TEXT("[SYSTEM] Terminating Controller!\n"));

		}

		//	_getch();
		_getch();

		return 0;
	}