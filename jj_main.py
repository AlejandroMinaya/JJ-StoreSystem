from jj_setup import *

def load():
    product_inventory.load()
    categories.load()
    sales_ledger.load()

"""
SALES MENU
This function displays the Sales' menu
"""
def salesMenu():
    print "\tMENU CUENTAS"
    print "0. Regresar"
    print "1. Imprimir cuentas"
    print "2. Obtener ganancias por periodo"
    print "3. Obtener costos por periodo"
    print "4. Obtener balance neto por periodo"
    opcion = int(raw_input("Seleccionar una opcion: "))
    
    if opcion == 0:
        mainMenu()
    elif opcion == 1:
        sales_ledger.printSales()
    elif opcion == 2:
        start_date = raw_input("Ingrese la fecha de inicio en formato dd/mm/yyyy: ")
        end_date = raw_input("Ingrese la fecha de fin en formato dd/mm/yyyy: ")
        
        print "Ganancias: %.2f" % sales_ledger.moneyInPeriod(start_date, end_date, 1) 
    elif opcion == 3:
        start_date = raw_input("Ingrese la fecha de incio en formato dd/mm/yyyy: ")
        end_date = raw_input("Ingrese la fecha de fin en formato dd/mm/yyyy: ")
        
        print "Costos: %.2f"  % sales_ledger.moneyInPeriod(start_date, end_date, 0) 
    elif opcion == 4:
        start_date = raw_input("Ingrese la fecha de inicio en formato dd/mm/yyyy: ")
        end_date = raw_input("Ingrese la fecha de fin en formato dd/mm/yyyy: ")
        
        print "Balance: %.2f" % (sales_ledger.moneyInPeriod(start_date, end_date, 0) + sales_ledger.moneyInPeriod(start_date, end_date, 1))
    else:
        print "ERROR: Lo siento, no tenemos esa opcion."

"""
CATEGORIES MENU
This procedure displays the Categories' menu
"""
def categoriesMenu(opcion=-1): #AGREGAR opcion DENTRO DE PARENTESIS$
    print "\tMENU CATEGORIAS\n"
    print "0. Regresar"
    print "1. Agregar categoria"
    print "2. Eliminar categoria"
    print "3. Editar categoria"
    print "4. Imprimir categoria"
    if opcion == -1:
        opcion = int(raw_input("Selecciona una opcion: "))
    if opcion == 0:
        inventoryMenu()
    elif opcion == 1:
        category_name = raw_input("Ingrese el nombre de la nueva categoria: ")
        category_description = raw_input("Ingrese la descripcion de la nueva categoria: ")
        tmp_category = Category(category_name, category_description,1,categories)
        categories.addCategory(tmp_category)
        categories.printCategories()
    elif opcion == 2:
        category_id = int(raw_input("Ingrese el ID de la categoria a eliminar: "))
        categories.deleteCategory(category_id)
        categories.printCategories()
    elif opcion == 3:
        edit_category_id = int(raw_input(("\nIngrese el ID de la categoria a editar: ")))
        print "\tEDICION CATEGORIA"
        print "\t\t0. Cancelar"
        print "\t\t1. Nombre de la categoria"
        print "\t\t2. Descripcion de la categoria"
        opcion = int(raw_input("\tSelecione una opcion: "))
        
        if opcion == 0:
            inventoryMenu()
        elif opcion == 1:
            category_name = raw_input("\tIngrese el nuevo nombre: ")
            categories.findCategory(edit_category_id).edit(category_name, "\0")
        elif opcion == 2:
            category_description = raw_input("\tIngrese la nueva descripcion: ")
            categories.findCategory(edit_category_id).edit("\0", category_description)
        else:
            print "ERROR: Lo siento, no tenemos esa opcion"
        categories.printCategories()
    elif opcion == 4:
        categories.printCategories()
    else:
        print "ERROR: Lo siento, no tenemos esa opcion"

