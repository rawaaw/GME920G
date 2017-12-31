#!/bin/sh
. /etc/config/config

phone=$SMS_PHONE
t=`cat /tmp/t1 /tmp/t2 | tr '\n' ' ' | sed 's/HUM/H/g' | sed 's/TEMP/T/g'`

chat TIMEOUT 2 ECHO 1 "" "ATE1" "OK" >/dev/ttyS1 </dev/ttyS1
chat TIMEOUT 2 ECHO 1 "" "AT+CREG=2" "OK" >/dev/ttyS1 </dev/ttyS1
chat TIMEOUT 2 ECHO 1 "" "AT+CREG?" "OK" >/dev/ttyS1 </dev/ttyS1

chat TIMEOUT 2 ECHO 1 "" "AT+CMGS=\"$phone\"\\r$t\\032" "OK" >/dev/ttyS1 </dev/ttyS1

exit 0

