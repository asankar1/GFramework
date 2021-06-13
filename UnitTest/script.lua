--local GVariant = require("GVariant")
--local Classes = require("Classes1")
--local test = require("test2")

print("LUA SCRIPT")
print("----------")



--Classes.new_class("dummy");	


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

print("new=",new);
print("sphere=",sphere);

print_metatable(new)
print_metatable(sphereasds)


--print_metatable(s)
--r = s:getRadius()
--print("r:" , s:getRadius())

--print("count:" , new.count)



--print(s:getRadius())

