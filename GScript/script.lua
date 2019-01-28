--local GVariant = require("GVariant")
--local Classes = require("Classes")
local test = require("test")

print("script test")
print("-----------")
print(GObject)

s = test.new_sphere()

print(s)

function print_metatable(o)
	if(getmetatable(o) == nil) then
		print("No metatable available")
		return
	end
	print("Metatable:")
	for k,v in pairs(getmetatable(o)) do
	  print(k ,"::", v)
	end
	print("__index table:")
	for k,v in pairs(getmetatable(o).__index) do
	  print(k ,"::", v)
	end
end

--print_metatable(s)
--r = s:getRadius()
print("r:" , s:getRadius())
--print(s:getRadius())

