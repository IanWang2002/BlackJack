#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <iomanip>

using namespace std;

struct Card {
	string rank;
	string suit;
	int value;
};

vector<Card> createDeck() {
	vector<Card> deck;
	string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
	string ranks[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
	int values[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

	for (const string& suit : suits) {
		for (int i = 0; i < 13; ++i) {
			deck.push_back({ranks[i], suit, values[i]});
		}
	}
	random_shuffle(deck.begin(), deck.end());
	return deck;
}

int handValue(const vector<Card>& hand);

void printHand(const vector<Card>& hand, const string& name, bool hideFirst = false) {
	cout << "\n------------------------------\n";
	cout << name << "'s Hand: ";
	for (size_t i = 0; i < hand.size(); ++i) {
		if (hideFirst && i == 0)
			cout << "[Hidden] ";
		else
			cout << hand[i].rank << " of " << hand[i].suit << "  ";
	}
	if (!hideFirst) {
		cout << "\n" << name << "'s Total: " << handValue(hand);
	}
	cout << "\n------------------------------\n";
}

int handValue(const vector<Card>& hand) {
	int total = 0;
	int aces = 0;
	for (const auto& card : hand) {
		total += card.value;
		if (card.rank == "Ace") ++aces;
	}
	while (total > 21 && aces--) total -= 10;
	return total;
}

void playBlackjack() {
	int playerChips = 100;
	srand(static_cast<unsigned int>(time(0)));

	while (playerChips > 0) {
		vector<Card> deck = createDeck();
		vector<Card> playerHand, dealerHand;
		int bet;
		char act;

		cout << "\n\nYou have $" << playerChips << ". Are you ready to face the dealer? (y/n): ";
		cin >> act;
		if (act == 'n') {
			cout << "Game stopped. Come back when you're feeling lucky!\n";
			exit(1);
		}

		cout << "Enter your bet: ";
		cin >> bet;
		if (bet > playerChips || bet <= 0) {
			cout << "Invalid bet. Try again.\n";
			continue;
		}

		bool doubledDown = false;

		playerHand.push_back(deck.back());
		deck.pop_back();
		dealerHand.push_back(deck.back());
		deck.pop_back();
		playerHand.push_back(deck.back());
		deck.pop_back();
		dealerHand.push_back(deck.back());
		deck.pop_back();

		printHand(playerHand, "Player", false);
		printHand(dealerHand, "Dealer", true);

		bool isFirstMove = true;  // Add this above the while loop

		while (handValue(playerHand) < 21) {
			char choice;
			cout << "\nPlease make your move...\n";
			cout << "[h] Hit\n[s] Stand";
			if (isFirstMove) cout << "\n[d] Double Down";
			cout << "\n> ";
			cin >> choice;

			if (choice == 'h') {
				playerHand.push_back(deck.back());
				deck.pop_back();
				printHand(playerHand, "Player");
			} else if (choice == 's') {
				break;
			} else if (choice == 'd') {
				if (!isFirstMove) {
					cout << "Double Down is only allowed on your first move.\n";
					continue;
				}
				if (playerChips >= 2 * bet) {
					playerChips -= bet;
					bet *= 2;
					doubledDown = true;
					playerHand.push_back(deck.back());
					deck.pop_back();
					printHand(playerHand, "Player");
					break;
				} else {
					cout << "Not enough chips to double down.\n";
				}
			} else {
				cout << "Invalid option. Try again.\n";
			}

			isFirstMove = false;  // p This makes sure Double Down is no longer allowed
		}

		int playerScore = handValue(playerHand);
		if (playerScore > 21) {
			cout << "\033[1;31m*** You BUSTED! Dealer wins this round. ***\033[0m\n"; // red text
			playerChips -= bet;
			continue;
		}

		printHand(dealerHand, "Dealer", false);
		while (handValue(dealerHand) < 17) {
			dealerHand.push_back(deck.back());
			deck.pop_back();
			printHand(dealerHand, "Dealer", false);
		}

		int dealerScore = handValue(dealerHand);
		cout << "\nFinal Scores -> Player: " << playerScore << " | Dealer: " << dealerScore << "\n";

		if (dealerScore > 21 || playerScore > dealerScore) {
			cout << "\033[1;32m*** You WIN the hand! ***\033[0m\n"; // green text
			playerChips += bet;
		} else if (playerScore < dealerScore) {
			cout << "\033[1;31m*** Dealer WINS the hand... ***\033[0m\n"; // red text
			playerChips -= bet;
		} else {
			cout << "*** It's a PUSH. Nobody wins. ***\n";
		}
	}

	cout << "\nYou're out of chips. Better luck next time!\n";
}

int main() {
	cout << "\n==============================\n";
	cout << "  Welcome to Blackjack!" << endl;
	cout << "==============================\n";
	playBlackjack();
	return 0;
}


