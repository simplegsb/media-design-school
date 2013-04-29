
function getAsciiTable()

	asciiTable = {}
	for i = 32, 126, 1 do
		asciiTable[i] = (char) i
	end
	
	return asciiTable
end

function printAsciiTable(asciiTable, rowLength)

	for k in pairs(asciiTable) do
		io.write(k .. "=" .. asciiTable[k] .. " ")
		if (k - 1) % rowLength == 0 then
			io.write("\n")
		end
	end
end

printAsciiTable(getAsciiTable(), 3)
