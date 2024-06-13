#include "colenda.h"
#include <unistd.h>
#include <time.h>

int main(void)
{
    FILE *gpu = NULL;

    // Abrindo o driver
    if (gpu_open(&gpu, DRIVER_NAME) != 0) {
        printf("Erro ao abrir o driver da GPU!\n");
        return -1;
    }

    // Definindo o plano de fundo
    if (set_background(gpu, 3, 6, 7) != 0) {
        printf("Erro ao definir o background!\n"); 
        return -1;
    }

    // Criando os canos
    if (set_poligon(gpu, 0, 0, 400, 394, 0b001, 0b101, 0b010, 5) != 0) {
	    return -1;
    }
    
    if (set_poligon(gpu, 1, 0, 380, 360, 0b001, 0b101, 0b010, 3) != 0) {
	    return -1;
    }

    if (set_poligon(gpu, 2, 0, 420, 360, 0b001, 0b101, 0b010, 3) != 0) {
	    return -1;
    }

    if (set_poligon(gpu, 3, 0, 400, 90, 0b001, 0b101, 0b010, 5) != 0) {
	    return -1;
    }

    if (set_poligon(gpu, 4, 0, 380, 140, 0b001, 0b101, 0b010, 3) != 0) {
	    return -1;
    }

    if (set_poligon(gpu, 5, 0, 420, 140, 0b001, 0b101, 0b010, 3) != 0) {
	    return -1;
    }

    if (set_poligon(gpu, 6, 0, 400, 30, 0b001, 0b101, 0b010, 5) != 0) {
	    return -1;
    }
    
    // Criando a moeda
    if (set_sprite(gpu, 1, 1, 390, 240, 9) != 0) {
	    return -1;
    }

    if (set_sprite(gpu, 2, 0, 100, 410, 23) != 0) {
	    return -1;
    }

    if (set_sprite(gpu, 3, 0, 240, 415, 23) != 0) {
	    return -1;
    }

    if (set_sprite(gpu, 4, 0, 500, 410, 23) != 0) {
	    return -1;
    }

    // Criando o chão
    for (int i = 53; i < 54; i++) {
        for (int j = 0; j < 79; j+=2) {
            if (set_background_block(gpu, i, j, 0, 7, 3) != 0){
                return -1;
            }
        }
    }

    for (int i = 53; i < 54; i++) {
        for (int j = 1; j < 79; j+=2) {
            if (set_background_block(gpu, i, j, 2, 5, 3) != 0){
                return -1;
            }
        }
    }

    for (int i = 54; i < 55; i++) {
        for (int j = 1; j < 79; j+=2) {
            if (set_background_block(gpu, i, j, 0, 7, 3) != 0){
                return -1;
            }
        }
    }

    for (int i = 54; i < 55; i++) {
        for (int j = 0; j < 79; j+=2) {
            if (set_background_block(gpu, i, j, 2, 5, 3) != 0){
                return -1;
            }
        }
    }

    for (int i = 55; i < 58; i++) {
        for (int j = 0; j < 79; j++) {
            if (set_background_block(gpu, i, j, 7, 6, 3) != 0){
                return -1;
            }
        }
    }

    for (int i = 58; i < 60; i++) {
        for (int j = 0; j < 79; j++) {
            if (set_background_block(gpu, i, j, 6, 5, 3) != 0){
                return -1;
            }
        }
    }

    // Criando o pássaro
    for (int i = 30; i < 36; i++) {
        for (int j = 18; j < 25; j++) {
            if (set_background_block(gpu, i, j, 7, 7, 1) != 0){
                return -1;
            }
        }
    }

    for (int i = 19; i < 24; i++) {
        if (set_background_block(gpu, 36, i, 7, 7, 1) != 0){
                return -1;
            }
    }

    for (int i = 31; i < 35; i++) {
        for (int j = 17; j < 22; j++) {
            if (set_background_block(gpu, i, j, 7, 7, 5) != 0){
                return -1;
            }
        }
    }

    for (int i = 31; i < 35; i++) { // bico
        for (int j = 24; j < 28; j++) {
            if (set_background_block(gpu, i, j, 7, 4, 2) != 0){
                return -1;
            }
        }
    }
    
    for (int i = 28; i < 31; i++) { // olho
        for (int j = 23; j < 25; j++) {
            if (set_background_block(gpu, i, j, 7, 7, 7) != 0){
                return -1;
            }
        }
    }

	if (set_background_block(gpu, 29, 24, 0, 0, 0) != 0){
		return -1;
	}
	
	if (set_poligon(gpu, 7, 1, 500, 413, 1, 4, 1, 1) != 0) {
	    return -1;
	}
	
	if (set_poligon(gpu, 8, 1, 510, 413, 1, 5, 1, 2) != 0) {
	    return -1;
	}

    // Definindo as nuvens
    for (int j = 0; j < 10; j++) {
        for (int i = 382; i >= 377; i -= 5) {
            if (set_poligon(gpu, 14, 0, 30, i + 10, 7, 7, 7, 5) != 0) {
                return -1;
            }

            if (set_poligon(gpu, 13, 0, 80, i, 7, 7, 7, 7) != 0) {
                return -1;
            }

            if (set_poligon(gpu, 12, 0, 130, i + 10, 7, 7, 7, 5) != 0) {
                return -1;
            }

            if (set_poligon(gpu, 11, 0, 250, i + 10, 7, 7, 7, 5) != 0) {
                return -1;
            }

            if (set_poligon(gpu, 10, 0, 310, i, 7, 7, 7, 7) != 0) {
                return -1;
            }

            if (set_poligon(gpu, 9, 0, 360, i + 10, 7, 7, 7, 5) != 0) {
                return -1;
            }

            sleep(1);
        }

        for (int i = 372; i <= 377; i += 5) {
            if (set_poligon(gpu, 14, 0, 30, i + 10, 7, 7, 7, 5) != 0) {
                return -1;
            }

            if (set_poligon(gpu, 13, 0, 80, i, 7, 7, 7, 7) != 0) {
                return -1;
            }

            if (set_poligon(gpu, 12, 0, 130, i + 10, 7, 7, 7, 5) != 0) {
                return -1;
            }

            if (set_poligon(gpu, 11, 0, 250, i + 10, 7, 7, 7, 5) != 0) {
                return -1;
            }

            if (set_poligon(gpu, 10, 0, 310, i, 7, 7, 7, 7) != 0) {
                return -1;
            }

            if (set_poligon(gpu, 9, 0, 360, i + 10, 7, 7, 7, 5) != 0) {
                return -1;
            }

            sleep(1);
        }
    }

    gpu_close(&gpu);
   
    return 0;
}
