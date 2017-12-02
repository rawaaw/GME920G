#!/bin/sh
. /etc/config/config

phone=$SMS_PHONE
t=`cat /tmp/t1 /tmp/t2 | tr '\n' ' ' | sed 's/HUM/H/g' | sed 's/TEMP/T/g'`

chat -t 2 -e "" "ATE1" "OK" >/dev/ttyS1 </dev/ttyS1
chat -t 2 -e "" "AT+CREG=2" "OK" >/dev/ttyS1 </dev/ttyS1
chat -t 2 -e "" "AT+CREG?" "OK" >/dev/ttyS1 </dev/ttyS1

chat -t 2 -e "" "AT+CMGS=\"$phone\"\\r$t\\032" "OK" >/dev/ttyS1 </dev/ttyS1

exit 0
