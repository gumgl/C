#include <iostream>
#include <string>
#include "card.h"

using namespace std;

Card::Card()
{
	faceValue = Two;
	suit = Hearts;
}
Card::Card(FaceValue inFaceValue, Suit inSuit)
{
	faceValue = inFaceValue;
	suit = inSuit;
}

bool Card::operator==(Card card)
{
	//return faceValue == card.GetFaceValue() && suit == card.GetSuit(); // compares face values & suits
	return faceValue == card.GetFaceValue(); // compares only face values
}

bool Card::operator>(Card card)
{
	return faceValue > card.GetFaceValue(); // compares only face values
}

bool Card::operator<(Card card)
{
	return faceValue < card.GetFaceValue(); // compares only face values
}

ostream &operator<<(ostream &out, Card &card)
{
	out << card.Print();
	return out;
}

void Card::Set(FaceValue inFaceValue, Suit inSuit)
{
	faceValue = inFaceValue;
	suit = inSuit;
}

FaceValue Card::GetFaceValue()
{
	return faceValue;
}

Suit Card::GetSuit()
{
	return suit;
}

int Card::Strength()
{
	//[2,3,4,5,6,7,8,9,10,J,Q,K,A] = [-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6]
	return faceValue - 6;
}

string Card::Print()
{
	string faces[] = {"Two", "Three", "Four", "Five", 
		"Six", "Seven", "Eight", "Nine", 
		"Ten", "Jack", "Queen", "King", 
		"Ace"};
	string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};

	return faces[faceValue] + " of " + char(suit+3);
}