"""
TITLE: JJ Store Management System Setup
Version: 0.1 (alpha)
"""
import time #We import time to manage the timestamps for sales
import io #We import io to manage files

MAX_COLUMN_WIDTH = 36 #This is a constant to print aesthetically in the cli
TAB_SIZE = 8 #This is a constant to print aesthetically in the cli

#This is a list containing the paths to all the needed files to store our data
files = ["categories.csv","products.csv","sales.csv"]


#We create global counter to keep track of the categories ID
category_counter = 0

"""
FIND MAIN FILES -
This procedure tries to open the databases files
@return if it finds them, it return 0, if it doesn't it returns 1
"""
def findMainFiles():
    for file_path in files:
        try:
            io.open(file_path, "r")
        except IOError:
            return 0
    return 1

"""
CREATE MAIN FILES -
Creates the main files, navigating through the files list
"""
def createMainFiles():
    for file_path in files:
       io.open(file_path, "w").close

"""
VERIFY INSTALLATION -
Verifies if the necesary files are present, if they aren't it creates them
"""
def verifyInstallation():
    if not findMainFiles():
        print "You are missing main files. \n"
        possible_answers = ["Y", "N"]
        answer = input("Would you like to create these files (Y/y/N/n)?")
        if answer[0].upper() in possible_answers:
            createMainFiles()
        else:
            print "Couldn't complete the setup process.\n"

"""
PRINT HEADING -
Procedure to print heading for each file aesthetically
@param file_path, the path of the file containing the header
"""
def printHeading(file_path):
    with io.open(file_path, "r") as working_file: #We open the file in a context manager to avoid leaks
        columns = working_file.readline().split(",") #We turn the comma separated values into a list
        tabs_to_append = "" #We create an empty string where we will store the tabs we will later on append
        table_heading = "" #We create an empty string to store the heading we are going to print
        for i in range(MAX_COLUMN_WIDTH / TAB_SIZE - 1): #We assume each header to occupy no more than one tab equivalen number of characters and fill the remaining space with tabs
            tabs_to_append += "\t"
        for i in range(len(columns)): #For each column heading...
            columns[i] += tabs_to_append #We append the tabs...
            if i < len(columns) - 1: #While it is not the last column we add the separator...
                columns[i] += " || "
            table_heading += columns[i] #we append the formatted column to the table header string
        print table_heading #We print the table header


"""
PRINT STRING -
Procedure to print string aesthetically for the cli
@param string_to_print, the string we are going to format
@param separator, the column separator
"""
def printString(string_to_print, separator):
    amount_tabs_to_append = 0 #This variable will contain the number fo tabs we will add
    if (MAX_COLUMN_WIDTH - len(string_to_print)) % TAB_SIZE == 0: #If the tab equivalence of the remaining space is divisible by TAB_SIZE...
        amount_tabs_to_append = (MAX_COLUMN_WIDTH - len(string_to_print) - TAB_SIZE) / TAB_SIZE #We substract an extra tab
    else:
        amount_tabs_to_append = (MAX_COLUMN_WIDTH - len(string_to_print)) / TAB_SIZE #Else, we use that quotient

    if amount_tabs_to_append < 1: #If there is no remaining space...
        string_to_print = " " + string_to_print[:MAX_COLUMN_WIDTH - TAB_SIZE - 3] + ".." #We add the string minus its last three characters and fill with periods
    else:
        for i in range(amount_tabs_to_append): #Else, append this number of tabs to the string to print
            string_to_print += "\t"
        string_to_print = " " + string_to_print
    print string_to_print,separator, #We print the string


"""
STRING TO TIME -
Function to convert a string into a struct_time structure
@param time_string, is the string containing the time information
@return the corresponding struct_time structure
"""
def stringToTime(time_string):
    if "/" in time_string: #If the time string contains /, meaning it is in a dd/mm/yyyy format...
        time_info = map(int, time_string.split("/")) #Convert this into a list
        return time.struct_time((time_info[2] + 1900, time_info[1], time_info[0], 0, 0, 0, 0, 0, 0)) #Fill a struct_time structure with the corresponding info
    else: #Else, it took it from the database, and it is separated by spaces...
        time_info = map(int, time_string.split(" "))
        #Fill the struct_time accordingly
        return time.struct_time((time_info[5], time_info[4], time_info[3], time_info[2], time_info[1], time_info[0], time_info[6], time_info[7], time_info[8]))


