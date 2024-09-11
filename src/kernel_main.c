

char glbl[128];

void kernel_main() {
    
    extern int __bss_start, __bss_end;
    char *bssstart, *bssend;

    bssstart = &__bss_start;
    bssend = &__bss_end;

    // Loop to clear bss from start to end
    for(char *p = bssstart; p < bssend; p++) {
	*p = 0; //Sets each byte to zero
    }

    while(1){
    }
}
