frequency = 0;
for l in io.lines() do
	frequency = frequency + l
end

io.write("Part 1: ", frequency, "\n")

found = {}
common = false
frequency = 0;
::redo::
io.input():seek("set")
for l in io.lines() do
	frequency = frequency + l

	found[frequency] = found[frequency] or 0
	found[frequency] = found[frequency] + 1

	if found[frequency] > 1 then
		common = frequency
		break
	end
end

if not common then
	goto redo
end

io.write("Part 2:", common, "\n")
