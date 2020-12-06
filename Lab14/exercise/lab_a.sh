#!/bin/bash
#----------------------------------------------------------------------------
# Simple command exercises
# student copy
#----------------------------------------------------------------------------
# Create a 2 word variable and print it out
X="dog cat"
echo "The variable is "$X

# Read a number from the command line
read REPLY
echo "The variable is "$REPLY

# Use the number in a simple if statement
if   [ "$REPLY" -lt "3" ]; then
	echo "number is less than 3"
elif [ "$REPLY" -gt "3" ]; then
	echo "number is greater than 3"
else
	echo "number is equal to 3"
fi

# Do some math with the number
REPLY=$((REPLY + 2))
echo "after simply adding two, number is "$REPLY

# Do some math with the number using the alternative method
let REPLY+=1
echo "after adding 1 with let cmd, number is "$REPLY

# Count from 0 to 3
COUNTER=0
while [ "$COUNTER" -lt "4" ]
do
	echo $COUNTER
	COUNTER=$((COUNTER + 1))
done

# Cycle through a list
for items in One Two Three
do
	echo $items
done

# Verify string comparison functions
echo "Enter dog(or not)"
read input
if [ "$input" != "dog" ]; then
	echo "input was not dog"
else
	echo "input was dog"
fi

# Exercise the case statement
echo "enter a number [1-3]"
read value
case $value in
1) echo -n "case statement 1"
   ;;
2) echo -n "case statement 2"
   ;;
3) echo -n "case statement 3"
   ;;
*) echo -n "other"
   ;;
esac

# Get and print the current directory
var=$(pwd)
echo $var
