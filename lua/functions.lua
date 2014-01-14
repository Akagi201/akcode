
-- Define a function without parameters or return value.

function myFirstLuaFunction()
	print("My first lua function was called")
end

-- Call myFirstLuaFunction.
myFirstLuaFunction()

-- Define a function with a return value.
function  mySecondLuaFunction()
	return "string from my second function."
end

-- Call function returning a value.
a = mySecondLuaFunction("string")
print(a)

-- Define function with multiple parameters and multiple return values.
function myFirstLuaFunctionWithMultipleReturnValues(a, b, c)
	return a, b, c, "My first lua function with multiple return values", 1, true
end

a, b, c, d, e, f = myFirstLuaFunctionWithMultipleReturnValues(1, 2, "three")

print(a, b, c, d, e, f)

-- Variable scoping and functions.
-- All variables are global in scope by default.

b = "global variable"

-- To make local variables you must put the keyword 'local' in font.
function myfunc()
	local b = " local variable"
	a = "global variable"
	print(a, b)
end

myfunc()
print(a, b)