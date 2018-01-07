extern void AT91F_DBGU_Printk(char *);

extern char _stext,_etext,_sdata,_edata;

#define BOOT_VERSION	"boot 1.0.2"

#define SRC 0x10010000
#define DST 0x21f00000
#define LEN 0x020000

void boot() {
  int i;
  char* ptr=(char*)DST;
  for(i=0;i<LEN;i++) ptr[i]=0;
  AT91F_DBGU_Printk ("\n\n\r"BOOT_VERSION" (" __DATE__ " - " __TIME__ ")\n\r");
  AT91F_DBGU_Printk("\n\rUncompressing image...\n\n\r");
  decompress_image(SRC,DST,LEN);
  asm("mov pc,%0" : : "r" (DST));
};

void recover(char* s) {
  for(;;);
};
