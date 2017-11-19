/*
TITLE: JJ Store Management System Sales
VERSION: 0.1 (alpha)
This library contains the main functions to manage products categories.
*/

/*
FIND PRODUCT -
This function looks through in_file_products and returns a given product
@param Receives the product barcode
@return Returns the product structure to the corresponding barcode
*/
struct product findProduct(long product_barcode)
{
    for(int i = 0; in_file_products[i].barcode != END_PRODUCT.barcode; i++)
    {
        if(in_file_products[i].barcode == product_barcode)
        {
            return in_file_products[i];
        }
    }
    return END_PRODUCT;
}

/*
LOAD SALES TO LEDGER -
Procedure to read files and add to the array
*/
void loadLedger(void)
{
    FILE * sales = fopen("assets/sales.csv","r"); //We open the file for reading
    char heading[HEADING_SIZE];
    fgets(heading, HEADING_SIZE, sales); //We read the heading to move the pointer forward
    int i = 0;
    for (; i < MAX_SALES; i++) 
    {
        struct sale tmp_sale;
        if(feof(sales)){break;}
        fscanf(sales, "%i %i %i %i %i %i %i %i %i, %f, %li, %i, %i", &tmp_sale.timestamp.tm_sec, &tmp_sale.timestamp.tm_min, &tmp_sale.timestamp.tm_hour, &tmp_sale.timestamp.tm_mday, &tmp_sale.timestamp.tm_mon, &tmp_sale.timestamp.tm_year, &tmp_sale.timestamp.tm_wday, &tmp_sale.timestamp.tm_yday, &tmp_sale.timestamp.tm_isdst, &tmp_sale.amount, &tmp_sale.product, &tmp_sale.quantity, &tmp_sale.is_order);
        ledger[i] = tmp_sale;
        if(feof(sales)){break;}
    }
    fclose(sales);
    for(;i < MAX_SALES; i++)
    {
        ledger[i] = END_SALE;
    }
}

/*
SAVE LEDGER TO SALES -
Procedure to save sales to files
*/
void saveLedger(void)
{
    FILE * sales = fopen("assets/sales.csv","r"); //We open the file for reading to get the heading
    char heading[HEADING_SIZE];
    fgets(heading, HEADING_SIZE, sales); //We read the heading and store it in the char array heading
    freopen("assets/sales.csv", "w", sales); //We reopen the file for writing to save the sales
    fputs(heading,sales);
    for(int i = 0; ledger[i].is_order != END_SALE.is_order; i++)
    {
        fprintf(sales, "%i %i %i %i %i %i %i %i %i, %.2f, %li, %i, %i\n", ledger[i].timestamp.tm_sec, ledger[i].timestamp.tm_min, ledger[i].timestamp.tm_hour, ledger[i].timestamp.tm_mday, ledger[i].timestamp.tm_mon, ledger[i].timestamp.tm_year, ledger[i].timestamp.tm_wday, ledger[i].timestamp.tm_yday, ledger[i].timestamp.tm_isdst, ledger[i].amount, ledger[i].product, ledger[i].quantity, ledger[i].is_order);
    }
    fclose(sales);
}

/*
ADD SALE TO LEDGER -
This function adds a sale to our ledger including
@param amount, the amount of money transacted
@param product_barcode, the barcode of the product bought or sold
@param quantity, the amount bought or sold
@param is_order, Boolean to determine if sale or order
*/
void addSale(float amount, long product_barcode, int quantity, int is_order)
{
    int i = 0;
    for(;ledger[i].is_order != END_SALE.is_order; i++);
    time_t current_time = time(NULL);
    ledger[i].timestamp = *localtime(&current_time);
    ledger[i].amount = amount;
    ledger[i].product = product_barcode;
    ledger[i].quantity = quantity;
    ledger[i].is_order = is_order;
    saveLedger();
}

/*
EARNED/SPENT IN PERIOD -
This function returns the total amount of money earned/spent in a period of time
@param Receives a tm structure containing the start_date
@param Receives a tm structure containing the end_date
@return Total amount of money 
*/
float moneyInPeriod(struct tm start_date, struct tm end_date, int earnings)
{
    time_t start = mktime(&start_date);
    time_t end = mktime(&end_date);
    time_t current_time;
    float total_money = 0.0;
    for(int i = 0; ledger[i].is_order != ledger[i].is_order; i++)
    {
        current_time = mktime(&ledger[i].timestamp);
        if(current_time >= start && current_time <= end)
        {
            if(earnings)
            {
                total_money += ledger[i].amount;
            }
            else
            {
                total_money -= ledger[i].amount;
            }
        }
        
    }
	return total_money;
}

/*
PRINT SALES -
Procedure to print all sale information
*/
void printSales(void)
{
    printHeading("assets/sales.csv");
   	
	char timestamp[255]; //Here we will store the result of asctime()
	for(int i = 0; ledger[i].is_order != END_SALE.is_order; i++)
	{
		//Print the time
		strcpy(timestamp, asctime(&ledger[i].timestamp));
		//We eliminate the last character which is a line break
		timestamp[strlen(timestamp) - 1] = '\0';
		printString(timestamp, " ||");

		//Print the amount
		if(ledger[i].is_order)
		{
			printf("%s",' -');
			printFloat(ledger[i].amount, " ||");
		}
		else
		{
			printFloat(ledger[i].amount, " ||");
		}

		//Print the product name
		struct product found_product = findProduct(ledger[i].product);
		printString(found_product.name, " ||");

		//Print the quantity
		printInt(ledger[i].quantity, " ||");

		//Print if it is order
		if(ledger[i].is_order)
		{
			printString("Si", "\n");
		}
		else
		{
			printString("No", "\n");
		}

	}

    
}

/*
    RECUERDA CUANDO HAGAMOS PRINT SALES CONSIDERAR SI ES ORDEN PARA IMPRIMIR EL
    VALOR NEGATIVO.
    
    CAMBIAR LA CONDICION DEL FOR PARA PRINT CATEGORY A DESIGUALDAD ENTRE CURRENT Y EL NULL
*/
