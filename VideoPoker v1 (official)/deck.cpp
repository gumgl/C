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
	//cout << "size: " << size << endl << endl;;
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

void Deck::AddDeck(Deck &inDeck)
{
	int count;
	int inSize = inDeck.Count();

	if (size + inSize <= 52)
	{
		for (count = 1; count <= inSize; count ++)
			AddBottom(inDeck.DealACard());
	}
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
	
	if (index < size && index >= 0)
	{
		for(count = size - 1; count >= index; count --)
			theDeck[count] = theDeck[count + 1];
		size --;
	}
}

void Deck::Replace(int index, Card inCard)
{
	if (index < size && index >= 0)
		theDeck[index] = inCard;
}

int Deck::Strength()
{
	int count;
	int faces[13];
	int suits[4];
	int following = 0;
	int pairs;
	bool straight = false;
	bool flush = false;
	for (count = 0; count < 13; count ++) // Count faces
		faces[count] = Count((FaceValue) count);

	for (count = 0; count < 4; count ++) // Count suits
		suits[count] = Count((Suit) count);

	for (count = 0; count < 13; count ++) // Look for a straight or pairs
	{
		if (!straight && faces[count] == 1)
			straight = true;
		else if (straight && faces[count] != 1)
		{
			straight = false;
			break;
		}
		if (faces[count] == 1)
			following = ++;
		else if (following >= 1 )

	}

	flush = (suits[theDeck[0].GetSuit()] == 5);


	return count;
}