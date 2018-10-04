/* farmingCalculator.c ---- Calculates the efficiency of using different types of tree saplings while training
 * the farming skill on Runescape. The program calculates the GP/XP based on prices of tree 
 * saplings that are updated daily on Runescape's website, total GP loss, and number of days to reach
 * the desired goal level.
 *
 * By Myles Holt - 3 October 2018
 */

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <inttypes.h>
#include "libcs50/webpage.h"
#include "libcs50/hashtable.h"

/* Functions */
int extractPrice(char* string);
char* getHTMLFromPage(char* url);
char* assignNormalURL(char* normalTree);
char* assignFruitURL(char* fruitTree);
void insertIntoHashtable(hashtable_t* saplingXPRates);
int convertCurrXPtoLevel(int currXP);
float experiencePerDay (hashtable_t *ht, char* normalTree, char* fruitTree, int currXP);
float convertGoalLevelToXP(int goalLevel);
int daysTillGoal(int currXP, int goalLevel, int XPPerDay);
void gpPerXP(hashtable_t *ht, int currXP, int normalPrice, int fruitPrice, char* normalTree, char* fruitTree);
int totalGPLoss(int normalPrice, int fruitPrice, int days);

/* urls for each item from Runescape's grand exchange website, will be used to extract item price info, which changes daily */
char* oakURL = "http://services.runescape.com/m=itemdb_oldschool/Oak_sapling/viewitem?obj=5370";
char* willowURL = "http://services.runescape.com/m=itemdb_oldschool/Willow_sapling/viewitem?obj=5371";
char* mapleURL = "http://services.runescape.com/m=itemdb_oldschool/Maple_sapling/viewitem?obj=5372";
char* yewURL = "http://services.runescape.com/m=itemdb_oldschool/Yew_sapling/viewitem?obj=5373";
char* magicURL = "http://services.runescape.com/m=itemdb_oldschool/Magic_sapling/viewitem?obj=5374";
char* appleURL = "http://services.runescape.com/m=itemdb_oldschool/Apple_sapling/viewitem?obj=5496";
char* bananaURL = "http://services.runescape.com/m=itemdb_oldschool/Banana_s3apling/viewitem?obj=5497";
char* curryURL = "http://services.runescape.com/m=itemdb_oldschool/Curry_sapling/viewitem?obj=5499";
char* orangeURL = "http://services.runescape.com/m=itemdb_oldschool/Orange_sapling/viewitem?obj=5498";
char* pineappleURL = "http://services.runescape.com/m=itemdb_oldschool/Pineapple_sapling/viewitem?obj=5500";
char* palmURL = "http://services.runescape.com/m=itemdb_oldschool/Palm_sapling/viewitem?obj=5502";
char* papayaURL = "http://services.runescape.com/m=itemdb_oldschool/Papaya_sapling/viewitem?obj=5501";
char* calquatURL = "http://services.runescape.com/m=itemdb_oldschool/Calquat_sapling/viewitem?obj=5503";

int main(int argc, char* argv[]) {

	if (argc == 5) {

	// Hashtable that will hold the amount of experience granted for each sapling type 
	hashtable_t* saplingXPRates = hashtable_new(20);

	insertIntoHashtable(saplingXPRates);

	// Tree types that will be assigned URLs based on command line input
	char* normal = argv[1];
	char* fruit = argv[2];

	// How much experience the player currently has
	int currXP = atoi(argv[3]);

	/* Check to see if current xp is below 83. Players begin with having 83 experience
	 * in this skill at level 1
	 */
	if (currXP < 83) {
		fprintf(stderr, "it is impossible to have below 83 xp\n");
		exit(1); 
	}

	int goalLevel = atoi(argv[4]);

	// The maximum level that can be achieved is 99 
	if (goalLevel > 99 || goalLevel < 2) {
		fprintf(stderr, "the goal level must be between 2 and 99\n");
		exit(2);
	}

	// Check to see if current level is greater than the desired goal level 
	if (convertCurrXPtoLevel(currXP) >= goalLevel) {
		fprintf(stderr, "your goal level must be greater than your current level\n");
		exit(3);
	}

	// URLs associated with each chosen tree type
	char* normalURL;
	char* fruitURL;

	normalURL = assignNormalURL(normal);
	fruitURL = assignFruitURL(fruit);

	/* In the function "assignNormal/FruitURL, the string 'null' is assigned to the respective
	   url if it has not been successfully changed. These will always be due to a typo on the
	   command line for either the normalTree or fruitTree*/ 
	if (strcmp(normalURL, "null") == 0) {
		fprintf(stderr, "Invalid input for normalTree type\nTypes include: 'oak' 'willow 'maple' 'yew' or 'magic'\n");
		fprintf(stderr, "Usage: ./farmingCalculator normalTree fruitTree currentXP goalLevel\n");
		exit(4);
	}

	if (strcmp(fruitURL, "null") == 0) {
		fprintf(stderr, "Invalid input for fruitTree type\nTypes include: 'apple' 'banana' 'curry' 'orange' 'pineapple' 'palm' or 'papaya\n");
		fprintf(stderr, "Usage: ./farmingCalculator normalTree fruitTree currentXP goalLevel\n");
		exit(5);
	}

	/* Prices associated with each chosen tree type*/
	int normalPrice = extractPrice(getHTMLFromPage(normalURL));
	int fruitPrice = extractPrice(getHTMLFromPage(fruitURL));

	gpPerXP(saplingXPRates, currXP, normalPrice, fruitPrice, normal, fruit);

	int daysToGoal = daysTillGoal(currXP, goalLevel, experiencePerDay(saplingXPRates, normal, fruit, currXP));

	totalGPLoss(normalPrice, fruitPrice, daysToGoal);
	
	return 0;

	}

	fprintf(stderr, "Error, usage: ./farmingCalculator normalTree fruitTree currentXP goalLevel\n");
	exit(3);

}

