#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define Max_Cartas 60
#define Num_Cartas_Iniciales 3
#define Num_Cartas_A_Mostrar 15
#define Num_Cartas_Restantes 12

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
    struct Guardian cartasRestantes[Num_Cartas_Restantes];
    int numCartasRestantes;
};

void cargarCartasDesdeArchivo(struct Guardian cartas[], int *numCartas);
void mostrarMenu();
void revolverCartas(struct Guardian cartas[], int numCartas);
void seleccionarCartasIniciales(struct Jugador *jugador, struct Guardian lote[], int *numCartasEnLote);

struct Guardian cartas[Max_Cartas];
int numCartas = 0;

int main() {
    
    cargarCartasDesdeArchivo(cartas, &numCartas);
    revolverCartas(cartas, numCartas);

    int opcion;
    int numCartasRestantes = Num_Cartas_A_Mostrar;
    do {
        mostrarMenu();
        printf("Selecciona una opcion: \n");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                // Implementar inicio del juego
                struct Jugador jugador1;
                struct Jugador jugador2;

                // Copiar cartas al segundo jugador
                struct Guardian cartasJugador2[Max_Cartas];
                memcpy(cartasJugador2, cartas, sizeof(cartas));
                revolverCartas(cartasJugador2, numCartas);

                seleccionarCartasIniciales(&jugador1, cartas, &numCartasRestantes);
                
                printf("\n\nTurno del segundo jugador\n");
                
                seleccionarCartasIniciales(&jugador2, cartas, &numCartasRestantes);
                break;
            case 2:
                break;
            case 3:
                printf("Saliendo del juego...\n");
                break;
            default:
                printf("Opcion no valida. Intenta de nuevo.\n");
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
    printf(".\n");

    for (int i = numCartas - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        struct Guardian temp = cartas[i];
        cartas[i] = cartas[j];
        cartas[j] = temp;
    }
}

void seleccionarCartasIniciales(struct Jugador *jugador, struct Guardian lote[], int *numCartasEnLote) {
    jugador->numCartasEnMano = 0; // Inicializa la mano del jugador
    printf("Selecciona tus 3 cartas iniciales:\n");

    while (jugador->numCartasEnMano < Num_Cartas_Iniciales && *numCartasEnLote > 0) {
        for (int i = 0; i < *numCartasEnLote && jugador->numCartasEnMano < Num_Cartas_Iniciales; i++) {
            printf("%d. %s (%s, PV:%d, PA:%d, PD:%d)\n", i + 1, lote[i].nombre, lote[i].tipo, lote[i].PV, lote[i].PA, lote[i].PD);
        }

        printf("Elige una carta (1-%d): ", *numCartasEnLote);
        int eleccion;
        scanf("%d", &eleccion);

        if (eleccion >= 1 && eleccion <= *numCartasEnLote) {
            // Agrega la carta seleccionada a la mano del jugador
            jugador->mano[jugador->numCartasEnMano].guardian = lote[eleccion - 1];
            jugador->numCartasEnMano++;

            // Elimina la carta seleccionada del lote
            for (int i = eleccion - 1; i < *numCartasEnLote - 1; i++) {
                lote[i] = lote[i + 1];
            }
            
            (*numCartasEnLote)--;
            
            printf("Carta seleccionada.\n\n");
        } else {
            printf("Selección no valida. Elige una carta del 1 al %d.\n", *numCartasEnLote);
        }
    }

    printf("\n\nTus cartas iniciales son:\n");

    for (int i = 0; i < Num_Cartas_Iniciales; i++) {
        printf("%s (%s, PV:%d, PA:%d, PD:%d)\n", jugador->mano[i].guardian.nombre, jugador->mano[i].guardian.tipo, jugador->mano[i].guardian.PV, jugador->mano[i].guardian.PA, jugador->mano[i].guardian.PD);
    }
}


