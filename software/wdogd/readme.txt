это сервис для AT91 watchdog.
помимо этого система содержит независимый watchdog (см. hardware)

mknod /dev/wdt c 10 130

tftp -r wdogd -g 192.168.1.252
chmod 755 wdogd

kill 758

./wdogd -F -t 2000ms -T 60 -S 1440 -v 2 /dev/wdt

/bin/ext/gme920/wdogd -t 2000ms -T 60 -S 1440 -v 1 /dev/wdt
