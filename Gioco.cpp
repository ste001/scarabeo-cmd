#include "Gioco.h"
#include <time.h>
#include <fstream>
#include <algorithm>


Gioco::Gioco(size_t numGiocatori) {
	// Setta i tipi delle caselle per la griglia
	inizializzaGriglia();
	// Inizializza i giocatori
	for (size_t i = 0; i < numGiocatori; i++) {
		std::cout << "Giocatore " << i + 1 << " inserisci il tuo nome: ";
		std::string nomeG;
		std::cin >> nomeG;
		// Controllo sulla lunghezza del nome giocatore
		while (nomeG.size() < 3 || nomeG.size() > 32) {
			std::cout << "Nome troppo corto o troppo lungo." <<
				std::endl <<  "Inserire un nome fra 3 e 32 caratteri: ";
			std::cin >> nomeG;
		}
		Giocatore g = Giocatore(nomeG);
		push_back(g);
	}
	// Aggiunge e mischia le tessere
	inizializzaTessere();
	// Lancia il loop principale del gioco
	gameLoop();
}

void Gioco::gameLoop() {
	std::cout << "-------------------------------------------" << std::endl <<
		"---------------SCARABEO-CONSOLE------------" << std::endl <<
		"-------------------------------------------" << std::endl;
	distribuisciTessere();
	printGriglia();
	while (!gameStops((turno - 1) % _players.size())) {
		// Stabilisco il giocatore a cui tocca giocare la mano
		size_t pNumb = turno % _players.size();
		// Stampa lo status del giocatore
		_players[pNumb].printStatus();

		// Schermata con scelta del giocatore
		sceltaGiocatore(_players[pNumb]);
		// Inserimento parola
		std::string word;
		int x, y;
		char dir;
		inserisciDati(x, y, dir, word);

		// Controllo sull'incrocio delle parole nella griglia
		while (!paroleIncrociate(word, x, y, dir) && turno != 0) {
			std::cout << "Le parole non si incrociano. Reinserire posizione, direzione e parola." << std::endl;
			inserisciDati(x, y, dir, word);
		}

		// Aggiungo temporaneamente le lettere che si incrociano a quelle del giocatore
		std::vector <char> toAdd = lettereIncrociate(word, x, y, dir);
		while (toAdd.size() != 0) {
			_players[pNumb].addTessera(toAdd[0]);
			toAdd.pop_back();
		}

		// Controllo sulla posizione della parola nella griglia
		while (dir == 'o' && y + word.size() > N-1) {
			std::cout << "Impossibile inserire la parola in questa posizione." << std::endl
				<< "Reinserire posizone, direzione e parola" << std::endl;
			inserisciDati(x, y, dir, word);
		}
		while (dir == 'v' && x + word.size() > N - 1) {
			std::cout << "Impossibile inserire la parola in questa posizione." << std::endl
				<< "Reinserire posizione, direzione e parola " << std::endl;
			inserisciDati(x, y, dir, word);
		}

		// Controllo sull'inserimento della parola
		while (!_players[pNumb].parolaComponibile(word) || !parolaInDizionario(word, "words_italian.win.txt")) {
			std::cout << "Parola non valida. Reinserire posizione, direzione e parola " << std::endl;
			inserisciDati(x, y, dir, word);
		}

		// Inserimento della parola
		inserisciParolaInGriglia(word, x, y, dir, pNumb);

		// Stampa della griglia dopo l'inserimento
		printGriglia();
		// Toglie le lettere usate
		_players[pNumb].removeTessere(word);
		// Aggiorna le lettere del giocatore
		while (_players[pNumb].size() != MAXTASS) {
			char randTessera = pickRandomLetter();
			_players[pNumb].addTessera(randTessera);
		}
		// Stampa le info del giocatore aggiornate
		_players[pNumb].printStatus();
		std::cout << std::endl;

		// Prossimo turno
		turno++;
	}
	size_t winner = findAWinner();
	std::cout << "Vincitore della partita: giocatore " << winner + 1 << ", " << _players[winner].getName()
		<< " con un punteggio di " << _players[winner].getPunteggio() << " punti." << std::endl;
}

