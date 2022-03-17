io.write("-- Day 01 --\n")

freq = 0
count = {}
part1 = false
repeated = false

::redo::
io.input():seek("set")

for l in io.lines() do
	freq = freq + l
	count[freq] = count[freq] or 0
	count[freq] = count[freq] + 1

	if count[freq] > 1 then
		repeated = freq
		break
	end
end

if not part1 then
	part1 = freq
end

if not repeated then
	goto redo
end

io.write("Part 1: ", part1, "\n")
io.write("Part 2: ", repeated, "\n")
