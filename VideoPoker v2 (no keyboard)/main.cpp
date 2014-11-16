#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <tchar.h>
#include "deck.h"
#include "card.h"
using namespace std;

string PrintTime(time_t, string, string);
string lang(int, int);
void pause(int);

void main(void)
{
	//system("color F0");
	SetConsoleTitleW(_T("Video Poker by Guillaume L.")); // Change Console's Title

	const int INIT_BANKROLL = 1000000;
	const int HAND_SIZE = 5;
	const int MIN_HOLD = 1;
	const int MAX_DISCARD = HAND_SIZE - MIN_HOLD;
	const int PAYOUTS[] = {0, 1, 2, 3, 4, 6, 9, 25, 50, 250};
		
	int  bankroll = INIT_BANKROLL; // Player bank roll in $
	int  bet;
	int  input = 0; // User input
	int  currLang = 0;
	int  cardsHeld;
	bool hold[HAND_SIZE];
	Deck deck; // The deck
	Deck hand(0); // Player's hand
	PokerRank strength; // Player's hand strength. Used to prevent too many calculations

	// For statistics:
	int rounds = 0;
	int wins = 0;
	int hands[RoyalFlush + 1] = {0}; // Number of times each hand was played
	int highest = INIT_BANKROLL;
	int lowest = INIT_BANKROLL;
	int biggestWin = 0;
	int biggestLoss = 0;
	int biggestBet = 0;
	int totalBet = 0;
	int difference = 0; // Gain/Loss for current round. Only used to prevent too many calculations
	int cardsDealt = 0;
	int cardsDiscarded = 0;
	int winStreak = 0;
	int lossStreak = 0;
	int biggestWinStreak = 0;
	int biggestLossStreak = 0;
	time_t timeSpent = time (NULL); // Initialize to current time

	cout << "1. English (default)\n";
	cout << "2. Francais\n";
	cout << "3. Espa\xA4ol\n";
	cout << endl;
	cout << "Choose/Choisissez/Elegir: ";
	cin  >> currLang;

	if (currLang < 1 || 3 < currLang)
		currLang = 1;

	while (true)
	{
		bet = 0; // Reset bet for a new round
		hand.Reset(0); // Make the hand empty
		deck.Reset();//      Get a new
		deck.Shuffle(); //   and shuffled deck every round
		cardsHeld = 0; // We start a new round with no card held

		for (int count = 0; count < HAND_SIZE; count ++) // Reset array
			hold[count] = false;

		system("cls");
		cout << lang(currLang, 10) << " " << rounds + 1 << endl; // round + 1 because this round is not yet confirmed
		cout << lang(currLang, 11) << ": $" << bankroll << endl;
		cout << endl;

		do
		{
			cout << lang(currLang, 12) << " $";
			cin  >> bet;
		} while (bet < 0 || bet > bankroll);

		if (bet == 0)
		{
			system("cls");
			cout << lang(currLang, 13) << ": $" << bankroll << endl;
			cout << endl;
			cout << lang(currLang, 14) << ".\n";
			break;
		}
		rounds ++; // Increment # of rounds now that we start a new round

		bankroll -= bet; // Take bet from their bank roll
		totalBet += bet; // Count a total of all the bets (for stats)

		hand = deck.Deal(HAND_SIZE); // Deal 5 cards to player
		cardsDealt += 5;

		while (true) // Choose which cards to hold
		{
			cardsHeld = 0;
			for (int count = 0; count < HAND_SIZE; count ++) // Count # of discarded cards
				if (hold[count])
					cardsHeld ++;
			
			system("cls");
			cout << lang(currLang, 15) << ": $" << bet << endl;
			cout << endl;
			cout << lang(currLang, 16) << ": \n";
			cout << endl;
			for (int count = 0; count < hand.Count(); count ++) // Print H for held cards
				cout << "  " << ((hold[count]) ? char(239) : ' ') << "  ";
			cout << endl;

			hand.PrintHand(); // Draw cards in hand side-by-side with color
			
			for (int count = 0; count < hand.Count(); count ++) // Print card #s
				cout << "  " << count + 1 << "  ";
			cout << endl;
			cout << endl;
			cout << lang(currLang, 19) << ": ";
			while (true)
			{
				cin >> input;
				
				if (input < 0 || input > hand.Count())
					cout << lang(currLang, 19) << ": ";
				else if (input == 0 && cardsHeld < MIN_HOLD)
					cout << lang(currLang, 17) << " " << MIN_HOLD << " " << lang(currLang, 18) << ((MIN_HOLD > 1) ? "s" : "") << ": ";
				else
					break;
			}
			if (input == 0)
				break;
			else
				hold[input - 1] ^= true;
		}

		for (int count = 0; count < HAND_SIZE; count ++)
			if (!hold[count]) // If card is discarded (not held)
			{
				cardsDiscarded ++; // Count number of cards discarded (for stats)
				cardsDealt ++;
				hand.Replace(count, deck.DealACard()); // Replace that specific card with the top card of the deck
			}

		//cardsDealt += HAND_SIZE - hand.Count(); // Add cards dealt to total (for stats)
		hand.Add(deck.Deal(HAND_SIZE - hand.Count())); // Add cards to hand so it has 5 cards

		system("cls");
		cout << lang(currLang, 15) << ": $" << bet << endl;
		cout << endl;
		cout << lang(currLang, 16) << ": \n";
		cout << endl;
		for (int count = 0; count < hand.Count(); count ++) // Print H for held cards
				cout << "  " << ((hold[count]) ? "H" : " ") << "  ";
		cout << endl;
		hand.PrintHand(); // Display player hand
		cout << endl;

		strength = hand.Strength(); // Calculate hand strength/rank

		difference = bet * (PAYOUTS[strength] - 1); // Calculate gain/loss from this round

		if (strength > Nothing)
		{
			wins ++;
			winStreak ++;
			lossStreak = 0;
		}
		else
		{
			lossStreak ++;
			winStreak = 0;
		}
		biggestWinStreak = (winStreak > biggestWinStreak) ? winStreak : biggestWinStreak;
		biggestLossStreak = (lossStreak > biggestLossStreak) ? lossStreak : biggestLossStreak;

		biggestBet = (bet > biggestBet) ? bet : biggestBet;

		bet *= PAYOUTS[strength]; // Add multiply bet
		cout << lang(currLang, 20) << ": " << lang(currLang, strength) << ((strength > Nothing) ? "!" : "...")  << "\n"; // Display hand rank
		cout << lang(currLang, 21) << " $" << bet << lang(currLang, 22) << ".\n";


		hands[strength] ++; // Save hand for statistics
		bankroll += bet; // Add bet to bankroll
				
		highest = (bankroll > highest) ? bankroll : highest; // Record highest bankroll for stats
		lowest = (bankroll < lowest) ? bankroll : lowest; // Record lowest bankroll for stats
		if (difference > 0) // It is a gain
			biggestWin = (abs(difference) > biggestWin) ? abs(difference) : biggestWin;
		else // It is a loss
			biggestLoss = (abs(difference) > biggestLoss) ? abs(difference) : biggestLoss;

		pause(currLang);
		
		if (bankroll == 0) // They're broke
		{
			system("cls");
			cout << lang(currLang, 23) << " ($" << INIT_BANKROLL << "). " << lang(currLang, 24) << "\n";
			break;
		}
	}

	timeSpent = difftime(time(NULL), timeSpent);
	cout << endl;
	cout << "  --" << lang(currLang, 25) << "--\n"; //  --Game Statistics--
	cout << lang(currLang, 26) << ((INIT_BANKROLL - bankroll > 0) ? lang(currLang, 27) : lang(currLang, 28)) << ": $" << abs(INIT_BANKROLL - bankroll) << endl; // Money Gained/Lost
	cout << lang(currLang, 29) << ": " << rounds << endl; // Number of Rounds
	if (rounds > 0)
	{
		cout << lang(currLang, 30) << ": "; // Win/Loss Ratio
		if (wins == rounds)
			cout << lang(currLang, 31) << "\n"; // Infinity
		else
			cout << setprecision(2) << (double) wins / (rounds - wins) << endl;
	}
	if (biggestWinStreak > 0)
		cout << lang(currLang, 32) << ": " << biggestWinStreak << endl; // Biggest Win Streak
	if (biggestLossStreak > 0)
		cout << lang(currLang, 33) << ": " << biggestLossStreak << endl; // Biggest Loss Streak
	cout << lang(currLang, 34) << ": " << PrintTime(timeSpent, lang(currLang, 44), lang(currLang, 45)) << endl; // Total Time Spent
	cout << lang(currLang, 35) << ": " << cardsDealt << endl; // Total Number of Cards Dealt
	if (rounds > 0)
		cout << lang(currLang, 36) << ": " << setprecision(3) << (double) cardsDiscarded / rounds << endl; // Average Number of Cards Discarded
	cout << lang(currLang, 37) << ": $" << highest << endl; // Highest Bankroll
	cout << lang(currLang, 38) << ": $" << lowest << endl; // Lowest Bankroll
	if (rounds > 0)
		cout << lang(currLang, 39) << ": $" << fixed << setprecision(2) << (double) totalBet / rounds << endl; // Average Bet
	if (biggestBet > 0)
		cout << lang(currLang, 40) << ": $" << biggestBet << endl; // Biggest Bet
	if (biggestWin > 0)
		cout << lang(currLang, 41) << ": $" << biggestWin << endl; // Biggest Gain
	if (biggestLoss > 0)
		cout << lang(currLang, 42) << ": $" << biggestLoss << endl; // Biggest Loss
	for (int count = 1; count < RoyalFlush + 1; count ++)
		if (hands[count] > 0)
			cout << lang(currLang, 43) << lang(currLang, count) << ": " << hands[count] << endl; // Number of Pair, Two Pair, etc.
	cout << endl;
	pause(currLang);
}

string PrintTime(time_t aTime, string strMin, string strSec)
{
	stringstream output;
	int mins = aTime / 60;
	int secs = aTime % 60;
	if (mins > 0)
		output << mins << " " << strMin << ((mins > 1) ? "s" : "") << " and ";
	output << secs << " " << strSec << ((secs > 1) ? "s" : "");
	return output.str();
}

void pause(int langID)
{
	cout << lang(langID, 46);
	system ("Pause > nul");
}