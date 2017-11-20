/*
TITLE: JJ Store Management System Categories
VERSION: 0.1 (alpha)
This library contains the main functions to manage products categories.
*/

/*
LOAD CATEGORIES -
This procedure reads the categories previously saved on file.
*/
void loadCategories(void)
{
    FILE * categories = fopen("categories.csv", "r");
    char current_file_line[HEADING_SIZE];
    fgets(current_file_line, HEADING_SIZE, categories); // This line reads the heading to move the pointer and avoid runtime error.
    int i = 0;
    for(;i < MAX_CATEGORIES;i++) //for categories in the saved file or as long as it doesn't exceed MAX_CATEGORIES
    {
        struct category tmp_category; //Temporary category to store in in_file_categories
        if(feof(categories)){break;} //We check before moving any further if we are at the end of the file
         //We read the comma separated values and store them accordingly.
        fscanf(categories,"%i,%[^,],%[^,],%i", &tmp_category.ID, tmp_category.name, tmp_category.description, &tmp_category.enabled); //Read the comma separated values
        in_file_categories[i] = tmp_category;
         if(feof(categories)){break;} //We check before moving any further if we are at the end of the file
        
    }
    for(;i < MAX_CATEGORIES; i++)
    {
        in_file_categories[i] = END_CATEGORY; //We fill the remaining spaces with null categories
    }
    fclose(categories);
}

/*
SAVE CATEGORIES -
This procedure allows the user to add the in program changes to the file database.
*/
void saveCategories(void)
{
    char heading[HEADING_SIZE];
    FILE * categories = fopen("categories.csv", "r"); // We open the file in reading mode...
    fgets(heading,HEADING_SIZE,categories); //...and store the heading to include it once we save
    freopen("categories.csv", "w", categories);
    fprintf(categories, "%s",heading); //We print the heading since we are overwriting.
    for(int i = 0; in_file_categories[i].ID != END_CATEGORY.ID; i++) //While the name of the category is not null;
    {
        fprintf(categories, "%i,%s,%s,%i\n", in_file_categories[i].ID, in_file_categories[i].name, in_file_categories[i].description, in_file_categories[i].enabled); //We add a new entry
    }
    fclose(categories);
}

/*
ADD CATEGORIES -
*/
void addCategory(char nombre[255], char description[255])
{
    int new_entry_id = 0;
    for(;in_file_categories[new_entry_id].ID != END_CATEGORY.ID; new_entry_id++);
    in_file_categories[new_entry_id].ID = new_entry_id;
    strcpy(in_file_categories[new_entry_id].name, nombre);
    strcpy(in_file_categories[new_entry_id].description, description);
    in_file_categories[new_entry_id].enabled = 1;
    
    saveCategories();
}

/*
DELETE CATEGORIES -
This procedure deletes an exisiting category after being loaded
*/
void deleteCategory(int category_id)
{
    for(int i = 0; in_file_categories[i].ID != END_CATEGORY.ID; i++)
    {
        if (in_file_categories[i].ID == category_id)
        {
            in_file_categories[i].enabled = 0;
            saveCategories();
            return;
        } 
    }
    printf("\nCATEGORY NOT FOUND!\n");
}

/*
PRINT CATEGORIES -
This procedure prints all categories present in the file
*/
void printCategories(void)
{
    printHeading("categories.csv");
    
    //We print in file categories
    for(int i = 0; i < MAX_CATEGORIES; i++)
    {
        if(in_file_categories[i].ID != END_CATEGORY.ID  && in_file_categories[i].enabled) //If the first character of the name is not null and the category is enabled.
        {
            int tabs_to_add = 0;//This will determine the amount of tabs we will print depending on its length
          
			//We print the category ID	
			printInt(in_file_categories[i].ID, " ||"); 
			//We print the category name
			printString(in_file_categories[i].name, " ||");
			//We print the category description	
			printString(in_file_categories[i].description, "\n");
        }
    }
}


/*
FIND CATEGORY -
@param Receives the id of the category we wish to find
@return Returns the category structure for the corresponding category.
*/
struct category findCategory(int category_id)
{
    for(int i = 0; in_file_categories[i].ID != END_CATEGORY.ID; i++)
    {
        if(in_file_categories[i].ID == category_id)
        {
            return in_file_categories[i];
        }
    }
    return END_CATEGORY;
}


/*
EDIT CATEGORY -
This procedure edits an already existing category
@param the category id to identify the object
@param the category name
@param the category description
*/
void editCategory(int category_id, char category_name[], char category_description[])
{
	for(int i = 0; in_file_categories[i].ID != END_CATEGORY.ID; i++)
	{	
		if(in_file_categories[i].ID == category_id)
		{
			if(category_name[0] != '\0')
			{
				strcpy(in_file_categories[i].name, category_name);
			}

			if(category_description[0] != '\0')
			{
				strcpy(in_file_categories[i].description, category_description);
			}
			saveCategories();
			return;
		}
	}
	printf("\nERROR: Aun no se crea esta categoria\n");
}
