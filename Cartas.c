#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define Max_Cartas 60
#define Num_Cartas_Iniciales 3
#define Num_Cartas_A_Mostrar 15

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
    struct Carta mano[Num_Cartas_Iniciales];
    int numCartasEnMano;
};

void cargarCartasDesdeArchivo(struct Guardian cartas[], int *numCartas);
void mostrarMenu();
void revolverCartas(struct Guardian cartas[], int numCartas);
void seleccionarCartasIniciales(struct Jugador *jugador, struct Guardian lote[], int numCartasEnLote);

int main() {
    struct Guardian cartas[Max_Cartas];
    int numCartas = 0;

    cargarCartasDesdeArchivo(cartas, &numCartas);
    revolverCartas(cartas, numCartas);

    int opcion;
    do {
        mostrarMenu();
        printf("Selecciona una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                // Implementar inicio del juego
                struct Jugador jugador1;
                struct Jugador jugador2;
                seleccionarCartasIniciales(&jugador1, cartas, Num_Cartas_A_Mostrar);
                seleccionarCartasIniciales(&jugador2, cartas, Num_Cartas_A_Mostrar);
                break;
            case 2:

                break;
            case 3:
                printf("Saliendo del juego...\n");
                break;
            default:
                printf("Opcion no válida. Intenta de nuevo.\n");
        }
    } while (opcion != 3);

    return 0;
}

void cargarCartasDesdeArchivo(struct Guardian cartas[], int *numCartas) {
    FILE *archivo = fopen("Cartas.txt", "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        exit(1);
    }

    char linea[100];
    char *token;

    fgets(linea, sizeof(linea), archivo);

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        token = strtok(linea, ",");
        strcpy(cartas[*numCartas].nombre, token);
        token = strtok(NULL, ",");
        strcpy(cartas[*numCartas].tipo, token);
        token = strtok(NULL, ",");
        cartas[*numCartas].PV = atoi(token);
        token = strtok(NULL, ",");
        cartas[*numCartas].PA = atoi(token);
        token = strtok(NULL, ",");
        cartas[*numCartas].PD = atoi(token);

        (*numCartas)++;
    }

    fclose(archivo);
}

void mostrarMenu() {
    printf("---- The Clash of the Guardians ----\n");
    printf("1. Comenzar juego\n");
    printf("2. Historial de la partida\n");
    printf("3. Salir\n");
}

void revolverCartas(struct Guardian cartas[], int numCartas) {
    srand(time(NULL));
    printf("Se estan revolviendo las cartas....\n");
    printf("....\n");
    printf("...\n");
    printf("..\n");
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
            // Agrega la carta seleccionada a la mano del jugador
            jugador->mano[jugador->numCartasEnMano].guardian = lote[eleccion - 1];
            jugador->numCartasEnMano++;

            // Elimina la carta seleccionada del lote
            for (int i = eleccion - 1; i < numCartasEnLote - 1; i++) {
                lote[i] = lote[i + 1];
            }

            numCartasEnLote--;

            printf("Carta seleccionada.\n");
        } else {
            printf("Selección no válida. Elige una carta del 1 al %d.\n", numCartasEnLote);
        }
    }

    printf("\n\nTus cartas iniciales son:\n");
    for (int i = 0; i < Num_Cartas_Iniciales; i++) {
        printf("%s (%s, PV:%d, PA:%d, PD:%d)\n", jugador->mano[i].guardian.nombre, jugador->mano[i].guardian.tipo, jugador->mano[i].guardian.PV, jugador->mano[i].guardian.PA, jugador->mano[i].guardian.PD);
    }
}



