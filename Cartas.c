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
    int puntosVida ;
    struct Carta mano[Num_Cartas_Iniciales];
    int numCartasEnMano;
    struct Guardian mazoPropio[Num_Cartas_A_Mostrar];
    struct Carta campoBatalla[Max_Cartas];
};

void cargarCartasDesdeArchivo(struct Guardian cartas[], int *numCartas);
void mostrarMenu();
void revolverCartas(struct Guardian cartas[], int numCartas);
void seleccionarCartasIniciales(struct Jugador *jugador, int numCartasRestantes);
void mostrarCartasEnMano(struct Jugador jugador);
void iniciarCombate(struct Jugador *jugador1, struct Jugador *jugador2);
void seleccionarCartaCampoBatalla(struct Jugador *jugador);
void atacarOponente(struct Jugador *atacante, struct Jugador *oponente, struct Jugador *jugador1, struct Jugador *jugador2);


int main() {
    struct Guardian cartas[Max_Cartas];
    int numCartas = 0;

    cargarCartasDesdeArchivo(cartas, &numCartas);
    revolverCartas(cartas, numCartas);

    int opcion;
    do {
        mostrarMenu();
        printf("Selecciona una opcion: \n");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                // Implementar inicio del juego
                struct Jugador jugador1;
                struct Jugador jugador2;

                // Copiar las cartas al mazo propio de cada jugador
                memcpy(jugador1.mazoPropio, cartas, sizeof(struct Guardian) * Num_Cartas_A_Mostrar);
                memcpy(jugador2.mazoPropio, cartas, sizeof(struct Guardian) * Num_Cartas_A_Mostrar);

                revolverCartas(jugador1.mazoPropio, Num_Cartas_A_Mostrar);
                revolverCartas(jugador2.mazoPropio, Num_Cartas_A_Mostrar);

                // Inicializar el número de cartas en mano
                jugador1.numCartasEnMano = 0;
                jugador2.numCartasEnMano = 0;

                // Jugadores seleccionan sus cartas iniciales
                seleccionarCartasIniciales(&jugador1, Num_Cartas_A_Mostrar);
                seleccionarCartasIniciales(&jugador2, Num_Cartas_A_Mostrar);

                printf("\n\n");
                // Iniciar el combate
                iniciarCombate(&jugador1, &jugador2);
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

    // Leer la primera línea (encabezado) y descartarla
    fgets(linea, sizeof(linea), archivo);

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        // Utilizar sscanf para leer los valores directamente en la estructura Guardian
        if (sscanf(linea, "%49[^,],%19[^,],%d,%d,%d\n",
                   cartas[*numCartas].nombre, cartas[*numCartas].tipo,
                   &cartas[*numCartas].PV, &cartas[*numCartas].PA, &cartas[*numCartas].PD) == 5) {
            (*numCartas)++;
        } else {
            fprintf(stderr, "Error al leer la linea: %s", linea);
        }
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

void seleccionarCartasIniciales(struct Jugador *jugador, int numCartasRestantes) {
    printf("Selecciona tus 3 cartas iniciales:\n");

    while (jugador->numCartasEnMano < Num_Cartas_Iniciales && numCartasRestantes > 0) {
        for (int i = 0; i < Num_Cartas_A_Mostrar && jugador->numCartasEnMano < Num_Cartas_Iniciales; i++) {
            if (jugador->mazoPropio[i].nombre[0] != '-' && jugador->mazoPropio[i].nombre[1] != '-') {
                printf("%d. %s (%s, PV:%d, PA:%d, PD:%d)\n", i + 1, jugador->mazoPropio[i].nombre, jugador->mazoPropio[i].tipo, jugador->mazoPropio[i].PV, jugador->mazoPropio[i].PA, jugador->mazoPropio[i].PD);
            } else {
                printf("%d. --\n", i + 1);
            }
        }

        printf("Elige una carta (1-%d): ", Num_Cartas_A_Mostrar);
        int eleccion;
        scanf("%d", &eleccion);

        if (eleccion >= 1 && eleccion <= Num_Cartas_A_Mostrar && jugador->mazoPropio[eleccion - 1].nombre[0] != '-' && jugador->mazoPropio[eleccion - 1].nombre[1] != '-') {
            jugador->mano[jugador->numCartasEnMano].guardian = jugador->mazoPropio[eleccion - 1];
            jugador->numCartasEnMano++;

            jugador->mazoPropio[eleccion - 1].nombre[0] = '-';
            jugador->mazoPropio[eleccion - 1].nombre[13] = '-';

            printf("Carta seleccionada.\n\n");
        } else {
            printf("Selección no valida. Elige una carta valida del 1 al %d.\n", Num_Cartas_A_Mostrar);
        }
    }

    printf("\n\nTus cartas iniciales son:\n");
    mostrarCartasEnMano(*jugador);
}

void mostrarCartasEnMano(struct Jugador jugador) {
    for (int i = 0; i < Num_Cartas_Iniciales; i++) {
        printf("%s (%s, PV:%d, PA:%d, PD:%d)\n", jugador.mano[i].guardian.nombre, jugador.mano[i].guardian.tipo, jugador.mano[i].guardian.PV, jugador.mano[i].guardian.PA, jugador.mano[i].guardian.PD);
    }
}
void mostrarMazoCartasElegidas(struct Jugador jugador) {
    printf("Mazo de Cartas Elegidas:\n");
    for (int i = 0; i < jugador.numCartasEnMano; i++) {
        printf("%d. %s (%s, PV:%d, PA:%d, PD:%d)\n", i + 1, jugador.campoBatalla[i].guardian.nombre,
               jugador.campoBatalla[i].guardian.tipo, jugador.campoBatalla[i].guardian.PV,
               jugador.campoBatalla[i].guardian.PA, jugador.campoBatalla[i].guardian.PD);
    }
}
void agregarPrimeraCartaMazoElegidas(struct Jugador *jugador) {
    // Verificar si hay cartas restantes en el mazo propio
    if (jugador->numCartasEnMano > 0) {
        // Agregar la primera carta del mazo propio al mazo de cartas elegidas
        jugador->campoBatalla[jugador->numCartasEnMano].guardian = jugador->mazoPropio[0];
        jugador->numCartasEnMano++;

        // Eliminar el primer guardián del mazo propio y desplazar los demás
        for (int i = 0; i < jugador->numCartasEnMano - 1; i++) {
            jugador->mazoPropio[i] = jugador->mazoPropio[i + 1];
        }
        jugador->numCartasEnMano--;
    } else {
        printf("No quedan mas cartas en el mazo propio.\n");
    }
}
void iniciarCombate(struct Jugador *jugador1, struct Jugador *jugador2) {
    int cartaElegida1, cartaElegida2;

    printf("Turno de los jugadores para seleccionar cartas para el mazo de combate:\n");

    // Jugador 1 selecciona una carta
    printf("Cartas del Jugador 1:\n");
    mostrarCartasEnMano(*jugador1);
    printf("Elige una carta para poner en el campo de batalla (1-%d): ", jugador1->numCartasEnMano);
    scanf("%d", &cartaElegida1);

    if (cartaElegida1 >= 1 && cartaElegida1 <= jugador1->numCartasEnMano) {
        jugador1->campoBatalla[0] = jugador1->mano[cartaElegida1 - 1];
        jugador1->campoBatalla[0].enCampoBatalla = 1;

        for (int i = cartaElegida1 - 1; i < jugador1->numCartasEnMano - 1; i++) {
            jugador1->mano[i] = jugador1->mano[i + 1];
        }
        jugador1->numCartasEnMano--;
    } else {
        printf("Seleccion no válida para el Jugador 1. Elige una carta valida del 1 al %d.\n", jugador1->numCartasEnMano);
    }

    // Jugador 2 selecciona una carta
    printf("Cartas del Jugador 2:\n");
    mostrarCartasEnMano(*jugador2);
    printf("Elige una carta para poner en el campo de batalla (1-%d): ", jugador2->numCartasEnMano);
    scanf("%d", &cartaElegida2);

    if (cartaElegida2 >= 1 && cartaElegida2 <= jugador2->numCartasEnMano) {
        jugador2->campoBatalla[0] = jugador2->mano[cartaElegida2 - 1];
        jugador2->campoBatalla[0].enCampoBatalla = 1;

        for (int i = cartaElegida2 - 1; i < jugador2->numCartasEnMano - 1; i++) {
            jugador2->mano[i] = jugador2->mano[i + 1];
        }
        jugador2->numCartasEnMano--;
    } else {
        printf("Seleccion no valida para el Jugador 2. Elige una carta valida del 1 al %d.\n", jugador2->numCartasEnMano);
    }

    // Mostrar el contenido del mazo de combate
    printf("\nMazo de Combate del Jugador 1:\n");
    printf("%s (%s, PV:%d, PA:%d, PD:%d)\n", jugador1->campoBatalla[0].guardian.nombre,
           jugador1->campoBatalla[0].guardian.tipo, jugador1->campoBatalla[0].guardian.PV,
           jugador1->campoBatalla[0].guardian.PA, jugador1->campoBatalla[0].guardian.PD);

    printf("\nMazo de Combate del Jugador 2:\n");
    printf("%s (%s, PV:%d, PA:%d, PD:%d)\n", jugador2->campoBatalla[0].guardian.nombre,
           jugador2->campoBatalla[0].guardian.tipo, jugador2->campoBatalla[0].guardian.PV,
           jugador2->campoBatalla[0].guardian.PA, jugador2->campoBatalla[0].guardian.PD);

    // Mostrar el mazo de cartas elegidas
    mostrarMazoCartasElegidas(*jugador1);
    mostrarMazoCartasElegidas(*jugador2);

    int opcion;
    do {
        // Mostrar menú para que los jugadores puedan seleccionar su acción
        printf("---- Menu de Combate ----\n");
        printf("Jugador 1 (%s) - PV:%d\n", jugador1->campoBatalla[0].guardian.nombre, jugador1->puntosVida);
        printf("Jugador 2 (%s) - PV:%d\n", jugador2->campoBatalla[0].guardian.nombre, jugador2->puntosVida);
        printf("1. Atacar a oponente\n");
        printf("2. Pasar turno\n");
        printf("Selecciona una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                // Los jugadores pueden atacar al oponente
                if (jugador1->campoBatalla[0].enCampoBatalla && jugador2->campoBatalla[0].enCampoBatalla) {
                    // Jugador 1 ataca a Jugador 2
                    atacarOponente(jugador1, jugador2, jugador1, jugador2);
                } else {
                    printf("No puedes atacar sin cartas en el campo de batalla.\n");
                }
                break;
            case 2:
                // Pasar turno
                break;
            default:
                printf("Opcion no valida. Elige una opción valida.\n");
        }
    } while (opcion != 2);
}

void seleccionarCartaCampoBatalla(struct Jugador *jugador) {
    int cartaElegida;

    printf("Cartas en mano del Jugador:\n");
    mostrarCartasEnMano(*jugador);
    printf("Elige una carta para dejarla en el campo de batalla (1-%d): ", jugador->numCartasEnMano);
    scanf("%d", &cartaElegida);

    if (cartaElegida >= 1 && cartaElegida <= jugador->numCartasEnMano) {
        jugador->campoBatalla[jugador->numCartasEnMano] = jugador->mano[cartaElegida - 1];
        jugador->campoBatalla[jugador->numCartasEnMano].enCampoBatalla = 1;
        jugador->numCartasEnMano--;

        printf("Carta seleccionada y dejada en el campo de batalla.\n");
    } else {
        printf("Selección no valida. Elige una carta valida del 1 al %d.\n", jugador->numCartasEnMano);
    }
}

void atacarOponente(struct Jugador *atacante, struct Jugador *oponente, struct Jugador *jugador1, struct Jugador *jugador2) {
    int cartaElegida;

    if (atacante->campoBatalla[0].enCampoBatalla) {
        printf("Carta en el campo de batalla del Jugador %d:\n", atacante == jugador1 ? 1 : 2);
        printf("%s (%s, PV:%d, PA:%d, PD:%d)\n", atacante->campoBatalla[0].guardian.nombre,
               atacante->campoBatalla[0].guardian.tipo, atacante->campoBatalla[0].guardian.PV,
               atacante->campoBatalla[0].guardian.PA, atacante->campoBatalla[0].guardian.PD);
    } else {
        printf("No hay cartas en el campo de batalla del Jugador %d.\n", atacante == jugador1 ? 1 : 2);
        return;
    }

    printf("Cartas en el campo de batalla del Jugador %d:\n", oponente == jugador1 ? 1 : 2);
    for (int i = 0; i < oponente->numCartasEnMano; i++) {
        printf("%d. %s (%s, PV:%d, PA:%d, PD:%d)\n", i + 1, oponente->campoBatalla[i].guardian.nombre,
               oponente->campoBatalla[i].guardian.tipo, oponente->campoBatalla[i].guardian.PV,
               oponente->campoBatalla[i].guardian.PA, oponente->campoBatalla[i].guardian.PD);
    }

    printf("Elige una carta para atacar a tu oponente (1-%d): ", oponente->numCartasEnMano);
    scanf("%d", &cartaElegida);

    if (cartaElegida >= 1 && cartaElegida <= oponente->numCartasEnMano) {
        // Realizar el ataque (implementación del ataque según las reglas del juego)
        // Restar puntos de vida, considerar el poder de ataque (PA) y el poder de defensa (PD).
        // Luego, actualiza el estado de las cartas, por ejemplo, marcándolas como destruidas si es necesario.

        printf("Ataque realizado.\n");
    } else {
        printf("Selección no valida. Elige una carta valida del 1 al %d.\n", oponente->numCartasEnMano);
    }
}





