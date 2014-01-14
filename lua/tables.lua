
a = {} -- {} creates an empty table

b = {1, 2, 3} -- creates a table containing numbers 1, 2, 3

c = {"a", "b", "c"} -- creates a table containing strings a, b, c

print(a, b, c) -- tables don't print directly, we'll get back to this!!

-- Associate index style.

address={} -- empty address
address.Street = "Wyman Street"
address.StreetNumber = 360
address.AptNumber = "2a"
address.City = "Watartown"
address.State = "Vermont"
address.Country = "USA"

print(address.StreetNumber, address["AptNumber"])


