#! usr/bin/bash

echo $'-------------------------------------- BUILDING NACHOS --------------------------------------'
make
if [ $? -eq 0 ]; then
    echo $'\n-------------------------------------- DONE --------------------------------------'
    echo $'Here is output: \n'
    ./userprog/nachos -rs 1023 -x ./test/$1
else
    echo $'\n-------------------------------------- FAILED -------------------------------------- \n'
fi
