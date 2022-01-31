/*******************************************************************/
/*                          IZP Projekt 1                          */
/*                   Jakub Masek     xmasek19                      */
/*                                                                 */
/*              Datum: 28.10.2021        Verze: 6                  */
/*                                                                 */
/*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

//Function for checking validity of argument (argument is number, number is between 0 and maximum)
int isArgValid(char endChar[], long long input, int max)
{
    if (endChar[0] == '\0' && (input > 0 && (input <= max || max == 0)))
    {
        return TRUE;
    }
    return FALSE;
}

//Fuction gets every single character from current password and compares them with characters in string uniquChars, if character is new, add it to uniqueChars
void addNewChars(char uniqueChars[], char buffer[])
{
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        for (int j = 0; uniqueChars[j] != '\0'; j++)
        {
            if (uniqueChars[j] == buffer[i])                //if character from current password is in unique chars string
            {
                break;
            }
            else if (uniqueChars[j + 1] == '\0' && (buffer[i] >= 32 && buffer[i] <= 126)) //character from password not found in uniquChars and is valid
            {
                uniqueChars[j + 1] = buffer[i];             //add new character to uniquChars
                break;
            }
        }
    }
}

//Checks level 1, returns true when the level passes
int checkLevel1(char buffer[])
{
    int isLower = 0, isUpper = 0;
    for (int i = 0; buffer[i] != '\0'; i++)         //take every single char of one password
    {
        switch (buffer[i])
        {
        case 'a' ... 'z':                           //character is lower alphabet
            isLower = 1;
            break;
        case 'A' ... 'Z':                           //character is upper alphabet
            isUpper = 1;
            break;
        }
        if (isLower && isUpper)                     //is level 1 is successful
            return TRUE;
    }
    return FALSE;
}

//Checks level 2, checks max value of parameter, returns true when the level passes
int checkLevel2(char buffer[], long long parameter)
{
    int isLower = 0, isUpper = 0, isNum = 0, isSpec = 0;
    if (parameter > 4) // max value of parameter is 4 -> if its bigger set 4
    {
        parameter = 4;
    }
    
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        switch (buffer[i])
        {
        case 'a' ... 'z':
            isLower = 1;
            break;
        case 'A' ... 'Z':
            isUpper = 1;
            break;
        case '0' ... '9':
            isNum = 1;
            break;
        case 32 ... 47:
        case 58 ... 64:
        case 91 ... 96:
        case 123 ... 126:           //special characters without alphabet. and numbers
            isSpec = 1;
            break;
        }

        if ((isLower + isUpper + isNum + isSpec) >= parameter) //if sum of passed conditions is equal to parameter
            return TRUE;
    }
    return FALSE;
}

//Checks level 3, returns true if there are no same chars in row with lenght of paramater
int checkLevel3(char buffer[], long long parameter)
{
    int sameCharsCount = 1;
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        if (buffer[i] == buffer[i + 1])     //check if character with index i and next char are same
        {
            sameCharsCount++;               //add 1 to same chars counter
        }
        else
        {
            sameCharsCount = 1;             //reset counter
        }
        if (sameCharsCount >= parameter)    //if count of same chars in row is equal to paramater 
        {
            return FALSE;
        }
    }
    return TRUE;
}

//Checks level 4, returns true if there are not same substrings with length of parameter
int checkLevel4(char buffer[], long long parameter)
{
    for (int i = 0; buffer[i] != '\0'; i++) //take every single char of password
    {
        for (int j = i + 1; buffer[j] != '\0'; j++) //take every single char of password (starting at index i+1)
        {
            int sameSubLenght = 0;              //reset counter of length of substring
            char mainChar = buffer[i];          //set main character
            char compareChar = buffer[j];       //set character to compare with main character
            int iCopy = i, jCopy = j;
            while (compareChar == mainChar)     //compare if 2 chars in password are same
            {
                sameSubLenght++;                //same substrings length counter +1
                iCopy++;                        //index of main char +1
                jCopy++;                        //index of compare char +1
                mainChar = buffer[iCopy];       //save next character in potencional substring 1
                compareChar = buffer[jCopy];    //save next character in potencional substring 2
                if (sameSubLenght >= parameter) //if there are same substrings with length of parameter
                {
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

//Checks is stats are enabled
int isStatsAllowed(int argc, char *argv[])
{
    char statsText[] = "--stats";
    int pos = 0;
    while ( pos < argc && !(argv[pos][0] == '-' && argv[pos][1] == '-'))  //search argument starting with --
    {
        pos++;
    }
    if (pos == argc)    // "--" not found
    {
        return FALSE; 
    }
    for (int i = 0; argv[pos][i - 1] != '\0' || i == 0; i++) //take every single char from argument
    {
        if (argv[pos][i] != statsText[i])                    //check if argument is --stats, if not return false
        {
            return FALSE;
        }
    }
    return TRUE;                                             //stats enabled
}

//Counts and checks length of password
int strLenCheck(char buffer[], int *passwordLength)
{
    int i = 0;
    while (buffer[i] != '\0' && buffer[i] != '\n'){  //chars in password counter
        i++;
    }

    if (i > 100 && buffer[100] != '\n')          //if password is too long
    {
        fprintf(stderr, "%s", "The password is too long! (max 100)");
        return FALSE;
    }
    *passwordLength = i; //save length of password
    return TRUE;
}

//Compares length of current password with current minimal length
int checkMinCountOfChars(int passwordLength, int minLength)
{
    if (passwordLength < minLength)
    {
        return passwordLength;
    }
    return minLength;
}

//Returns sum of passed levels (0 - 4)
int levelsPassedCount(int levelsChecked[])
{
    int sum = 0;
    for (int i = 0; i < 4; i++)
    {
        sum += levelsChecked[i];
    }
    return sum;
}

//Find arguments "-l" and "-p" and read argument after them, if "-l" and "-p" not found and number of argumets is 
//valid, read argument 1 as level and 2 as parameter
int readArguments(int argc, char *argv[], long long *level, long long *parameter)
{
    int posLevel = 1, posParameter = 1;
    while ((argv[posLevel][0] != '-' || argv[posLevel][1] != 'l') && posLevel < argc - 1){   //finding "-l" in arguments
        posLevel++;
    }
    while ((argv[posParameter][0] != '-' || argv[posParameter][1] != 'p') && posParameter < argc - 1){  //finding "-p" in arguments
        posParameter++;
    }

    char *endChar;
    if (posLevel < argc - 1 && posParameter < argc - 1)             //if "-l" and "-p" found
    {
        *level = strtoll(argv[posLevel + 1], &endChar, 10);         //read argument after "-l"
        *parameter = strtoll(argv[posParameter + 1], &endChar, 10); //read argument after "-p"
    }
    else if (argc < 5)      //if "-l" and "-p" not found and count of argument is valid
    {
        *level = strtoll(argv[1], &endChar, 10);                    //read argument 1
        *parameter = strtoll(argv[2], &endChar, 10);                //read argument 2
    }
    else    //invalid arguments
    {
        return FALSE;
    }

    if (!isArgValid(endChar, *level, 4) || !isArgValid(endChar, *parameter, 0)){ //check validity of arguments
        return FALSE;
    }
    return TRUE;
}

//Function counts and print stats
void printStats(char uniqueChars[], int minLength, int charsCount, int stringCount)
{
    int uniqueCharsCount = 0;
    while (uniqueChars[uniqueCharsCount + 1] != '\0'){ //sum of unique chars
        uniqueCharsCount++;
    }
    if(minLength == 101){                              //no password entered (101 is default)
        minLength = 0;
    }
    printf("Statistika:\nRuznych znaku: %d\nMinimalni delka: %d\nPrumerna delka: %.1f\n", uniqueCharsCount, minLength, (float)charsCount / (float)stringCount);
}


int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 6)
    {
        fprintf(stderr, "You entered wrong count of arguments!");
        return EXIT_FAILURE;
    }

    long long level = 0, parameter = 0;
    if (!readArguments(argc, argv, &level, &parameter))     //read and save arguments + check validity
    {
        fprintf(stderr, "You entered wrong arguments!");
        return EXIT_FAILURE;
    }
    int statsAllowed = 0;
    if(argc == 4 || argc == 6){                             //if count of arguments is valid for stats
        statsAllowed = isStatsAllowed(argc, argv);
        if(!statsAllowed)
        {
            fprintf(stderr, "%s", "You entered wrong stats argument!");
            return EXIT_FAILURE;
        }
    }

    char buffer[103], uniqueChars[95] = {EOF};
    int minLength = 101, charsCount = 0, stringCount = 0;

    while (fgets(buffer, 103, stdin) != NULL)       //cycle for getting every single password
    {
        int passwordLength = 0, levelsChecked[] = {0, 0, 0, 0};

        if (!strLenCheck(buffer, &passwordLength))
        {
            return EXIT_FAILURE;
        }

        if (statsAllowed)
        {
            addNewChars(uniqueChars, buffer);                            //checks if character was used before, if not, adds character to string of unique characters
            minLength = checkMinCountOfChars(passwordLength, minLength); //checks if this password is shortest
            charsCount += passwordLength;                                //add lenght of password to count of total used characters
            stringCount++;
        }

        if (level >= 1)
        {
            levelsChecked[0] = checkLevel1(buffer);
        }

        if (level >= 2)
        {
            levelsChecked[1] = checkLevel2(buffer, parameter);
        }

        if (level >= 3)
        {
            levelsChecked[2] = checkLevel3(buffer, parameter);
        }

        if (level >= 4)
        {
            levelsChecked[3] = checkLevel4(buffer, parameter);
        }
        if (levelsPassedCount(levelsChecked) >= level) //if sum of passed levels is enough to print password
        {
            printf("%s", buffer);
        }
    }
    if (statsAllowed)           //if stats are allowed, print stats
    {
        printStats(uniqueChars, minLength, charsCount, stringCount);
    }

    return EXIT_SUCCESS;
}