bool Gioco::gameStops(size_t currPlayer) {
	if (_players[currPlayer].isEmpty()){
	// Quando il gioco si ferma, l'ultimo giocatore guadagna anche le lettere degli altri
		int pointsToAdd = 0;
		for (size_t i = 0; i < _players.size(); i++) {
			if (i != currPlayer) {
				for (size_t j = 0; j < _players[i].size(); j++) {
					pointsToAdd = calcolaPunteggioLettera(_players[i].getTessera(j));
				}
			}
		}
		// Aggiorna il punteggio finale del giocatore
		_players[currPlayer].updatePunteggio(pointsToAdd);
		return true;
	}
	return false;
}

void Gioco::inizializzaGriglia() {
	_griglia[0][0].setType(type::tp); _griglia[0][4].setType(type::dl);
	_griglia[0][8].setType(type::tp); _griglia[0][12].setType(type::dl);
	_griglia[0][16].setType(type::tp); _griglia[1][1].setType(type::dp);
	_griglia[1][6].setType(type::tl); _griglia[1][10].setType(type::tl);
	_griglia[1][15].setType(type::dp); _griglia[2][2].setType(type::dp);
	_griglia[2][7].setType(type::dl); _griglia[2][9].setType(type::dl);
	_griglia[2][14].setType(type::dp); _griglia[3][3].setType(type::dp);
	_griglia[3][8].setType(type::dl); _griglia[3][13].setType(type::tp);
	_griglia[4][0].setType(type::dl); _griglia[4][4].setType(type::dp);
	_griglia[4][12].setType(type::dp); _griglia[4][16].setType(type::dl);
	_griglia[5][5].setType(type::dp); _griglia[5][11].setType(type::dp);
	_griglia[6][1].setType(type::tl); _griglia[6][6].setType(type::tl);
	_griglia[6][10].setType(type::tl); _griglia[6][15].setType(type::tl);
	_griglia[7][2].setType(type::dl); _griglia[7][7].setType(type::dl);
	_griglia[7][9].setType(type::dl); _griglia[7][14].setType(type::dl);
	_griglia[8][0].setType(type::tp); _griglia[8][3].setType(type::dl);
	_griglia[8][16].setType(type::tp); _griglia[8][13].setType(type::dl);
	_griglia[9][2].setType(type::dl); _griglia[9][7].setType(type::dl);
	_griglia[9][9].setType(type::dl); _griglia[9][14].setType(type::dl);
	_griglia[10][1].setType(type::tl); _griglia[10][6].setType(type::tl);
	_griglia[10][10].setType(type::tl); _griglia[10][15].setType(type::tl);
	_griglia[11][5].setType(type::dp); _griglia[11][11].setType(type::dp);
	_griglia[12][0].setType(type::dl); _griglia[12][4].setType(type::dp);
	_griglia[12][12].setType(type::dp); _griglia[12][16].setType(type::dl);
	_griglia[13][3].setType(type::dp); _griglia[13][8].setType(type::dl);
	_griglia[13][13].setType(type::tp); _griglia[14][2].setType(type::dp);
	_griglia[14][7].setType(type::dl); _griglia[14][9].setType(type::dl);
	_griglia[14][14].setType(type::dp); _griglia[15][1].setType(type::dp);
	_griglia[15][6].setType(type::tl); _griglia[15][10].setType(type::tl);
	_griglia[15][15].setType(type::dp); _griglia[16][0].setType(type::tp); 
	_griglia[16][4].setType(type::dl); _griglia[16][8].setType(type::tp);
	_griglia[16][12].setType(type::dl); _griglia[16][16].setType(type::tp);
}

void Gioco::inizializzaTessere() {
	
    push_back('A', 12);		push_back('E', 12);		push_back('I', 12);
	push_back('O', 12);		push_back('U', 4);		push_back('C', 7);
	push_back('R', 7);		push_back('S', 7);		push_back('T', 7);
	push_back('L', 6);		push_back('M', 6);		push_back('N', 6);
	push_back('B', 4);		push_back('D', 4);		push_back('F', 4);
	push_back('G', 4);		push_back('P', 4);		push_back('V', 4);
	push_back('H', 2);		push_back('Q', 2);		push_back('Z', 2);
	push_back('J', 2);  // Jolly 

	// Mischia le lettere del vettore dopo averle inserite
	std::random_shuffle(_tessereTot.begin(), _tessereTot.end());
}

