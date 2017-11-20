/*
*/
#include "jj_setup.h"
#include "jj_categories.h"
#include "jj_products.h"

/*
*/
void load()
{
	loadProducts();
	loadCategories();
	loadSales();
}

void salesMenu()
{

}

void inventoryMenu()
{

}

void purchaseMenu()
{

}

void mainMenu()
{

}

void start()
{
	verifyInstallation();
	load();

}
