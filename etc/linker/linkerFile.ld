INCLUDE etc/linker/attiny2313.ld

MEMORY
{
	FLASH (rx)	: ORIGIN = FLASH_BASE,            LENGTH = FLASH_SIZE
	SRAM  (rw!x)	: ORIGIN = SRAM_BASE + SRAMSTART, LENGTH = SRAM_SIZE
}

SECTIONS
{
	.text :
	{
		_text = .;
		*(.vectors)
		*(.text*)
		_etext = .;
	} > FLASH

	.rodata :
	{
		_srodata = .;
		*(.rodata*)
		_erodata = .;
	} > FLASH

	.data :
	{
		_sdata = .;
		*(.data*)
		_edata = .;
	} > SRAM

	.bss :
	{
		_sbss = .;
		*(.bss*)
		_ebss = .;
	} > SRAM
}


/****                       Notes                       ****/
/*
    - make sure the SECTION declarations have a space between the section name and the semi-colon

*/
