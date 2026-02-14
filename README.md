# progetto-esame-2026-Cosestrane

##NOME: Leonardo

##COGNOME: Virgili

##MATRICOLA: 395136

--Funzioni implementate--

- Sistema di punti vita: Aggiunto un sistema di HP per ogni giocatore e nemico, in modo da rendere piu immediato il combattimento. 

- Difficolta del gioco: Permette di scegliere la difficolta del gioco (FACILE, MEDIO, DIFFICILE). Le difficolta modificano i seguenti aspetti: 1) Comparsa dei mostri. 2) Comparsa degli oggetti. 

        === Probabilità di comparsa dei nemici ===

        1) Mondo reale
        -------------------------------------
        Difficoltà | Nessun nemico | Democane | Billi
        -----------|---------------|----------|------
        Facile (1) |     50%       |   25%    |  25%
        Medio  (2) |   33,3%       | 33,3%    | 33,3%
        Difficile(3)|    25%       | 41,7%    | 33,3%

        2) Mondo Soprasotto (DEMOTORZONE GARANTITO IN UNA ZONA CASUALE) 
        ----------------------------------------------
        Difficoltà | Nessun nemico | Democane
        -----------|---------------|----------
        Facile (1) |    66,7%       | 33,3%
        Medio  (2) |     50%        | 50%
        Difficile(3)|    25%        | 75%
        
        === HP dei nemici === 
        
        1) HP base: 
        Nemico	HP base
        Billi	25
        Democane	40
        Demotorzone	90
        
        2) Moltiplicatore della difficolta: 
        --------------------------------
        Difficolta	  | Moltiplicatore HP
        --------------------------------
        Facile (1)	  |     ×1
        Normale (2)	  |     ×2
        Difficile (3) |     ×3
        
        - In base alla difficolta, si applica un moltiplicatore agli hp dei nemici. 

        === Probabilità di comparsa di oggetti ===

        LIVELLO FACILE
        - Nessun oggetto:        25%
        - Oggetti comuni:        50%
          - Bicicletta:          25%
          - Bussola:             25%
        - Oggetti rari:          25%
          - Maglietta Fuoco Inferno:    12.5%
          - Schitarrata Metallica:      12.5%

        LIVELLO MEDIO
        - Nessun oggetto:        33.3%
        - Oggetti comuni:        50%
          - Bicicletta:          25%
          - Bussola:             25%
        - Oggetti rari:          16.7%
          - Maglietta Fuoco Inferno:    8.3%
          - Schitarrata Metallica:      8.3%

        LIVELLO DIFFICILE
        - Nessun oggetto:        58.3%
        - Oggetti comuni:        33.3%
          - Bicicletta:          16.7%
          - Bussola:             16.7%
        - Oggetti rari:          8.3%
          - Maglietta Fuoco Inferno:    4.15%
          - Schitarrata Metallica:      4.15%
  
  - Menu se il gioco è gia stato impostato: Quando il gioco è gia stato impostato, viene visualizzato un menu che permette la modifica dei parametri creati precedentemente in imposta gioco. 
  
  
        

