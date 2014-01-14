
-- value = test and x or y   ==  ? : ; in C

a = 1
b = (a == 1) and "one" or "not one"
print(b)

-- is equivalent to

a = 1

if a == 1 then
	b = "one"
else
	b = "not one"
end

print(b)

