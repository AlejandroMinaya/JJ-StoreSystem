/*
TITLE: JJ Store Management System Setup
VERSION: 0.1 (alpha)
This library contains the main functions to manage products categories.
*/
#include <string.h>
#define MAX_CATEGORIES 100 //This is the maximum amount of categories the program can work with at once.
#define HEADING_SIZE 1000 //This is the longest the heading may be.
#define TAB_SIZE 8 //HOW MANY SPACES REPRESENTS A TAB
/*
CATEGORY DATA TYPE -
We define the category data type
*/
struct category
{
    int ID; //Unique identifier for the category.
    char name[255]; //The category name.
    char description[255]; //The category description.
    int enabled; //If the category is currently active.
};

/*
IN-FILE CATEGORIES ARRAY -
*/
struct category in_file_categories [MAX_CATEGORIES];

/*
LOAD CATEGORIES -
This procedure reads the categories previously saved on file.
*/
void loadCategories()
{
    FILE * categories = fopen("assets/categories.csv", "r+");
    char current_file_line[HEADING_SIZE];
    fgets(current_file_line, HEADING_SIZE, categories); // This line reads the heading to move the pointer and avoid runtime error.
    for(int i = 0; !feof(categories) && i < MAX_CATEGORIES; i++) //For categories in the saved file or as long as it doesn't exceed MAX_CATEGORIES
    {
        struct category tmp_category; //Temporary category to store in in_file_categories
        fscanf(categories,"%i,%[^,],%[^\n]", &tmp_category.ID, tmp_category.name, tmp_category.description); //Read the comma separated values;
        in_file_categories[i] = tmp_category;
        
    }
    fclose(categories);
}

/*
SAVE CATEGORIES -
*/

/*
PRINT CATEGORIES -
This procedure prints all categories present in the file
*/
void printCategories(void)
{
    //We open the file to print the heading
    FILE * categories = fopen("assets/categories.csv","r");
    char heading[HEADING_SIZE];
    fgets(heading,HEADING_SIZE,categories);
    for(int i = 0; heading[i] != '\n'; i++)
    {
        if(heading[i] == ',')
        {
            printf("\t\t\t\t || ");
            continue;
        }
        printf("%c",heading[i]);
    }
    putc('\n', stdout);
    fclose(categories);
    
    //We print the in file categories
    for(int i = 0; i < MAX_CATEGORIES; i++)
    {
        if(in_file_categories[i].name[0] != '\0') //If the first character of the name is not null
        {
            printf("%i", in_file_categories[i].ID);
            if(in_file_categories[i].ID >= 10000000) //If the number is greater than or equal to 1000000 print in the following format...
            { 
                printf("\t\t\t ||");
            }
            else
            {
                printf("\t\t\t\t ||");
            }
            
            
            int tab_equivalence = strlen(in_file_categories[i].name) / TAB_SIZE; //This will determine the amount of tabs we will print depending on its length
            if(tab_equivalence >= 4)
            {
                for(int j = 0; j < 32; j++)
                {
                    if(i <= 28) // We print the first 24 characters of the string
                    {
                        putc(in_file_categories[i].name[j]);
                    }
                    else // We fill the remainder with dots
                    {
                        putc('.');
                    }
                    
                }
            }
            else
            {
               printf(" %s", in_file_categories[i].name); // We print the name of the category
                for(int j = 0; j < 4-tab_equivalence; j++)
                {
                    putc('\t',stdout);
                }
            }
            printf(" ||"); 
            
            
            printf(" %s", in_file_categories[i].description); //We print the description.
            tab_equivalence = strlen(in_file_categories[i].description) / TAB_SIZE; //This will determine the amount of tabs we will print depending on its length
            for(int j = 0; j < 4-tab_equivalence; j++)
            {
                putc('\t',stdout);
            }
            printf("\n");
        }
    }
    
}

