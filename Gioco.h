#pragma once

#include "Giocatore.h"
#include "Casella.h"

#define N 17

class Gioco	:  public Giocatore
{
public:
	// Costruttore per il gioco a 2,3 o 4 giocatori
	Gioco(size_t numGiocatori);

	// Il turno di gioco
	size_t turno = 0;

	// Loop principale del gioco
	void gameLoop();

	// Condizioni di vittoria/stop del gioco
	bool gameStops(size_t currPlayer);

	// Riempe la griglia con i valori iniziali dello scarabeo
	void inizializzaGriglia();
	// Inizializza il vettore delle tessere
	void inizializzaTessere();
	// Stampa la griglia
	void printGriglia();

	
	// Metodi pubblici per aggiungere giocatori (max 4) e tessere totali
	void push_back(const Giocatore &g);
	void push_back(const char &c, const size_t numTessere);

	// Converte tutta la stringa in uppercase
	std::string toUpper(std::string base) {
		std::string upper;
		for (size_t i = 0; i < base.size(); i++) {
			upper.push_back(toupper(base[i]));
		}
		return upper;
	}

	// Permette di recuperare il numero dei giocatori attuali
	size_t getPlayerNumbers();

	// Ritorna un singolo giocatore
	Giocatore &getPlayer(const size_t playerNumber);
	// Modifica un giocatore
	void setPlayer(const Giocatore &g, const size_t playerNumber);

	// Funzione che prende una lettera random tra quelle delle tessere, e la inserisce nella
	// mano di un giocatore
	char pickRandomLetter();

	// Stampa le informazioni attuali relative a tutti i giocatori
	void printStatus();

	// Funzione per la distribuzione iniziale delle tessere
	void distribuisciTessere();

	// Dice se la griglia di gioco è vuota
	bool emptyGrid();

	// Restituisce le lettere incrociate tra una parola e la griglia di gioco
	std::vector <char> lettereIncrociate(const std::string parola, const size_t x,
		const size_t y, const char direzione);

	// Controlla che la nuova parola inserita incroci almeno una lettera nella griglia
	bool paroleIncrociate(const std::string parola, const size_t x,
		const size_t y, const char direzione);

	// Aggiorna il punteggio di uno dei giocatori dopo aver inserito
	// la parola nella griglia con successo
	void updatePunteggioGiocatore(int punteggio, size_t turn);

	// Inserisce una parola all'interno della griglia
	bool inserisciParolaInGriglia(const std::string parola, const size_t x, 
		const size_t y, const char direzione, const size_t pNumb);

	// Calcola il punteggio di una parola inserita nella griglia
	int calcolaPunteggio(const std::string parola, const size_t x,
		const size_t y, const char direzione);

	// Aggiunge gli ultimi punti extra riguardanti grandezza della parola e casi speciali
	int extraPoints(const std::string parola);

	// L'helper che cerca di trovare la parola migliore possibile da inserire
	std::string findBestWord(const std::string dizionario, Giocatore g);

	// Schermata che permette al giocatore di compiere azioni
	void sceltaGiocatore(Giocatore g);

	// Funzione che ritorna il tipo di una casella
	void casellaType(size_t x, size_t y);
	
	// calcola il punteggio di una singola lettera
	int calcolaPunteggioLettera(char c);

	// Trova il vincitore fra i quattro giocatori
	size_t findAWinner();

	// Inserisce i dati relativi a posizione (x,y), direzione e parola da inserire
	void inserisciDati(int &x, int &y, char &dir, std::string &word);

private:
	std::vector <Giocatore> _players;	// Numero e nome dei giocatori
	Casella _griglia[N][N];				// La griglia del gioco
	std::vector <char> _tessereTot;		// Le tessere disponibili
};

