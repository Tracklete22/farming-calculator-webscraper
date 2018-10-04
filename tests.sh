#!/bin/bash
# test.sh - Tests various cases on the command line for "fruitCalculator.c"
# by Myles Holt

# First 7 tests should work correctly

./farmingCalculator oak apple 100000 99

# The test below checks to see if the calquat will be included in the calculation
./farmingCalculator oak apple 6000000 99

./farmingCalculator willow banana 500000 70

./farmingCalculator maple curry 500000 70

./farmingCalculator yew orange 600000 70

./farmingCalculator magic pineapple 600000 70

./farmingCalculator magic papaya 83 80

./farmingCalaculator magic palm  3000 60

# Invalid number off arguments
./farmingCalculator magic papaya random 3400 60 

# Normal tree type does not exist 
./farmingCalculator fake pineapple 10000 50

# Fruit tree type does not exist
./farmingCalculator maple fake 10000 50

# The level associated with the current xp is greater than the goal level
./farmingCalculator maple curry 10000000 50 

# Goal level out of range, must be between 2 - 99
./farmingCalculator maple curry 500000 150

# Current xp cannot be under 83 (level 1 begins with 83 xp)
./farmingCalculator maple curry 40 2
