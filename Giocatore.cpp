#include "Giocatore.h"
#include "Gioco.h"
#include <iostream>
#include <algorithm>
#include <fstream>

Giocatore::Giocatore() : _punteggio(0), _nome("") {}

Giocatore::Giocatore(std::string nome) : _punteggio(0), _nome(nome) {}

void Giocatore::setPunteggio(const int &p) {
	_punteggio = p;
}

int Giocatore::getPunteggio() const {
	return _punteggio;
}

size_t Giocatore::size() const {
	return _tessere.size();
}

std::string Giocatore::getName() const {
	return _nome;
}

char Giocatore::getTessera(const size_t pos) const {
	return _tessere.at(pos);
}

void Giocatore::updatePunteggio(const int &p) {
	_punteggio += p;
}

void Giocatore::addTessera(const char &c) {
	_tessere.push_back(c);
}

void Giocatore::removeTessere(const std::string word) {
	size_t endIter = word.size();
	// Rimuove le tessere dall'inizio dal vettore fino a endIter
	_tessere.erase(_tessere.begin(), _tessere.begin() + endIter);
}

void Giocatore::printStatus() {
	std::cout << _nome << std::endl
		<< "Punteggio: " << _punteggio << std::endl
		<< "Tessere: ";
	for (size_t i = 0; i < _tessere.size(); i++) {
		std::cout << _tessere.at(i) << " - ";
	}
	std::cout << std::endl;
}

void Giocatore::shuffle() {
	std::random_shuffle(_tessere.begin(), _tessere.end());
}

bool Giocatore::parolaComponibile(std::string parola) {
	for (size_t i = 0; i < parola.size(); i++) {
		bool charFound = false;		// Dice se il char corrispondente è stato trovato
		bool jollyFound = false;	// Dice se è stato trovato un jolly fra i caratteri
		size_t jollyPos;			// La posizione del jolly (se è stato trovato)
		for (size_t j = i; j < _tessere.size(); j++) {
			if (_tessere.at(j) == 'J') {
				jollyFound = true;
				jollyPos = j;
			}
			if (parola.at(i) == _tessere.at(j)) {
				// Il carattere è stato trovato
				charFound = true;
				// Se il carattere è stato trovato subito, è inutile scambiare;
				// viceversa, è necessario scambiare
				if (j != i){
					std::swap(_tessere.at(i), _tessere.at(j));
				}
				j = _tessere.size() - 1;
			}
		}
		if (!charFound && jollyFound) {
			// Il carattere non è stato trovato, ma è possibile sostituirlo con un jolly
			charFound = true;
			std::swap(_tessere.at(i), _tessere.at(jollyPos));
		}
		if (!charFound) {
			return false;
		}
	}
	return true;
}

bool Giocatore::parolaInDizionario(std::string parola, const std::string dizionario) {
	// Se la parola è lunga due caratteri, è reputata valida qualsiasi essa sia
	if (parola.size() <= 2) {
		return true;
	}

	for (size_t i = 0; i < parola.size(); i++) {
		// Porta la parola tutta in minuscolo
		parola[i] = tolower(parola[i]);
	}

	std::fstream file;
	if (!file.is_open()) {
		file.open(dizionario);
	}
	std::string riga;
	while (std::getline(file, riga)) {
		/*if (riga == "è") {
			getline(file, riga);
		}*/
		/*if (parola < riga) {
			file.close();
			return false;
		}*/
		if (parola == riga) {
			file.close();
			return true;
		}
	}
	file.close();
	return false;
}


bool Giocatore::isEmpty() {
	for (size_t i = 0; i < MAXTASS; i++) {
		if (_tessere.at(i) != ' ') {
			return false;
		}
	}
	return true;
}


std::string Giocatore::toLower(std::string base) {
	std::string upper;
	for (size_t i = 0; i < base.size(); i++) {
		upper.push_back(tolower(base[i]));
	}
	return upper;
}
