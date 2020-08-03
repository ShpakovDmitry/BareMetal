MEMORY
{
	FLASH (rx)	: ORIGIN = 0x00000000, LENGTH = 2K
	RAM   (rw!x)	: ORIGIN = 0x00800060, LENGTH = 128
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
