#include "gamelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//variabili
Giocatore *giocatori [4]; 
unsigned short int numero_giocatori = 0;
int c; 
time_t t;
bool Undici; 
struct Zona_mondoreale *prima_zona_mondoreale = NULL;
struct Zona_soprasotto *prima_zona_soprasotto = NULL;
unsigned short int lv_difficolta = 0; 
bool mappa_generata = false;
bool gameOver = false;
bool inCombattimento = false;
bool generataC = false;

//funzioni generali
void pulisciBuffer();
static void stampaGiocatore(Giocatore *giocatore); 
static void stampaZaino(Giocatore *giocatore);
static void stampaTipo(int tipo);
static void stampaNemico(int nemico);
static void stampaOggetto(int oggetto);
static bool esisteDemotorzone();
static void set_difficolta();

//funzioni per imposta gioco:
static void crea_giocatori();
static int lancia_dadi(int n_min, int n_max);
static void stampa_stat(Giocatore *giocatore);
static void set_Undici(Giocatore *giocatore);
static void inizzializza_zaino(Giocatore *giocatore); 
static void genera_mappa();

//funzioni per genera mappa
static void generaNemico_Reale(struct Zona_mondoreale*zonaR);
static void generaNemico_Soprasotto(struct Zona_soprasotto*zonaS);
static void generaOggetto(struct Zona_mondoreale*zonaR);
static void inserisci_zona();
static void cancella_zona(); 
static void stampa_mappa();
static void stampa_zona();
static void chiudi_mappa();

//funzioni per gioca
static void avanza(Giocatore *giocatore);
static void indietreggia(Giocatore *giocatore);
static void cambia_mondo(Giocatore *giocatore);
static void combatti(Giocatore *giocatore);
static void stampa_giocatore(Giocatore *giocatore);
static void stampa_zonaGiocatore(Giocatore *giocatore);
static void raccogli_oggetto(Giocatore *giocatore);
static int utilizza_oggetto(Giocatore *g, int slot);

//funzioni: 

void imposta_gioco(){
    srand((unsigned) time(&t)); 

    if(numero_giocatori > 0 || mappa_generata){
        int scelta_menu = 0;
        printf("Il gioco è gia stato impostato! Entrerai nel menu di modifica del gioco attuale.\n");
        do {
            printf("\nMenu modifica gioco:\n1. Mantieni solo i giocatori (HP rigenerati).\n2. Mantieni solo la mappa (non se il demotorzone è stato sconfitto).\n3. Cambia difficolta.\n4. Modifica mappa gia esistente.\n5. Reset completo.\n6. Esci dal menu\n-");
            scanf("%d", &scelta_menu); 
            pulisciBuffer();

            switch (scelta_menu){
            case 1:
                for(int i = 0; i < numero_giocatori; i++){
                    if(giocatori[i] != NULL){
                        giocatori[i]->HP = 50; 
                        inizzializza_zaino(giocatori[i]);
                        giocatori[i]->pos_mondoreale = prima_zona_mondoreale;
                        giocatori[i]->pos_soprasotto = prima_zona_soprasotto; 
                        giocatori[i]->mondo = false;
                    }
                }
                printf("Giocatori rigenerati.\n");
                genera_mappa();
                chiudi_mappa();
                break;
            case 2:
                if(esisteDemotorzone()){
                    printf("Mappa presente valida. Mappa mantenuta.\n");
                    crea_giocatori();
                    break;
                }else{
                    printf("Non puoi mantenere questa mappa: non c'è un demotorzone.\n");
                }
                break;
            case 3:
                int scelta;
                printf("Il cambio di difficolta necessita un cambio nella mappa. Vuoi continuare?\n1. Si.\n2. No.\n-"); 
                do{
                    scanf("%d", &scelta);
                    pulisciBuffer();
                    if(scelta != 1 && scelta != 2){
                        printf("Inserisci un valore valido!\n");
                    }
                }while(scelta != 1 && scelta != 2);

                if(scelta == 1){
                    set_difficolta();
                    genera_mappa();
                    chiudi_mappa();
                }
                break;
            case 4:
                int sceltaM = 0;
                printf("\nMenu modifica mappa.\n1. Inserisci Zona (Inserisci zona in una posizione a piacere e assegna i valori per tipo, nemico e oggetto).\n2. Cancella zona (Cancella una zona in posizione a piacere).\n3. Stampa mappa (stampa tutti i campi di tutte le zone).\n4. Chiudi mappa (Chiudi la mappa quando hai finito di crearla)\n");
                printf("-");
                scanf("%d", &sceltaM);
                pulisciBuffer();
                switch (sceltaM)
                    {
                    case 1:
                        inserisci_zona();
                        break;
                    case 2:
                        cancella_zona();
                        break;
                    case 3:
                        stampa_mappa();
                        break;
                    case 4:
                        chiudi_mappa();
                        break;
                    default:
                        printf("Scelta non valida!\n");
                        break;
                    }
               break;
            case 5:
                printf("Reset completo in corso...\n");
                termina_gioco();
                printf("\n");
                break;
            case 6:
                printf("Uscita dal menu...\n");
                break;
            default:
                printf("Scelta non valida.\n");
                break;
        }
        }while(scelta_menu != 6 && !(numero_giocatori == 0 && !mappa_generata));
        
        if(numero_giocatori > 0 && mappa_generata){
            printf("Tutto pronto. Puoi iniziare a giocare!\n");
            gameOver = false;
        return;
        }
    }

    gameOver = false;
    mappa_generata = false; 
    unsigned short int scelta;

    if (numero_giocatori == 0){
        printf("Inizio creazione giocatori:\n");
        crea_giocatori();
    }
    if(lv_difficolta == 0){
        set_difficolta();
    }

    do{
        printf("\nMenu creazione mappa:\n1. Genera mappa (Genera 15 zone iniziali ed un demotorzone in una di esse scelta casualmente).\n2. Inserisci Zona (Inserisci zona in una posizione a piacere e assegna i valori per tipo, nemico e oggetto).\n3. Cancella zona (Cancella una zona in posizione a piacere).\n4. Stampa mappa (stampa tutti i campi di tutte le zone).\n5. Stampa zona (Stampa tutti i campi di una zona a piacere).\n6. Chiudi mappa (Chiudi la mappa quando hai finito di crearla)\n");
        printf("-");
        scanf("%hu", &scelta);
        pulisciBuffer();
        switch (scelta)
        {
        case 1:
            genera_mappa();
            break;
        case 2:
            inserisci_zona();
            break;
        case 3:
            cancella_zona();
            break;
        case 4:
            stampa_mappa();
            break;
        case 5:
            stampa_zona();
            break;
        case 6:
            chiudi_mappa();
            break;
        default:
            printf("Scelta non valida!\n");
            break;
        }
    }while(!mappa_generata);
}

