## Farming Efficiency Calculator

### By Myles Holt

##### farmingCalculator

`farmingCalculator.c` Calculates the efficiency of using different types of tree saplings while training
the farming skill on Runescape. The program calculates the GP/XP based on prices of tree 
saplings that are updated daily on Runescape's website, total GP loss, and number of days to reach
the desired goal level.

##### Background Information

######---General information --
Oldschool Runescape is an MMO (massive multiplayer online) game with a thriving economy and plenty of skills to train. Each skill can be trained
from level 1 to a maximum level of 99. The farming skill (what this program is evaluating) is one of these skills. Each level in the skill
requires a certain amount of experience. The amount of experience required per level increases between levels, making it more difficult
as you progress

######-- How Farming is Trained --
Although there are several methods of training farming, this program **focuses on tree running** due to the large amount of xp the runs give
as well as the little amount of time they take per day (~ 5 minutes). Tree running is a method where a player buys saplings from the 
grand exchange (a trading system where players can buy and sell items), plants saplings at various patch locations around Runescape, and after 
waiting the desired amount of time for the saplings to grow (~ 6 to 21 hours depending on the tree type), revisits each of the patches and checks
their health to recieve sums of experience. From here, a player can then dig up the tree, plant a new sapling, and wait again. Due to the 
various growth times for the different tree types, players tend to do one tree run per day.

For more information on the farming skill, visit "http://oldschoolrunescape.wikia.com/wiki/Farming"

######--Tree Running in Detail-- 

1) Player buys both fruit and tree saplings from the grand exchange. If the player is level 72 farming,
   the calculator assumes that the player will also buy calquat saplings (a special tree type) due to their low cost and high experience rate. 

   Normal sapling types include: oak, willow, maple, yew, magic
   Fruit sapling types include: apple, banana, orange, curry, pineapple, palm, papaya

   All 13 of these saplings have various, fixed experience rates

   Prices of the saplings are taken from "http://services.runescape.com/m=itemdb_oldschool"

2) There are 5 normal tree patches and 5 fruit tree patches, as well as one special calquat tree patch found in Runescape. One sapling can be 
   planted per patch for the respective tree type, with each sapling granting a certain amount of experience. This means that 5 normal saplings,
   5 fruit saplings, and possibly 1 calquat sapling depending on the player's farming level, will be planted per day. 

3) Based on the information provided above, the player will (generally via quick teleportation methods) travel to the various patches, plant their saplings, and wait a day for each of the saplings to grow. 

4) Once fully grown, the player will revist the patches and click the "check health" option on each of the trees, which will grant the player some experience.

5) Plant new saplings, and repeat

##### Inspiration

The program calculates important information regarding costs and time. This is a skill that me and many others are currently
training, and this program will help players plan an effective strategy based on the player's care for the cost, time, 
or both. I also found it pretty fun and cool to make!

##### Usage

the farmingCalculator module is defined and implemented in farmingCalculator.c 

/*Initializes structs and variables, checks validity of command line arguments, and calls various functions that allow
  the program to operate*/
int main(int argc, char* argv[]);


/*Grab and return price from the given URL (assumes the URL is taken from
the Runescape's grand exchange website*/
int extractPrice(char* string);

/*Grabs the HTML from a given URL*/
char* getHTMLFromPage(char* url);


/*Assign url to the correct normal tree type*/
char* assignNormalURL(char* normalTree);


/*Assign url to the correct fruit tree type*/
char* assignFruitURL(char* fruitTree);


/*Inserts XP rates for each sapling into a hashtable. 
   Key = tree type, item = XP granted per farming patch for the sapling*/  
void insertIntoHashtable(hashtable_t* saplingXPRates);


/*Calculates the amount of experience a player can expect per day based on the normal tree
and fruit tree that are chosen. Also assumes one farm run per day. Used in the daysTillGoal function*/
float experiencePerDay (hashtable_t *ht, char* normalTree, char* fruitTree, int currXP);


/*Converts player's current experience in the farming skill to their level
   based on Runescape's algorithm*/
int convertCurrXPtoLevel(int currXP);


/*Converts player's current level in the farming skill to their experience
   based on Runescape's algorithm*/
float convertGoalLevelToXP(int goalLevel);


/*Calculates the number of days it will take the player to reach their
   goal level*/
int daysTillGoal(int currXP, int goalLevel, int XPPerDay);


/*Calculates the number of gp that will be lost per experienced gained*/
void gpPerXP(hashtable_t *ht, int currXP, int normalPrice, int fruitPrice, char* normalTree, char* fruitTree);


/*Calculates the total number of GP (gold pieces) that will be lost as
   as a result of achieving the desired goal level*/
int totalGPLoss(int normalPrice, int fruitPrice, int days);


##### Exit Codes

Exit 1 - Current xp is less than 83 (not possible, player begins with 83 xp at lvl 1)
Exit 2 - Goal level must be between 2 and 99 
Exit 3 - Goal level must be greater than the current level 
Exit 4 - Invalid input for normalTree type on cmd line
Exit 5 - Invalid input for fruitTree type on cmd line

###### Assumptions

- Only one farming run is executed per day
- A player has access to the five normal and fruit tree patches 
- If the player is at least 72 farming, the calquat tree type will automatically be included in the calculation

##### Libraries

- The program makes use of the library `libcs50`, which is a library that was used in my CS50 course (Software
  Design and Implementation). Most of these modules were written by professor David Kotz. However, `hashtable.c/h, bag.c/h, set.c/h, and counters.c/h` 
  were written by me as a part of lab 3 in my CS50 class.

- farmingCalculator.c makes use of hashtable.h and webpage.h from the libcs50 library 

##### Compilation

- Utilizes a makefile to compile all of the necessary components
- To compile, simply type `make`
- To decompile, type `make clean`
