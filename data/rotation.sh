#!/bin/sh

for FILE in car*
do
   echo $FILE
   convert $FILE -rotate -90 $FILE
done
