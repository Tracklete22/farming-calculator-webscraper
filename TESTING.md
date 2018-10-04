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
