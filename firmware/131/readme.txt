131_modified_config.ext2.zip 
- GATEWAY_IP = 10.138.3.66 in /mnt/config/config
- added SLAVE_IP = 192.168.1.100/SLAVE_NETMASK=255.255.255.0 in /mnt/config/config
- added SMS_PHONE in /mnt/config/config
- added /mnt/config/crontab.tmpl     
    after modification to store new config run savecfg.sh


131_modified_userfs.ext2.zip:
- deleted default files in /mnt/userfs/usr/bin; 

- added: i2c tools, lua, busybox (crontab, vi, ash, less, microcom, tr), am2320term; 

- modified generation /etc/config.sh in 2_prepare-config-script (exclude vpn)
- modified /mnt/userfs/etc/config.templ -- added initialization SLAVE_IP (eth0:1)
                                        -- added exit 0 after interface initialization (string 36)

- modified /mnt/userfs/etc/rc.d/3_boa_config -- all commened out
- modified /mnt/userfs/etc/rc.d/5_config_schema -- added crond and initialization crontab, removed configs for httpd 

- added scripts for thermometer in /usr/bin 
    get_temp.lua
    send_temp.lua
    send_temp.sh
