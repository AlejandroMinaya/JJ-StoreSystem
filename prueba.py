from jj_setup import *
new_product = Product("7501033954087", "Ensure 237ml", "25.50", "100", "0", "1")
print "Quantity: %i" % new_product.quantity
print "Purchasing..."
new_product.purchase()
print "Quantity: %i" % new_product.quantity
print "Purchasing 10..."
new_product.purchase(10)
print "Quantity: %i" % new_product.quantity

new_product.format_print()
