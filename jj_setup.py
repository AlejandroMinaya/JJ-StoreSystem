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
        self.timestamp = stringToTime(timestamp)
        self.amount = float(amount)
        self.product = int(product)
        self.quantity = int(quantity)
        self.is_order = int(is_order)

class Ledger():
    def __init__(self):
        self.sales = []
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
