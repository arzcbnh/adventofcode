io.write("-- Day 02 --\n")

utils = require "utils"

function calc_repeating()
	local double_l = 0
	local triple_l = 0

	for _, l in ipairs(data) do
		local twice, thrice = char_repeats(l)

		double_l = double_l + (twice and 1 or 0)
		triple_l = triple_l + (thrice and 1 or 0)
	end

	return double_l * triple_l
end

function char_repeats(s)
	local freq2 = false
	local freq3 = false
	local count = {}

	for c in s:gmatch('.') do
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

function find_id()
	local matched = false
	local id1 = nil
	local id2 = nil

	for i = 1, #data do
		for j = i + 1, #data do
			local count, id = letter_missing(data[i], data[j])
			if count == #data[i] - 1 then return table.concat(id) end
		end
	end
end

function letter_missing(s1, s2)
	local iter1 = s1:gmatch('.')
	local iter2 = s2:gmatch('.')
	local same_l = {}
	local count = 0

	for i = 1, #s1 do
		local c = iter1

		if c == iter2() then
			count = count + 1
			same_l[#same_l + 1] = c
		end
	end

	return count, same_l
end

data = utils.input()

io.write("Part 1: ", calc_repeating(), '\n')
io.write("Part 2: ", find_id(), '\n')
