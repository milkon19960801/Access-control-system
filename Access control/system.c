#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include<Windows.h>
#include <time.h>
#include "SafeInput.h"

typedef struct {
	int cardNumber;
	time_t time;
	bool vaildCard;
	bool openDoor;
}CARD;

typedef struct {
	CARD* allCards;
	int numberOfCards;
}CARD_LISTING;

void listAllCards(CARD_LISTING* list) {
	printf("Listing all cards\n");
	if (list->numberOfCards == 0)
		printf("There are no cards registered.\n");
	for (int i = 0; i < list->numberOfCards; i++) {
		CARD c = list->allCards[i];
		struct tm* date = localtime(&c.time);
		if (c.vaildCard == 1)
			printf("Card %d with access is added to system at: %d-%d-%d %d:%d:%d\n", c.cardNumber, date->tm_year + 1900, date->tm_mon + 1, date->tm_mday, date->tm_hour, date->tm_min, date->tm_sec);
		else
			printf("Card %d with no access is added to system at: %d-%d-%d %d:%d:%d\n", c.cardNumber, date->tm_year + 1900, date->tm_mon + 1, date->tm_mday, date->tm_hour, date->tm_min, date->tm_sec);
	}
}

void unlockDoor(CARD_LISTING* list) {
	int cardNumber;
	CARD c;
	printf("Unlocking the door.\n");
	GetInputInt("Enter card number: ", &cardNumber);
	for (int i = 0; i < list->numberOfCards; i++) {
		if (list->allCards[i].cardNumber == cardNumber) {
			c = list->allCards[i];
			if (c.cardNumber == cardNumber && c.vaildCard == true) {
				printf("Card %d has access\n", c.cardNumber);
				printf("Door is unlocking for 3 seconds\n");
				printf("One\n");
				Sleep(1000);
				printf("Two\n");
				Sleep(1000);
				printf("Three\n");
				Sleep(1000);
				printf("Door is locked !\n");
				return;
			}
			else if (c.cardNumber == cardNumber && c.vaildCard == false) {
				printf("This card %d has no access.\n", c.cardNumber);
				printf("Door is locked.\n");
				return;
			}
		}
	}
	c.cardNumber = 0;
	if (c.cardNumber != cardNumber) {
		printf("Card doesn't exist.\n");
	}
}
void cardAccessHandler(CARD_LISTING* list) {
	int CardNr, access;
	CARD c;
	GetInputInt("Please enter card number: ", &CardNr);
	for (int i = 0; i < list->numberOfCards; i++) {
		if (list->allCards[i].cardNumber == CardNr) {
			c = list->allCards[i];
			if (list->allCards[i].vaildCard == true)
				printf("This card has access.\n");
			else if (list->allCards[i].vaildCard == false)
				printf("This card has no access.\n");
			GetInputInt("Please enter 1 for access or 0 for no access\n", &access);
			if (access == 1) {
				list->allCards[i].vaildCard = true;
				printf("You have added access for card %d\n", CardNr);
				return;
			}
			else if (access == 0) {
				list->allCards[i].vaildCard = false;
				printf("You have deletet the access for card %d\n", CardNr);
				return;
			}
			else
				printf("You have entered an invalid value.\n");
		}
	}
	c.cardNumber = 0;
	if (c.cardNumber == 0) {
		printf("Card doesn't exist please add it.\n");
		return;
	}
}

void addCard(CARD_LISTING* list) {
	int  indexforthenewcard, CardNumber;
	printf("Adding a new card.\n");
	GetInputInt("Cardnumber: ", &CardNumber);
	for (int i = 0; i < list->numberOfCards; i++) {
		CARD c = list->allCards[i];
		if (c.cardNumber == CardNumber) {
			printf("This card allready exists.\n");
			return;
		}
	}
	if (list->numberOfCards == 0) {
		list->allCards = malloc(sizeof(CARD));
		list->numberOfCards = 1;
		indexforthenewcard = 0;
	}
	else {
		list->allCards = realloc(list->allCards, sizeof(CARD) * (list->numberOfCards + 1));
		indexforthenewcard = list->numberOfCards;
		list->numberOfCards++;
	}
	list->allCards[indexforthenewcard].cardNumber = CardNumber;
	list->allCards[indexforthenewcard].time = time(NULL);
	list->allCards[indexforthenewcard].vaildCard = false;
	list->allCards[indexforthenewcard].openDoor = false;
	printf("Card number %d is successfully added.\n", CardNumber);
}

void Admin(CARD_LISTING* list) {
	while (true) {
		printf("***ADMIN_MENU****\n");
		printf("1:Remote open door.\n2:List all cards in system.\n3:Add a new Card.\n4:Manage card access.\n5:Exit.\n");
		int selection;
		GetInputInt("Make a choice: ", &selection);
		switch (selection) {
		case 1:
			unlockDoor(list);
			break;
		case 2:
			listAllCards(list);
			break;
		case 3:
			addCard(list);
			break;
		case 4:
			cardAccessHandler(list);
			break;
		case 5:
			printf("Press any key to exit");
			return;
		}
	}
}

int main() {
	CARD_LISTING list;
	list.allCards = NULL;
	list.numberOfCards = 0;
	Admin(&list);
	getch();
}