void gioca (){
    if(!mappa_generata){
        printf("Mappa non generata! Completa la creazione della mappa per iniziare a giocare.\n");
        return;
    }

    if(numero_giocatori == 0){
        printf("Nessun giocatore presente! Imposta il gioco prima di iniziare.\n");
        return;
    }

    printf("\n---INIZIO GIOCO---\n");
    //inizio gioco

    int turniGiocatori[numero_giocatori]; // array per vedere i turni di ogni giocatore
    int giocatoriPassati;
    bool turnoFinito = false;

    for(int i = 0; i < numero_giocatori ; i++){ 
        giocatori[i]->pos_mondoreale = prima_zona_mondoreale; //tutti i giocatori nella prima posizione della mappa
        giocatori[i]->pos_soprasotto = prima_zona_soprasotto;
        turniGiocatori[i] = 0; //all'inizio tutti hanno giocato 0 turni
    }

    while(!gameOver){
        turnoFinito = false;
        while(!turnoFinito){
            int minTurni = turniGiocatori[0]; //numero minore di turni giocati tra i giocatori
            for (int i = 0; i < numero_giocatori ; i++){ 
                if(minTurni > turniGiocatori[i]){ 
                    minTurni = turniGiocatori[i]; 
                }
            }
            int turno;
            do{
                turno = lancia_dadi(0, numero_giocatori-1);
            }while(turniGiocatori[turno] != minTurni); //scelta casuale per chi deve giocare tra chi ha il minor numero di turni


            if(giocatori[turno]->HP <= 0){
                printf("%s è stato sconfitto, salta il turno.\n", giocatori[turno]->nome_giocatore);
                turnoFinito = true;
            }

            printf("\nTocca a %s:\n", giocatori[turno]->nome_giocatore);

            printf("Scegli cosa fare!\n");
            printf("1. Avanza\n2. Indietreggia\n3. Cambia mondo\n4. Combatti\n5. Info giocatore\n6. Guarda zona\n7. Raccogli oggetto\n8. Usa oggetto\n9. Passa il turno\n-");
            
            int sceltaMossa;
            scanf("%d", &sceltaMossa);
            pulisciBuffer();

            switch (sceltaMossa)
            {
            case 1:
            avanza(giocatori[turno]);
            turnoFinito = true;
                break;
            case 2:
            indietreggia(giocatori[turno]);
            turnoFinito = true;
                break;
            case 3:
            cambia_mondo(giocatori[turno]);
            turnoFinito = true;
                break;
            case 4:
            combatti(giocatori[turno]);
            turnoFinito = true;
                break;
            case 5:
            stampa_giocatore(giocatori[turno]);
            turnoFinito = false;
                break;
            case 6:
            stampa_zonaGiocatore(giocatori[turno]);
            turnoFinito = false; 
                break;
            case 7:
            raccogli_oggetto(giocatori[turno]);
            turnoFinito = true;
                break;
            case 8:
                printf("Che oggetto vuoi usare? (0-2)\n");
                int scelta_zaino;
                do{
                    stampaZaino(giocatori[turno]);
                    printf("-");
                    scanf("%d", &scelta_zaino);
                    pulisciBuffer();
                    if(scelta_zaino < 0 || scelta_zaino > 2){
                        printf("Scelta non valida!\n");
                    }
                }while (scelta_zaino < 0 || scelta_zaino > 2);
                int effetto = utilizza_oggetto(giocatori[turno], scelta_zaino + 1);
                turnoFinito = false;
                break;
            case 9:
            printf("Turno passato.\n");
            turnoFinito = true;
                break;
            default:
            printf("Scelta non valida!\n");
            turnoFinito = false;
                break;
            }
            if(turnoFinito){
                turniGiocatori[turno]++;
            } 
        }
    }

}

void termina_gioco(){   //termina il gioco, resetta tutte le variabili globali e libera la memoria
    for(int i = 0; i < numero_giocatori; i++){
        if(giocatori[i] != NULL){
            free(giocatori[i]); 
            giocatori[i] = NULL;
        }
    }
    numero_giocatori = 0; 
    struct Zona_mondoreale *tmprR;
    struct Zona_soprasotto *tmprS;

    while(prima_zona_mondoreale != NULL){
        tmprR = prima_zona_mondoreale;
        prima_zona_mondoreale = prima_zona_mondoreale ->avanti; 
        free(tmprR);
    }
    while(prima_zona_soprasotto != NULL){
        tmprS = prima_zona_soprasotto; 
        prima_zona_soprasotto = prima_zona_soprasotto->avanti;
        free(tmprS);
    }
    mappa_generata = false; 
    gameOver = false; 
    inCombattimento = false; 
    lv_difficolta = 0; 
    Undici = false;

    printf("Gioco terminato.\n");
}

