ENTRY(_start)

SECTIONS
{
    .text 0x40000000 : {
        KEEP(*(.text.boot))
        *(.text*)
        . = ALIGN(8);
    }
    .rodata : {
        . = ALIGN(8);
        __global_pointer$ = . + 0x800;
        *(.rodata*)
        *(.srodata*)
        . = ALIGN(8);
    }
    .eh_frame : {
        *(.eh_frame)
    }
    .data : {
        . = ALIGN(8);
        *(.sdata*)
        *(.data*)
        . = ALIGN(8);
    }
    .bss : {
        . = ALIGN(8);
        __bss_start = .;
        *(.sbss*)
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
    }
    _end = .;

    /DISCARD/ : { *(.comment .note) }
}
