

char glbl[128];

unsigned long get_timer_count() {
        unsigned long *timer_count_register = 0x3f003004;
        return *timer_count_register;
    }

void kernel_main() {

    get_timer_count();

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

