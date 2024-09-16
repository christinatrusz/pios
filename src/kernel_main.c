

char glbl[128];

unsigned long get_timer_count() {
        unsigned long *timer_count_register = 0x3f003004;
        return *timer_count_register;
    }

void wait_1ms() {
    unsigned long start_time = get_timer_count();
    while ((get_timer_count() - start_time) < 1000) {
	// Waiting... 
    }
}

void kernel_main() {

    unsigned long before = get_timer_count();
    wait_1ms();
    unsigned long after = get_timer_count();

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

