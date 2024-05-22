#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include "structs.h"
#include "functions.h"

#define HEART "\3"
#define DIAMOND "\4"
#define CLUB "\5"
#define SPADE "\6"

#define ANSI_COLOR_RESET   "\x1b[0m"
#define DIAMOND_COLOR_DARK_RED     "\x1b[31m"
#define CLUB_COLOR_DARK_WHITE     "\x1b[37m"
#define SPADE_COLOR_BRIGHT_WHITE     "\x1b[90m"
#define HEART_COLOR_BRIGHT_RED     "\x1b[91m"

const int cardAmount = 52;
const int handSize = 8;
const char(*handName[20]) = { "Royal Flush", "Four of a Kind", "Full House",
							  "Flush",       "Straight",       "Three of a Kind",
							  "Two Pair",    "One Pair",	   "High Card",	      "Empty hand" };

const int handTypeChipsValues[10] = { 100, 60, 40,
									 35, 30, 30,
									 20, 10, 5, 0 };

const int handTypeMultValues[10] = { 8, 7, 4,
									 4, 4, 3,
									 2, 2, 1, 0 };

extern int deckIndex;

// ASSIGN CARDS AND SPACE
CARD* dynamicSpace() {
	CARD* card = (CARD*)malloc(cardAmount * sizeof(CARD));
	if (card == NULL) {
		return NULL;
	}
	return card;
}
void assignRank(CARD* card) {
	//Give rank to all 52 cards
	int ranks = 1;
	for (int i = 0; i < cardAmount; i++) {
		(card + i)->rank = ranks;
		if (ranks == 13) {
			ranks = 0;
		}
		ranks++;
	}
}
void assignSuit(CARD* card) {
	//Give one of four suits to all cards
	char(*a[4]) = { HEART, DIAMOND, CLUB, SPADE };
	int j = 0;
	for (int i = 0; i < cardAmount; i++) {
		strcpy((card + i)->suit, a[j]);
		if ((i + 1) % 13 == 0) {
			j++;
		}
	}
}
void shuffleDeck(CARD* deck[], CARD* allCardsInOrder[]) {
	srand((unsigned)time(NULL));
	int randCard = 0;
	int sortedNumbers[52] = { 0 };
	int cardsLeft = 52;
	for (int i = 0; i < 52; i++)
		sortedNumbers[i] = i;

	for (int i = 0; i < 52; i++) {
		int randNum = rand() % cardsLeft;
		randCard = sortedNumbers[randNum];
		deck[i] = allCardsInOrder[randCard];
		arraySwitchPlaceInt(sortedNumbers, randNum, cardsLeft - 1);
		cardsLeft--;
	}
}


// PRINT HAND
void printGame(const PLAYER_INFO* playerInfo,const int discardUsed) {
	if (playerInfo->score != 0 && discardUsed == 0) {
		Sleep(5000);
	}
	
	system("cls");
	printInstructions();
	printf("Score  %d", playerInfo->score);
	printf("\t\t\t\t  %d.  Stage\n", playerInfo->statistics->stage);
	printf("Discards  %d", playerInfo->discards);
	printf("\t\t\t\t  %d.  Round\n", playerInfo->statistics->round);
	for (int i = 0; i < handSize; i++) {
		if (strcmp(playerInfo->hand[i]->suit, HEART) == 0) {
			printCardColor(playerInfo->hand[i], 0);
		}
		else if (strcmp(playerInfo->hand[i]->suit, DIAMOND) == 0) {
			printCardColor(playerInfo->hand[i], 1);
		}
		else if (strcmp(playerInfo->hand[i]->suit, CLUB) == 0) {
			printCardColor(playerInfo->hand[i], 2);
		}
		else if (strcmp(playerInfo->hand[i]->suit, SPADE) == 0) {
			printCardColor(playerInfo->hand[i], 3);
		}
	}

	putchar('\n');
	for (int i = 0; i < handSize; i++) {
		printf("%d.     ", i + 1);
	}
	printf("  %d. end round\n", 0);
}
void printInstructions() {
	printf("Poker hands\t\t\tBonus\n");
	printf("Royal Flush\t   100 X 8\t\t\t\n");
	printf("Four of a Kind\t   60 X 7\t\t\t\n");
	printf("Full House\t   40 X 4\t\t\t\n");
	printf("Flush\t\t   35 X 4\t\t\t\n");
	printf("Straight\t   30 X 4\t\t\t\n");
	printf("Three of a Kind\t   30 X 3\t\t\t\n");
	printf("Two pair\t   20 X 2\t\t\t\n");
	printf("Pair\t\t   10 X 2\t\t\t\n");
	printf("High Card\t   5 X 1\t\t\t\n");
	printf("\n\n");
}
void FindCardSuit(const PLAYER_INFO* playerInfo) {
	sortPlayedHand(playerInfo);

	for (int i = 0; i < playerInfo->cardsPlayed; i++) {
		if (strcmp(playerInfo->playedHand[i]->suit, HEART) == 0) {
			printCardColor(playerInfo->playedHand[i], 0);
		}
		else if (strcmp(playerInfo->playedHand[i]->suit, DIAMOND) == 0) {
			printCardColor(playerInfo->playedHand[i], 1);
		}
		else if (strcmp(playerInfo->playedHand[i]->suit, CLUB) == 0) {
			printCardColor(playerInfo->playedHand[i], 2);
		}
		else if (strcmp(playerInfo->playedHand[i]->suit, SPADE) == 0) {
			printCardColor(playerInfo->playedHand[i], 3);
		}
	}
}
void printCardColor(const CARD* hand, const int ChoseColor) {
	char (*collor[15]) = {HEART_COLOR_BRIGHT_RED, DIAMOND_COLOR_DARK_RED, CLUB_COLOR_DARK_WHITE, SPADE_COLOR_BRIGHT_WHITE};

	if (hand->rank < 10 && hand->rank != 1) {
		printf("%s%d%s     ", collor[ChoseColor], hand->rank, hand->suit);
	}
	else if (hand->rank == 10) {
		printf("%s%d%s    ", collor[ChoseColor], hand->rank, hand->suit);
	}
	else if (hand->rank == 1) {
		printf("%s%c%s     ", collor[ChoseColor], 'A', hand->suit);
	}
	else if (hand->rank == 11) {
		printf("%s%c%s     ", collor[ChoseColor], 'J', hand->suit);
	}
	else if (hand->rank == 12) {
		printf("%s%c%s     ", collor[ChoseColor], 'Q', hand->suit);
	}
	else if (hand->rank == 13) {
		printf("%s%c%s     ", collor[ChoseColor], 'K', hand->suit);
	}
	printf(ANSI_COLOR_RESET);
	return;

}


