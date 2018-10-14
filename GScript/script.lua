--local GVariant = require("GVariant")
--local Classes = require("Classes")
local test = require("test")

print("script test")
print("-----------")
print(Object)

s = test.new_sphere()
--s:about()
--s:setRadius(27)
--s:setPosition(11,22,33)
--s:rename("renamed_sphere")
--s:about()

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
s:setRadius(29)
print("-----------")
print("get vec3 from C++")
--s:setRadius(5)
--sphere.overview()
print(s.getName)
--print_metatable(s)

print("-- object--")

--print_metatable(Object)

print(".....")
Object.overview(34)




print("overview return ")
print(v1)
p1=s:getPosition()
print("-----------")


print("get userdata from C++")
p = s:getParent()
if p == nil then 
	print("parent is nil") 
	else
	print(p:getName())
end
p:setPosition(41,42,43)
n = test.new_node()
n:about()
p:about()
s:about()
--s:setRadius(14, 23, 45)
--test.about(j)
--print(j:getName())
--print(j:getRadius())



