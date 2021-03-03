
# This is a bash script that generates random records

#!/bin/bash

# Functions
checkFile(){
    retval=-1

    if [ -r $1 ] && [ -f $1 ];
    then
        retval=1 
    else
        retval=0
    fi
    return "$retval"
}

# Define Colors 
NOCOLOR="\033[0m"
GREEN="\033[0;32m"
RED="\033[0;31m"

# Checking here if the arguments are correct 
if [[ $# != 4 ]]
then 
    echo -e "${RED}Usage: ./testFile.sh 'virusesFile' 'countriesFile' 'numLines' 'duplicatesAllowed' ${NOCOLOR}"
    exit 1
fi 

#echo -e "${GREEN}Given 4 arguments ${NOCOLOR} " 

# Parse the arguments into variables
VFILE=$1
CFILE=$2
NLINES=$3
DUPFLAG=$4

# Check if the virusesFile and the countries file are readable
checkFile "$VFILE"
VCHECK=$?
# echo -e "vcheck is $VCHECK"

checkFile "$CFILE"
CCHECK=$?
# echo -e "ccheck is $CCHECK"

# if [[ ${CCHECK} == 1 && ${VCHECK} == 1 ]];  
if [[ ${CCHECK} && ${VCHECK} ]];  
then 
    echo -e "Files are ready to read"
else
    echo -e "Error! Check the files."
    exit 1
fi

# input file
INPUTFILE="inputfile.txt"

# Create the input file.
# If exists a file with the INPUTFILE name, these instructions
# delete and recreate it
if [[ -e ${INPUTFILE} ]];
then 
    # echo -e "input file exists"
    rm -f ${INPUTFILE}
    touch ${INPUTFILE}
    # echo -e "input file deleted and created new empty"
else 
    # echo -e "input file does not exists"
    touch ${INPUTFILE}
    # echo -e "input file created"
fi


# GENERATE THE VALUES

CUR_LINE=0
ID=0
while [ ${CUR_LINE} -lt ${NLINES} ]
do
    # Generates random num in range [3,12] that represents 
    # the length of the generated name and surname.
    NAME_LENGTH=$((3 + $RANDOM %10 ))
    SURNAME_LENGTH=$((3 + $RANDOM %10 ))

    # Generates random age in range [1,120]
    AGE=$((1+ $RANDOM %120 ))

    # Generates random strings with the specified length
    NAME=$(cat \/dev/urandom | tr \-dc A-Z | head \-c ${NAME_LENGTH} )
    SURNAME=$(cat \/dev/urandom | tr \-dc A-Z| head \-c ${SURNAME_LENGTH} )

    # Generate random countries and viruses from the files that contain them
    COUNTRY=$(shuf \-n 1 ${CFILE} )
    VIRUS=$(shuf \-n 1 ${VFILE} )

    
    # ID generation. If duplicates allowed [DUPFLAG=1] we can generate 
    # random IDs. Either if are not allowed we can increment the previous ID
    if [[ ${DUPFLAG} == 1 ]];
    then
        ID=$((1+ $RANDOM % ${NLINES} ))
    else
        ID=$((${ID} + 1))
    fi

    VACCINATED_FLAG=$(($RANDOM % 2))
    if [[ ${VACCINATED_FLAG} == 1 ]];
    then
        ISVACCINATED="YES"
        DAY=$((1+ $RANDOM %30 ))
        MONTH=$((1+ $RANDOM %12 ))
        YEAR=$((2015 + $RANDOM %7 ))
        echo -e "${ID} ${NAME} ${SURNAME} ${COUNTRY} ${VIRUS} ${ISVACCINATED} ${DAY}/${MONTH}/${YEAR} " >> ${INPUTFILE} 
    else
        ISVACCINATED="NO"
        echo -e "${ID} ${NAME} ${SURNAME} ${COUNTRY} ${VIRUS} ${ISVACCINATED}" >> ${INPUTFILE} 
    fi

    # Append the data on the input file
    
    
    # increment CUR_LINE
    CUR_LINE=$((${CUR_LINE} + 1))
done

# Make the input file as read only 
chmod -w ${INPUTFILE}