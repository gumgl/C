#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "deck.h"
#include "card.h"

using namespace std;

int random( int min, int max) {
	int range = max - min + 1;
	return rand() % range + min;
}

Deck::Deck()
{
	Reset();
}

Deck::Deck(int inSize)
{
	Reset(inSize);
}

void Deck::Reset(int inSize) //inSize is optional, default: 52
{
	int count;

	for (count = 0; count < inSize; count ++)
		theDeck[count].Set( (FaceValue) (count % 13), (Suit) (count / 13));

	size = count;
}

void Deck::Print()
{
	int count;
	for (count = 0; count < size; count ++)
	{
		cout << count + 1 << ". " << theDeck[count] << endl;
	}
	//cout << "size: " << size << endl << endl;
}

void Deck::PrintHand(int selected)
{
	int level;
	int card;
	for (level = 1; level <= 4; level ++) // Print each level (Top, Face, Suit, Bottom)
	{
		for (card = 0; card < size; card ++) // Print level for each card
			theDeck[card].PrintHand(level, card == selected);
		cout << endl;
	}
}

void Deck::Shuffle()
{
	int count;
	srand ( (unsigned int)time(NULL) );

	for(count = 0; count < size; count++)
		swap(theDeck[count], theDeck[random(count, size - 1)]);
}

bool Deck::IsEmpty()
{
	return size == 0;
}

Card Deck::DealACard()
{
	size --;
	return theDeck[size];
}

Deck Deck::Deal(int cards)
{
	Deck newDeck(0); //create an empty deck
	int count;

	for (count = 1; count <= cards; count ++)
		newDeck.Add(DealACard());//add last card of theDeck to newDeck

	return newDeck;
}

void Deck::Add(Card inCard)
{
	if (size <= 52)
	{
		theDeck[size] = inCard;
		size ++;
	}
}

void Deck::Add(Deck &inDeck)
{
	int count;
	int inSize = inDeck.Count();

	if (size + inSize <= 52)
		for (count = 1; count <= inSize; count ++)
			Add(inDeck.DealACard());
}

void Deck::AddBottom(Card inCard)
{
	int count;
	if (size <= 52)
	{
		size ++;
		for (count = size - 1; count > 0; count --)
			theDeck[count] = theDeck[count - 1];
		theDeck[0] = inCard;
	}
}

void Deck::AddBottom(Deck &inDeck)
{
	int count;
	int inSize = inDeck.Count();

	if (size + inSize <= 52)
		for (count = 1; count <= inSize; count ++)
			AddBottom(inDeck.DealACard());
}

int Deck::Count()
{
	return size;
}

int Deck::Count(FaceValue inFaceValue)
{
	int count;
	int sum = 0;
	for (count = 0; count < size; count ++)
		if (theDeck[count].GetFaceValue() == inFaceValue)
			sum ++;

	return sum;
}

int Deck::Count(Suit inSuit)
{
	int count;
	int sum = 0;
	for (count = 0; count < size; count ++)
		if (theDeck[count].GetSuit() == inSuit)
			sum ++;

	return sum;
}

void Deck::Discard(int index)
{
	int count;
	
	if (index >= 0 && index < size)
	{
		for(count = index; count < size; count ++)
			theDeck[count] = theDeck[count + 1];
		size --;
	}
}

void Deck::Replace(int index, Card inCard)
{
	if (index < size && index >= 0)
		theDeck[index] = inCard;
}

PokerRank Deck::Strength()
{
	/*theDeck[0].Set(Ace, Hearts);
	theDeck[1].Set(King, Hearts);
	theDeck[2].Set(Queen, Hearts);
	theDeck[3].Set(Jack, Hearts);
	theDeck[4].Set(Ten, Hearts);

	Print();*/

	const FaceValue MIN_PAIR = Jack;
	int faces[13]; // first derivative of the deck
	int suits[4]; // first derivative of the deck
	int following = 0;
	FaceValue pairFace;

	// Rankings:
	int dups[5] = {0}; // second derivative of the deck; saves number of duplicates (dups[3]==1 means there is one three-of-a-kind)
	bool straight = false;
	bool flush = false;

	for (int count = 0; count < 13; count ++) // Count faces
		faces[count] = Count((FaceValue) count);

	for (int count = 0; count < 4; count ++) // Count suits
		suits[count] = Count((Suit) count);

	for (int count = 0; count < 13; count ++) // Look for a straight, pairs, 3-of-a-kind and 4-of-a-kind
	{
		if (faces[count] == 1)
			following ++;
		else if (following >= 1)
		{
			if (following == 5)
				break;
			else
				following = 0;
		}
		dups[faces[count]] ++;
		if (faces[count] == 2) // If Pair, then save index
			pairFace = (FaceValue) count;
		if (dups[4] > 0) // If Four-of-a-kind, no need to keep searching
			break;
	}
	straight = (following == 5);

	if (dups[1] == 5) // If we have 5 cards alone (no duplicates)
		flush = (suits[theDeck[0].GetSuit()] == 5); // Look for a flush
	
	// Return the right PokerRank:
	if (straight && flush && faces[Ace] == 1)
		return RoyalFlush;
	if (straight && flush)
		return StraightFlush;
	if (dups[4] == 1)
		return FourOfAKind;
	if (dups[3] == 1 && dups[2] == 1)
		return FullHouse;
	if (flush)
		return Flush;
	if (straight)
		return Straight;
	if (dups[3] == 1)
		return ThreeOfAKind;
	if (dups[2] == 2)
		return TwoPair;
	if (dups[2] == 1 && pairFace >= MIN_PAIR)
		return Pair;

	return Nothing;
	/* Hand Values:
	0 - Nothing
	1 - 1 Pair (Jacks or Better)
	2 - 2 Pair
	3 - Three-of-a-kind
	4 - Straight
	5 - Flush
	6 - Full House
	7 - Four-of-a-kind
	8 - Straight Flush
	9 - Royal Flush
	*/
}