#include "Gioco.h"
using namespace std;

// Seleziona il numero di giocatori
size_t playerSelecter() {
	int n = 0;
	std::cout << "Numero di giocatori: ";
	std::cin >> n;
	while (n < 2 && n > 4) {
		std::cin >> n;
	}
	std::cout << std::endl;
	return n;
}

int main() {
	size_t gameContinue = 1;
	while (gameContinue) {
		size_t players = playerSelecter();
		Gioco g(players);
		std::cout << "Vuoi giocare un'altra partita oppure no? " << std::endl << "0: No  1: Si" << std::endl;
		std::cin >> gameContinue;
		while (gameContinue != 0 && gameContinue != 1) {
			std::cin >> gameContinue;
		}
	}
	std::cout << "FINE DEL GIOCO.";
	system("PAUSE");
	return 0;
}