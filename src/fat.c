#include "fat.h"
#include <ctype.h> // For toupper
#include <string.h> // For strcmp and strncpy

// Global variables for the FAT file system driver
struct boot_sector *bs;
char bootSector[512];
char fat_table[8 * 512]; // Adjust size based on filesystem
unsigned int root_sector;
struct root_directory_entry current_rde; // Holds the RDE of the currently opened file

// fatInit: Initializes the FAT filesystem by reading the boot sector and FAT
int fatInit() {
    // Read the boot sector (sector 0) into bootSector array
    sd_readblock(0, bootSector, 1);  
    bs = (struct boot_sector*) bootSector;  

    // Validate the boot signature = 0xaa55
    if (bs->boot_signature != 0xaa55) {
        return -1;  // Invalid boot signature
    }

    // Validate fs_type = "FAT12" using strcmp
    if (strcmp(bs->fs_type, "FAT12") != 0) {
        return -1;  // Invalid filesystem type
    }

    // Read FAT table from the SD card into fat_table array
    int fatStartSector = bs->num_reserved_sectors;
    sd_readblock(fatStartSector, fat_table, bs->num_sectors_per_fat); 

    // Compute root sector
    root_sector = bs->num_fat_tables * bs->num_sectors_per_fat 
                  + bs->num_reserved_sectors 
                  + bs->num_hidden_sectors;

    return 0;
}

// fatOpen: Opens a file by locating its root directory entry (RDE)
int fatOpen(const char *filename) {
    // Convert filename to uppercase for comparison
    char uppercase_filename[12] = {0};
    int i;
    for (i = 0; i < 11 && filename[i] != '\0'; i++) {
        uppercase_filename[i] = toupper(filename[i]);
    }

    // Load the root directory sector into memory
    char root_dir_sector[512];
    sd_readblock(root_sector, root_dir_sector, 1);

    // Search for the file in the root directory
    struct root_directory_entry *entry = (struct root_directory_entry *)root_dir_sector;
    for (i = 0; i < bs->num_root_dir_entries; i++, entry++) {
        if (strncmp(entry->file_name, uppercase_filename, 11) == 0) {
            // File found: save RDE information
            current_rde = *entry;
            return 0;  // File found
        }
    }

    return -1;  // File not found
}

// fatRead: Reads data from the opened file into the buffer
int fatRead(char *buffer, int bytes_to_read) {
    if (current_rde.file_size < bytes_to_read) {
        bytes_to_read = current_rde.file_size;  // Adjust bytes to read if file is smaller
    }

    int cluster = current_rde.cluster;
    int bytes_read = 0;

    // Read each cluster until all requested bytes are read
    while (bytes_to_read > 0) {
        int sector = root_sector + (cluster - 2) * bs->num_sectors_per_cluster;
        sd_readblock(sector, buffer + bytes_read, bs->num_sectors_per_cluster);

        bytes_read += bs->bytes_per_sector * bs->num_sectors_per_cluster;
        bytes_to_read -= bs->bytes_per_sector * bs->num_sectors_per_cluster;

        // Get the next cluster from the FAT
        int fat_entry = fat_table[cluster];  // Modify based on FAT indexing
        if (fat_entry >= 0xFF8) break;  // End of file
        cluster = fat_entry;
    }

    return bytes_read;
}

