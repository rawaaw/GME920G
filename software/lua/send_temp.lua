-- send SMS with current temperatute (in crontab)
local do_lock = true
if (#arg > 0 and arg[1] == '0') then
  do_lock = false
end

local try_count = 0
local p = require "posix"
local fd
local lock = {
  l_type = p.F_WRLCK;     -- Exclusive lock
  l_whence = p.SEEK_SET;  -- Relative to beginning of file
  l_start = 0;            -- Start from 1st byte
  l_len = 0;              -- Lock whole file
}
while do_lock do
  print("try lock")
  fd = p.creat("/tmp/ttylock", "rw-r--r--")
  local result = p.fcntl(fd, p.F_SETLK, lock)
  if (result ~= nil) then break end
  os.execute("sleep " .. tonumber(1))
  try_count = try_count + 1
  if (try_count > 10) then
    print "ttyS1 locked"
    os.exit(1)
  end
end

local try_count = 0
local sent_ok = false

repeat
  os.execute('send_temp1.sh 2>/tmp/send.log')
  local f=io.open("/tmp/send.log","r")
  io.input(f)

  local do_check_result = false
  sent_ok = false
  while true do
    local l = io.read("*line")
    print(l)
    if l == nil then break end
    if do_check_result == true then
      if string.find(l, "OK")~=nil then
        sent_ok = true
      end
    elseif string.find(l, "AT%+CMGS=")~= nil then
--      print("!!!!\n")
      do_check_result = true
    end
  end
  try_count = try_count + 1
  io.close(f)
until sent_ok == true or try_count == 3
if sent_ok == true then
  print "OK\n"
else
  os.execute("/bin/mdmrst")
  print "ERROR\n"
end

if (do_lock == true) then
  print("unlock")
  lock.l_type = p.F_UNLCK
  p.fcntl(fd, p.F_SETLK, lock)
end

os.exit(0)

