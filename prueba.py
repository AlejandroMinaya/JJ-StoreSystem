from jj_setup import *
sales = Ledger()
sales.load()
holis = Sale("0 0 0 0 0 0 0 0 0","12.50","7503033954085","10","0")
sales.addSale(holis)
sales.save()
print sales.sales