"""
PRODUCTS MENU
This procedure displays the Product's menu
"""
def productsMenu(opcion=-1):
    print "\t MENU PRODUCTOS\n"
    print "0. Regresar"
    print "1. Imprimir productos"
    print "2. Agregar producto"
    print "3. Eliminar producto"
    print "4. Ordenar producto"
    print "5. Editar producto"
    if opcion == -1:
        opcion = int(raw_input("\nSelecciona una opcion: "))
    
    if opcion == 0:
        inventoryMenu()
    elif opcion == 1:
        product_inventory.printProducts()
    elif opcion == 2:
        barcode = int(raw_input("Introduce el codigo de barras: "))
        name = raw_input("Introduce el nombre: ")
        price = float(raw_input("Introduce el precio: "))
        quantity = int(raw_input("Introduce la cantidad: "))
        categories.printCategories()
        category = int(raw_input("Introduce la categoria: "))
        tmp_product = Product(barcode, name, price, quantity, category, 1, product_inventory)
        if(categories.findCategory(category) != categories.NULL):
            product_inventory.addProduct(tmp_product)
        else:
            print "Esta categoria no existe. Creela a continuacion:\n"
            categoriesMenu(1)
            product_inventory.addProduct(tmp_product)
        product_inventory.printProducts()
    
    elif opcion == 3:
        barcode = int(raw_input("Introduce el codigo de barras: "))
        product_inventory.deleteProduct(barcode)
        product_inventory.printProducts()
    
    elif opcion == 4:
        barcode = int(raw_input("Introduce el codigo de barras: "))
        price = float(raw_input("Introduce el precio: "))
        quantity = int(raw_input("Introduce el quantity: "))
        
        
        tmp_product = product_inventory.findProduct(barcode)
        if tmp_product == product_inventory.NULL:
            #NO ENCONTRO
            print "Este producto no existe. Creelo a continuacion."
            productsMenu(2)
        else:
            tmp_product.order(quantity, price)
        product_inventory.printProducts()
    elif opcion == 5:
        product_inventory.printProducts()
        barcode = int(raw_input("Ingrese el codigo de barra del producto que quiera editar: "))
        if product_inventory.findProduct(barcode) == product_inventory.NULL:
            print "Este producto no existe. Creelo a continuacion."
            productsMenu(2)
        print "\tEDICION PRODUCTO"
        print "\t0. Cancelar"
        print "\t1. Nombre"
        print "\t2. Precio"
        print "\t3. Cantidad"
        print "\t4. Categorias"
        opcion = int(raw_input("\tSelecciona una opcion: "))
        name = "\0"
        price = -1
        quantity = -1
        category = -1

        if opcion == 0:
            inventoryMenu()
        elif opcion == 1:
            name = raw_input("Introduce el nombre: ")
        elif opcion == 2:
            price = float(raw_input("Introduce el precio: "))
        elif opcion == 3:
            quantity = int(raw_input("Introduce la cantidad: "))
        elif opcion == 4:
            categories.printCategories()
            category = int(raw_input("Introduce la categoria: "))
            if categories.findCategory(category) == categories.NULL:
                print "No existe la categoria. Creela a continuacion"
                categoriesMenu(1)
        else:
            print "ERROR: Lo siento, no tenemos esa opcion."

        product_inventory.findProduct(barcode).edit(name, price, quantity, category)

"""
INVENTORY MENU
This procedure displays the Inventory's Menu
"""
def inventoryMenu():
    print "\tMENU INVENTARIO\n"
    print "0. Regresar"
    print "1. Menu de Productos"
    print "2. Menu de Categorias"
    print "Selecciona una opcion: "
    opcion = int(raw_input("> "))
    
    if opcion == 0:
        mainMenu()
    elif opcion == 1:
        productsMenu() 
    elif opcion == 2:
        categoriesMenu()
    else:
        print "ERROR: Lo siento, no tenemos esa opcion."

"""
MAIN MENU
This procedure shows the main menu of the program, which is displayed until user choses to exit
"""
def mainMenu():
    opcion = 10
    while opcion != 0:
        print "Bienvenido\n"
        print "0. Salir"
        print "1. Comprar un producto"
        print "2. Revisar inventario"
        print "3. Hacer cuentas"
        opcion = int(raw_input("Selecciona una opcion: "))
        
        if opcion == 0:
            print "Gracias por visitarnos"
        elif opcion == 1:
            #from jj_products import purchaseProduct
            barcode = int(raw_input("Introduce el codigo de barras: "))
            quantity = int(raw_input("Introduce la cantidad: "))
            product = product_inventory.findProduct(barcode)
            if product == product_inventory.NULL or product.quantity < quantity:
                print "ERROR: Producto inexistente."
            else:
                product.purchase(quantity) 
        elif opcion == 2:
            inventoryMenu()
        elif opcion == 3:
            salesMenu()
        else:
            print "ERROR: Lo siento, no tenemos esa opcion."

"""
START
This procedure sets up the environment to run the program
"""
def start ():
    verifyInstallation()
    print "\t\t'Sistema de gestion de tiendas JJ'"
    load()
    mainMenu()