void crea_giocatori(){
    Undici = false;
    //numero di giocatori
    do{
        printf("Inserisci il numero di giocatori (1-4)\n-");
        scanf("%hu", &numero_giocatori);
        pulisciBuffer();

        if(numero_giocatori == 0 || numero_giocatori>4){
            printf("Valore non valido!\n");
        }
    }while (numero_giocatori == 0 || numero_giocatori>4);

    for(int i = 0; i < numero_giocatori; i++){
        giocatori[i] = (Giocatore*) (malloc(sizeof(Giocatore)));

        //imposta nome
        do {
            printf("Inserire nome giocatore %d (MAX 19 CARATTERI):\n", i + 1);

            if (fgets(giocatori[i]->nome_giocatore, sizeof(giocatori[i]->nome_giocatore), stdin) == NULL) {
                printf("Errore di input!\n");
                pulisciBuffer();
                continue;   
            }
            size_t len = strlen(giocatori[i]->nome_giocatore);

            //nome troppo lungo
            if (len == sizeof(giocatori[i]->nome_giocatore) - 1 && giocatori[i]->nome_giocatore[len-1] != '\n') {
                printf("Nome non valido! Max 19 caratteri!\n"); 
                pulisciBuffer();
                continue;
            }
            // rimuove newline
            if(len > 0 && giocatori[i]->nome_giocatore[len -1] == '\n'){
                giocatori[i]->nome_giocatore[len -1] = '\0'; 
                len--;
            }
            //nome vuoto
            if(len == 0){
                printf("Nome non valido! Inserisci almeno un carattere.\n");
                continue;
            }
            
            
            break; //nome valido
        }while (1);
        

        //lancia dado stats
        giocatori[i]->attacco_psichico = lancia_dadi(1,20);
        giocatori[i]->difesa_psichica = lancia_dadi(1,20);
        giocatori[i]->fortuna = lancia_dadi(1,20);

        printf("\nEcco le tue stat:\n");
        stampa_stat(giocatori[i]);

        //scelta stat da aumentare 
        unsigned short int scelta_stat = 0;

        do{
            printf("\nCosa vuoi aumentare?\n1. attacco +3 (difesa -3)\n2. difesa +3 (attacco -3)\n3. Nessuna modifica.\n-");

            scanf("%hu", &scelta_stat); 
            pulisciBuffer();
            if(scelta_stat!= 1 && scelta_stat != 2 && scelta_stat != 3){
                printf("Scegli una delle opzioni disponibili.\n");
            }else{
                if(scelta_stat== 1){           
                    if((giocatori[i]->difesa_psichica - 3) < 0){    //controllo stat non siano < 0
                        giocatori[i]->difesa_psichica = 1; 
                    } else { 
                        giocatori[i]->difesa_psichica -= 3;
                    }
                    giocatori[i]->attacco_psichico += 3;
                    
                }else if(scelta_stat == 2){
                    if((giocatori[i]->attacco_psichico - 3) < 0){   //controllo stat non siano < 0
                        giocatori[i]->attacco_psichico = 1; 
                    }else{
                        giocatori[i]->attacco_psichico -= 3;
                    }
                    giocatori[i]->difesa_psichica += 3; 
                }else if(scelta_stat == 3){
                    printf("Nessuna statistica modificata!\n");
                }
            }
        }while(scelta_stat!= 1 && scelta_stat != 3);

        //controllo Undici
        if(Undici == false){
            int scelta_Undici = 0; 
            printf("Vorresti essere il giocatore UndiciVirgolacinque? (Aumenta attacco psichico e difesa pischica di 4 punti, ma diminuisci la fortuna di 7 punti.)\n1. Si\n2. No\n-");
            do{
                scanf("%d", &scelta_Undici); 
                pulisciBuffer();

                if(scelta_Undici == 1){
                    set_Undici(giocatori[i]); 
                    printf("Il giocatore %d è UndiciVirgolaCinque.\n", i+1); 
                    Undici = true;
                }else if(scelta_Undici != 1 && scelta_Undici != 2){
                    printf("Inserisci un valore valido!\n-");
                }
            }while(scelta_Undici != 1 && scelta_Undici != 2); 
        }else{
            printf("Il ruolo di UndiciVirgolaCinque è gia stato selezionato.\n"); 
        }

        //set hp
        giocatori[i]->HP = 50; 

        //set zaino a vuoto
        inizzializza_zaino(giocatori[i]); 

        printf("\nGiocatore %d è stato creato!\n", i+1);
        stampaGiocatore(giocatori[i]);
        printf("\n");
        


        giocatori[i]->pos_mondoreale = prima_zona_mondoreale;
        giocatori[i]->pos_soprasotto = prima_zona_soprasotto;
        giocatori[i]->mondo = false;
    }
}

void pulisciBuffer(){   //leggi carattere all'inifinito finchè non incontri /n o fine del file
    while(1){   
        c = getchar();

        if(c == '\n'){
            break;
        }else if(c == EOF){
            break;
        }
    }
}

int lancia_dadi(int n_min, int n_max){
    int risultato = n_min + rand() % (n_max-n_min +1);
    return risultato;
}

void stampa_stat(Giocatore *giocatore){
    printf("Attacco Psichico: %d\n", giocatore->attacco_psichico);
    printf("Difesa Psichica: %d\n", giocatore->difesa_psichica);
    printf("Fortuna: %d\n", giocatore->fortuna);
}

void set_Undici(Giocatore *giocatore){
    if(giocatore-> fortuna > 7){
        giocatore->fortuna -= 7; 
    }else{
        giocatore->fortuna = 1; 
    }
    giocatore->attacco_psichico += 4;
    giocatore->difesa_psichica += 4;
    strcpy(giocatore->nome_giocatore, "UndiciVirgolaCinque"); 
}

void inizzializza_zaino(Giocatore *giocatore){
    for(int i = 0; i < 3; i++){
        giocatore->zaino[i] = nessun_oggetto; 
    }
}

void stampaGiocatore(Giocatore *giocatore){
    printf("Nome: %s\n", giocatore->nome_giocatore); 
    printf("Attacco Psichico: %hu\n", giocatore->attacco_psichico);
    printf("Difesa Psichica: %hu\n", giocatore->difesa_psichica); 
    printf("Fortuna: %hu\n", giocatore->fortuna);
    printf("Contenuto zaino: \n");
    stampaZaino(giocatore); 
}

void stampaZaino(Giocatore *giocatore){
    for(int i = 0; i < 3; i++){
        printf(" - ");
        switch(giocatore->zaino[i]){
            case nessun_oggetto: 
                printf("nessun oggetto"); 
                break;
            case bicicletta: 
                printf("bicicletta"); 
                break;
            case maglietta_fuocoinferno: 
                printf("maglietta fuoco inferno"); 
                break;
            case bussola: 
                printf("bussola"); 
                break;
            case schitarrata_metallica: 
                printf("schitarrata metallica"); 
                break;
        }
        printf("\n");
    }
}

