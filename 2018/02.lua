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

	for _, l in ipairs(data) do
		for _, m in ipairs(data) do
			if compare(l, m) then
				asdasd, dude = l, m
				break
			end
		end
	end

	amb, lab = asdasd:gmatch('.'), dude:gmatch('.')

	c1 = nil
	c2 = nil
	ss = ""
	for i = 1, #asdasd do
		c1, c2 = amb(), lab()
		if c1 == c2 then ss = ss .. c1 end
	end

	print(ss)
end

function compare(l, m)
	amb, lab = l:gmatch('.'), m:gmatch('.')
	local count = 0

	for i = 1, #l do
		count = count + (amb() == lab() and 1 or 0)
	end

	return count == #l - 1
end

data = utils.input()

io.write("Part 1: ", calc_repeating(), '\n')
find_id()
