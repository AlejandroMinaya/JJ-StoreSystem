/*
*/
#include "jj_setup.h"
#include "jj_categories.h"
#include "jj_products.h"
#define NOT_OPTION_MSSG "\nERROR: Lo siento, no tenemos esa opcion.\n"
/*
*/
void load()
{
	loadProducts();
	loadCategories();
	loadLedger();
}

void salesMenu()
{
	printf("\tMENU CUENTAS\n");
	printf("\n0. Regresar");
	printf("\n1. Imprimir Cuentas");
	printf("\n2. Obtener ganancias por periodo");
	printf("\n3. Obtener costos por periodo");
	printf("\n4. Obtener balance neto por periodo");
	printf("\nSelecciona una opcion: ");
	
	struct tm start_date;
	struct tm end_date;
	
	switch(getOption())
	{
		case 0:
			return;
		case 1:
			printSales();
			break;
		case 2:
			printf("\nIngrese la fecha de inicio en formato dd/mm/yyyy: ");
			start_date = stringToDate();
			printf("Ingrese la fecha de fin en formato dd/mm/yyyy: ");
			end_date = stringToDate();
			
			printf("\nGanancias: %.2f\n", moneyInPeriod(start_date, end_date, 1));
			break;
		case 3:
			printf("\nIngrese la fecha de inicio en formato dd/mm/yyyy: ");
			start_date = stringToDate();
			printf("Ingrese la fecha de fin en formato dd/mm/yyyy: ");
			end_date = stringToDate();
			
			printf("\nCostos: %.2f\n", moneyInPeriod(start_date, end_date, 0));
		case 4:
			
			printf("\nIngrese la fecha de inicio en formato dd/mm/yyyy: ");
			start_date = stringToDate();
			printf("Ingrese la fecha de fin en formato dd/mm/yyyy: ");
			end_date = stringToDate();
			
			printf("\nBalance: %.2f\n", moneyInPeriod(start_date, end_date, 1) + moneyInPeriod(start_date, end_date, 0));
			break;
		default:
			printf("%s", NOT_OPTION_MSSG);
			break;
	}
	
}

/*
@param option. In case the function is called with a predetermined option chosen
*/
void categoriesMenu(int option)
{
	printf("\tMENU CATEGORIES\n");
	printf("\n0. Regresar");
	printf("\n1. Agregar categoria");
	printf("\n2. Eliminar categoria");
	printf("\n3. Editar categoria");
	printf("\n4. Imprimir categoria");
	printf("\nSelecciona una opcion: ");
	
	char category_name[255];
	char category_description[255];
	int edit_category_id = 0;
	
	if(option == -1)
	{
		option = getOption();
	}
	switch(option)
	{
		case 0:
			return;
		case 1:
			printf("\nIngrese el nombre de la nueva categoria: ");
			fgets(category_name, 255, stdin);
			eliminateNL(category_name); 
			printf("\nIngrese la descripcion de la nueva categoria: ");
			fgets(category_description, 255, stdin);
			eliminateNL(category_description); 
			addCategory(category_name, category_description);
			printCategories();
			break;
		case 2:
			printf("\nIngrese el ID de la categoria a eliminar: ");
			deleteCategory(getOption());
			printCategories();
			break;
		case 3:
			printf("\nIngrese el ID de la categoria a editar: ");
			edit_category_id = getOption();
			printf("\tEDICION CATEGORIA");
			printf("\n\t\t0. Cancelar");
			printf("\n\t\t1. Nombre de la categoria");
			printf("\n\t\t2. Descripcion de la categoria");
			printf("\n\n\t\tSeleccione una opcion: ");
			switch(getOption())
			{
				case 0:
					break;
				case 1:
					printf("\n\tIngrese el nuevo nombre: ");
					fgets(category_name, 255, stdin);
					eliminateNL(category_name);
					editCategory(edit_category_id, category_name, "\0");
					break;
				case 2:
					printf("\n\tIngrese la nueva descripcion: ");
					fgets(category_description, 255, stdin);
					eliminateNL(category_description);
					editCategory(edit_category_id, "\0", category_description);
					break;
				default:
					printf("%s", NOT_OPTION_MSSG);
					break;
					
			}
			printCategories();
			break;
		case 4:
			printCategories();
			break;
		default:
			printf("%s", NOT_OPTION_MSSG);
			break;
	}
	
}

