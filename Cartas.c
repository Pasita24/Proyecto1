#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define Num_Cartas_Iniciales 3
struct Guardian {
    char nombre[20];
    char tipo[20];
    int PV;
    int PA;
    int PD;
};
struct Jugador {
    char nombre[20];
    struct Guardian cartasEnMano[Num_Cartas_Iniciales];
    int numCartasEnMano;
};
void revolverCartas(struct Guardian cartas[], int numCartas) {
    srand(time(NULL));
    printf("Se estan revolviendo las cartas....\n");
    printf("....\n");
    printf("...\n");
    printf("..\n");
    printf(".\n");
    printf(".\n");
    printf(".\n");
    for (int i = numCartas - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        struct Guardian temp = cartas[i];
        cartas[i] = cartas[j];
        cartas[j] = temp;
    }
}
void seleccionarCartasIniciales(struct Jugador *jugador, struct Guardian lote[], int numCartasEnLote) {
    jugador->numCartasEnMano = 0; // Inicializa la mano del jugador
    printf("Selecciona tus 3 cartas iniciales:\n");
    for (int i = 0; i < numCartasEnLote && jugador->numCartasEnMano < Num_Cartas_Iniciales; i++) {
        printf("%d. %s (%s, PV:%d, PA:%d, PD:%d)\n", i + 1, lote[i].nombre, lote[i].tipo, lote[i].PV, lote[i].PA, lote[i].PD);
    }
    while (jugador->numCartasEnMano < Num_Cartas_Iniciales) {
        printf("Elige una carta (1-%d): ", numCartasEnLote);
        int eleccion;
        scanf("%d", &eleccion);
        if (eleccion >= 1 && eleccion <= numCartasEnLote) {
            jugador->cartasEnMano[
jugador->numCartasEnMano] = lote[eleccion - 1];
            jugador->numCartasEnMano++;
        } else {
            printf("Eleccion no valida. Intenta de nuevo.\n");
        }
    }
}

int main() {
    struct Guardian lote[] = {
        {"Guardián 1", "Tipo 1", 100, 10, 5},
        {"Guardián 2", "Tipo 2", 120, 12, 6},
        {"Guardián 3", "Tipo 3", 140, 14, 7},
        {"Guardián 4", "Tipo 4", 160, 16, 8},
        {"Guardián 5", "Tipo 5", 180, 18, 9},
    };
    int numCartasEnLote = sizeof(lote) / sizeof(lote[0]);

    struct Jugador jugador;
    printf("Ingrese el nombre del jugador: ");
    scanf("%s", &jugador.nombre);

    revolverCartas(lote, numCartasEnLote);
    seleccionarCartasIniciales(&jugador, lote, numCartasEnLote);

    printf("Las cartas del jugador %s son:\n", jugador.nombre);
    for (int i = 0; i < jugador.numCartasEnMano; i++) {
        printf("%s (%s, PV:%d, PA:%d, PD:%d)\n", jugador.cartasEnMano[i].nombre, jugador.cartasEnMano[i].tipo, jugador.cartasEnMano[i].PV, jugador.cartasEnMano[i].PA, jugador.cartasEnMano[i].PD);
    }

    return 0;
}
