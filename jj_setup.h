/*
TITLE: JJ Store Management System Setup
VERSION: 0.1 (alpha)
This library contains the main functions to setup the environment to run properly 
the JJ store management system
*/
#include <string.h> //We import this library to use strcmp and strcpy.
#include <math.h> //We import this library to use pow and log.

#define HEADING_SIZE 1000 //This is the longest the heading may be.
#define TAB_SIZE 8 //How many spaces represents a tab.
#define MAX_COLUMN_WIDTH 36 //The maximum width of a column.

/*
FIND MAIN FILES -
The following function looks tries to access the files where the products, categories 
and sales will be stored.
@return 1, if the files were found.
@return 0, if the files were not found.
*/
int findMainFiles(void)
{
    //Declaring files array
    FILE * files[3]; //There are three main files: products.csv, category.csv, and sales.csv
    files[0] = fopen("assets/products.csv", "r");
    files[1] = fopen("assets/categories.csv", "r");
    files[2] = fopen("assets/sales.csv", "r");
    
    //We cycle through the file streams to determine if any of them are NULL.
    for(int i = 0; i < 3; i++)
    {
        if(files[i] == NULL)
        {
            return 0; //The function exits telling us the system was not set up.
        }
        fclose(files[i]); //If the file exists, close the stream.
    }
    return 1; //The function exits telling us the system was set up correctly.
}

/*
CREATE MAIN FILES -
The following procedure, if not found, creates the files.
*/
void createMainFiles(void)
{
    FILE * archive;
    
    archive = fopen("categories.csv", "w");
    fputs("ID Nombre,Nombre,Descripcion", archive);
    fputs("\n",archive);
    fclose(archive);
    
    archive = fopen("products.csv", "w");
    fputs("Codigo,Nombre,Precio,Cantidad,Categoria", archive);
    fputs("\n",archive);
    fclose(archive);
    
    archive = fopen("sales.csv", "w");
    fputs("Tiempo,Flujo,Producto,Cantidad,Pedido", archive);
    fputs("\n",archive);
    fclose(archive);
}

/*
VERIFY INSTALLATION -
The following procedure makes sure that the system was correctly installed, if
not, allows the user to fix it.
*/
void verifyInstallation(void)
{
    //Checks for system files
    if (!findMainFiles())
    {
        printf("You are missing main files.\n");
        //We ask the user if it wants to create the files
        printf("Would you like to create these files (Y/y/N/n)? ");
        char user_answer;
        scanf("%c%*c", &user_answer);
        switch(user_answer)
        {
            case 'Y':
            case 'y':
                createMainFiles();
                break;
            case 'n':
            case 'N':
                printf("Couldn't complete setup process.\n");
                return;
        }
    }
}

/*
PRINT HEADING -
Procedure for the printing procedures
@param the path to the file where the heading is stored
*/
void printHeading(char file_path[])
{
    FILE * open_file = fopen(file_path,"r"); //We open the file in reading mode
    char heading[HEADING_SIZE]; //We create the array where we will store the heading
    fgets(heading, HEADING_SIZE, open_file); //We read the first line, which will be the heading
    printf(" "); //Format space for the first heading title
    for(int i = 0; heading[i] != '\n'; i++) //For to process each character of our heading
    {
        if(heading[i] == ',') //If the current character is a comma...
        {
            //...print tabs to occupy the width
            for(int j = 0; j < MAX_COLUMN_WIDTH / TAB_SIZE - 1; j++)
            {
                printf("\t");
            }
            printf(" || "); //Print column separator
            continue;
        }
        printf("%c",heading[i]);
    }
    printf("\n");
    fclose(open_file); //We close the file after reading the heading
}

/*
PRINT STRING WITH FORMAT -
Procedure that receives a string and prints it with the correct format
@param the string to be print
 */
void printString(char string_to_print[], char separator[])
{
	//We print the name
    int tabs_to_add = (MAX_COLUMN_WIDTH - strlen(string_to_print)) / TAB_SIZE; //We convert the character length to tabs
    if(tabs_to_add < 1) //If there are no tabs to add, meaning the name length exceeds the column width...
    {
        printf(" "); //...we print the initial space and...
        //...we print all letters up to certain length
        for(int j = 0; j < MAX_COLUMN_WIDTH - TAB_SIZE - 3; j++)
        {
            printf("%c", string_to_print[j]);
        }
        printf("..."); //We print the final three points
    }
    else //...otherwise we print the name and add the tabs
    {
        printf(" %s", string_to_print);
        for(int j = 0; j < tabs_to_add; j++)
        {
            printf("\t");
        }
    }
    printf("%s",separator); //We print the column separator

}
