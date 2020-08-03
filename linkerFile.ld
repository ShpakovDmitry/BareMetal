INCLUDE attiny2313.ld

MEMORY
{
	FLASH (rx)	: ORIGIN = FLASH_BASE, LENGTH = FLASH_SIZE
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

	.data :
	{
		_data = .;
		*(.data*)
		*(.bss*)
		*(.rodata*)
		_edata = .;
	} > SRAM
}


/****                       Notes                       ****/
/*
    - make sure the SECTION declarations have a space between the section name and the semi-colon

*/