void Gioco::printGriglia() {
	std::cout << "   ";		// Spazi per i numeri delle righe
	// Inserimento numeri colonne
	for (size_t i = 0; i < N; i++) {
		if (i < 10) {
			std::cout << i << "  ";
		}
		else {
			std::cout << i << " ";
		}
	}
	// Spaziatura tra numeri colonne e griglia di gioco
	std::cout << std::endl;
	for (size_t i = 0; i < N; i++) {
		// Inserimento numeri righe
		if (i < 10) {
			std::cout << i << "  ";
		}
		else {
			std::cout << i << " ";
		}
		// Stampa il resto della griglia
		for (size_t j = 0; j < N; j++){
			if (_griglia[i][j].getLetter() != ' ') {
				std::cout << _griglia[i][j] << " |";
			}
			else if (_griglia[i][j].getType() == type::dl) {
				std::cout << "2L|";
			}
			else if (_griglia[i][j].getType() == type::tl) {
				std::cout << "3L|";
			}
			else if (_griglia[i][j].getType() == type::dp) {
				std::cout << "2P|";
			}
			else if (_griglia[i][j].getType() == type::tp) {
				std::cout << "3P|";
			}
			else {
				std::cout << "  |";
			}

		}
		std::cout << std::endl;
	}
}

void Gioco::push_back(const Giocatore &g) {
	_players.push_back(g);
}
void Gioco::push_back(const char &c, const size_t numTessere) {
	for (size_t i = 0; i < numTessere; i++) {
		_tessereTot.push_back(c);
	}
}

size_t Gioco::getPlayerNumbers() {
	return _players.size();
}

Giocatore &Gioco::getPlayer(const size_t playerNumber) {
	return _players.at(playerNumber);
}

void Gioco::setPlayer(const Giocatore &g, const size_t playerNumber) {
	_players.at(playerNumber) = g;
}

char Gioco::pickRandomLetter() {
	// Se il vettore tessereTot è vuoto, torna il carattere spazio
	if (_tessereTot.size() <= 0) {
		return ' ';
	}
	srand(time(NULL));
	// Randomizza fra le tessere
	size_t randPosition = rand() % _tessereTot.size() + 0;
	char randTessera = _tessereTot.at(randPosition);
	// Rimuove la tessera che è stata estratta
	_tessereTot.erase(_tessereTot.begin() + randPosition);
	return randTessera;
}

void Gioco::printStatus() {
	for (size_t i = 0; i < _players.size(); i++) {
		_players.at(i).printStatus();
	}
}

void Gioco::distribuisciTessere() {
	for (size_t j = 0; j < _players.size(); j++) {
		for (size_t i = 0; i < MAXTASS; i++) {
			char c = pickRandomLetter();
			// Crea un giocatore temporaneo, al quale verranno aggiunte le tessere, per poi trasferirle
			// al giocatore vero e proprio
			Giocatore gtemp = getPlayer(j);
			gtemp.addTessera(c);
			_players.at(j) = gtemp;
		}
	}
}

bool Gioco::emptyGrid() {
	for (size_t i = 0; i < N; i++) {
		for (size_t j = 0; j < N; j++) {
			if (_griglia[i][j].getLetter() != ' ') {
				return false;
			}
		}
	}
	return true;
}

std::vector <char> Gioco::lettereIncrociate(const std::string parola, const size_t x,
	const size_t y, const char direzione) {
	// Il vettore che tiene conto delle lettere incrociate
	std::vector <char> incroci;
	if (direzione == 'v') {
		if (x + parola.size() < N - 1) {
			for (size_t i = x, j = 0; i < x + parola.size(); i++, j++) {
				// Quando c'è un incrocio, aggiunge la lettera a quel vettore
				if (_griglia[i][y].getLetter() == toupper(parola[j])) {
					incroci.push_back(toupper(parola[j]));
				}
			}
		}
	}


	else {
		if (y + parola.size() < N - 1) {
			for (size_t i = y, j = 0; i < y + parola.size(); i++, j++) {
				// Quando c'è un incrocio, aggiunge la lettera a quel vettore
				if (_griglia[x][i].getLetter() == toupper(parola[j])) {
					incroci.push_back(toupper(parola[j]));
				}
			}
		}
	}

	return incroci;
}

