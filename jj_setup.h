/*
TITLE: JJ Store Management System Setup
VERSION: 0.1 (alpha)
This library contains the main functions to setup the environment to run properly 
the JJ store management system
*/
#include <string.h> //We import this library to use strcmp and strcpy.
#include <math.h> //We import this library to use pow and log.
#include <time.h> //We import this library for the timelogs
#define HEADING_SIZE 1000 //This is the longest the heading may be.
#define TAB_SIZE 8 //How many spaces represents a tab.
#define MAX_COLUMN_WIDTH 36 //The maximum width of a column.
#define MAX_SALES 1000 //This is the maximum amount of sales the program can work with at once.
#define MAX_PRODUCTS 1000 //Maximum amount of products the program can work with
#define BARCODE_LENGTH 13//The maximum length for barcodes
#define MAX_CATEGORIES 100 //This is the maximum amount of categories the program can work with at once.

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
    files[0] = fopen("products.csv", "r");
    files[1] = fopen("categories.csv", "r");
    files[2] = fopen("sales.csv", "r");
    
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
    fputs("Tiempo Compra,Flujo,Producto,Cantidad,Pedido", archive);
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
    int tabs_to_add = 0; 
    if((MAX_COLUMN_WIDTH - strlen(string_to_print)) % TAB_SIZE == 0)
    {
        tabs_to_add = (MAX_COLUMN_WIDTH - strlen(string_to_print) - TAB_SIZE) / TAB_SIZE; 
    }
	else
	{
		tabs_to_add = (MAX_COLUMN_WIDTH - strlen(string_to_print)) / TAB_SIZE; //We convert the character length to tabs
	}
	
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

/*
PRINT INTEGER IN FORMAT -
This procedure takes an integer and displays it in the correct table format
@param int_to_print, which is the integer we are going to print
@param separator, which is the character to separate our columns
*/
void printInt(int int_to_print, char separator[])
{

	int tabs_to_add = 0;
	if((MAX_COLUMN_WIDTH - (int)ceil(log10(int_to_print))) % TAB_SIZE == 0)
	{
		tabs_to_add = (MAX_COLUMN_WIDTH - (int)ceil(log10(int_to_print)) - TAB_SIZE )/ TAB_SIZE;
	}
	else
	{
		tabs_to_add = (MAX_COLUMN_WIDTH - (int)ceil(log10(int_to_print))) / TAB_SIZE;
	}
	if(tabs_to_add < 1 && int_to_print > 0)
	{
		printf(" %i", int_to_print / (int)pow(10.0, (abs(tabs_to_add) + 3.0)));
		printf("...");
	}
	else
	{
		if(int_to_print == 0)
		{
			tabs_to_add = MAX_COLUMN_WIDTH / TAB_SIZE;
		}
		printf(" %i", int_to_print);
		for(int j = 0; j < tabs_to_add; j++)
		{
			printf("\t");
		}
	}
	printf("%s",separator);
         
}


/*
PRINT FLOAT IN FORMAT -
This is procedure takes a float value and displays it in the correct table format
@param float_to_print, which is the integer we are goint to print
@param separator, which is the character to separate our columns
*/
void printFloat(float float_to_print, char separator[])
{
	
	int tabs_to_add = 0;
	if((MAX_COLUMN_WIDTH - (int)log10(round(float_to_print)) - 6) / TAB_SIZE == 0)
	{
		tabs_to_add = (MAX_COLUMN_WIDTH - (int)log10(round(float_to_print)) - TAB_SIZE - 6); 
	}
	else
	{
		tabs_to_add = (MAX_COLUMN_WIDTH - (int)ceil(log10(float_to_print)) - TAB_SIZE )/ TAB_SIZE;
	}
	if(tabs_to_add < 1 && float_to_print > 0)
	{
		printf(" %.2f", float_to_print / (int)pow(10.0, (abs(tabs_to_add) + 3.0)));
		printf("...");
	}
	else
	{
		if(float_to_print == 0.0)
		{
			tabs_to_add = MAX_COLUMN_WIDTH / TAB_SIZE;
		}
		printf(" %.2f", float_to_print);
		for(int j = 0; j < tabs_to_add; j++)
		{
			printf("\t");
		}
	}
	printf("%s",separator);
}

/*
STRING TO DATE -
Waits for user input of a date in the format dd/mm/yyyy
@return tm structure
*/
struct tm stringToDate()
{
	int day = -1, month = -1, year = -1;
	time_t _time = time(NULL);
	struct tm current_time;
	current_time = *localtime(&_time);
	
	scanf("%i/%i/%i", &day, &month, &year);
	/*We substract 1900 in terms with the specifications of tm_year in
	  reference*/
	if(day < 0 || month < 0 || year < 0 || day > 31 || month > 12 || year - 1900 > current_time.tm_year)
	{
	    return current_time;
	}
	struct tm date = {.tm_mday = day, .tm_mon = month, .tm_year = year - 1900, .tm_wday = 0, .tm_hour = 0, .tm_isdst=0, .tm_sec = 0, .tm_min = 0, .tm_yday = 0};
	return date;
}

/*
GET OPTION -
Waits for user input to get the option from the menu
@return the user option
*/
int getOption()
{
    int option = 0;
    scanf("%i%*c", &option);
    return option;
}

/*
ELIMINATE FGETS NEW LINE -
Converts the automatic line-break produced by fgets into a null character
@param the string to fix
*/
void eliminateNL(char string_to_fix[])
{
    string_to_fix[strlen(string_to_fix) - 1] = '\0';
}

//SALES//

/*
SALE DATA TYPE -
*/
struct sale
{
    struct tm timestamp; //Time of sale.
    float amount; //Amount of money transacted
    long product; //Barcode of the product sold/ordered.
    int quantity; //Amount of product sold/ordered.
    int is_order; //Boolean to detemine if sale or order.
};

/*
SALE END (NULL VALUE) -
A null value for our structure.
*/
struct sale END_SALE = {.is_order = -1};


/*
LEDGER (IN FILE SALES) -
An array containing all sales as structures.
*/
struct sale ledger[MAX_SALES];


//PRODUCTS//

/*
PRODUCT DATA TYPE -
*/
struct product
{
    char name[255]; //The name of the product.
    int enabled; //If it is available.
    long barcode; //Unique identifier of the product.
    int category; //To which category the product belongs.
    int quantity; //How much the product costs.
    float price; //The price of the product.
};

/*
PRODUCT END -
A null product
*/
struct product END_PRODUCT = {.barcode = -1};

/*
IN-FILE PRODUCTS ARRAY
*/
struct product in_file_products [MAX_PRODUCTS];


//SALES//

/*
CATEGORY DATA TYPE -
*/
struct category
{
    int ID; //Unique identifier for the category.
    char name[255]; //The category name.
    char description[255]; //The category description.
    int enabled; //If the category is currently active.
};

/*
END VALUE - 
Null equivalent for our category struct
*/
struct category END_CATEGORY = {.ID = -1};

/*
IN-FILE CATEGORIES ARRAY -
*/
struct category in_file_categories [MAX_CATEGORIES];

