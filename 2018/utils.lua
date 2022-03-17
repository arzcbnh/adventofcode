function make_counter()
	local counter = 0
	return function ()
		counter = counter + 1
		return counter
	end
end