bool Gioco::paroleIncrociate(const std::string parola, const size_t x,
	const size_t y, const char direzione) {
	if (direzione == 'v') {
		// Controlla se la parola non eccede i limiti della griglia di gioco
		if (x + parola.size() < N - 1) {
			for (size_t i = x, j = 0; i < x + parola.size(); i++, j++) {
				if (_griglia[i][y].getLetter() == toupper(parola[j])) {
					return true;
				}
			}
		}
	}
	
	else {
		// Controlla se la parola non eccede i limiti della griglia di gioco
		if (y + parola.size() < N - 1) {
			for (size_t i = y, j = 0; i < y + parola.size(); i++, j++) {
				if (_griglia[x][i].getLetter() == toupper(parola[j])) {
					return true;
				}
			}
		}
	}

	return false;
}


void Gioco::updatePunteggioGiocatore(int punteggio, size_t turn) {
	// Ricava il numero del giocatore attraverso turno e numero di giocatori
	size_t numGiocatore = turn % getPlayerNumbers();
	// Aggiorna il punteggio
	_players[numGiocatore].updatePunteggio(punteggio);
}

bool Gioco::inserisciParolaInGriglia(const std::string parola, const size_t x,
	const size_t y, const char direzione, const size_t pNumb) {
	int punteggioParola = 0;		// Il punteggio parziale dell'intera parola
	// Controllori per casella di doppia parola, tripla parola o se non vi sono
	// jolly nella parola inserita
	bool doubleWord = false, tripleWord = false, noJolly = true;
	// Controllo se la griglia è vuota o meno
	if (emptyGrid() || (paroleIncrociate(parola, x, y, direzione))) {
		// Parola inserita verticalmente nella griglia
		if (direzione == 'v') {
				for (size_t i = x, j = 0; i < x + parola.size(); i++, j++) {
					if (_griglia[i][y].isJolly() && _griglia[i][y].getLetter() == toupper(parola[j])) {
						_griglia[i][y].noJolly();
						// Il jolly viene aggiunto alla mano del giocatore
						_players[pNumb].addTessera('J');
					}
					_griglia[i][y] = toupper(parola[j]);
					// Aggiorna il punteggio della lettera all'inserimento
					punteggioParola += _griglia[i][y].calcolaPunteggioLettera();
					if (_players[pNumb].getTessera(j) == 'J') {
						noJolly = false;
						_griglia[i][y].jolly();
					}
					if (_griglia[i][y].getType() == dp) {
						doubleWord = true;
					}
					else if (_griglia[i][y].getType() == tp) {
						tripleWord = true;
					}
				}
				if (doubleWord == true && tripleWord == false) {
					punteggioParola *= 2;
				}
				else if (tripleWord == true) {
					punteggioParola *= 3;
				}
				if (noJolly == true) {
					punteggioParola += 10;
				}
				// Entra nel punteggio "extra" solo se la lunghezza della parola è 6 o più
				if (parola.size() > 5) {
					punteggioParola += extraPoints(parola);
				}
				updatePunteggioGiocatore(punteggioParola, turno);
				return true;
			return false;
		}

		// Parola inserita orizzontalmente nella griglia
		else {
				for (size_t i = y, j = 0; i < y + parola.size(); i++, j++) {
					// Se viene sostituito un jolly, quella casella non è più jolly
					if (_griglia[x][i].isJolly() && _griglia[x][i].getLetter() == toupper(parola[j])) {
						_griglia[x][i].noJolly();
						// Il jolly viene aggiunto alla mano del giocatore
						_players[pNumb].addTessera('J');
					}
					_griglia[x][i] = toupper(parola[j]);
					// Aggiorna il punteggio della lettera all'inserimento
					punteggioParola += _griglia[x][i].calcolaPunteggioLettera();
					if (_players[pNumb].getTessera(j) == 'J') {
						noJolly = false;
						_griglia[x][i].jolly();
					}
					if (_griglia[x][i].getType() == dp) {
						doubleWord = true;
					}
					else if (_griglia[x][i].getType() == tp) {
						tripleWord = true;
					}
				}
				if (doubleWord == true && tripleWord == false) {
					punteggioParola *= 2;
				}
				else if (tripleWord == true) {
					punteggioParola *= 3;
				}
				if (noJolly == true) {
					punteggioParola += 10;
				}
				// Entra nel punteggio "extra" solo se la lunghezza della parola è 6 o più
				if (parola.size() > 5) {
					punteggioParola += extraPoints(parola);
				}
				updatePunteggioGiocatore(punteggioParola, turno);
				return true;
			return false;
		}
	}

	else {
		return false;
	}
	return false;
}


