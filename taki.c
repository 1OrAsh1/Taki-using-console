//Or Asherov - TAKI

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // in order to use the "rand" and "srand" functions
#include <time.h>	// in order to use "time" function
#include <stdbool.h>
#define Max_Name 21
#define Max_Card_Types 14

const char BasicCards [9][2]={ "1","2","3","4","5","6","7","8","9" };
const char AllCards[Max_Card_Types][6] = { "1","2","3","4","5","6","7","8","9","+","STOP","<->","TAKI","COLOR"};
const char Colors [4]= {'R','G','B','Y'};

struct Card
{
	char color;
	char type [6];
}; typedef struct Card Card;

struct Player
{
	char name[Max_Name];
	Card *cards;
	int Cards_physSize;
	int Cards_logSize;
}; typedef struct Player Player;
void Intro();
void red();
void yellow();
void green();
void blue();
void reset();
Player* GetPlayersInfo(int* numPlayers);
void CardsPerPlayer(Player p[], int numPlayers);
void PrintCard(Card c);
Card RandomBasicCard();
void CurrPlayerDetailes(Player p);
Card RandomCard();
void RemoveCard(Player *player1, int indxCard);
void StartStatistics(int statistics[Max_Card_Types][2]);
Player AddCardFromDeck(Player p, int statistics[Max_Card_Types][2]);
bool CardMatchingToUpper(Card chosen, Card upper);
void UpdateStatistics(char type[] , int statistics [Max_Card_Types][2]);
void SortStatistics(int statistics[Max_Card_Types][2]);
int findMaxIndex(int statistics[Max_Card_Types][2], int start, int end);
void swap(int * x, int * y);
void PrintStatistics(int statistics[Max_Card_Types][2]);
void FreeCardsArrays(Player p[], int numPlayers);

void main()
{
	srand(time(NULL));
	int numPlayers;
	Intro();
	Player* players = GetPlayersInfo(&numPlayers);
	CardsPerPlayer(players, numPlayers);
	bool gameOver = false;
	int IndxWinner;
	Card upper = RandomBasicCard();
	int curr=0;
	int choice;
	bool dropped = false;
	bool TAKI = false;
	bool turn;
	int direction = 1;
	int statistics[Max_Card_Types][2] = {0};
	StartStatistics(statistics);
	while (!gameOver)
	{
		turn = false;
		while (!turn)
		{
			choice = -1;
			printf("\nUpper card:\n");
			PrintCard(upper);
			CurrPlayerDetailes(players[curr]);
			if (!TAKI) {
				while (choice < 0 || choice>players[curr].Cards_logSize)
				{
					printf("\nPlease enter 0 if you want to take a card from the deck\n");
					printf("or 1-%d if you want to put one of your cards in the middle:\n", players[curr].Cards_logSize);
					scanf("%d", &choice);
					if (choice < 0 || choice>players[curr].Cards_logSize)
						printf("Invalid choice! Try again.\n");
				}
			}
			else {
				printf("TAKI! Please enter 1-%d if you want to put one of your cards in the middle:\n", players[curr].Cards_logSize);
				printf("or press 0 if you want to finish your turn.\n");
				scanf("%d", &choice);
			}
			if (choice == 0)
			{
				if (!TAKI)
					players[curr] = AddCardFromDeck(players[curr], statistics);
				TAKI = false;
				turn = true;
			}
			else
			{
				if (CardMatchingToUpper(players[curr].cards[choice - 1], upper))
				{
					dropped = true;
					upper = players[curr].cards[choice - 1];
					RemoveCard(&players[curr], choice - 1);
					if (players[curr].Cards_logSize == 0)
					{
						gameOver = true;
						turn = true;
						IndxWinner = curr;
					}
					else if ((strcmp(upper.type, "TAKI") == 0) || TAKI) {
						TAKI = true;
					}
					else if (strcmp(upper.type, "+") != 0) {
						turn = true; //this player turn is finished
					}
				}
				else
				{
					printf("Invalid card! Try again\n");
				}

			}
		}
		if ((strcmp(upper.type, "<->") == 0) && dropped)
			direction *= -1;
		else if (strcmp(upper.type, "STOP") == 0 && dropped)
			curr++;
		else if (strcmp(upper.type,"COLOR") == 0 && dropped)
		{
			int c1 = -1;
			while (c1 <= 0 || c1 > 4)
			{
				printf("\nPlease enter your color choice:\n");
				printf("1- Yellow\n");
				printf("2- Red\n");
				printf("3- Blue\n");
				printf("4- Green\n");
				scanf("%d", &c1);
				if (c1 < 1 || c1 > 4)
					printf("Invalid choice. Try again.\n");
			}
			if (c1 == 1)
				upper.color = 'Y';
			else if (c1 == 2)
				upper.color = 'R';
			else if (c1 == 3)
				upper.color = 'B';
			else
				upper.color = 'G';
		}
		if (direction == 1)
			curr++;
		else
			curr--;
		if (curr > numPlayers - 1 && direction == 1)
			curr = curr - numPlayers;
		else if (curr < 0 && direction == -1)
			curr = numPlayers + curr;
		dropped = false;

	}
	printf("The winner is...%s! Congratulations!\n",players[IndxWinner].name);
	PrintStatistics(statistics);
	FreeCardsArrays(players, numPlayers);
	free(players);
}

