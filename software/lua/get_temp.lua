-- read temperature
-- print(os.date("%Y:%m:%d %H:%M:%S"))
local rc
os.execute("sleep " .. tonumber(7))
for i=1,3,1 do
  rc = os.execute('date +\'%d-%m-%y %H:%M.%S \' >/tmp/t1.tmp&&am2320term 1 >>/tmp/t1.tmp')
  if rc == true then break end
  os.execute("sleep " .. tonumber(5))
end
if rc == true then
  os.execute('mv -f /tmp/t1.tmp /tmp/t1')
else
  os.execute('echo sensor 1 error >/tmp/t1')
end
--print(tostring(rc))

for i=1,3,1 do
  rc = os.execute('date +\'%d-%m-%y %H:%M.%S \' >/tmp/t2.tmp&&am2320term 2 >>/tmp/t2.tmp')
  if rc == true then break end
  os.execute("sleep " .. tonumber(5))
end
if rc == true then
  os.execute('mv -f /tmp/t2.tmp /tmp/t2')
else
  os.execute('echo sensor 2 error >/tmp/t2')
end
--print(tostring(rc))

os.exit(0)