// PLAYING HAND
void playHand(PLAYER_INFO* playerInfo) {
	int numOfCardsPlayed = 0;
	int input = 0;
	do {
		input = getInput();
		if (input == 0)
			break;
		for (int i = 0; i < numOfCardsPlayed; i++) {
			if (playerInfo->playedHand[i]->rank == playerInfo->hand[input - 1]->rank &&
				playerInfo->playedHand[i]->suit == playerInfo->hand[input - 1]->suit) {
				input = getInput(); //Ako je karta vec u ruci iskoristena ponovo upisi input
				if (input == 0)
					break;
			}
		}

		if (input < 0) { //used for discarding cards
			if (playerInfo->discards == 0) {
				printf(" Used all discards!!!\n");
				continue;
			}
			playerInfo->hand[(input * -1) - 1] = playerInfo->deck[deckIndex];
			deckIndex++;
			playerInfo->discards--;
			printGame(playerInfo, 1);
			continue;

		}

	
	playerInfo->playedHand[numOfCardsPlayed] = playerInfo->hand[input - 1];
	numOfCardsPlayed++;
	playerInfo->statistics->allCardsPlayedInRound++;
	//playedHand su karte koje se igraju i uzimaju se iz arraya hand sa indexom input (input - odabrana karta)
	} while (numOfCardsPlayed < 5);
	playerInfo->cardsPlayed = numOfCardsPlayed;
	FindCardSuit(playerInfo);
}
int getInput() {
	int input = 0;
	char keyInput[50];
	do {
		printf("Input cards to play: ");
		if (fgets(keyInput, 40, stdin) == NULL) {
			memset(keyInput, '\0', 40);
			input = getInput();
		}
		if (keyInput[0] == '0') {
			input = 0;
			return input;
		}
		input = atoi(keyInput);
		if (input == 0) {
			memset(keyInput, '\0', 40);
			input = getInput();
		}
		printf("\n");
		
	} while (input < -8 || input > 8);
	return input;
}
void sortPlayedHand(PLAYER_INFO* playerInfo) {
	int arraySorted = 0;
	int count = 0;
	while (arraySorted == 0) {
		for (int i = 0; i < playerInfo->cardsPlayed - 1; i++) {
			if (playerInfo->playedHand[i]->rank > playerInfo->playedHand[i + 1]->rank) {
				arraySwitchPlace(playerInfo->playedHand, i, i + 1);
				count++;
			}
		}
		if (count == 0)
			arraySorted = 1;

		count = 0;
	}
}
inline void arraySwitchPlace(CARD* array[], const int indexA, const int indexB) {
	CARD* temp = array[indexA];
	array[indexA] = array[indexB];
	array[indexB] = temp;
}
inline void arraySwitchPlaceInt(int* array, const int indexA, const int indexB) {
	int temp = array[indexA];
	array[indexA] = array[indexB];
	array[indexB] = temp;
}


