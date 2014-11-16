#include <iostream>
#include <iomanip>
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

void Card::PrintHand(int level)
{
	string faces[] = {"J", "Q", "K", "A"};
	// White: 7 (default)
	// Red:   12 or 0x0C
	int color = 0x07; 
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	if (suit < 2) // If suit is one of the first two (Hearts or Diamonds)
		color = 0x0C;

	switch (level)
	{
		case 1: // Top
			cout << char(218) << char(196) << char(196) << char(191) << " ";
			break;
		case 2: // Face
			cout << char(179);
			SetConsoleTextAttribute(h, color);
			cout << left << setfill(' ') << setw(2);
			if (faceValue < Jack)
				cout << faceValue + 2;
			else 
				cout << faces[faceValue - Jack];
			SetConsoleTextAttribute(h, 0x07);
			cout << char(179) << " ";
			break;
		case 3: // Suit
			cout << char(179);
			SetConsoleTextAttribute(h, color);
			cout << char(suit + 3);
			SetConsoleTextAttribute(h, 0x07);
			cout << " " << char(179) << " ";
			break;
		case 4: // Bottom
			cout << char(192) << char(196) << char(196) << char(217) << " ";
			break;
	}
}