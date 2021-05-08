#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include "Client.h"


int _tmain(int argc, TCHAR* argv[]) {
#ifdef UNICODE
	_Check_return_ _setmode(_fileno(stdin), _O_WTEXT);
	_Check_return_ _setmode(_fileno(stdout), _O_WTEXT);
	_Check_return_ _setmode(_fileno(stderr), _O_WTEXT);
#endif


	int aux, response = 0;
	boot_struct boot_values;

	boot_values._max_planes = num_de_avioes();
	boot_values._max_aeroports = num_aeroporto();

	if (response == 0 && boot_values._max_aeroports != 0 && boot_values._max_planes != 0) {

		Request novo_pedido;
		novo_pedido.Id = GetCurrentThreadId();
		novo_pedido.aviao->aviao_Id = GetCurrentThreadId();
		novo_pedido.Id = novo_pedido.aviao->aviao_Id;


		_tprintf(TEXT("******\ Bem vindo pilto /******\n"));

		//espera ate o haver vagas para a criacao de avioes e depois entr

	//	WaitForSingleObject(_AviaoMax, INFINITE);


		_tprintf(TEXT("\n Digite a capacidade max:"));
		_tscanf_s(TEXT("%99d"), &novo_pedido.aviao->capacidade);

		_tprintf(TEXT("\n Digite a velocidade:"));
		_tscanf_s(TEXT("%99d"), &novo_pedido.aviao->speed);

		//		_tprintf(TEXT("\n[WARNING] - Numero Invalido, tente escolher um numero inteiro nao nulo\n"));

		_tprintf(TEXT("\n Digite o nome do aviao:"));
		_tscanf_s(TEXT("%99s"), novo_pedido.aviao->aviao_name, BUFFERSIZE);

		_tprintf(TEXT("\n Digite o aeroporto destino:"));
		_tscanf_s(TEXT("%99s"), novo_pedido.aviao->next_destionation, BUFFERSIZE);

		_tprintf(TEXT("\n Digite o nome do aeroporto:"));
		_tscanf_s(TEXT("%99s"), novo_pedido.aviao->original_aeorporto, BUFFERSIZE);


		novo_pedido.Id = GetCurrentThreadId();
		novo_pedido.aviao->ground = true;
		novo_pedido.aviao->exit = false;

		aux = login(novo_pedido, boot_values._max_planes);


		if (aux == 1)
		{
			_tprintf(TEXT("\nPASSOU!\n"));
			if (recebeTecla() == 1)
			{
				system("cls");
				_tprintf(TEXT("\nACABOU!\n"));
				Sleep(1000);
				return 0;
			}
		}
		else
		{
			_tprintf(TEXT("\nNÃO PASSOU!\n"));
			Sleep(1000);

			return 0;
		}
		//Realse um dos semaforos para poder entrar um novo aviao.
	//	ReleaseSemaphore(_AviaoMax, 1, NULL);

		Sleep(INFINITE);
	}
}

