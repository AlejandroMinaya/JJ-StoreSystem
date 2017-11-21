"""
TITLE: JJ Store Management System Setup
Version: 0.1 (alpha)
"""
import math
import time
import io

MAX_COLUMN_WIDTH = 36
TAB_SIZE = 8

files = ["categories.csv","products.csv","sales.csv"]

def findMainFiles():
    for file_path in files:
        try:
            io.open(file_path, "r")
        except IOError:
            return 0
    return 1

def createMainFiles():
    for file_path in files:
       io.open(file_path, "w").close
    

def verifyInstallation():
    if not findMainFiles():
        print "You are missing main files. \n"
        possible_answers = ["Y", "N"]
        answer = input("Would you like to create these files (Y/y/N/n)?")
        if answer[0].upper() in possible_answers:
            createMainFiles()
        else:
            print "Couldn't complete the setup process.\n"

def printHeading(file_path):
    with io.open(file_path, "r") as working_file:
        columns = working_file.readline().split(",")
        tabs_to_append = ""
        table_heading = ""
        for i in range(MAX_COLUMN_WIDTH / TAB_SIZE - 1):
            tabs_to_append += "\t"
        for i in range(len(columns)):
            columns[i] += tabs_to_append
            if i < len(columns) - 1:
                columns[i] += " || "
            table_heading += columns[i]
        print table_heading

def printString(string_to_print, separator):
    amount_tabs_to_append = 0
    if (MAX_COLUMN_WIDTH - len(string_to_print)) % TAB_SIZE == 0:
        amount_tabs_to_append = (MAX_COLUMN_WIDTH - len(string_to_print) - TAB_SIZE) / TAB_SIZE
    else:
        amount_tabs_to_append = (MAX_COLUMN_WIDTH - len(string_to_print)) / TAB_SIZE

    if amount_tabs_to_append < 1:
        string_to_print = " " + string_to_print[:MAX_COLUMN_WIDTH - TAB_SIZE - 3] + ".."
    else:
        for i in range(amount_tabs_to_append):
            string_to_print += "\t"
        string_to_print = " " + string_to_print
    print string_to_print,separator,


def stringToTime(time_string):
    if "/" in time_string:
        time_info = map(int, time_string.split("/"))
        return time.struct_time((time_info[2] + 1900, time_info[1], time_info[0], 0, 0, 0, 0, 0, 0))
    else:
        time_info = map(int, time_string.split(" "))
        return time.struct_time((time_info[5], time_info[4], time_info[3], time_info[2], time_info[1], time_info[0], time_info[6], time_info[7], time_info[8]))

class Sale():
    def __init__(self, timestamp, amount, product, quantity, is_order):
        if type(timestamp) is str:
            self.timestamp = stringToTime(timestamp)
        else:
            self.timestamp = timestamp
        self.amount = float(amount)
        self.product = int(product)
        self.quantity = int(quantity)
        self.is_order = int(is_order)

    
    def formatPrint(self):
        printString(time.asctime(self.timestamp), "||")
        printString(str(self.amount), "||")
        printString(str(self.product), "||")
        printString(str(self.quantity),"||")
        if(self.is_order):
            printString("Si", "\n")
        else:
            printString("No", "\n")


class Ledger():
    def __init__(self):
        self.sales = []
        self.NULL = Sale("0 0 0 0 0 0 0 0 0","-1","-1","-1","-1")
    
    def addSale(self, sale):
        self.sales.append(sale)
    
    
    def load(self):
        with io.open("sales.csv", "r") as sales_db:
            sales = sales_db.readlines()
            sales.pop(0) #We eliminate the header from the list
            for sale in sales:
                sale_attributes = sale.split(",")
                sale_object = Sale(sale_attributes[0],sale_attributes[1],sale_attributes[2],sale_attributes[3],sale_attributes[4])
                self.addSale(sale_object)
    
    
    def save(self):
        heading = ""
        with io.open("sales.csv", "r") as sales_db:
            heading = sales_db.readline()
        with io.open("sales.csv", "w") as sales_db:
            sales_db.write(heading)
            for sale in self.sales:
                sale_string = "%i %i %i %i %i %i %i %i %i, %f, %i, %i, %i\n" % (sale.timestamp.tm_sec, sale.timestamp.tm_min, sale.timestamp.tm_hour, sale.timestamp.tm_mday, sale.timestamp.tm_mon, sale.timestamp.tm_year, sale.timestamp.tm_wday, sale.timestamp.tm_yday, sale.timestamp.tm_isdst, sale.amount, sale.product, sale.quantity, sale.is_order)
                sales_db.write(unicode(sale_string))


    def printSales(self):
        heading = ""
        with io.open("sales.csv", "r") as sales_db:
            heading = sales_db.readline()
        printHeading(heading)
        for sale in self.sales:
            sale.formatPrint()


sales_ledger = Ledger()

class Product():
    def __init__(self, barcode, name, price, quantity, category, enabled):
        self.barcode = int(barcode)
        self.name = name
        self.price = float(price)
        self.quantity = int(quantity)
        self.category = int(category)
        self.enabled = int(enabled)
    
    
    def edit(self, barcode, name, price, quantity, category):
        if int(barcode) == -1:
            barcode = self.barcode
        if name == "\0":
            name = self.name
        if float(price) == -1:
            price = self.price
        if int(quantity) == -1:
            quantity = self.quantity
        if int(category) == -1:
            category = self.category
        enabled = 1
        self.__init__(barcode, name, price, quantity, category, enabled)
    
    
    def purchase(self, quantity=1):
        self.quantity -= quantity
        sale = Sale(time.time(), self.price * self.quantity, self.barcode, self.quantity, 0)
        sales_ledger.addSale(sale)
    
    
    def order(self, quantity=1):
        self.quantity += quantity
        sale = Sale(time.time(), self.price * self.quantity, self.barcode, self.quantity, 1)
        sales_ledger.addSale(sale)


    def formatPrint(self):
        if self.enabled:
            printString(str(self.barcode), "||")
            printString(self.name, "||")
            printString(str(self.price), "||")
            printString(str(self.quantity), "||")
            printString(str(self.category), "\n")  

class Inventory():
    def __init__(self):
        self.products = []
        self.NULL = Product("-1", "-1", "-1", "-1", "-1", "-1")
    
    
    def load(self):
        with io.open("products.csv", "r") as products_db:
            products = products_db.readline()
            products.pop(0) #We remove the heading from the list
            for product in products:
                product_attributes = product.split(",")
                product_object = Product(product_attributes[0], product_attributes[1], product_attributes[2], product_attributes[3], product_attributes[4], product_attributes[5])
                self.addProduct(product_object)

    def save(self):
        heading = ""
        with io.open("products.csv", "r") as products_db:
            heading = products_db.readline()
        with io.open("products.csv", "w") as products_db:
            products_db.write(heading)
            for product in self.products:
                product_string = "%i,%s,%f,%i,%i,%i" % (product.barcode, product.name, product.price, product.quantity, product.category, product.enabled) 
                products_db.write(unicode(heading))

    def printProducts(self):
        heading = ""
        with io.open("products.csv", "r") as products_db:
            heading = products_db.readline()
        printHeading(heading)
        for product in self.products:
            product.formatPrint()
    
    def findProduct(self, barcode):
        for product in self.products:
            if product.barcode == barcode:
                return product
        return self.NULL

    def addProduct(self, product):
        self.products.append(product)


    def deleteProduct(self, barcode):
        self.products.remove(self.findProduct(barcode))

product_inventory = Inventory()
