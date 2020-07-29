MEMORY
{
	FLASH (rx)	: ORIGIN = 0x00000000, LENGTH = 2K
	RAM   (rw!x)	: ORIGIN = 0x00800000, LENGTH = 128
}

SECTIONS
{
	.text :
	{
		./object/runtime/startup.o (INT_VECTOR)
		*(.text*)
		. = ALIGN(2);
	} > FLASH

	.rodata :
	{
		*(.rodata*)
		. = ALIGN(1);
	} > FLASH

	.data :
	{
		*(.data*)
		. = ALIGN(1);
	} > RAM

	.bss :
	{
		*(.bss*)
		. = ALIGN(1);
	} > RAM
}

ENTRY( _Reset ) /* refers to _Reset vector table in assembly file */




/****                       Notes                       ****/
/*
    - make sure the SECTION declarations have a space between the section name and the semi-colon

*/