"""
SALE CLASS
This class is used to define the sales that our program we will be registering
"""
class Sale():
    def __init__(self, timestamp, amount, product, quantity, is_order, parent):
        if type(timestamp) is str: #If the timestamp is a string....
            self.timestamp = stringToTime(timestamp) #Parse it into a struct_time
        elif type(timestamp) is float: #If it is a float...
            self.timestamp = time.localtime(timestamp) #Parse it into a struct_time
        elif type(timestamp) is time.struct_time: #If it is a struct_time... 
            self.timestamp = timestamp #Just assign it.
        self.amount = float(amount)
        self.product = int(product)
        self.quantity = int(quantity)
        self.is_order = int(is_order)
        self.parent = parent

    """
    FORMAT PRINT
    This procedure prints the Sale object as an aesthetic row for the cli
    """
    def formatPrint(self):
        printString(time.asctime(self.timestamp), "||")
        printString(str(self.amount), "||")
        printString(product_inventory.findProduct(self.product).name, "||")
        printString(str(self.quantity),"||")
        if(self.is_order):
            printString("Si", "\n")
        else:
            printString("No", "\n")


"""
LEDGER CLASS
This class is used to define the container for sales.
"""
class Ledger():
    def __init__(self):
        self.sales = []
        self.NULL = Sale("0 0 0 0 0 0 0 0 0","-1","-1","-1","-1",None) #NULL SALE VALUE
    
    def addSale(self, sale):
        self.sales.append(sale)
        self.save()
    
    """
    LOAD
    This procedure loads the sales from the files into memory
    """
    def load(self):
        with io.open("sales.csv", "r") as sales_db:
            sales = sales_db.readlines() #We convert the file lines into a list
            sales.pop(0) #We eliminate the header from the list
            for sale in sales: #For each sale in file
                sale_attributes = sale.split(",") #Split the string into a list
                #We create a Sale object containing this information
                sale_object = Sale(sale_attributes[0],sale_attributes[1],sale_attributes[2],sale_attributes[3],sale_attributes[4])
                self.addSale(sale_object) #We add the object to the general list
    
    
    def save(self):
        heading = "" #We create a empty string to store the heading of the file to save
        with io.open("sales.csv", "r") as sales_db: 
            heading = sales_db.readline() #We read just the first line of the file
        with io.open("sales.csv", "w") as sales_db:
            sales_db.write(heading) #We write the heading
            for sale in self.sales: #For each sale in memory, we stringify it into a csv value and write it to file
                sale_string = "%i %i %i %i %i %i %i %i %i, %f, %i, %i, %i\n" % (sale.timestamp.tm_sec, sale.timestamp.tm_min, sale.timestamp.tm_hour, sale.timestamp.tm_mday, sale.timestamp.tm_mon, sale.timestamp.tm_year, sale.timestamp.tm_wday, sale.timestamp.tm_yday, sale.timestamp.tm_isdst, sale.amount, sale.product, sale.quantity, sale.is_order)
                sales_db.write(unicode(sale_string))


    def printSales(self):
        heading = ""
        with io.open("sales.csv", "r") as sales_db:
            heading = sales_db.readline()
        printHeading(heading) #We print the read heading
        for sale in self.sales: #For each sale in memory we print it aesthetically
            sale.formatPrint()

    """
    MONEY IN PERIOD -
    This function returns the total money earned/spent in a period of time specified by the user
    @param start_date, start considering sales starting from this date
    @param end_date, stop considering salse ending this date
    @param Whether to include just earning or just costs
    @return the total money earned or spent
    """
    def moneyInPeriod(self, start_date, end_date, earning):
        total_money = 0.0
        start = time.mktime(stringToTime(start_date))
        end = time.mktime(stringToTime(end_date))
        for sale in self.sales:
            if time.mktime(sale.timestamp) >= start and time.mktime(sale.timestamp) <= end:
                if (earning):
                    if not sale.is_order:
                        total_money += sale.amount
                else:
                    if sale.is_order:
                        total_money -= sale.amount
        return total_money


#We create a default ledger container
sales_ledger = Ledger()



