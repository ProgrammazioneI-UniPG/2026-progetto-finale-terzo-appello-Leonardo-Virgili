void imposta_gioco();
void gioca();
void termina_gioco();
typedef enum bool {true = 1, false = 0} bool; //tipo bool da usare all'interno della struct giocatore.

typedef enum Tipo_zona{bosco, scuola, laboratorio, caverna, strada, giardino, supermercato, centrale_elettrica, deposito_abbandonato, stazione_polizia}Tipo_zona;
typedef enum Tipo_nemico{nessun_nemico, billi, democane, demotorzone}Tipo_nemico;
typedef enum Tipo_oggetto{nessun_oggetto, bicicletta, maglietta_fuocoinferno, bussola, schitarrata_metallica}Tipo_oggetto;

typedef struct Zona_mondoreale {
    Tipo_zona tipo;
    Tipo_nemico nemico; 
    Tipo_oggetto oggetto; 
    struct Zona_mondoreale *avanti;
    struct Zona_mondoreale *indietro;
    struct Zona_soprasotto *link_soprasotto; 
}Zona_mondoreale;

typedef struct Zona_soprasotto{
    Tipo_zona tipo;
    Tipo_nemico nemico;

    struct Zona_soprasotto *avanti;
    struct Zona_soprasotto *indietro;
    struct Zona_mondoreale *link_mondoreale;
}Zona_soprasotto;

typedef struct Giocatore {
    char nome_giocatore [21];
    bool mondo;                         //true = soprasotto, false = mondo reale
    Zona_mondoreale *pos_mondoreale;
    Zona_soprasotto *pos_soprasotto; 
    unsigned short int attacco_psichico;
    unsigned short int difesa_psichica;
    unsigned short int fortuna;
    int HP;
    Tipo_oggetto zaino [3]; 
}Giocatore;