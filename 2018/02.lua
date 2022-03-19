io.write("-- Day 02 --\n")

local utils = require "utils"
local data = utils.input()

local function letters_matching(id1, id2)
	local iter1 = id1:gmatch(".")
	local iter2 = id2:gmatch(".")
	local match = {}
	local count = 0

	for i = 1, #id1 do
		local c1 = iter1()
		local c2 = iter2()

		if c1 == c2 then
			count = count + 1
			match[#match + 1] = c1
		end
	end

	return count, match
end

local function find_id()
	-- Might look confusing because it's ptimized to never check previous ids nor the last one
	for i = 1, #data - 1 do
		for j = i + 1, #data do
			local count, match = letters_matching(data[i], data[j])
			if count == #data[i] - 1 then return table.concat(match) end
		end
	end
end

local function char_repeats(id)
	local freq2 = false
	local freq3 = false
	local count = {}

	for c in id:gmatch('.') do
		local i = c:byte() - 96

		count[i] = count[i] or 0
		count[i] = count[i] + 1
	end

	for i = 1, 26 do
		local n = count[i]

		freq2 = freq2 or n == 2
		freq3 = freq3 or n == 3
	end

	return freq2, freq3
end

local function count_repeating()
	local double_l = 0
	local triple_l = 0

	for _, l in ipairs(data) do
		local twice, thrice = char_repeats(l)

		double_l = double_l + (twice and 1 or 0)
		triple_l = triple_l + (thrice and 1 or 0)
	end

	return double_l * triple_l
end

io.write("Part 1: ", count_repeating(), '\n')
io.write("Part 2: ", find_id(), '\n')
