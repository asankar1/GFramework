vec3 = {x=10, y=20, z=30}
local vv = require("vector2d")
print("script test")
print("-----------")
print(vv)
function vec3:new(x,y,z)
	copy = {}
	copy.x = x or self.x
	copy.y = y or self.y
	copy.z = z or self.z
	setmetatable(copy, self)
	self.__index = self
	return copy
end

function PrintTable(t)
	print("printing table..")
	local k,v
	for k,v in pairs(t) do
		print(k,v)
	end
end
	

a = vv.new()

local b = vv.new()
b:print()