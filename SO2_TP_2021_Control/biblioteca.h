#pragma once
#ifndef BIBLIOTECA_H_
#define BIBLIOTECA_H_

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <stdbool.h>

// variaveis globais
#define max_planes 5
#define max_aeroports 10
#define BUFFERSIZE 100
#define Buffer 5
#define TAM 200
#define Buffer 5
#define BUFFERSIZE 1000

#define BUFFERS 20
#define NomeMemoria TEXT("SharedMemory")
#define NomeFicheiro TEXT("RequestLogin.txt")


typedef struct {
	int aero_Id; // aeroporto id
	int x, y; //coordinates
	TCHAR aero_Name[BUFFERSIZE]; // name of the aeroporto
} Aeroporto;

typedef struct {
	int _max_planes, _max_aeroports; //coordinates

} boot_struct;

typedef struct {
	int x, y; //coordenadas
	int direction; //direcao movimento   1 - cima direita , 2 - baixo direita, 3 - esquerda baixo, 4 - cima esquerda
	int speed; //velocidade di avuai
	int id; //id do ultimo aviao
} Movimento;

typedef struct {
	int passeg_Id; // Passager id
	TCHAR passeg_name[BUFFERSIZE]; // name of the Passager
	TCHAR from_airoport_name[BUFFERSIZE]; //airport where the passenger is
	TCHAR to_airoport_name[BUFFERSIZE]; // airport where the passenger wants to go
	int awaits_x_seconds; //(optional) time the passenger waits for the plane
} Passager;

typedef struct {

	int aviao_Id; // 
	int capacidade;
	int speed; // speef of the plain
	int direcao; //direcao movimento   1 - cima direita , 2 - baixo direita, 3 - esquerda baixo, 4 - cima esquerda
	TCHAR aviao_name[BUFFERSIZE]; // name of the Passager
	TCHAR original_aeorporto[BUFFERSIZE];
	TCHAR next_destionation[BUFFERSIZE];
<<<<<<< Updated upstream

=======
	int  tempo_de_espera;
	int tempo;
>>>>>>> Stashed changes
	bool ground; // if is currently grounded (or flying)
	bool exit; // pilots ends
	Passager* passager;// passagers in the plain
	int tempo;

} Aviao;


typedef struct controller {
	Aeroporto* aeroporto;
	Aviao* aviao;
} Controller;

<<<<<<< Updated upstream
=======
typedef struct
{
	bool autenticado;
	bool server_disable_comms;
	int AEROPORTO_ORIGEM_X, AEROPORTO_ORIGEM_Y, AEROPORTO_DESTINO_X, AEROPORTO_DESTINO_Y;
	int error;
	HANDLE id_of_other_thread;
	HANDLE paraterminar_o_sleep;

	int terminar;
	int tempo_de_aguardar;
	int estado_piloto;
}Autentication;
>>>>>>> Stashed changes

typedef struct
{
	int Id; // aviao id
	int resposta; // tipo de pedido
<<<<<<< Updated upstream
	int tipo;
	Aviao aviao[10];
} Request;

typedef struct
{
	Request requests[BUFFERS];
	int posRead, posWrite;
} RequestGeral;
=======
	int tipo; // 1- aceite, 2 - nao aceite por motivos de autenticacaoo, 3- o controlador nao permite mais avioes por parte do admin
	Autentication autentication;
	Aviao aviao;
	int tempo;
	bool permite_entrada;

} Request;

>>>>>>> Stashed changes

//
Request reqSair;
Request* reqSairPipe;
HANDLE PodeEscrever; // semaforo
HANDLE PodeLer; // semaforo
HANDLE PodeLerResposta;
RequestGeral* ptrMemoriaServer;

// server
HANDLE PodeEscreverInfo;
HANDLE PodeLerInfo;
HANDLE ficheiro;
HANDLE pedido;
HANDLE PodeLerRespostaServer;
HANDLE PodeEntrar;
HANDLE hEvent;
HANDLE hPipeServer;
HANDLE hTerminar;
HANDLE htDadosJogo;
HANDLE htPipesRemotos;
HANDLE hEventData;
HANDLE hThreadDados;
HANDLE hEventDivulga;
HANDLE hEventDados;
HANDLE hBrinde;
RequestGeral* PtrMemoria;

HANDLE Semaforo_PlanesMax; //Semaforo que controla o max de avioes.
Aviao aviao[max_planes];


//Func3

int Boot();
int num_de_avioes();
int num_aeroporto();
<<<<<<< Updated upstream
DWORD login(Request a, int maximo_de_avioes);
=======
Autentication login(Request a, int maximo_de_avioes);
int convertTimeToSec(SYSTEMTIME time);
int get_time();
>>>>>>> Stashed changes

#endif