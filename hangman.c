#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "hangman.h"

char secretword[WORD_LENGTH];
char chances[26];
int attempts = 0;
void opening()
{

    printf("**********************\n");
    printf("*    Hangman Game    *\n");
    printf("**********************\n\n");
}

void try()
{
    char try;
    scanf(" %c", &try);

    chances[attempts] = try;
    attempts++;
}

int tried(char letter)
{
    int found = 0;

    for (int j = 0; j < attempts; j++)
    {
        if (chances[j] == letter)
        {
            found = 1;
            break;
        }
    }

    return found;
}

void drawgallows()
{

    int errors = wrongattempts();
    printf("  _______       \n");
    printf(" |/      |      \n");
    printf(" |      %c%c%c  \n", (errors >= 1 ? '(' : ' '),
           (errors >= 1 ? '_' : ' '), (errors >= 1 ? ')' : ' '));
    printf(" |      %c%c%c  \n", (errors >= 3 ? '\\' : ' '),
           (errors >= 2 ? '|' : ' '), (errors >= 3 ? '/' : ' '));
    printf(" |       %c     \n", (errors >= 2 ? '|' : ' '));
    printf(" |      %c %c   \n", (errors >= 4 ? '/' : ' '),
           (errors >= 4 ? '\\' : ' '));
    printf(" |              \n");
    printf("_|___           \n");
    printf("\n\n");

    for (int i = 0; i < strlen(secretword); i++)
    {

        int found = tried(secretword[i]);

        if (found)
        {
            printf("%c", secretword[i]);
        }
        else
        {
            printf("_ ");
        }
    }
    printf("\n");
}

void addword()
{

    char want;

    printf("Would you like to add a new word to the game? (Y/N)");
    scanf(" %c", &want);

    if (want == 'Y')
    {

        char newword[WORD_LENGTH];
        printf("What is the new word?");
        scanf("%s", newword);

        FILE *f;

        f = fopen("word.txt", "r+");
        if (f == 0)
        {
            printf("Sorry, database not available\n\n");
            exit(1);
        }

        int qtt;
        fscanf(f, "%d", &qtt);
        qtt++;

        fseek(f, 0, SEEK_SET);
        fprintf(f, "%d", qtt);

        fseek(f, 0, SEEK_END);

        fprintf(f, "\n%s", newword);
        fclose(f);
    }
}

void chooseword()
{
    FILE *f;

    f = fopen("word.txt", "r");
    if (f == 0)
    {
        printf("Sorry, database not available\n\n");
        exit(1);
    }

    int wordnumber;
    fscanf(f, "%d", &wordnumber);

    srand(time(0));
    int random = rand() % wordnumber;

    for (int i = 0; i <= random; i++)
    {
        fscanf(f, "%s", secretword);
    }

    fclose(f);
}

int correct()
{
    for (int i = 0; i < strlen(secretword); i++)
    {
        if (!tried(secretword[i]))
        {
            return 0;
        }
    }
    return 1;
}
int wrongattempts()
{

    int errors = 0;

    for (int i = 0; i < attempts; i++)
    {
        int exist = 0;

        for (int j = 0; j < strlen(secretword); j++)
        {
            if (chances[i] == secretword[j])
            {
                exist = 1;
                break;
            }
        }
        if (!exist)
            errors++;
    }
    return errors;
}

int hangged()
{

    return wrongattempts() >= 5;
}

int main()
{
    chooseword();
    opening();

    do
    {
        drawgallows();

        try();

    } while (!correct() && !hangged());

    if (correct())
    {
        printf("You won!\n");
    }
    else
    {
        printf("You loose!\n");
    }

    addword();
}