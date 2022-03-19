local utils = {}

--[[
local function make_counter()
	local counter = 0
	return function (n)
		counter = counter + (n or 1)
		return counter
	end
end
--]]

function utils.input(fn)
	fn = fn or function (a) return a end
	local t = {}

	for l in io.lines() do
		t[#t + 1] = fn(l)
	end

	return t
end

return utils
