#!/bin/sh
. /etc/config/config

phone=$SMS_PHONE
t=`cat /tmp/t1 /tmp/t2 2>/dev/null`
res=$?
try_count=0
while [ $res -gt 0 ];  do
  try_count=$((try_count+1))
  if [ $try_count -gt 3 ]; then
    exit 1
  fi
  t=`cat /tmp/t1 /tmp/t2 2>/dev/null`
  res=$?
  sleep 1
#  echo try cat
done

t=`echo $t | tr '\n' ' ' | sed 's/HUM/H/g' | sed 's/TEMP/T/g'`

/usr/bin/chat -t 2 -e "" "ATE1" "OK" >/dev/ttyS1 </dev/ttyS1
/usr/bin/chat -t 2 -e "" "AT+CREG=2" "OK" >/dev/ttyS1 </dev/ttyS1
/usr/bin/chat -t 2 -e "" "AT+CREG?" "OK" >/dev/ttyS1 </dev/ttyS1

/usr/bin/chat -t 2 -e "" "AT+CMGS=\"$phone\"\\r$t\\032" "OK" >/dev/ttyS1 </dev/ttyS1

exit 0