void Intro()
{
	printf("************  Welcome to ");
	green(); //Set the text to the color green
	printf("T");
	red(); //Set the text to the color red
	printf("A");
	yellow(); //Set the text to the color yellow
	printf("K");
	blue(); //Set the text to the color blue
	printf("I");
	reset(); //Resets the text to default color
	printf(" game !!!***********\n");
}

void red() //prints in the color red
{
	printf("\033[1;31m");
}

void yellow() // prints in the color yellow
{
  printf("\033[1;33m");
}

void green() //prints in the color green
{
	printf("\033[1;32m");
}

void blue() //prints in the color blue
{
	printf("\033[1;36m");
}
void reset() //prints in the regular color
{
	printf("\033[0m");
}

Player* GetPlayersInfo(int* numPlayers) //gets every player's details and returns an array of all players
{
	int num;
	printf("Please enter the number of players:\n");
	scanf("%d", &num);
	while (num <= 0)
	{
		printf("Invalid number, please try again.\n\n");
		printf("Please enter the number of players:\n");
		scanf("%d", &num);
	}
	Player* players= (Player*)malloc((num) * sizeof(Player));
	if (!players) //null
	{
		printf("Memory allocation failure!!!\n");
		exit(1);
	}
	for (int i = 0; i < num; i++)
	{
		printf("Please enter the first name of player #%d:\n", i+1);
		scanf("%s", players[i].name);
	}
	*numPlayers = num;
	return players;
}

void CardsPerPlayer(Player p [],int numPlayers) //Dividing 4 cards for each player
{
	Card* tmp;
	int type1,color1;
	for (int i = 0; i < numPlayers; i++)
	{
		p[i].Cards_logSize = 4;
		p[i].Cards_physSize = p[i].Cards_logSize*2;
		tmp = (int*)malloc(p[i].Cards_logSize * sizeof(Card));
		for (int j = 0; j < p[i].Cards_logSize; j++)
		{
			tmp[j] = RandomCard();
			p[i].cards = tmp;
		}
	}
}

void PrintCard(Card c) //prints a card
{
	int spaces = 3-strlen(c.type)/2;
	char spacesStr[4] = "";
	for (int i = 0; i < spaces; i++) {
		strcat(spacesStr, " ");
	}
	if (c.color == 'R')
		red();
	if (c.color == 'Y')
		yellow();
	if (c.color == 'G')
		green();
	if (c.color == 'B')
		blue();
	printf("*********\n");
	printf("*       *\n");
	if ((strlen(c.type))%2!=0)
		printf("*%s%s%s*\n", spacesStr, c.type, spacesStr);
	else
		printf("*%s%s%s *\n", spacesStr, c.type, spacesStr);
	if (c.color!='N')
		printf("*   %c   *\n", c.color);
	else
		printf("*       *\n");
	printf("*       *\n");
	printf("*********\n");
	reset();
}

Card RandomBasicCard() //return a random basic card
{
	Card res;
	int type1 = rand() % 9;
	int color1 = rand() % 4;
	strcpy(res.type, BasicCards[type1]);
	res.color = Colors[color1];
	return res;
}


