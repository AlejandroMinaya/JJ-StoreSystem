#include <stdlib.h>
#include <stdio.h>
#include "jj_setup.h"
#include "jj_categories.h"
#include "jj_products.h"

int main() {
    loadCategories();
    deleteCategory(7);
    addCategory("Bebes", "Hermosos");
    loadProducts();
    loadLedger();
   // purchaseProduct(7501055307906, 2);
    purchaseProduct(7501055354306, 1);
    printProducts();
    printf("\n\n\n");
    printCategories();
    orderProduct(7501055354306, 8.00, 100);
    orderProduct(7501055307906, 5.00, 1);
    printf("\n\n\n");
	printSales();
	editCategory(8,"Refrescos","\0");
	editProduct(7501055354306, "Xaviercito", 100.0, -1, -1);
	printCategories();
	printf("\n\n");
	printProducts();
	printf("\n\n");
	printSales();
	printf("\n\n");
	//printCategories();
    return 0;

}
