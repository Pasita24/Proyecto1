#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Guardian {
    char nombre[50];
    int PV;
    int PA;
    int PD;
    char tipo[20];
};
struct Carta {
    struct Guardian guardian;
    int enCampoBatalla;
};

struct Jugador {
    int puntosVida;
    struct Carta mano[15];
    int numCartasEnMano;
};
int main() {
    cargarCartasDesdeArchivo();
    
    int opcion;
    do {
        mostrarMenu();
        printf("Selecciona una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                crearNuevaCarta();
                break;
            case 2:
                iniciarJuego();
                break;
            case 3:
                break;
            case 4:
                printf("Saliendo del juego.\n");
                break;
            default:
                printf("Opcion no válida. Intenta de nuevo.\n");
        }
    } while (opcion != 4);

    return 0;
}

void cargarCartasDesdeArchivo() {
}

void mostrarMenu() {
    printf("---- The Clash of the Guardians ----\n");
    printf("1. Crear nueva carta\n");
    printf("2. Comenzar juego\n");
    printf("3. Historial de la partida\n");
    printf("4. Salir\n");
}

