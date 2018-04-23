#pragma once

#include <string>
#include <vector>

#define MAXTASS 8

class Giocatore
{
public:
	// Costruttore di default e con il nome del giocatore
	Giocatore();
	Giocatore(std::string nome);

	// Setter e getter
	void setPunteggio(const int &p);
	int getPunteggio() const;
	// Ritorna la grandezza del vettore _tessere
	size_t size() const;
	std::string getName() const;
	// Ritorna la tessera in una certa posizione
	char getTessera(const size_t pos) const;

	// Aggiorna il punteggio del giocatore
	void updatePunteggio(const int &p);
	
	// Aggiunge una tessera al giocatore
	void addTessera(const char &c);

	// Rimuove le tessere della parola inserita
	void removeTessere(const std::string word);

	// Stampa nome, punteggio e tessere in possesso del giocatore
	void printStatus();

	// Mischia casualmente le tessere del giocatore
	void shuffle();

	// Controlla se la parola è componibile con le tessere possedute
	bool parolaComponibile(std::string parola);

	// Controlla se la parola è presente nel vocabolario
	bool parolaInDizionario(std::string parola, const std::string dizionario);

	// Controlla se non vi sono più tessere all'interno del vettore
	bool isEmpty();

	// Converte una stringa in caratteri minuscoli
	std::string toLower(std::string base);


private:
	int _punteggio;			// Il punteggio corrente del giocatore
	std::vector <char> _tessere;  // I tasselli attualmente posseduti dal giocatore
	std::string _nome;		// Il nome del giocatore

};