int Gioco::calcolaPunteggio(const std::string parola, const size_t x,
	const size_t y, const char direzione) {
	int punteggioParola = 0;		// Il punteggio parziale dell'intera parola
	// Controllori per casella di doppia parola, tripla parola o se non vi sono
	// jolly nella parola inserita
	bool doubleWord = false, tripleWord = false, noJolly = true;

	if (direzione == 'v') {
		for (size_t i = x, j = 0; i < x + parola.size(); i++, j++) {

			punteggioParola += calcolaPunteggioLettera(parola[j]);
			if (parola[j] == 'J') {
				noJolly = false;
			}
			if (_griglia[i][y].getType() == dp) {
				doubleWord = true;
			}
			else if (_griglia[i][y].getType() == tp) {
				tripleWord = true;
			}
			if (_griglia[i][y].getType() == dl) {
				punteggioParola *= 2;
			}
			else if (_griglia[i][y].getType() == tl) {
				punteggioParola *= 3;
			}

		}

		// Entra nel punteggio "extra" solo se la lunghezza della parola è 6 o più
		if (parola.size() > 5) {
			punteggioParola += extraPoints(parola);
		}
		return punteggioParola;
	}

	else {
		for (size_t i = y, j = 0; i < y + parola.size(); i++, j++) {
			punteggioParola += calcolaPunteggioLettera(parola[j]);
			if (parola[j] == 'J') {
				noJolly = false;
			}
			if (_griglia[x][i].getType() == dp) {
				doubleWord = true;
			}
			else if (_griglia[x][i].getType() == tp) {
				tripleWord = true;
			}
			if (_griglia[x][i].getType() == dl) {
				punteggioParola *= 2;
			}
			else if (_griglia[x][i].getType() == tl) {
				punteggioParola *= 3;
			}
		}
		// Entra nel punteggio "extra" solo se la lunghezza della parola è 6 o più
		if (parola.size() > 5) {
			punteggioParola += extraPoints(parola);
		}
		return punteggioParola;
	}
	return punteggioParola;
}


int Gioco::extraPoints(const std::string parola) {
	int extra = 0;
	// Parola lunga 6 caratteri
	if (parola.size() == 6) {
		extra += 10;
	}
	// Parola lunga 7 caratteri
	else if (parola.size() == 7) {
		extra += 30;
	}
	// Parola lunga 8 caratteri
	else if (parola.size() == 8) {
		extra += 50;
	}
	else if (parola == "SCARABEO" || parola == "SCARABEI") {
		extra += 100;
	}
	return extra;
}


std::string Gioco::findBestWord(const std::string dizionario, Giocatore g)
{
	// Apre il dizionario
	std::fstream file;
	if (!file.is_open()) {
		file.open(dizionario);
	}
	std::string riga, bestWord;
	int punteggioParola = 0;
	// Per ogni parola del dizionario, controlla che questa sia componibile dal giocatore e ne calcola
	// il punteggio. Ritornerà quella col punteggio maggiore.
	while (std::getline(file, riga)) {
		riga = toUpper(riga);
		int punteggioCurr = calcolaPunteggio(riga, 0, 3, 'o');
		if (punteggioCurr > punteggioParola && g.parolaComponibile(riga)) {
			bestWord = riga;
			punteggioParola = punteggioCurr;
		}
	}
	// Chiude il dizionario
	file.close();
	return bestWord;
}

