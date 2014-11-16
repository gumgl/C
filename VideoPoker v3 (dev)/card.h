#ifndef _CARD_
#define _CARD_

#include <string>
#include <windows.h>   // WinApi header (for colors)

enum FaceValue {Two, Three, Four, Five, 
		Six, Seven, Eight, Nine, 
		Ten, Jack, Queen, King, 
		Ace};

enum Suit {Hearts, Diamonds, Clubs, Spades};

class Card
{
	private:
		FaceValue faceValue;
		Suit suit; 
	public:
		// Default Constructor
		Card();
		// Regular Constructor
		Card(FaceValue, Suit);

		// The following 3 operators are based on FaceValue
		bool operator==(Card);
		bool operator>(Card);
		bool operator<(Card);
		  
		void Set(FaceValue, Suit);
		FaceValue GetFaceValue();
		Suit GetSuit();
		int Strength();
  
		//Prints the card, example: Nine of Clubs
		std::string Print();

		// Prints the top, middle and bottom of cards (with color)
		void PrintHand(int, bool);
};
std::ostream& operator<<(std::ostream&, Card&);

#endif