#include "gamelib.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    extern void pulisciBuffer();
    
    unsigned short int scelta = 0; 
    system("clear"); //pulisci schermo
    printf("Benvenuto in Cosestrane!\n");
    //menu iniziale
    do{
        printf("Menu principale:\n1. Imposta gioco\n2. Gioca\n3. Termina gioco\n-");
        scanf("%hu", &scelta);
        pulisciBuffer();
        switch (scelta)
        {
        case 1:
            imposta_gioco();
            break;
        
        case 2:
            /* gioca */
            gioca();
            break;
    
        case 3:
            /* termina_gioco */
            //termina_gioco();
            break;
        
        default:
        printf("Input non valido!!! \n");
            break;
        }
        printf("\n");
    }while (scelta != 3); 
    
}