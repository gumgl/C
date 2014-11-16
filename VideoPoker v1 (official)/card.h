#ifndef _CARD_
#define _CARD_

#include <string>

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

		//The following 3 operators are based on FaceValue
		bool operator==(Card);
		bool operator>(Card);
		bool operator<(Card);
		  
		void Set(FaceValue, Suit);
		FaceValue GetFaceValue();
		Suit GetSuit();
		int Strength();
  
		//Prints the card, example: Nine of Clubs
		std::string Print();
};
std::ostream& operator<<(std::ostream&, Card&);

#endif