void genera_mappa(){

    struct Zona_mondoreale *pulisciR;
    struct Zona_soprasotto *pulisciS;

    //pulisci mappa di prima(se presente)
    while(prima_zona_mondoreale!=NULL){
        pulisciR = prima_zona_mondoreale;
        prima_zona_mondoreale = prima_zona_mondoreale->avanti;
        free(pulisciR);
    }

    while(prima_zona_soprasotto!=NULL){
        pulisciS = prima_zona_soprasotto;
        prima_zona_soprasotto = prima_zona_soprasotto->avanti;
        free(pulisciS);
    }

    struct Zona_mondoreale *currR = NULL; 
    struct Zona_soprasotto *currS = NULL; 
 
    int zona_demotorzone = lancia_dadi(0,14);

    for(int i = 0; i < 15; i++){
        //crea le 15 zone
        struct Zona_mondoreale *zona_R = malloc(sizeof(Zona_mondoreale));
        struct Zona_soprasotto *zona_S = malloc(sizeof(Zona_soprasotto));

        //imposta tipo zona
        enum Tipo_zona tipoZona = (Tipo_zona) lancia_dadi(0,9);
        zona_R->tipo = tipoZona; 
        zona_S->tipo = tipoZona; 

        //crea nemici 
        generaNemico_Reale(zona_R);

        if(i == zona_demotorzone){
            zona_S->nemico = demotorzone;
        }else{
            generaNemico_Soprasotto(zona_S);
        }
 
        //crea oggetti
        generaOggetto(zona_R);

        //collegamento zone
        zona_R->avanti=NULL;
        zona_R->indietro=currR;
        zona_R->link_soprasotto=zona_S;

        zona_S->avanti=NULL;
        zona_S->indietro=currS;
        zona_S->link_mondoreale=zona_R;

        if(currR!=NULL){
            currR->avanti=zona_R;
        }else{
            prima_zona_mondoreale=zona_R;
        }

        if(currS!=NULL){
            currS->avanti=zona_S;
        }else{
            prima_zona_soprasotto=zona_S;
        }

        currR = zona_R;
        currS = zona_S;

    }
    printf("Mappa generata!\n");
}

    void inserisci_zona(){
        int i; 

        struct Zona_mondoreale *curr = prima_zona_mondoreale;

        int nZone = 0;

        while(curr != NULL){
            nZone ++;
            curr = curr->avanti;
        }

        do{
            printf("Inserisci la posizione in cui vuoi inserire la nuova zona: (0 - %d)\n-", nZone);
            scanf("%d", &i);
            pulisciBuffer();

            if (i > nZone){
                printf("Inserisci un valore valido!\n");
            }
        }while(i > nZone);

        struct Zona_mondoreale *currR = prima_zona_mondoreale;
        struct Zona_soprasotto *currS = prima_zona_soprasotto;
        struct Zona_mondoreale *prevR = NULL;
        struct Zona_soprasotto *prevS = NULL; 

        for(int j = 0 ; j < i; j++){
            prevR = currR; 
            prevS = currS; 
            currR = currR->avanti; 
            currS = currS->avanti;
        }

        struct Zona_mondoreale *nuovaR = malloc(sizeof(struct Zona_mondoreale));
        struct Zona_soprasotto *nuovaS = malloc(sizeof(struct Zona_soprasotto));

        //controllo malloc
        if (nuovaR == NULL || nuovaS == NULL){
            printf ("Allocazione in memoria fallita!/n");
            if(nuovaR != NULL){
                free(nuovaR);
            }
            if(nuovaS != NULL){
                free(nuovaS);
            }
            return; //termina funzione se puntatori sono a NULL 
        }

        enum Tipo_zona tipoZona = (Tipo_zona) lancia_dadi(0,9);
        nuovaR->tipo = tipoZona; 
        nuovaS->tipo = tipoZona; 

        printf("SCEGLI GLI ELEMENTI PRESENTI NELLA ZONA:\n"); 
        int sceltaNemico; 
        do{
            printf("Scegli il nemico della tua zona:\n1. Nessun nemico.\n2. Democane(Mondo reale).\n3. Democane(Soprasotto).\n4. Democane(Soprasotto e Mondo reale).\n5. Billi.\n6. Demotorzone.\n-");
            scanf("%d", &sceltaNemico);
            pulisciBuffer();

            if(sceltaNemico < 1 || sceltaNemico > 6){
                printf("Scelta non valida!\n");
            }

            if(sceltaNemico == 6 && esisteDemotorzone()){
                printf("Scelta non valida! Demotorzone gia presente nella mappa.\n");
                sceltaNemico = 0;
            }

        }while(sceltaNemico < 1 || sceltaNemico > 6);

        switch (sceltaNemico)
        {
        case 1:
            nuovaR->nemico = nessun_nemico;
            nuovaS->nemico = nessun_nemico;
            break;
        case 2:
            nuovaR->nemico = democane;
            nuovaS->nemico = nessun_nemico; 
            break;
        case 3:
            nuovaR->nemico = nessun_nemico;
            nuovaS->nemico = democane;
            break;
        case 4:
            nuovaR->nemico = democane;
            nuovaS->nemico = democane; 
            break;
        case 5:
            nuovaR->nemico = billi;
            nuovaS->nemico = nessun_nemico; 
            break;
        case 6:
            nuovaR->nemico = nessun_nemico;
            nuovaS->nemico = demotorzone; 
        }

        int sceltaOggetto;
        printf("Che oggetto vuoi aggiungere a questa zona?:\n");
        do{
            printf("1. Nessun oggetto.\n2. Bicicletta.\n3. Maglietta Fuocoinferno.\n4. Bussola\n5. Schitarrata metallica\n-");
            scanf("%d", &sceltaOggetto);
            pulisciBuffer();

            if(sceltaOggetto < 1 || sceltaOggetto > 5){
                printf("Scelta non valida!\n");
            }
        }while(sceltaOggetto < 1 || sceltaOggetto > 5);

        switch (sceltaOggetto)
        {
        case 1:
            nuovaR->oggetto = nessun_oggetto;
            break;
        case 2:
            nuovaR->oggetto = bicicletta;
            break;
        case 3:
            nuovaR->oggetto = maglietta_fuocoinferno;
            break;
        case 4:
            nuovaR->oggetto = bussola;
            break;
        case 5:
            nuovaR->oggetto = schitarrata_metallica;
            break;
        }

        // Collegamento tra mondoreale e soprasotto, inizzializzazione puntatori indietro 
        nuovaR->avanti = currR;
        nuovaS->avanti = currS;
        nuovaR->indietro = prevR; 
        nuovaS->indietro = prevS;
        
        nuovaR->link_soprasotto = nuovaS;
        nuovaS->link_mondoreale = nuovaR; 

        if(prevR != NULL){
            prevR->avanti = nuovaR;
        } else {
            prima_zona_mondoreale = nuovaR;
        }

        if(prevS != NULL){
            prevS->avanti = nuovaS;
        }else{
            prima_zona_soprasotto = nuovaS;
        }

        if(currR != NULL){
            currR->indietro = nuovaR;
        }
        if(currS != NULL){
            currS->indietro = nuovaS;
        }

        printf("Zona inserita correttamente alla posizione %d\n", i);
    }

    void cancella_zona(){
        int i;

        printf("Insersisci la posizione della zona che vuoi cancellare\n");
        do{
        scanf("%d",&i);
        pulisciBuffer();

        if(i < 0){
            printf("Posizione non valida!\n");
        }
        }while(i < 0);

        struct Zona_mondoreale *currR = prima_zona_mondoreale;
        struct Zona_soprasotto *currS = prima_zona_soprasotto;

        struct Zona_mondoreale *prevR = NULL;
        struct Zona_soprasotto *prevS = NULL; 

        int j = 0; 
        while (j < i && currR != NULL && currS != NULL){
            prevR = currR; 
            prevS = currS; 
            currR = currR->avanti; 
            currS = currS->avanti;
            j++;
        }

        if(currR == NULL || currS == NULL){
            printf("Posizione inserita non valida!\n");
            return;
        }
        
        if(prevR == NULL){  //se si vuole cancellare la prima zona
            prima_zona_mondoreale = currR->avanti;
            prima_zona_soprasotto = currS->avanti;

            if(currR->avanti != NULL){
                currR->avanti->indietro = NULL; 
            }

            if(currS->avanti != NULL){
                currS->avanti->indietro = NULL; 
            }
        } else {    //se si vuole cancellare zona in mezzo o in fondo
            prevR->avanti = currR->avanti;
            prevS->avanti = currS->avanti;

            if(currR->avanti != NULL){
                currR->avanti->indietro = prevR;
            }

            if(currS->avanti != NULL){
                currS->avanti->indietro = prevS;
            }
        }

        //scollego le zone
        currR->link_soprasotto = NULL;
        currS->link_mondoreale = NULL;

        free(currR);
        free(currS);
        printf("Zona %d cancellata.\n", i);
    }

    void stampa_mappa(){
        struct Zona_mondoreale *currR = prima_zona_mondoreale;
        struct Zona_soprasotto *currS = prima_zona_soprasotto;
        int tipoR, tipoS, nemicoR, nemicoS, oggettoR, i = 0;

        while(currR != NULL && currS != NULL){
            tipoR = currR -> tipo;
            tipoS = currS -> tipo;
            nemicoR = currR -> nemico;
            nemicoS = currS-> nemico;
            oggettoR = currR -> oggetto;
            printf("(%d) Mondo Reale: Zona = ", i);
            stampaTipo(tipoR);
            printf(" , Nemico = ");
            stampaNemico(nemicoR);
            printf(" , Oggetto = ");
            stampaOggetto(oggettoR);
            printf(" | Soprasotto: Zona = ");
            stampaTipo(tipoS);
            printf(" , Nemico = ");
            stampaNemico(nemicoS);
            printf("\n");

            currR = currR->avanti;
            currS = currS->avanti;
            i++;
        }
        printf("\n");
    }

    void stampa_zona(){
        int i;

        printf("Insersisci la posizione della zona che vuoi stampare\n");
        do{
        scanf("%d",&i);
        pulisciBuffer();

        if(i < 0){
            printf("Posizione non valida!\n");
        }
        }while(i < 0);

        struct Zona_mondoreale *currR = prima_zona_mondoreale;
        struct Zona_soprasotto *currS = prima_zona_soprasotto;

        int j = 0; 
        while (j < i && currR != NULL && currS != NULL){
            currR = currR->avanti; 
            currS = currS->avanti;
            j++;
        }

        if(currR == NULL || currS == NULL){
            printf("Posizione inserita non valida!\n");
            return;
        }
        int tipoR, tipoS, nemicoR, nemicoS, oggettoR;

        tipoR = currR -> tipo;
        tipoS = currS -> tipo;
        nemicoR = currR -> nemico;
        nemicoS = currS-> nemico;
        oggettoR = currR -> oggetto;
        printf("Mondo Reale: Zona = ");
        stampaTipo(tipoR);
        printf(" , Nemico = ");
        stampaNemico(nemicoR);
        printf(" , Oggetto = ");
        stampaOggetto(oggettoR);
        printf(" | Soprasotto: Zona = ");
        stampaTipo(tipoS);
        printf(" , Nemico = ");
        stampaNemico(nemicoS);
        printf("\n");

    }

    void chiudi_mappa(){
        struct Zona_mondoreale *curr = prima_zona_mondoreale;
        int nZone = 0;

        while(curr != NULL){
            nZone ++;
            curr = curr->avanti;
        }

        if(nZone < 15){
            printf("Mappa non generata correttamente! (Ci sono meno di 15 zone)\n");
            mappa_generata = false;
            return;
        }

        if(!esisteDemotorzone()){
            printf("Mappa non generata correttamente! (Non è presente un demotorzone)\n");
            mappa_generata = false;
            return;
        }

        printf("Mappa generata correttamente!\n");
        mappa_generata = true;
        return;
    }

    void stampaTipo(int tipo){
        switch (tipo){
            case 0:
                printf("bosco"); 
                break;
            case 1: 
                printf("scuola");
                break;
            case 2:
                printf("laboratorio");
                break;
            case 3:
                printf("caverna");
                break;
            case 4:
                printf("strada");
                break;
            case 5:
                printf("giardino");
                break;
            case 6:
                printf("supermercato");
                break;
            case 7:
                printf("centrale elettrica");
                break;
            case 8:
                printf("deposito abbandonato");
                break;
            case 9:
                printf("stazione polizia");
                break;
            default:
                printf("Tipo Sconosciuto.");
                break;
        }
    }

    void stampaNemico(int nemico){
        switch (nemico){
            case 0:
                printf("nessun nemico"); 
                break;
            case 1: 
                printf("billi");
                break;
            case 2:
                printf("democane");
                break;
            case 3:
                printf("demotorzone");
                break;
            default:
                printf("Nemico Sconosciuto.");
                break;
        }
    }
    void stampaOggetto(int oggetto){
        switch (oggetto){
            case 0:
                printf("nessun oggetto"); 
                break;
            case 1: 
                printf("bicicletta");
                break;
            case 2:
                printf("maglietta fuocoinferno");
                break;
            case 3:
                printf("bussola");
                break;
            case 4:
                printf("schitarrata metallica");
                break;
            default:
                printf("Oggetto Sconosciuto.");
                break;
        }
    }


    void generaNemico_Reale(struct Zona_mondoreale *zonaR) {
        int lancioNemici = lancia_dadi(1,12);
        //crea nemici
        switch (lv_difficolta) {
        case 1:
            if(lancioNemici <= 6){
                zonaR->nemico = nessun_nemico; 
            }else if(lancioNemici <= 9){
                zonaR->nemico = democane;
            }else{
                zonaR->nemico = billi;
            }
            break;
        case 2:
            if(lancioNemici <= 4){
                zonaR->nemico = nessun_nemico; 
            }else if(lancioNemici <= 8){
                zonaR->nemico = democane;
            }else{
                zonaR->nemico = billi;
            }
            break;
        case 3:
            if(lancioNemici <= 3){
                zonaR->nemico = nessun_nemico; 
            }else if(lancioNemici <= 8){
                zonaR->nemico = democane;
            }else{
                zonaR->nemico = billi;
            }
            break;
        }
    }

    void generaNemico_Soprasotto(struct Zona_soprasotto *zonaS){
        int lancioNemici = lancia_dadi(1,12);
        //crea nemici
        switch (lv_difficolta) {
        case 1:
            if(lancioNemici <= 8){
                zonaS->nemico = nessun_nemico; 
            }else{
                zonaS->nemico = democane;
            }
            break;
        case 2:
            if(lancioNemici <= 6){
                zonaS->nemico = nessun_nemico; 
            }else{
                zonaS->nemico = democane;
            }
            break;
        case 3:
            if(lancioNemici <= 3){
                zonaS->nemico = nessun_nemico; 
            }else{
                zonaS->nemico = democane;
            }
            break;
        }
    }

    void generaOggetto(struct Zona_mondoreale*zonaR){
        int lancioOggetto = lancia_dadi(1,12);
        int lancioTipoOggetto = lancia_dadi(1,2);
        switch (lv_difficolta)
        {
        case 1:     //LIVELLO FAICLE
            if(lancioOggetto <= 3){
                zonaR->oggetto=nessun_oggetto;
            }else if(lancioOggetto <= 9){
                switch (lancioTipoOggetto)
                {
                case 1:
                    zonaR->oggetto = bicicletta; 
                    break;
                case 2:
                    zonaR->oggetto = bussola; 
                    break;
                }
            }else {
                switch (lancioTipoOggetto)
                {
                case 1:
                    zonaR->oggetto = maglietta_fuocoinferno; 
                    break;
                case 2:
                    zonaR->oggetto = schitarrata_metallica; 
                    break;
                }
            }
            break;

        case 2:     //LIVELLO MEDIO
            if(lancioOggetto <= 4){
                zonaR->oggetto=nessun_oggetto;
            }else if(lancioOggetto <= 10){
                switch (lancioTipoOggetto)
                {
                case 1:
                    zonaR->oggetto = bicicletta; 
                    break;
                case 2:
                    zonaR->oggetto = bussola; 
                    break;
                }
            }else {
                switch (lancioTipoOggetto)
                {
                case 1:
                    zonaR->oggetto = maglietta_fuocoinferno; 
                    break;
                case 2:
                    zonaR->oggetto = schitarrata_metallica; 
                    break;
                }
            }
            break;

            case 3:     //LIVELLO DIFFICILE
            if(lancioOggetto <= 7){
                zonaR->oggetto=nessun_oggetto;
            }else if(lancioOggetto <= 11){
                switch (lancioTipoOggetto)
                {
                case 1:
                    zonaR->oggetto = bicicletta; 
                    break;
                case 2:
                    zonaR->oggetto = bussola; 
                    break;
                }
            }else {
                switch (lancioTipoOggetto)
                {
                case 1:
                    zonaR->oggetto = maglietta_fuocoinferno; 
                    break;
                case 2:
                    zonaR->oggetto = schitarrata_metallica; 
                    break;
                }
            }
            break;
        }
    }
    bool esisteDemotorzone(){
            struct Zona_soprasotto *curr = prima_zona_soprasotto;

            while(curr != NULL){
            if(curr->nemico == demotorzone){
                return true;
            }
            curr = curr->avanti;
            }
            return false;  
    }

    void avanza(Giocatore *giocatore){
        if(giocatore == NULL){
            printf("Errore, giocatore non esistente.\n");
            return;
        }
        if(giocatore->mondo == false){
            if(giocatore->pos_mondoreale != NULL){
                struct Zona_mondoreale *zona = giocatore->pos_mondoreale;

                if(zona->nemico != nessun_nemico){
                    printf("Devi prima combattere il nemico della zona!\n");

                    combatti(giocatore);
                    return;
                }

                if(zona->avanti == NULL){
                    printf("Sei gia all'ultima zona del mondo reale!\n");
                    return;
                }

                giocatore->pos_mondoreale = zona->avanti;
                printf("Sei avanzato nella zona successiva!\n");
            }
        }else{

            if(giocatore->pos_soprasotto != NULL){
            struct Zona_soprasotto *zona = giocatore->pos_soprasotto;

            if(zona->nemico != nessun_nemico){
                printf("Devi prima combattere il nemico della zona!\n");

                combatti(giocatore);
                return;
            }

            if(zona->avanti == NULL){
                printf("Sei gia all'ultima zona del soprasotto!\n");
                return;
            }

            giocatore->pos_soprasotto = zona->avanti;
            printf("Sei avanzato nella zona successiva!\n");
            }
        }
    }

    void indietreggia (Giocatore *giocatore){
        if(giocatore == NULL){
            printf("Errore, giocatore non esistente.\n");
            return;
        }
        if(giocatore->mondo == false){
            if(giocatore->pos_mondoreale != NULL){
                struct Zona_mondoreale *zona = giocatore->pos_mondoreale;

                if(zona->nemico != nessun_nemico){
                    printf("Devi prima combattere il nemico della zona!\n");

                    combatti(giocatore);
                    return;
                }

                if(zona->indietro == NULL){
                    printf("Non puoi indietreggiare!\n");
                    return;
                }

                giocatore->pos_mondoreale = zona->indietro;
                printf("Sei tornato alla zona precedente!\n");
            }
        }else{

            if(giocatore->pos_soprasotto != NULL){
            struct Zona_soprasotto *zona = giocatore->pos_soprasotto;

            if(zona->nemico != nessun_nemico){
                printf("Devi prima combattere il nemico della zona!\n");

                combatti(giocatore);
                return;
            }

            if(zona->indietro == NULL){
                printf("Non puoi indietreggiare!\n");
                return;
            }

            giocatore->pos_soprasotto = zona->indietro;
            printf("Sei tornato alla zona precedente!\n");
            }
        }
    }

    void cambia_mondo(Giocatore *giocatore){
        if(giocatore == NULL){
            printf("Errore giocatore.\n");
            return;
        }
        //DA MONDO REALE A SOPRASOTTO:
        if(giocatore->mondo == false){

            Zona_mondoreale *zonaR = giocatore->pos_mondoreale;
            if(zonaR == NULL){
                printf("Errore zona!\n");
                return;
            }

            if(zonaR->nemico != nessun_nemico){
                printf("Devi prima sconfiggere il nemico!\n");
                combatti(giocatore);
                return;
            }

            giocatore->pos_soprasotto = zonaR->link_soprasotto;
            giocatore->mondo = true;

            printf("Hai cambiato mondo! (Reale->Soprasotto).\n");
            return;
        }else{ //DA SOPRASOTTO A MONDO REALE:

            Zona_soprasotto *zonaS = giocatore->pos_soprasotto;
            if(zonaS == NULL){
                printf("Errore zona!\n");
                return;
            }

            int lancio = lancia_dadi(1,20);

            printf("Tentativo di fuga dal soprasotto...\n");
            if(lancio > giocatore->fortuna){
                printf("Fuga fallita! (Tiro: %d | Fortuna: %d)\n", lancio, giocatore->fortuna);
                return;
            }

            giocatore->pos_mondoreale = zonaS->link_mondoreale;
            giocatore->mondo = false;

            printf("Hai cambiato mondo! (Soprasotto->Reale).\n");
            return;
        }
    }

    void combatti(Giocatore *giocatore){
        bool fine_combattimento = false;
        Tipo_nemico nemico; 
        int hp_nemico;
        int att_nemico, dif_nemico;
        int moltiplicatore = 1;
        inCombattimento = true;

        if(giocatore->mondo == true){
            nemico = giocatore->pos_soprasotto->nemico;
        }else{
            nemico = giocatore->pos_mondoreale->nemico;
        }  

        switch(nemico){
        case billi:
            hp_nemico = 25;
            att_nemico = 6;
            dif_nemico = 5;
            break;
        case democane:
            hp_nemico = 40;
            att_nemico = 10;
            dif_nemico = 8;
            break;
        case demotorzone:
            hp_nemico = 90;
            att_nemico = 14;
            dif_nemico = 12;
            break;
        default:
            return;
    }

        if(lv_difficolta == 2) {
            moltiplicatore = 2;
        }
        if(lv_difficolta == 3) {
            moltiplicatore = 3;
        }

        hp_nemico = hp_nemico * moltiplicatore;

        // turni combattimento
        int turni[numero_giocatori];
        for(int i = 0; i < numero_giocatori; i++)
            turni[i] = 0;

        printf("\n--- INIZIO COMBATTIMENTO ---\n");

        while(!fine_combattimento){

        /* ---------------- TURNO GIOCATORI ---------------- */
            int minTurni = -1;
            for(int i = 0; i < numero_giocatori; i++){
                if(giocatori[i]->HP > 0){
                    if(minTurni == -1 || turni[i] < minTurni){
                        minTurni = turni[i];
                    }
                }
            }
            int idx;
            do{
                idx = lancia_dadi(0, numero_giocatori - 1);
            }while(turni[idx] != minTurni || giocatori[idx]->HP <= 0);

            Giocatore *g = giocatori[idx];
            turni[idx]++;

            printf("\nTocca a %s\n", g->nome_giocatore);
            printf("Nemico: ");
            stampaNemico(nemico);
            printf(" | HP nemico: %d\n", hp_nemico);

            int scelta;
            printf("1. Attacco psichico\n2. Usa Oggetto\n3. Fuga\n-");
            scanf("%d", &scelta);
            pulisciBuffer();
            int effetto = 0;

            if(scelta == 1){
                int tiro = lancia_dadi(1,20);

                int perc_att = g->attacco_psichico * 4;
                if(perc_att > 95) perc_att = 95;

                int danno = (tiro * perc_att) / 100;

                int perc_dif = dif_nemico * 4;
                if(perc_dif > 80) perc_dif = 80;

                danno -= (danno * perc_dif) / 100;
                if(danno < 1) danno = 1;

                // critico
                if(lancia_dadi(1,20) <= g->fortuna){
                    danno += danno / 2;
                    printf("COLPO CRITICO!\n");
                }

                hp_nemico -= danno;
                printf("%s infligge %d danni!\n", g->nome_giocatore, danno);
                
                if(hp_nemico <= 0){
                    if(nemico == demotorzone){
                            printf("Demotorzone sconfitto! Partita conclusa.\n");
                            gameOver = true;
                            fine_combattimento = true;
                            mappa_generata = false; 
                            return; 
                    }else{
                        printf("Nemico sconfitto!\n");
                        if(g->mondo)
                            g->pos_soprasotto->nemico = nessun_nemico;
                        else
                            g->pos_mondoreale->nemico = nessun_nemico;

                        fine_combattimento = true;
                    }
                }
            }else if(scelta == 2){
                printf("Che oggetto vuoi usare? (0-2)\n");
                int scelta_zaino;
                do{
                    stampaZaino(g);
                    printf("-");
                    scanf("%d", &scelta_zaino);
                    pulisciBuffer();
                    if(scelta_zaino < 0 || scelta_zaino > 2){
                        printf("Scelta non valida!\n");
                    }
                }while (scelta_zaino < 0 || scelta_zaino > 2);
                
                
                effetto = utilizza_oggetto(g, scelta_zaino + 1);

                if(effetto > 0){
                    printf("Il nemico ha subito %d danni dall'oggetto!\n", effetto);

                    hp_nemico-=effetto;

                    if(hp_nemico <= 0){

                        if(nemico == demotorzone){
                            printf("Demotorzone sconfitto! Partita conclusa.\n");
                            gameOver = true;
                            fine_combattimento = true;
                            return; 
                        }else{
                            printf("Nemico sconfitto!\n");
                            if(g->mondo){
                                g->pos_soprasotto->nemico = nessun_nemico;
                            }else{
                                g->pos_mondoreale->nemico = nessun_nemico;
                            }
                            fine_combattimento=true;
                        }
                    }
                }

            }else if(scelta == 3){
                int fuga = lancia_dadi(1,20);
                if(fuga <= g->fortuna){
                    printf("Fuga riuscita!\n");
                    fine_combattimento = true;
                    break;
                }else{
                    printf("Fuga fallita!\n");
                }
            }

            /* TURNO NEMICO */
            if(effetto != -1){
                printf("\nIl nemico contrattacca!\n");

                int tiro = lancia_dadi(1,20);
                int perc_att = att_nemico * 4;
                if(perc_att > 90) perc_att = 90;

                int danno = (tiro * perc_att) / 100;

                int perc_dif = g->difesa_psichica * 4;
                if(perc_dif > 80) perc_dif = 80;

                danno -= (danno * perc_dif) / 100;
                if(danno < 1) danno = 1;

                g->HP -= danno;
                printf("%s subisce %d danni! (HP rimasti: %d)\n",
                    g->nome_giocatore, danno, g->HP);

                if(g->HP <= 0){
                    printf("%s è stato sconfitto!\n", g->nome_giocatore);
                }
            }

            // controllo game over
            bool tutti_morti = true;
            for(int i = 0; i < numero_giocatori; i++)
                if(giocatori[i]->HP > 0)
                    tutti_morti = false;

            if(tutti_morti){
                printf("\nTUTTI I GIOCATORI SONO STATI SCONFITTI!\n");
                gameOver = true;
                fine_combattimento = true;
                mappa_generata = false; 
                return; 
            }
    }

    printf("\n--- FINE COMBATTIMENTO ---\n");


    inCombattimento = false;
    return; 
        
    }
 
    int utilizza_oggetto(Giocatore *g, int slot){ //return: 0 = nessun effetto , -1 = nessun oggetto selezionato , >0 = danno. 
        int risultato = 0;
        int indice = slot -1; 
        int oggetto = g->zaino[indice]; 

        switch(oggetto) {
        case nessun_oggetto:
            printf("Non hai selezionato alcun oggetto.\n");
            return -1; 

        case bicicletta:
                int regen = lancia_dadi(10,40); 
                
                if(g->HP+regen > 50){
                    regen = 50 - g->HP; 
                }
                g->HP += regen; 
                printf("%s usa la bicicletta! Vengono rigenerati %d Hp!\n", g->nome_giocatore, regen);
            break;

        case maglietta_fuocoinferno:
            printf("%s indossa la maglietta fuocoinferno! Attacco e difesa aumentati!\n", g->nome_giocatore);
            g->attacco_psichico += 5; 
            g->difesa_psichica += 5;
            break;

        case bussola:
            printf("%s usa la bussola! Fortuna aumentata!\n", g->nome_giocatore);
            g->fortuna += 7; 
            break;

        case schitarrata_metallica:
            if(inCombattimento){
                printf("%s suona la schitarrata metallica! Attacco potente inflitto!\n", g->nome_giocatore);
                risultato = 30; 
            }else{
                printf("Puoi usare la schitarrata metallica solo in combattimento!\n");
            }
            break;
        }   

        // rimuovi oggetto dallo zaino
        g->zaino[indice] = nessun_oggetto; 

        return risultato;
    }

    void stampa_zonaGiocatore(Giocatore *giocatore){
        int indice = 0; 

        if(giocatore->mondo == false){
            Zona_mondoreale *zona = prima_zona_mondoreale;

            while(zona != NULL && zona != giocatore->pos_mondoreale){
                zona = zona->avanti;
                indice++;
            }
            
            printf("Mondo Reale: Zona (%d)= ", indice);
            stampaTipo(zona->tipo);
            printf(" , Nemico = ");
            stampaNemico(zona->nemico);
            printf(" , Oggetto = ");
            stampaOggetto(zona->oggetto);
            printf("\n");
            
        }else{
            Zona_soprasotto *zona = prima_zona_soprasotto;

            while(zona != NULL && zona != giocatore->pos_soprasotto){
                zona = zona->avanti;
                indice++;
            }
            
            printf("Soprasotto: Zona (%d)= ", indice);
            stampaTipo(zona->tipo);
            printf(" , Nemico = ");
            stampaNemico(zona->nemico);
            printf("\n");
        }        
        
    }

    void stampa_giocatore(Giocatore *giocatore){
        printf("Nome: %s\nAtk: %hu\nDef: %hu\nFor: %hu\nHp: %d\nZaino:\n", giocatore->nome_giocatore, giocatore->attacco_psichico, giocatore->difesa_psichica , giocatore-> fortuna , giocatore->HP);
        stampaZaino(giocatore);
    }

    void raccogli_oggetto(Giocatore *giocatore){
        int oggetto;
        int slot = -1;
        
        if(giocatore->mondo){
            printf("Non ci sono oggetti nel Soprasotto!\n");
            return;
        }else{
            oggetto = giocatore->pos_mondoreale->oggetto;
        }

        for(int i = 0; i < 3; i++){
            if(giocatore->zaino[i] == nessun_oggetto){
                slot = i;
                break;
            }
        }

        if(slot == -1){
            printf("Il tuo zaino è pieno! Non puoi raccogliere l'oggetto.\n");
            return;
        }

        giocatore->pos_mondoreale->oggetto = nessun_oggetto;
        giocatore->zaino[slot] = oggetto;

        switch(oggetto) {
        case nessun_oggetto:
            printf("Non ci sono oggetti in questa zona.\n");
            break; 
        case bicicletta:
            printf("Hai raccolto una bicicletta!\n");
            break;
        case maglietta_fuocoinferno:
            printf("Hai raccolto una maglietta fuocoinferno!\n");
            break;
        case bussola:
            printf("Hai raccolto una bussola!\n");
            break;
        case schitarrata_metallica:
            printf("Hai raccolto schitarrata metallica!\n");
            break;
        }   
    }

    void set_difficolta(){
        printf("Scegli la difficolta:\n1. Facile.\n2. Normale.\n3. Difficile.\n-");
            do{
                scanf("%hu", &lv_difficolta);
                pulisciBuffer();
                if(lv_difficolta < 1 || lv_difficolta > 3){
                    printf("Scelta non valida!\n");
                }
            }while(lv_difficolta < 1 || lv_difficolta > 3);
        printf("Difficolta impostata!\n");
    }