void CurrPlayerDetailes(Player p) //prints the cards of the player
{
	printf("\n%s's turn:\n", p.name);
	for (int i = 0; i < p.Cards_logSize; i++)
	{
		printf("\nCard #%d\n", i + 1);
		PrintCard(p.cards[i]);
	}
}

Card RandomCard() //return a random card
{
	Card res;
	int type1 = rand() % 14;
	int color1 = rand() % 4;
	strcpy(res.type ,AllCards[type1]);
	if (strcmp(res.type, "COLOR") != 0)
		res.color = Colors[color1];
	else {
		res.color = 'N';
	}
	return res;
}

void RemoveCard(Player* player, int indxCard) //remove a cards (by its index) from the player's cards
{
	Player p=*player;
	for (int i = indxCard; i < p.Cards_logSize-1; i++)
	{
		p.cards[i] = p.cards[i + 1];
	}
	p.Cards_logSize--;
	*player = p;
}

void StartStatistics(int statistics[Max_Card_Types][2]) 
{
	for (int i = 0; i < Max_Card_Types; i++)
	{
		statistics[i][0] = i;
	}
}

Player AddCardFromDeck(Player p, int statistics[Max_Card_Types][2]) //adding card to the player's cards from deck
{
	Card* tmp;

	if (p.Cards_physSize== p.Cards_logSize)
	{
		p.Cards_physSize *= 2;
		tmp = (int*)malloc(p.Cards_physSize * sizeof(Card));
		for (int i = 0; i < p.Cards_logSize; i++)
			tmp[i] = p.cards[i];
		free(p.cards);
		p.cards = tmp;
	}
	p.Cards_logSize++;
	Card newC = RandomCard();
	p.cards[p.Cards_logSize - 1] = newC;
	UpdateStatistics(newC.type,statistics);
	return p;
}

bool CardMatchingToUpper(Card chosen, Card upper) //return if the card is matching the upper card (by the game's rules)
{
	if (chosen.color == upper.color || strcmp(chosen.type, upper.type) == 0|| strcmp(chosen.type, "COLOR") == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UpdateStatistics(char type[], int statistics[Max_Card_Types][2]) //updates the statistics of the game 
{
	int i = 0;
	while (strcmp(AllCards[i], type) != 0) //not equal
		i++;
	statistics[i][1]++;
}

void SortStatistics(int statistics[Max_Card_Types][2]) //sorting the array of the statistics
{
	int i, maxInd;
	for (i = 0; i < Max_Card_Types; i++)
	{
		maxInd = findMaxIndex(statistics, i, Max_Card_Types-1);
		swap(&statistics[i][1], &statistics[maxInd][1]);
		swap(&statistics[i][0], &statistics[maxInd][0]);
	}

}

int findMaxIndex(int statistics[Max_Card_Types][2], int start, int end)// returns the indx of the maximum integer in the second pol of the array
{
	int max = start;
	for (int i = start + 1; i <= end; i++)
	{
		if (statistics[max][1] < statistics[i][1])
			max = i;
	}
	return max;
}

void swap(int *x, int* y) //swaping between two integers' values 
{
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}
void PrintStatistics(int statistics[Max_Card_Types][2]) //prints the statistics of the game from higher to lower
{
	SortStatistics(statistics);
	int spaces;
	char spacesStr[4] = "";
	char empty[4] = "";
	printf("************ Game Statistics ************\n");
	printf("Card # | Frequency\n");
	printf("___________________\n");
	for (int i = 0; i < Max_Card_Types; i++)
	{
		int spaces = 3 - strlen(AllCards[statistics[i][0]]) / 2;
		for (int k = 0; k < spaces; k++) {
			strcat(spacesStr, " ");
		}
		spaces = 3 - (strlen(AllCards[statistics[i][0]]))/2;
		if ((strlen(AllCards[statistics[i][0]])) % 2 != 0)
			printf("%s%s%s|   %d\n", spacesStr, AllCards[statistics[i][0]], spacesStr, statistics[i][1]);
		else
			printf("%s%s%s |   %d\n", spacesStr, AllCards[statistics[i][0]], spacesStr, statistics[i][1]);
		strcpy(spacesStr, empty);
	}
}

void FreeCardsArrays(Player p[], int numPlayers) //free the cards array of all players
{
	for (int i = 0; i < numPlayers; i++)
	{
		free(&p[i].cards);
	}
}
