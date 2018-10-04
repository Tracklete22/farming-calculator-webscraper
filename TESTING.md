## Testing for farmingCalculator.c

## By Myles Holt


### Testing

To test `farmingCalculator.c`, a bash script titled `tests.sh` is utilized.

This bash script tests various command line arguments, including both valid

and invalid ones. 

### How to Use the Bash Script

To run the bash script, type `bash tests.sh`

### Example 

The commandline follows the structure `./farmingCalculator normalTree fruitTree currXP goalLevel`

normalTree = normal sapling types, which include: oak, willow, maple, yew, magic

fruitTree = fruit sapling types: which include: apple, banana, orange, curry, pineapple, palm, papaya

currXP = the player's current experience in the farming skill

goalLevel = the farming level the player wishes to achieve

e.g. `./farmingCalculator yew papaya 243094 80`

**Output:**

The GP/XP for a yew tree is -11.473360

The GP/XP for a papaya tree is -2.345611

It will take 26 days to Achieve your goal, with a total GP loss of 12611430 
