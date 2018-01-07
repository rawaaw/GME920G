неверная инициализация памяти в u-boot 1.0.1
спасибо коллегам за патч (Desktop.7z) !!!
https://electronix.ru/forum/index.php?showtopic=47411&st=30

uboot-reflash.txt - как перешить

====
source code for build boot.bin loader.bin:

AT91RM9200-Boot.tar.gz - archive with source code for boot.bin loader.bin

patches: loader: initialize SDRAM controller for 48LC16M16A2 and for build with arm-linux-gcc-3.4.6 
         boot:   initialize SDRAM controller for 48LC16M16A2 (build with gcc 2.95.3)

