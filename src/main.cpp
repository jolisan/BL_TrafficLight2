#include "mbed.h"

InterruptIn botao(p5);
DigitalOut ledvermelho(p6);
DigitalOut ledamarelo(p7);
DigitalOut ledazul(p8);
Timeout timeout;
Ticker ticker;

enum ESTADO {
   OFF,
   ALERTA,
   AMARELO,
   AZUL,
   ON,
   VERMELHO,
   INICIAR
};

int estado_atual;
int estado_anterior;
int contador;
int estadoBotao;

void mudarEstado();
void estadoPadrao();
void mudarVermelho();
void mudarAzul();
void mudarAmarelo();
void mudarAlerta();
void mudarAmarelo1Hz();
void mudarParaOFF();
void contador();
void iniciarContador();
void finalizarContador();
void finalizarContador();
void estadoPadrao();

void mudarVermelho(){
   printf("Ligando vermelho\n");
   ledvermelho = 1;
   estado_anterior = estado_atual;
   estado_atual = VERMELHO;
   timeout.attach( &mudarEstado, 10);
}

void mudarAzul(){
   printf("Ligando azul\n");
   ledazul = 1;
   estado_anterior = estado_atual;
   estado_atual = AZUL;
   timeout.attach( &mudarEstado, 20);
}

void mudarAmarelo(){
   printf("Ligando amarelo\n");
   ledamarelo = 1;
   estado_anterior = estado_atual;
   estado_atual = AMARELO;
   timeout.attach( &mudarEstado, 4);
}

void mudarAlerta(){
   printf("Alerta\n");
   estado_anterior = estado_atual;
   estado_atual = ALERTA;
   mudarAmarelo1Hz();
}

void mudarAmarelo1Hz(){
   ledamarelo = !ledamarelo;
   ticker.attach(mudarAmarelo1Hz, 0.5);
}

void mudarParaOFF(){
   printf("Off\n");
   estado_anterior = estado_atual;
   estado_atual = OFF;
   estadoPadrao();
}

void contador(){
   contador++;
}

void iniciarContador(){
   printf("INICIANDO CONTADOR\n");
   timeout.detach();
   contador = 0;
   ticker.attach(contador, 1);
}

void finalizarContador(){
   printf("FINALIZANDO CONTADOR\n");
   ticker.detach();
   estadoBotao = ON;
   mudarEstado();
}

void estadoPadrao(){
   ledvermelho = 0;
   ledamarelo = 0;
   ledazul = 0;
   timeout.detach();
   ticker.detach();
}

void mudarEstado(){
   estadoPadrao();
   if(estado_atual == INICIAR && estado_anterior == INICIAR){
      printf("INICIANDO VERMELHO\n");
      ledvermelho = 1;
      estado_anterior = VERMELHO;
      estado_atual = VERMELHO;
      timeout.attach( &mudarEstado, 10);
   }
   else if(estadoBotao == OFF){
      if(estado_atual == VERMELHO){
         mudarAzul();
      }
	  else if(estado_atual == AZUL){
         mudarAmarelo();
      }
	  else if(estado_atual == AMARELO){
         mudarVermelho();
      }
   }
   else if(estadoBotao == ON){
      estadoBotao = OFF;
      if(estado_atual == VERMELHO) {
         if(contador >= 3 && contador <= 10){
            mudarAlerta();
         }
		 else if(contador > 10){
            mudarParaOFF();
         }
		 else{
            mudarVermelho();
         }
      } 
	  else if(estado_atual == AMARELO){
         if(contador >= 3 && contador <= 10){
            mudarAlerta();
         }
		 else if (contador > 10){
            mudarParaOFF();
         }
		 else {
            mudarAmarelo();
         }
      } 
	  else if(estado_atual == AZUL){
         if(contador >= 3 && contador <= 10){
            mudarAlerta();
         }
		 else if(contador == 20 || contador <= 3){
            mudarAmarelo();
         }
		 else if(contador > 10){
            mudarParaOFF();
         }
		 else{
            mudarAzul();
         }
      }
	  else if(estado_atual == ALERTA){
         if(contador >= 3 && contador <= 10){
            if(estado_anterior == VERMELHO){
               mudarVermelho();
            } 
			else if(estado_anterior == AMARELO){
               mudarAmarelo();
            } 
			else{
               mudarAzul();
            }
         } 
		 else if(contador > 10){
            mudarParaOFF();
         }
		 else{
            estado_atual = estado_anterior;
            mudarAlerta();
         }
      }
	  else if(estado_atual == OFF){
         if(contador > 10){
            mudarVermelho();
         }
		 else{
            mudarParaOFF();
         }
      }
   }
}

int main() {
	
   estado_atual = INICIAR;
   estado_anterior = INICIAR;
   mudarEstado();
   botao.rise(&iniciarContador);
   botao.fall(&finalizarContador);

   while (1) {
      wait(1);
      printf("CONTADOR: %d\n", contador);
   }
}