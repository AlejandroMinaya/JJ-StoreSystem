import jj_products
import jj_setup
import jj_categories

def load():
    loadProducts()
    loadCategories()
    loadLedger()


def salesMenu():
    print "\tMENU CUENTAS"
    print "0. Regresar"
    print "1. Imprimir cuentas"
    print "2. Obtener ganancias por periodo"
    print "3. Obtener costos por periodo"
    print "4. Obtener balance neto por periodo"
    opcion = int(raw_input("Seleccionar una opcion:"))
    
    if opcion == 0:
        mainMenu()
    elif opcion == 1:
        printSales()
    elif opcion == 2:
        "Ingrese la fecha de inicio en formato dd/mm/yyyy"
        "Ingrese la fecha de fin en formato dd/mm/yyyy"
        
        print "Ganancias: %.2f", #AQUI FALTAN COSAS
    elif opcion == 3:
        "Ingrese la fecha de incio en formato dd/mm/yyyy"
        "Ingrese la fecha de fin en formato dd/mm/yyyy"
        
        print "Costos: %.2f", #AQUI FALTAN COSAS
    elif opcion == 4:
        print "Ingrese la fecha de inicio en formato dd/mm/yyyy"
        
        print "Ingrese la fecha de fin en formato dd/mm/yyyy"
        
        print "Balance: %.2f", #AQUI FALTAN COSAS
    else:
        print "ERROR: Lo siento, no tenemos esa opcion."


def categoriesMenu(opcion): #AGREGAR opcion DENTRO DE PARENTESIS
    print "\tMENU CATEGORIAS\n"
    print "\n0. Regresar"
    print "\n1. Agregar categoria"
    print "\n2. Eliminar categoria"
    print "\n3. Editar categoria"
    print "\n4. Imprimir categoria"
    
    opcion = int(raw_input("\nSelecciona una opcion: "))
    if opcion == 0:
        inventoryMenu()
    elif opcion == 1:
        category_name = input("\nIngrese el nombre de la nueva categoria: ")
        category_description = input("\nIngrese la descripcion de la nueva categoria")
        addCategory(category_name, category_description)
        printCategories()
    elif opcion == 2:
        category_id = int(raw_input("\nIngrese el ID de la categoria a eliminar"))
        deleteCategory(category_id)
        printCategories()
    elif opcion == 3:
        edit_category_id = ("\nIngrese el ID de la categoria a editar: ")
        print "\tEDICION CATEGORIA"
        print "\n\t\t0. Cancelar"
        print "\n\t\t1. Nombre de la categoria"
        print "\n\t\t2. Descripcion de la categoria"
        opcion = int(raw_input("\n\n\tSelecione una opcion: "))
        
        if opcion == 0:
            inventoryMenu()
        elif opcion == 1:
            category_name = input("\n\tIngrese el nuevo nombre: ")
            editCategory(edit_category_id, category_name, "\0")
        elif opcion == 2:
            category_description = input("\n\tIngrese la nueva descripcion:")
            editCategory(edit_category_id, "\0", category_description)
        else:
            print "ERROR: Lo siento, no tenemos esa opcion"
        printCategories()
    elif opcion == 4:
        printCategories()
    else:
        print "ERROR: Lo siento, no tenemos esa opcion"


def productsMenu(): #AGREGARLE EL -1
    #opcion = -1
    print "\t MENU PRODUCTOS\n"
    print "0. Regresar"
    print "1. Imprimir productos"
    print "2. Agregar producto"
    print "3. Eliminar producto"
    print "4. Ordenar producto"
    print "5. Editar producto"
    opcion = int(raw_input("\nSelecciona una opcion: "))
    
    #if(opcion == -1):
    #    opcion = getOption()
    if opcion == 0:
        inventoryMenu()
    elif opcion == 1:
        printProducts()
    elif opcion == 2:
        barcode = input("Introduce el codigo de barras: ")
        name = input("Introduce el nombre: ")
        price = float(raw_input("Introduce el precio: "))
        quantity = int(raw_input("Introduce la cantidad: "))
        category = int(raw_input("Introduce la categoria: "))
        category = addProduct(barcode, name, price, quantity, category)
        
        if(category):
            print "Esta categoria no existe. Creela a continuacion:\n"
            categoriesMenu()
            addProduct(barcode, name, price, quantity, category)
        printProducts()
    elif opcion == 3:
        barcode = input("Introduce el codigo de barras: ")
        deleteProduct(barcode)
        printProducts()
    elif opcion == 4:
        barcode = input("Introduce el codigo de barras: ")
        price = float("Introduce el precio: ")
        quantity = int(raw_input("Introduce el precio: "))
        if (orderProduct(barcode, price, quantity)):
            print "Este producto no existe. Creelo a continuacion."
            productsMenu()
        printProducts()
    elif opcion == 5:
        barcode = input("\nIngrese el codigo de barra del producto que quiera editar")
        #if(findProduct(barcode).barcode == END_PRODUCT.barcode)
            #print "\nProducto inexistente."
        print "\tEDICION PRODUCTO"
        print "\t0. Cancelar"
        print "\t1. Nombre"
        print "\t2. Precio"
        print "\t3. Cantidad"
        print "\t4. Categorias"
        opcion = int(raw_input("\n\tSelecciona una opcion: "))
        
        if opcion == 0:
            inventoryMenu()
        elif opcion == 1:
            name = input("\nIntroduce el nombre: ")
            price = -1
            quantity = -1
            category = -1
        elif opcion == 2:
            price = float(raw_input("\nIntroduce el precio: "))
            name = []
            quantity = -1
            category = -1
        elif opcion == 3:
            quantity = int(raw_input("\nIntroduce la cantidad: "))
            name = []
            price = -1
            category = -1
        elif opcion == 4:
            category = int(raw_input("\nIntroduce la categoria: "))
            name = []
            price = -1
            quantity = -1
        else:
            print "ERROR: Lo siento, no tenemos esa opcion."

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
        productsMenu() #PONERLE EL -1
    elif opcion == 2:
        categoriesMenu()
    else:
        print "ERROR: Lo siento, no tenemos esa opcion."


def mainMenu():
    opcion = 10
    while opcion != 0:
        load()
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
            barcode = input("Introduce el codigo de barras: ")
            quantity = int(raw_input("Introduce la cantidad: "))
            
            purchaseProduct(barcode, quantity)
        elif opcion == 2:
            inventaryMenu()
        elif opcion == 3:
            salesMenu()
        else:
            print "ERROR: Lo siento, no tenemos esa opcion."

def start ():
    verifyInstallation()
    print "\t\t'Sistema de gestion de tiendas JJ'"
    load()
    mainMenu()
    #opcion = -1
    #while opcion != 0:
    #    print "Bienvenido\n"
    #    print "0. Salir"
    #    print "1. Comprar un producto"
    #    print "2. Revisar inventario"
    #    print "3. Hacer cuentas"
    #    opcion = int(raw_input("Selecciona una opcion: "))
    #    
    #    if opcion == 0:
    #        print "Gracias por visitarnos"
    #    elif opcion == 1:
    #        #from jj_products import purchaseProduct
    #        barcode = input("Introduce el codigo de barras: ")
    #        quantity = int(raw_input("Introduce la cantidad: "))
    #        
    #        purchaseProduct(barcode, quantity)
    #    elif opcion == 2:
    #        inventaryMenu()
    #    elif opcion == 3:
    #        salesMenu()
    #    else:
    #        print "ERROR: Lo siento, no tenemos esa opcion."
