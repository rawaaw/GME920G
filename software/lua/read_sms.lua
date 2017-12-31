-- read and delete SMS from GSM modem and answer to SMS with GETT in message text
local do_lock = true
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

local f = assert (io.popen("/usr/bin/chat -t 5 -e '' AT+CMGL=\\\"ALL\\\" OK >/dev/ttyS1 </dev/ttyS1 2>/tmp/atcmd.log"))
f:close()

local do_get_temp = nil

f = io.open("/tmp/atcmd.log","r")
io.input(f)
for line in f:lines() do
  -- print("->" .. line .. "<-")
  idx = string.match(line, "^%+CMGL:%s+(%d+)")
  if idx ~= nil then
    -- print(idx)
    local atcmd = "/usr/bin/chat -t 5 -e '' AT+CMGD=" .. idx .. " OK >/dev/ttyS1 </dev/ttyS1 2>/dev/null"
    print(atcmd)
    os.execute(atcmd)
  else
    cmd = string.match(string.upper(line), "GETT")
    if cmd ~= nil then
      do_get_temp = 1
      -- print(cmd)
    end
  end
end
   
f:close()
if do_get_temp ~= nil then
  -- send temp w/o lock
  os.execute("lua /usr/bin/send_temp.lua 0 >/dev/null 2>&1")
  -- os.execute("lua /usr/bin/send_temp.lua 0")
end

if (do_lock == true) then
  print("unlock")
  lock.l_type = p.F_UNLCK
  p.fcntl(fd, p.F_SETLK, lock)
end

os.exit(0)

