
print("Finished 10% of the \"test\".")

for i = 1, 99, 1 do
	print(i)
end

for i = 99, 1, -2 do
	print(i .. "," .. i - 1)
end

for i = 1, 99, 1 do
	if i % 3 == 0 then
		print(i)
	end
end