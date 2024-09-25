#define MU_IO 0x3F215040  // For Pi 3, use 0xFE215004 for Pi 4

void putc(int data) {
    // Cast the address to a pointer and assign the character data
    *(volatile unsigned int *)MU_IO = data;
}

void print_current_execution_level() {
    esp_printf(putc, "Current Execution Level is %d\r\n", get_current_el());
}