/* Grabs the HTML from a given URL */
char* getHTMLFromPage (char* url) {

	// create a new webpage based on the url passed through
	webpage_t *itemPage = webpage_new(url, 0, NULL);

	// fetch the HTML from the webpage
	webpage_fetch(itemPage);

	return webpage_getHTML(itemPage);
	
} 

/* Grab and return price from the given URL (assumes the URL is taken from
the Runescape's grand exchange website */
int extractPrice(char* string) {
	
	/* Splices out all of the html until "Guide" is seen, then 
	   until "'" is seen. The next characters in the HTML code
	   will be the sapling's price */

	char* remove = strstr(string, "Guide");
	char* start = strstr(remove, "'");

	char rawPrice[50]; // Price extracted from url, including commas
	

	int i = 0;
	
	/* Loop through the price and add it to the char array. 
	   e.g. should look something like: 73,203
	*/
	while (start[i] != '>')  {
		rawPrice[i] = start[i];
		i++;
	} 

	int x = 0;
	char splicedPrice[10]; // New array with the commas removed e.g. 73203

for (int j = 0; j < strlen(rawPrice) - 1; j++) {

			// If char is not a digit, it must be a comma, so skip it
			if (!(isdigit(rawPrice[j]))) {
				continue;
			}

			splicedPrice[x] = rawPrice[j];
			x++;
	}

	int priceAsInt;
	priceAsInt = atoi(splicedPrice); // Convert char array to int

	return priceAsInt;

}

/* Inserts XP rates for each sapling into a hashtable. 
   Key = tree type, item = XP granted per farming patch for the sapling */  
void insertIntoHashtable(hashtable_t* saplingXPRates) {

		hashtable_insert(saplingXPRates, "oak", (void*)481);
		hashtable_insert(saplingXPRates, "willow", (void*)1481);
		hashtable_insert(saplingXPRates, "maple",(void*) 3448);
		hashtable_insert(saplingXPRates, "yew", (void*)7151);
		hashtable_insert(saplingXPRates, "magic", (void*)13914);
		hashtable_insert(saplingXPRates, "apple", (void*)1272);
		hashtable_insert(saplingXPRates, "banana", (void*)1841);
		hashtable_insert(saplingXPRates, "curry", (void*)3037);
		hashtable_insert(saplingXPRates, "orange", (void*)2587);
		hashtable_insert(saplingXPRates, "pineapple",(void*) 4792);
		hashtable_insert(saplingXPRates, "palm", (void*)10510);
		hashtable_insert(saplingXPRates, "papaya", (void*)6380);
		hashtable_insert(saplingXPRates, "calquat", (void*)12244);
}

/* Converts player's current experience in the farming skill to their level
   based on Runescape's algorithm */
int convertCurrXPtoLevel(int currXP) {
	
		int level = 1;

		// Experience needed for the next level increase by a factor of ~1.10409 per level
		float factor = 1.10409; 
		// Characters begin with 83 experience in the farming skill
		float sumXP = 83.0; 

		// Keep increasing sumXP until the current XP is hit, while incrementing level
		while (sumXP < currXP) {

			sumXP = (sumXP * factor) + 83;
			level++;
		}

		return level;
}

/* Converts player's current level in the farming skill to their experience
   based on Runescape's algorithm */
float convertGoalLevelToXP(int goalLevel) {

	int level = 2;
	float factor = 1.10409;
	float sumXP = 83.0;

	while (level < goalLevel) {

		sumXP = (sumXP * factor) + 83;
		level++;
	}

	return sumXP;

}

