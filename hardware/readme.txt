reset_schematics.jpg - независимая схема сброса (не использует возможности watchdog AT91)
управляется через подачу импульсов с ножки PB28.
Если подача импульсов прекращается, то примерно через 1.5 минуты система сбрасывается.

Если нет желания использовать штатный wdogd от старой прошивки, управление можно сделать так:
(см. ../software/2.6.39/linux-at91/arch/arm/mach-at91/board-rm9200dk.c):
	{
		.name			= "watchdog",
		.gpio			= AT91_PIN_PB28, // PB28 initializes reset schematic (reset_schematics.jpg)
		.active_low		= 0,
		.default_trigger	= "heartbeat",
	}


желтый, оранжевый - leds
зеленый  - сброс модема (используется в mdmrst)
фиолетовый  - управление схемой сброса (используется в оригинальном wdogd и gme920_ready)

не знаю, что такое четырехногая микросхема с маркировкий .AEA на cpubrd_bottom_180.jpg (синий кружок) - видимо, супервизор питания.

В leds.txt расписана привязка с ножкам процессора