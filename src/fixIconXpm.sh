#!/bin/bash

for file in gui/icons/*.xpm
do
sed -i -e "s/static char \* /static char const \* const /" $file
done