/* Calculates the amount of experience a player can expect per day based on the normal tree
and fruit tree that are chosen. Also assumes one farm run per day. Used in the daysTillGoal function */
float experiencePerDay (hashtable_t *ht, char* normalTree, char* fruitTree, int currXP) {

	float total = 0.0; // Experience gained

	int normal = (uintptr_t)hashtable_find(ht, normalTree); // Grab the experience for the normal sapling
	int fruit = (uintptr_t)hashtable_find(ht, fruitTree); // Grab the experience for the fruit sapling

	/* Grab experience for the calquat sapling. A farming level of 72 is required to plant this special
	   tree type. Given their extremely low price and high xp rates, they are automatically included
	   in the calculation if the player has at least 72 farming. */
	int calquat = (uintptr_t)hashtable_find(ht, "calquat"); 

	/* Calculates xp per day for the normal and fruit tree types. Each type is multiplied
	   by 5 because there are 5 patches for each type found throughout Runescape. Thus a total
	   of 5 normal saplings and 5 fruit sapling will be planted per day */
	total += ((normal * 5) + (fruit * 5));

	// If player's level is 72 or higher, include calquat in the calculation. Only 1 can be planted per day
	if (convertCurrXPtoLevel(currXP) >= 72) {
		total += calquat;
	}

	return total;

}

/* Calculates the number of days it will take the player to reach their
   goal level */
int daysTillGoal(int currXP, int goalLevel, int XPPerDay) {

		int goalXP = convertGoalLevelToXP(goalLevel);
		int XPToGain = goalXP - currXP;

		int numDays = (XPToGain / XPPerDay) + 1;

		return numDays;
}

/* Calculates the total number of GP (gold pieces) that will be lost as
   as a result of achieving the desired goal level */
int totalGPLoss(int normalPrice, int fruitPrice, int days) {
		
		int totalLoss;

		// Multiply by 5 because there are 5 patches for each type
		int LossPerDay = (normalPrice * 5) + (fruitPrice * 5);

		totalLoss = LossPerDay * days;

		printf("It will take %d days to Achieve your goal, with a total GP loss of %d\n", days, totalLoss);

		return totalLoss;
}

/* Calculates the number of gp that will be lost per experienced gained */
void gpPerXP(hashtable_t *ht, int currXP, int normalPrice, int fruitPrice, char* normalTree, char* fruitTree) {
		
		// Divide price by xp and negate the result to get the GP/XP
		int normalTreeXP = (uintptr_t)hashtable_find(ht, normalTree);
		double normalGPPerXP = -((double) normalPrice / (double) normalTreeXP);

		printf("The GP/XP for a %s tree is %f\n", normalTree, normalGPPerXP);

		// Do the same for the fruit tree type 
		int fruitTreeXP = (uintptr_t)hashtable_find(ht, fruitTree);
		double fruitGPPerXP = -((double) fruitPrice / (double) fruitTreeXP);

		printf("The GP/XP for a %s tree is %f\n", fruitTree, fruitGPPerXP);

		// If the player's level is at least 72, do it for the special calquat tree as well
		if (convertCurrXPtoLevel(currXP) >= 72) {

			int calquatTreeXP = (uintptr_t)hashtable_find(ht, "calquat");
			int calquatPrice = extractPrice(getHTMLFromPage(calquatURL));

			double calquatGPPerXP = -((double) calquatPrice / (double) calquatTreeXP);

			printf("GP/XP for a calquat tree is %f\n", calquatGPPerXP);
		}	
}

/* Assign url to the correct normal tree type */
char* assignNormalURL(char* normalTree) {

	bool urlChanged = false;
	char *url;

	if (strcmp(normalTree, (void*)"oak") == 0) {
		url = oakURL;
		urlChanged = true;
	}

	else if (strcmp(normalTree, (void*)"willow") == 0) {
		url = willowURL;
		urlChanged = true;
	}

	else if (strcmp(normalTree, (void*)"maple") == 0) {
		url = mapleURL;
		urlChanged = true;
	}

	else if (strcmp(normalTree, (void*)"yew") == 0) {
		url = yewURL;
		urlChanged = true;

	}

	else if (strcmp(normalTree, (void*)"magic") == 0) {
		url = magicURL;
		urlChanged = true;
	}

	if (urlChanged == false) {
			url = "null";
	}

	return url;
}

/* Assign url to the correct fruit tree type */
char* assignFruitURL(char* fruitTree) {

	bool urlChanged = false;
	char *url;

	if (strcmp(fruitTree, (void*)"apple") == 0) {
		url = appleURL;
		urlChanged = true;
	}

	else if (strcmp(fruitTree, (void*)"banana") == 0) {
		url = bananaURL;
		urlChanged = true;
	}

	else if (strcmp(fruitTree, (void*)"curry") == 0) {
		url = curryURL;
		urlChanged = true;
	}

	else if (strcmp(fruitTree, (void*)"orange") == 0) {
		url = orangeURL;
		urlChanged = true;

	}

	else if (strcmp(fruitTree, (void*)"pineapple") == 0) {
		url = pineappleURL;
		urlChanged = true;
	}

	else if (strcmp(fruitTree, (void*)"palm") == 0) {
		url = palmURL;
		urlChanged = true;
	}

	else if (strcmp(fruitTree, (void*)"papaya") == 0) {
		url = papayaURL;
		urlChanged = true;

	}

	if (urlChanged == false) {
		url = "null";
	}

	return url;
}
