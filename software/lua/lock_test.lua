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

os.execute("sleep " .. tonumber(15))

if (do_lock == true) then
  print("unlock")
  lock.l_type = p.F_UNLCK
  p.fcntl(fd, p.F_SETLK, lock)
end

os.exit(0)

