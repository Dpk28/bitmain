#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
	HIGHEST_CARD=0,
	ONE_PAIR,
	TWO_PAIRS,
	THREE_OF_A_KIND,
	STRAIGHT,
	FLUSH,
	FULL_HOUSE,
	FOUR_OF_A_KIND,
	STRAIGHT_FLUSH
};

const char * hands[] = {
	"highest-card",
	"one-pair",
	"two-pairs",
	"three-of-a-kind",
	"straight",
	"flush",
	"full-house",
	"four-of-a-kind",
	"straight-flush",
};

char hand[5][3];
char deck[5][3];

char play[5][3];

int
flush(void) {
	int i;
	char c = play[0][1];

	for (i=1; i<5; i++) {
		if (c != play[i][1]) {
			return 0;
		}
	}
	return 1;
}

int straight(void) {
	if (
			(play[0][0]=='2' && play[1][0]=='3' && play[2][0]=='4' && play[3][0]=='5' && play[4][0]=='A') ||
			(play[0][0]=='2' && play[1][0]=='3' && play[2][0]=='4' && play[3][0]=='5' && play[4][0]=='6') ||
			(play[0][0]=='3' && play[1][0]=='4' && play[2][0]=='5' && play[3][0]=='6' && play[4][0]=='7') ||
			(play[0][0]=='4' && play[1][0]=='5' && play[2][0]=='6' && play[3][0]=='7' && play[4][0]=='8') ||
			(play[0][0]=='5' && play[1][0]=='6' && play[2][0]=='7' && play[3][0]=='8' && play[4][0]=='9') ||
			(play[0][0]=='6' && play[1][0]=='7' && play[2][0]=='8' && play[3][0]=='9' && play[4][0]=='T') ||
			(play[0][0]=='7' && play[1][0]=='8' && play[2][0]=='9' && play[3][0]=='J' && play[4][0]=='T') ||
			(play[0][0]=='8' && play[1][0]=='9' && play[2][0]=='J' && play[3][0]=='Q' && play[4][0]=='T') ||
			(play[0][0]=='9' && play[1][0]=='J' && play[2][0]=='K' && play[3][0]=='Q' && play[4][0]=='T') ||
			(play[0][0]=='A' && play[1][0]=='J' && play[2][0]=='K' && play[3][0]=='Q' && play[4][0]=='T')) {
		return 1;
	}
	return 0;
}

int
kind(int a) {
	int i,j;

	for (i=0; i<=5-a; i++) {
		for (j=1; j<a; j++) {
			if (play[i][0] != play[i+j][0]) {
				goto nopes1;
			}
		}
		return 1;
nopes1:
		;
	}
	return 0;
}

int
kkind(int a, int b) {
	int i,j,k;

	for (i=0; i<=5-a-b; i++) {
		for (j=i+a; j<=5-b; j++) {
			for (k=1; k<a; k++) {
				if (play[i][0] != play[i+k][0]) {
					goto nopes2;
				}
			}
			for (k=1; k<b; k++) {
				if (play[j][0] != play[j+k][0]) {
					goto nopes2;
				}
			}
			return 1;
nopes2:
			;
		}
	}
	return 0;
}

int
compar(const void *a, const void *b) {
	return *(char *)a - *(char *)b;
}

int
calc(void) {
	qsort(play, 5, sizeof(play) / 5, compar);
	if (straight() && flush()) {
		return STRAIGHT_FLUSH;
	} else if (kind(4)) {
		return FOUR_OF_A_KIND;
	} else if (kkind(2,3) || kkind(3,2)) {
		return FULL_HOUSE;
	} else if (flush()) {
		return FLUSH;
	} else if (straight()) {
		return STRAIGHT;
	} else if (kind(3)) {
		return THREE_OF_A_KIND;
	} else if (kkind(2,2)) {
		return TWO_PAIRS;
	} else if (kind(2)) {
		return ONE_PAIR;
	} else {
		return HIGHEST_CARD;
	}
}

int main(void) {
	int i,j,k;
	int best;

	while (1) {
		if (scanf(" %s %s %s %s %s %s %s %s %s %s", hand[0], hand[1], hand[2], hand[3], hand[4],
					deck[0], deck[1], deck[2], deck[3], deck[4]) != 10) {
			return 0;
		}
		best = 0;
		for (i=0; i<32; i++) {
			k=0;
			for (j=0; j<5; j++) {
				if (i & 1<<j) {
					strcpy(play[j], hand[j]);
				} else {
					strcpy(play[j], deck[k++]);
				}
			}
			j = calc();
			if (j > best) {
				best = j;
			}
		}
		printf("Hand:");
		for (i=0; i<5; i++) {
			printf(" %s", hand[i]);
		}
		printf(" Deck:");
		for (i=0; i<5; i++) {
			printf(" %s", deck[i]);
		}
		printf(" Best hand: %s\n", hands[best]);
	}
}
