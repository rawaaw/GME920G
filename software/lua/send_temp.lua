-- send SMS with current temperatute (in crontab)
local try_count = 0
local sent_ok = false

repeat
  os.execute('send_temp.sh 2>/tmp/send.log')
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

os.exit(0)
