FLASH_BASE = 0x00000000;
SRAM_BASE  = 0x00800000;

REG_FILE_SIZE = 0x20;
IO_FILE_SIZE  =  0x40;
SRAM_SIZE     = 0x80;
FLASH_SIZE    = 2K;

SRAMEND   = REG_FILE_SIZE + IO_FILE_SIZE + SRAM_SIZE;
SRAMSTART = REG_FILE_SIZE + IO_FILE_SIZE;


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
	} > RAM
}


/****                       Notes                       ****/
/*
    - make sure the SECTION declarations have a space between the section name and the semi-colon

*/