// Il giocatore può scegliere se:
// - Inserire una parola
// - Farsi suggerire una parola
// - Stampare le proprie informazioni
// - Stampare la griglia di gioco
// - Fare lo shuffle delle proprie tessere
// - Trovare una casella jolly
void Gioco::sceltaGiocatore(Giocatore g) {
	std::cout << "Inserisci la lettera corrispondente all'azione che si vuole effettuare:" << std::endl
		<< "1: Inserisci una parola nella griglia di gioco." << std::endl
		<< "2: Suggerimento parola ideale." << std::endl
		<< "3: Stampa nome giocatore, punteggio attuale e tasselli disponibili" << std::endl
		<< "4: Stampa la griglia di gioco." << std::endl
		<< "5: Mischia le lettere" << std::endl
		<< "6: Chiedi se una certa casella nella griglia contiene un jolly" << std::endl;
	int n = 0;
	std::cin >> n;
	while (n < 1 && n > 6) {
		std::cin >> n;
	}
	
	while (n != 1 && n!= 0) {
		size_t x = 0, y = 0;
		switch (n)
		{
		case 1:
			break;
		case 2:
			std::cout << "Suggerimento: " << findBestWord("words_italian.win.txt", g) << std::endl;
			break;
		case 3:
			g.printStatus();
			break;
		case 4:
			printGriglia();
			break;
		case 5:
			g.shuffle();
			break;
		case 6:
			std::cout << "X: ";
			std::cin >> x;
			std::cout << " Y: ";
			std::cin >> y;
			std::cout << std::endl;
			casellaType(x, y);
		default:
			break;
		}
		if (n != 1) {
			std::cout << "Vuoi effettuare un'altra azione? Se si, indica una delle azioni" << std::endl
				<< "disponibili tra 2 3 4 5 e 6 altrimenti premi 1" << std::endl
				<< "per inserire una parola nella griglia." << std::endl;
			std::cin >> n;
		}
	}
}

void Gioco::casellaType(size_t x, size_t y) {
	if (_griglia[x][y].isJolly()) {
		std::cout << "La casella in posizione " << x << " " << y << " contiene un jolly.";
	}
	else {
		std::cout << "La casella in posizione " << x << " " << y << " NON contiene un jolly.";
	}
}

int Gioco::calcolaPunteggioLettera(char c)
{
	switch (c)
	{
	case 'Q':
		return 10;
		break;
	case 'H': case 'Z':
		return 8;
		break;
	case 'B': case 'D': case 'F': case'G': case 'U': case 'V':
		return 4;
		break;
	case 'P':
		return 3;
		break;
	case 'L': case 'M': case 'N':
		return 2;
		break;
	case 'A': case 'C': case 'E': case 'I': case 'O': case 'R':
	case 'S': case 'T':
		return 1;
		break;
	default:
		break;
	}
	return 0;
}

size_t Gioco::findAWinner() {
	size_t winner = 0;
	// Cerca chi ha il punteggio massima, e ne ritorna la posizione del vettore _players
	for (size_t i = 0; i < _players.size(); i++) {
		if (_players[i].getPunteggio() > _players[winner].getPunteggio()) {
			winner = i;
		}
	}
	return winner;
}

void Gioco::inserisciDati(int &x, int &y, char &dir, std::string &word) {
	std::cout << std::endl << "Inserisci la posizione dove inserire la parola " <<
		"in coordinate x e y." << std::endl;
	std::cout << "X: ";		std::cin >> x;
	while (x < 0 && x > N) {
		std::cout << "X: ";
		std::cin >> x;
		std::cout << std::endl;
	}
	std::cout << "Y: ";		std::cin >> y;
	while (y < 0 && y > N) {
		std::cout << "Y: ";
		std::cin >> y;
		std::cout << std::endl;
	}
	std::cout << std::endl << "Inserisci la direzione in cui vuoi inserire la parola" <<
		" ('o' per orizzontale, 'v' per verticale): ";
	std::cin >> dir;
	while (dir != 'v' && dir != 'o') {
		std::cin >> dir;
		std::cout << std::endl;
	}

	std::cout << "Inserisci una parola nella griglia: ";
	std::cin >> word;
	word = toUpper(word);
}