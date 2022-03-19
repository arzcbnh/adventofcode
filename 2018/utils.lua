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

-- https://github.com/skeeto/getopt-lua
function utils.getopt(argv, optstring, nonoptions)
	local optind = 1
	local optpos = 2
	nonoptions = nonoptions or {}
	return function()
		while true do
			local arg = argv[optind]
			if arg == nil then
				return nil
			elseif arg == '--' then
				for i = optind + 1, #argv do
					table.insert(nonoptions, argv[i])
				end
				return nil
			elseif arg:sub(1, 1) == '-' then
				local opt = arg:sub(optpos, optpos)
				local start, stop = optstring:find(opt .. ':?')
				if not start then
					optind = optind + 1
					optpos = 2
					return '?', opt
				elseif stop > start and #arg > optpos then
					local optarg = arg:sub(optpos + 1)
					optind = optind + 1
					optpos = 2
					return opt, optarg
				elseif stop > start then
					local optarg = argv[optind + 1]
					optind = optind + 2
					optpos = 2
					if optarg == nil then
						return ':', opt
					end
					return opt, optarg
				else
					optpos = optpos + 1
					if optpos > #arg then
						optind = optind + 1
						optpos = 2
					end
					return opt, nil
				end
			else
				optind = optind + 1
				table.insert(nonoptions, arg)
			end
		end
	end
end

return utils
