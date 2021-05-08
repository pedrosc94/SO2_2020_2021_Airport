#pragma once

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

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

}Aeroporto;


typedef struct {
	int _max_planes, _max_aeroports; //coordinates

} boot_struct;



typedef struct {
	int x, y; //coordenadas
	int direction; //direcao movimento   1 - cima direita , 2 - baixo direita, 3 - esquerda baixo, 4 - cima esquerda
	int speed; //velocidade di avuai
	int id; //id do ultimo aviao
}Movimento;

typedef struct {

	int passeg_Id; // Passager id
	TCHAR passeg_name[BUFFERSIZE]; // name of the Passager
	TCHAR from_airoport_name[BUFFERSIZE]; //airport where the passenger is
	TCHAR to_airoport_name[BUFFERSIZE]; // airport where the passenger wants to go
	int awaits_x_seconds; //(optional) time the passenger waits for the plane

}Passager;


typedef struct {

	int aviao_Id; // 
	int capacidade;
	int speed; // speef of the plain
	int direcao; //direcao movimento   1 - cima direita , 2 - baixo direita, 3 - esquerda baixo, 4 - cima esquerda
	TCHAR aviao_name[BUFFERSIZE]; // name of the Passager
	TCHAR original_aeorporto[BUFFERSIZE];
	TCHAR next_destionation[BUFFERSIZE];

	bool ground; // if is currently grounded (or flying)
	bool exit; // pilots ends

	Passager* passager;// passagers in the plain

}Aviao;


typedef struct controller {

	Aeroporto* aeroporto;
	Aviao* aviao;

} Controller;


typedef struct
{

	int Id; // aviao id
	int resposta; // tipo de pedido
	int tipo;
	Aviao aviao[10];

} Request;

typedef struct
{
	Request requests[BUFFERS];
	int posRead, posWrite;
} RequestGeral;



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

Aviao aviao[max_planes];


//HANDLE _AviaoMax;				//Semaforo que controla o max de_avioes.

DLL_API int Boot();
DLL_API int num_de_avioes();
DLL_API int num_aeroporto();
DLL_API DWORD login(Request a, int maximo_de_avioes);
DLL_API DWORD WINAPI recebeDados();
DLL_API DWORD recebeTecla();