"""
PRODUCT CLASS -
This class represent each of the products we will be selling at our store
"""
class Product():
    def __init__(self, barcode, name, price, quantity, category, enabled, parent):
        self.barcode = int(barcode)
        self.name = name
        self.price = float(price)
        self.quantity = int(quantity)
        self.category = int(category)
        self.enabled = int(enabled)
        self.parent = parent
    
    
    def edit(self, name, price, quantity, category):
        if name == "\0": #If it is null... don't take the new value
            name = self.name
        if float(price) == -1:
            price = self.price
        if int(quantity) == -1:
            quantity = self.quantity
        if int(category) == -1:
            category = self.category
        enabled = 1 #If the product is automatically enabled if it was edited
        self.__init__(self.barcode, name, price, quantity, category, enabled)
        self.parent.save()
    

    def purchase(self, quantity=1):
        self.quantity -= quantity
        sale = Sale(time.localtime(time.time()), self.price * self.quantity, self.barcode, self.quantity, 0)
        sales_ledger.addSale(sale)
        self.parent.save()
    
    
    def order(self, quantity=1, price=1):
        self.quantity += quantity
        sale = Sale(time.localtime(time.time()), price * self.quantity, self.barcode, self.quantity, 1)
        sales_ledger.addSale(sale)
        self.parent.save()


    def formatPrint(self):
        if self.enabled:
            printString(str(self.barcode), "||")
            printString(self.name, "||")
            printString(str(self.price), "||")
            printString(str(self.quantity), "||")
            printString(categories.findCategory(self.category).name, "\n")  

class Inventory():
    def __init__(self):
        self.products = []
        self.NULL = Product("-1", "\0", "-1", "-1", "-1", "-1", None)
    
    
    def load(self):
        with io.open("products.csv", "r") as products_db:
            products = products_db.readlines()
            products.pop(0) #We remove the heading from the list
            for product in products:
                product_attributes = product.split(",")
                product_object = Product(product_attributes[0], product_attributes[1], product_attributes[2], product_attributes[3], product_attributes[4], product_attributes[5], self)
                self.addProduct(product_object)

    def save(self):
        heading = ""
        with io.open("products.csv", "r") as products_db:
            heading = products_db.readline()
        with io.open("products.csv", "w") as products_db:
            products_db.write(heading)
            for product in self.products:
                product_string = "%i,%s,%f,%i,%i,%i\n" % (product.barcode, product.name, product.price, product.quantity, product.category, product.enabled) 
                products_db.write(unicode(product_string))

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
        self.save()


    def deleteProduct(self, barcode):
        self.findProduct(barcode).enabled = 0
        self.save()

#We create a default inventory
product_inventory = Inventory()

class Category():
    def __init__(self, ID=category_counter+1, name="\0", description="\0", enabled=-1, parent=None):
        self.ID = int(ID)
        self.name = name
        self.description = description
        self.enabled = int(enabled)
        self.parent = parent

        global category_counter
        category_counter += 1
    
    def edit(self, name, description, enabled):
        if name == "\0":#If null, do not change...
            name = self.name
        if description == "\0":
            description = self.description
        self.__init__(self.ID, name, description, "1")
        parent.save()


    def formatPrint(self):
        if(self.enabled):
            printString(str(self.ID), "||")
            printString(self.name, "||")
            printString(self.description, "\n")

class Categories():
    def __init__(self):
        self.categories = []
        self.NULL = Category("-1","\0","\0","-1",None)
    def load(self):
        with io.open("categories.csv", "r") as categories_db:
            categories = categories_db.readlines()
            categories.pop(0) #We ignore the heading
            for category in categories:
                category_attribute = category.split(",")
                category_object = Category(category_attribute[0], category_attribute[1], category_attribute[2], category_attribute[3],self)
                self.addCategory(category_object)
        category_counter = len(self.categories)
    
    def save(self):
        heading = ""
        with io.open("categories.csv", "r") as categories_db:
            heading = categories_db.readline()
        with io.open("categories.csv", "w") as categories_db:
            categories_db.write(heading)
            for category in self.categories:
                category_string = "%i,%s,%s,%i\n" % (category.ID, category.name, category.description, category.enabled)
                categories_db.write(unicode(category_string))


    def findCategory(self, category_ID):
        for category in self.categories:
            if category.ID == category_ID:
                return category
        return self.NULL

    def addCategory(self, category):
        self.categories.append(category)
        self.save()

    def deleteCategory(self, category_ID):
        self.findCategory(category_ID).enabled = 0
        self.save()

    def printCategories(self):
        heading = ""
        with io.open("categories.csv", "r") as categories_db:
            heading = categories_db.readline()
        printHeading(heading)
        for category in self.categories:
            category.formatPrint()

#We create a default container for categories
categories = Categories()
