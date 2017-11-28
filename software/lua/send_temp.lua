local rc
rc = os.execute('am2320term 1')
print(tostring(rc))
os.exit(0)

local fo=io.open("/dev/ttyS1","r+")
io.output(fo)
io.input(fo)
io.write("ATE1\r")
print(io.read())
print(io.read())
io.write("AT+CREG?\r")
print(io.read())
print(io.read())
print(io.read())
print(io.read())
print(io.read())
~
