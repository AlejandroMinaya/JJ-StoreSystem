/*
TITLE: JJ Store Management System Categories
VERSION: 0.1 (alpha)
This library contains the main functions to manage products categories.
*/

#include "jj_sales.h"

/*
LOAD PRODUCTS -
Interprets csv values and turns them into products (structures).
*/
void loadProducts(void)
{
    FILE * products = fopen("assets/products.csv","r"); //We open the products.csv in read mode
    char current_file_line[HEADING_SIZE];
    fgets(current_file_line, HEADING_SIZE, products); //We read the heading to move the pointer
    int i = 0;
    for(;i < MAX_PRODUCTS;i++)
    {
        struct product tmp_product;
        if(feof(products)){break;} //If the file pointer reaches the end break the loop
        //We read the comma separated values and store them accordingly.
        fscanf(products,"%li,%[^,],%f,%i,%i,%i",&tmp_product.barcode,tmp_product.name,&tmp_product.price,&tmp_product.quantity,&tmp_product.category,&tmp_product.enabled);
        in_file_products[i] = tmp_product;
        if(feof(products)){break;} //We check before moving any further if we are at the end of the file
    }
    for(;i < MAX_PRODUCTS;i++)
    {
        in_file_products[i] = END_PRODUCT; //We fill the remaining spaces with null products
    }
    fclose(products);
}

/*
SAVE PRODUCTS -
Interprets product structs and parses them into CSV values
*/
void saveProducts(void)
{
    char heading[HEADING_SIZE]; 
    FILE * products = fopen("assets/products.csv", "r"); //We open products.csv in reading mode
    fgets(heading, HEADING_SIZE, products); //We save the heading to add it once we write
    freopen("assets/products.csv","w",products); //We reopen the file for writing
    fprintf(products,"%s",heading); //We write our heading in file
    for(int i = 0; in_file_products[i].barcode != END_PRODUCT.barcode; i++)
    {
            fprintf(products,"%li,%s,%.2f,%i,%i,%i\n", in_file_products[i].barcode, in_file_products[i].name, in_file_products[i].price, in_file_products[i].quantity, in_file_products[i].category, in_file_products[i].enabled); //We write a product to file
    }
    fclose(products);
}

/*
PRINT PRODUCT -
*/
void printProducts(void)
{
    printHeading("assets/products.csv");
    
    for(int i = 0; in_file_products[i].barcode != END_PRODUCT.barcode; i++) //While it doesn't exceed our limit
    {
        
        if(in_file_products[i].enabled) //If the product is not NULL and is enabled
        {
            //We print the barcode
            int tabs_to_add = (MAX_COLUMN_WIDTH - BARCODE_LENGTH) / TAB_SIZE; //We convert the character length to tabs
            printf(" %li", in_file_products[i].barcode); //We print the barcode of the current product
            for(int j = 0; j < tabs_to_add; j++)
            {
                printf("\t");
            }
            printf(" ||"); //We print the column separator
            
		    //We print the product name
			printString(in_file_products[i].name, " ||");

            //We print the price
            // We get the length of the integer part of the price and add three (the decimal point and two decimals), and give it a tab equivalent
			////tabs_to_add = (MAX_COLUMN_WIDTH - (int)log10(round(in_file_products[i].price)) - 6) / TAB_SIZE;
			////if(tabs_to_add < 1) //If there aren't any tabs to add...
			////{
			////	printf(" %.2f", in_file_products[i].price / pow(10.0, (abs(tabs_to_add) + 3.0)));
			////	printf("...");
			////}
			////else //...otherwise we simply print the number
			////{
			////	printf(" %.2f", in_file_products[i].price);
			////	for(int j = 0; j < tabs_to_add; j++)
			////	{
			////		printf("\t");
			////	}
			////}
			////printf(" ||");
          
			//We print the price
		   	printFloat(in_file_products[i].price, " ||");	
			
			//We print the quantity
		   	printInt(in_file_products[i].quantity, " ||");	
                        
            //We print the category
            char category_name[255];
            struct category product_category = findCategory(in_file_products[i].category);
            if(product_category.ID != END_CATEGORY.ID)
            {
                strcpy(category_name, findCategory(in_file_products[i].category).name); //We retrieve the category with its id and copy its name to our local varaiable
            }
            else
            {
                strcpy(category_name, "N/a");
            }
            printString(category_name, "\n");
        }
    }
}

/*
PURCHASE PRODUCT -
*/
void purchaseProduct(long barcode, int quantity)
{
    for(int i = 0; in_file_products[i].barcode != END_PRODUCT.barcode; i++) 
    { 
        if(in_file_products[i].barcode == barcode)
        {
            if(in_file_products[i].quantity >= quantity && in_file_products[i].enabled)
            {
                in_file_products[i].quantity -= quantity;
                addSale(in_file_products[i].price * quantity, in_file_products[i].barcode, quantity, 0); //We add record of the sale to the sales.csv
            } 
            else
            {
                printf("Producto no disponible.\n");
            }
            saveProducts();
            return;
        }
    }
    printf("Producto no encontrado.\n");
}

/*
ADD PRODUCT -
This function increases the quantity or creates products (if needed).
*/
int addProduct(long barcode, char name[255], float price, int quantity, int category) //NOTA: DARLE LA OPCION DE QUE AGREGUE UNA CATEGORIA
{
    for (int i = 0; in_file_categories[i].ID != END_CATEGORY.ID; i++)
    {
        if (category != in_file_categories[i].ID) // If the category given doesn't exist... 
        { 
           return 1; //...we create it
        }
        for (int j = 0; in_file_products[j].barcode != END_PRODUCT.barcode; j++) //Goes through all barcodes 
        {
            if (in_file_products[j].barcode != barcode) //If the barcode given doesn't exist already 
            {
                strcpy(in_file_products[j].name, name); 
                in_file_products[j].price = price;
                in_file_products[j].quantity += quantity;
                in_file_products[j].category = category;
                in_file_products[j].enabled = 1;
            } 
            else
            { //If the barcode given already exists
                in_file_products[j].quantity += quantity; //Adds the number of products to the already existing
            }
            
            if (in_file_products[j].enabled == 0) //Makes that product available if it wasn't already
            {
                    in_file_products[j].enabled = 1;
            }
        }
    }
    return 0;
}

/*
ORDER PRODUCT -
If the information this function recieves doesn't exist, it passes them to function ADD
*/
//NOTA: DEJAR UNA SECCION PARA SALES
int orderProduct(long barcode, float price, int quantity)
{
    for (int i = 0; in_file_products[i].barcode != END_PRODUCT.barcode; i++)
    {
        if(in_file_products[i].barcode == barcode)
        {
            in_file_products[i].quantity += quantity;
            if (in_file_products[i].enabled == 0)
            {
                in_file_products[i].enabled = 1;
            }
            saveProducts();
            addSale(quantity * price, barcode, quantity, 1);//ESPACIO PARA SALES
            return 0;
        }
    }
    return 1;
}

/*
DELETE PRODUCT-
This procedure states that the product exists, but it's not longer available
*/
void deleteProduct(int product_barcode)
{
    for(int i = 0; in_file_products[i].barcode != END_PRODUCT.barcode; i++)
    {
        if (in_file_products[i].barcode == product_barcode)
        {
            in_file_products[i].enabled = 0;
            saveProducts();
            return;
        }
    }
}

