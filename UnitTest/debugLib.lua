local debugLib = {}

function debugLib.printtable(t)
	if type(t) == "table" then
		for i, v in pairs( t ) do
			print( i, v )
		end
	else
		print( "the input arg is not a table, it is of type ", type(t))
	end	
end

return debugLib