/*
@param option. In case the function is called with a predetermined option chosen
*/
void productsMenu(int option)
{
	long barcode = 0;
	char name[255];
	float price = 0.0;
	int quantity = 0;
	int category = 0;
	
	printf("\tMENU PRODUCTS\n");
	printf("\n0. Regresar");
	printf("\n1. Imprimir productos");
	printf("\n2. Agregar producto");
	printf("\n3. Eliminar producto");
	printf("\n4. Ordenar producto");
	printf("\n5. Editar producto");
	printf("\nSelecciona una opcion: ");

	if(option == -1)
	{
		option = getOption();
	}
	
	switch(option){
		case 0:
			return;
		case 1:
			printProducts();
			break;
		case 2:
			printf("\nIntroduce el codigo de barras: ");
			scanf("%li%*c", &barcode);
			printf("\nIntroduce el nombre: ");
			fgets(name, 255, stdin);
			eliminateNL(name);
			printf("\nIntroduce el precio: ");
			scanf("%f%*c", &price);
			printf("\nIntroduce la cantidad: ");
			scanf("%i%*c", &quantity);
			printCategories();
			printf("\nIntroduce la categoria: ");
			scanf("%i%*c", &category);
			category = addProduct(barcode, name, price, quantity, category); 
			if(category)
			{
				
				printf("\nEsta categoria no existe. Creela a continuacion:\n");
				categoriesMenu(1);
				addProduct(barcode, name, price, quantity, category);
			}
			printProducts();
			break;
		case 3:
			printf("\nIntroduce el codigo de barras: ");
			scanf("%li%*c", &barcode);
			deleteProduct(barcode);
			printProducts();
			break;
		case 4:
			printf("\nIntroduce el codigo de barras: ");
			scanf("%li%*c", &barcode);
			printf("\nIntroduce el precio: ");
			scanf("%f%*c", &price);
			printf("\nIntroduce la cantidad: ");
			scanf("%i%*c", &quantity);
			if(orderProduct(barcode, price, quantity))
			{
				printf("\nEste producto no existe. Creelo a continuacion:\n");
				productsMenu(2);
			}
			printProducts();
			break;
		case 5: //EDIT PRODUCTS
			printf("Ingrese el codigo de barra del producto que quiere editar: ");
			scanf("%li%*c", &barcode);
			
			if(findProduct(barcode).barcode == END_PRODUCT.barcode)
			{
				printf("Producto inexistente.\n");
				break;
			}
			
			printf("\tEDICION PRODUCTO");
			printf("\n\t0. Cancelar");
			printf("\n\t1. Nombre");
			printf("\n\t2. Precio");
			printf("\n\t3. Cantidad");
			printf("\n\t4. Categorias");
			printf("\n\n\tSelecciona una opcion: ");

			switch(getOption())
			{
				case 0:
					return;
				case 1:
					printf("\nIntroduce el nombre: ");
					fgets(name, 255, stdin);
					eliminateNL(name);
					price = -1;
					quantity = -1;
					category = -1;
					break;
				case 2:
					printf("\nIntroduce el precio: ");
					scanf("%f%*c", &price);
					name[0] = '\0';
					quantity = -1;
					category = -1;
					break;
				case 3:
					printf("\nIntroduce la cantidad: ");
					scanf("%i%*c", &quantity);
					name[0] = '\0';
					price = -1;
					category = -1;
					break;
				case 4:
					printCategories();
					printf("\nIntroduce la categoria: ");
					scanf("%i%*c", &category);
					name[0] = '\0';
					price = -1;
					quantity = -1;
					break;
				default:
					printf("%s", NOT_OPTION_MSSG);
					break;
				
			}
			
			editProduct(barcode, name, price, quantity, category);
			printProducts();
			break;
		default:
			printf("%s", NOT_OPTION_MSSG);
			break;
	}
}

void inventoryMenu()
{
	printf("\tMENU INVENTARIO\n");
	printf("\n0. Regresar");
	printf("\n1. Menu de Productos");
	printf("\n2. Menu de Categorias");
	printf("\nSelecciona una opcion: ");
	
	switch(getOption()){
		case 0:
			return;
		case 1:
			productsMenu(-1);
			break;
		case 2:
			categoriesMenu(-1);
			break;
		default:
			printf("%s", NOT_OPTION_MSSG);
			break;
	}
}

/*
MAIN MENU
*/
int mainMenu()
{
	printf("\n¡Bienvenido!\n");
	printf("\n0. Salir");
	printf("\n1. Comprar un producto");
	printf("\n2. Revisar inventario");
	printf("\n3. Hacer cuentas");
	printf("\nSelecciona una opcion: ");
	
	long barcode = 0;
	int quantity = 0;
	
	switch(getOption()){
		case 0:
			printf("\n¡Gracias por visitarnos!\n");
			return 0;
		case 1:
			printf("Introduce el codigo de barras: ");
			scanf("%li%*c", &barcode);
			printf("Introduce la cantidad: ");
			scanf("%i%*c", &quantity);
			
			purchaseProduct(barcode, quantity);
			break;
		case 2:
			inventoryMenu();
			break;
		case 3:
			salesMenu();
			break;
		default:
			printf("%s", NOT_OPTION_MSSG);
			break;
	}
	return 1;
}

void start()
{
	verifyInstallation();
	printf("\n\t\t'Sistema de gestion de tiendas JJ'\n");
	do
	{
		load();
	}while(mainMenu());
}
