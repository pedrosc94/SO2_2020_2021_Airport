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
	bool sair; // ADICIONEI
	bool chegou_ao_destino;
	int tempo;
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
	TCHAR aviao_name[BUFFERSIZE]; // name of the Passager
	TCHAR original_aeorporto[BUFFERSIZE];
	TCHAR next_destionation[BUFFERSIZE];
	int  tempo_de_espera;
	int tempo;
	bool ground; // if is currently grounded (or flying)
	bool exit; // pilots ends
	Passager* passager;// passagers in the plain
	int tempo;

} Aviao;


typedef struct controller {
	Aeroporto* aeroporto;
	Aviao* aviao;
} Controller;

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

typedef struct
{
	int Id; // aviao id
	int resposta; // tipo de pedido
	int tipo; // 1- aceite, 2 - nao aceite por motivos de autenticacaoo, 3- o controlador nao permite mais avioes por parte do admin
	Autentication autentication;
	Aviao aviao;
	int tempo;
	bool permite_entrada;

} Request;


//
Request reqSair;
HANDLE PodeEscrever; // semaforo
HANDLE PodeLer; // semaforo
HANDLE PodeLerResposta;

// server
HANDLE PodeEscreverInfo;
HANDLE PodeLerInfo;
HANDLE PodeLerRespostaServer;
HANDLE PodeEntrar;
HANDLE hEvent;
HANDLE hTerminar;
HANDLE htDadosJogo;
HANDLE hEventData;
HANDLE hThreadDados;
HANDLE hEventDivulga;
HANDLE hEventDados;
HANDLE hmap;
HANDLE Semoforo_max_planes; // ontrola o max de avioes
Request* PtrMemoria;


HANDLE Semaforo_PlanesMax; //Semaforo que controla o max de avioes.

Aviao aviao[max_planes];
Aeroporto aeroporto[max_aeroports];


//Func3

int Boot();
int num_de_avioes();
int num_aeroporto();
Autentication login(Request a, int maximo_de_avioes);
int convertTimeToSec(SYSTEMTIME time);
int get_time();

#endif