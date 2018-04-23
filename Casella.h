#pragma once

#include <iostream>

enum type {normal, dp, dl, tp, tl};		// Il moltiplicatore della casella: può essere doppia o tripla lettera, doppia o 
										// tripla parola, oppure nessuno di questi.

class Casella
{
public:
	Casella()	:
		_letter (' '),
		_type (normal),
		_score(0),
		_isJolly(false)
	{}

	// Setter e getter
	char getLetter() const { return _letter; }
	void setLetter(const char &letter) { _letter = letter; }
	type getType() const { return _type; }
	void setType(const type &newtype) { _type = newtype; }
	void setScore(const int &score) { _score = score; }
	// Setta la casella come jolly
	void jolly() { _isJolly = true; }
	void noJolly() { _isJolly = false; }
	bool isJolly() { return _isJolly; }

	// Setta il punteggio della casella
	int calcolaPunteggioLettera() {
		int partialScore = 0;
		switch (_letter)
		{
		case 'Q':
			partialScore = 10;
			break;
		case 'H': case 'Z':
			partialScore = 8;
			break;
		case 'B': case 'D': case 'F': case'G': case 'U': case 'V':
			partialScore = 4;
			break;
		case 'P':
			partialScore = 3;
			break;
		case 'L': case 'M': case 'N':
			partialScore = 2;
			break;
		case 'A': case 'C': case 'E': case 'I': case 'O': case 'R':
		case 'S': case 'T':
			partialScore = 1;
			break;
		default:
			break;
		}
		
		if (_type == dl) {
			partialScore *= 2;
		}
		if (_type == tl) {
			partialScore *= 3;
		}
		setScore(partialScore);
		return partialScore;
	}

	// Overload dell'operatore di assegnamento
	void operator = (const Casella &c) {
		_letter = c._letter;
		_type = c._type;
	}
	void operator = (const char &c) {
		_letter = c;
	}


	// Overload dell'operatore <<
	friend std::ostream &operator << (std::ostream &os, const Casella &c) {
		os << c.getLetter();
		return os;
	}

private:
	char _letter;	// La lettera presente all'interno della casella
	type _type;		// Il moltiplicatore della casella corrente
	int _score;		// Il punteggio della casella
	bool _isJolly;	// Indica se la casella contiene un jolly
};

