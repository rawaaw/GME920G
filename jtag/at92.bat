rem addred 'adapter_khz 1000' into  target/at91rm9200.cfg 
start openocd.exe -f interface/ftdi/c232hm.cfg -f target/at91rm9200.cfg 
