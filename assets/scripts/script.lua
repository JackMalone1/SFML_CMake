testString = "LuaBridge works"
number = 42

window = {
	title = "Window v.0.4",
	size = {
		width = 800,
		height = 600
	}
}

sumNumbers = function(a, b)
	printMessage("You can call c++ functions from lua functions")
	return a + b
end