// SCORING
void scoreHand(PLAYER_INFO* gameInfo) {
	int cardChips = 0;  //Chips earned from cards rank
	int handTypeChips = calculateChips(gameInfo, &cardChips); //Chips earned from hand type
	int mult = calculateMult(gameInfo);
	printf("\n\n");
	printf("\tChips  X  Multiplier   Chip Bonus\n");
	printf("          %d     ", handTypeChips);
	printf("      %d ", mult);
	printf("\t  +%d \n", cardChips);
	printf("\n\n");
	printf("\t          Score: %d ", mult * handTypeChips + cardChips);
	gameInfo->score += mult * handTypeChips + cardChips;
}
int calculateChips(const PLAYER_INFO* playerInfo, int* cardChips) {
	long int handTypeChips = 0;
	char* handValue = handType(playerInfo);
	if (handValue == NULL) {
		printf("error");
		exit(-1);
	}
	for (int i = 0; i < playerInfo->cardsPlayed; i++) {
		if (playerInfo->playedHand[i]->rank == 1) {  // Aces give 14 chips while other cards give their value
			*cardChips += 14;
			continue;
		}
		*cardChips += playerInfo->playedHand[i]->rank;
	}

	for (int i = 0; i < 10; i++) {
		if (strcmp(handValue, handName[i]) == 0) {
			handTypeChips = handTypeChipsValues[i];
			break;
		}
	}
	
	return handTypeChips;
}
int calculateMult(const PLAYER_INFO* playerInfo) {
	char* handValue = handType(playerInfo);
	puts(handValue); //Shows played hand type
	for (int i = 0; i < 10; i++) {
		if (strcmp(handValue, handName[i]) == 0) {
			return handTypeMultValues[i];
		}
	}
	printf("\nError - hand name not found");
	exit(-1);
}
char* handType(const PLAYER_INFO* playerInfo) {
	int tempArray[5] = {0};
	int rankNum = arrayGetSingleNumber(playerInfo, sameRankCount(playerInfo, tempArray));
	if (playerInfo->cardsPlayed == 0) {
		return "Empty hand";
	}
	if (playerInfo->cardsPlayed == 1) {
		return "High Card";
	}
	if (playerInfo->cardsPlayed == 5) {
		if ((strcmp(playerInfo->playedHand[0]->suit, playerInfo->playedHand[1]->suit) == 0) && (strcmp(playerInfo->playedHand[1]->suit, playerInfo->playedHand[2]->suit) == 0) &&
			(strcmp(playerInfo->playedHand[2]->suit, playerInfo->playedHand[3]->suit) == 0) && (strcmp(playerInfo->playedHand[3]->suit, playerInfo->playedHand[4]->suit) == 0)) {
			
			if (playerInfo->playedHand[1]->rank == (playerInfo->playedHand[0]->rank) + 1 && playerInfo->playedHand[2]->rank == (playerInfo->playedHand[0]->rank) + 2 &&
				playerInfo->playedHand[3]->rank == (playerInfo->playedHand[0]->rank) + 3 && playerInfo->playedHand[4]->rank == (playerInfo->playedHand[0]->rank) + 4)
				return "Royal Flush";
			
			else if (rankNum != 4 && rankNum != 32)
				return "Flush";
		}
	}
	
	if (rankNum == 4)
		return "Four of a Kind";

	if (rankNum == 32)
		return "Full House";

	if (playerInfo->cardsPlayed == 5)
	if (playerInfo->playedHand[1]->rank == playerInfo->playedHand[0]->rank + 1 && playerInfo->playedHand[2]->rank == playerInfo->playedHand[0]->rank + 2 &&
		playerInfo->playedHand[3]->rank == playerInfo->playedHand[0]->rank + 3 && playerInfo->playedHand[4]->rank == playerInfo->playedHand[0]->rank + 4)
		return "Straight";

	if (rankNum == 3)
		return "Three of a Kind";

	if (rankNum == 22)
		return "Two Pair";

	if (rankNum == 2)
		return "One Pair";

	return "High Card";
}
int* sameRankCount(const PLAYER_INFO* playerInfo, int* array) {
	for (int i = 0; i < playerInfo->cardsPlayed; i++) {
		for (int j = 0; j < playerInfo->cardsPlayed; j++) {
			if ((playerInfo->playedHand[i]->rank == playerInfo->playedHand[j]->rank) && i != j) {

				array[i]++;
			}
		}
	}
	return array;
}
int arrayGetSingleNumber(const PLAYER_INFO* playerInfo, const int* sameRankList) {
	int arr[5] = { 0 };
	int numOfValues = 0;
	for (int i = 0; i < playerInfo->cardsPlayed; i++) {
		if (sameRankList[i] != 0) {
			numOfValues++;
			arr[i] = sameRankList[i];
		}
	}
	if (playerInfo->cardsPlayed == 5) {
		if (playerInfo->playedHand[0]->rank == playerInfo->playedHand[1]->rank &&
			playerInfo->playedHand[4]->rank == playerInfo->playedHand[3]->rank) {
			return 32;
		}
	}
	if (numOfValues == 4) {
		if ((playerInfo->playedHand[1]->rank == playerInfo->playedHand[2]->rank &&
			playerInfo->playedHand[2]->rank == playerInfo->playedHand[3]->rank) && 
			(playerInfo->playedHand[0]->rank == playerInfo->playedHand[1]->rank ||
			playerInfo->playedHand[4]->rank == playerInfo->playedHand[3]->rank)  ){
			return 4;
		}
		else return 22;
	}
	if (numOfValues == 3) {
		return 3;
	}
	if (numOfValues == 2) {
		return 2;
	}
	return 1;
}


//BONUSES
void chooseBonus() {
	system("cls");
	printf("\t\tChoose bonus:\n");

}



