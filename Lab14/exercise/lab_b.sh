#!/bin/bash
parmFun () {
read in1
read in2
echo "In the function, the passed parameters are:" $in1 $in2
}

returnNUM () {
retnum=999
}

echo "calling the parmFun function"
parmFun
echo "calling the return number function"
returnNUM
echo "returned number is:" $retnum


