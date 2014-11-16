#ifndef _DECK_
#define _DECK_

#include "card.h"

class Deck
{
	private:
		Card theDeck[52];
		int size;
	public:
		// Build a proper deck with size=52
		Deck();
		// Build a proper deck but also specify size
		Deck(int);


		// Reset deck to a proper deck
		void Reset(int inSize = 52);
		
		// Should call Print for each Card left in the deck.
		void Print();

		// Must use time as the random generator seed.
		// Shuffle resets the deck to full
		void Shuffle();
  
		// Is the deck empty?
		bool IsEmpty();
  
		// Pre-condition: The deck is not empty.
		Card DealACard();

		// Return a deck with the top X cards from this deck, removing them from this deck too
		Deck Deal(int);

		//Add a card to the top of the deck
		void Add(Card);

		//Add a deck on top of this deck
		void AddDeck(Deck&);

		//Add a card to the bottom of the deck
		void AddBottom(Card);

		//Return number of cards in deck
		int Count();

		//Return the number of cards with a specific face value
		int Count(FaceValue);

		//Return the number of cards with a specific suit
		int Count(Suit);

		// Discard a card with specified index in the array.
		void Discard(int = 0);

		// Replace a specified card with a new card
		void Replace(int, Card);

		int Strength();
};

int random(int, int);

#endif