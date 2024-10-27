#include "fat.h"
#include <ctype.h> // For toupper
#include <stdio.h> // For printf

// Global variable to hold the Root Directory Entry (RDE) of the currently opened file
struct rde current_rde;

// FatInit
int fatInit() {
    // Read sector 0 from the disk drive into bootSector array
    sd_readblock(0, bootSector, 1);  
    // Point boot_sector struct to the boot sector so we can read fields
    bs = (struct boot_sector*) bootSector;  
    // Print out the elements of the BIOS information block (use rprintf or similar)
    // Example: print bytes per sector
    rprintf("Bytes per sector: %d\n", bs->bytes_per_sector);
    // Validate the boot signature = 0xaa55
    if (bs->boot_signature != 0xaa55) {
        rprintf("Invalid boot signature!\n");
        return -1;  // Error
    }
    // Validate fs_type = "FAT12" using strcmp
    if (strcmp(bs->fs_type, "FAT12") != 0) {
        rprintf("Invalid filesystem type. Expected FAT12.\n");
        return -1;  // Error
    }
    // Read FAT table from the SD card into array fat_table
    int fatStartSector = bs->num_reserved_sectors;  // FAT starts after reserved sectors
    sd_readblock(fatStartSector, fat_table, bs->num_sectors_per_fat);  // Read FAT
    // Compute root_sector
    root_sector = bs->num_fat_tables * bs->num_sectors_per_fat 
                  + bs->num_reserved_sectors 
                  + bs->num_hidden_sectors;
    return 0;
}

// FatOpen
int fatOpen(const char *filename) {
    char uppercase_filename[12];
    int i;
    for (i = 0; i < 11 && filename[i] != '\0'; i++) {
        uppercase_filename[i] = toupper(filename[i]);
    }
    uppercase_filename[i] = '\0';
    // Load root directory sector into memory
    char root_dir_sector[512];
    sd_readblock(root_sector, root_dir_sector, 1);
    // Search for the file in the root directory
    struct rde *entry = (struct rde *)root_dir_sector;
    for (i = 0; i < bs->num_root_dir_entries; i++, entry++) {
        if (strncmp(entry->name, uppercase_filename, 11) == 0) {
            // File found: save RDE information
            current_rde = *entry;
            printf("File %s opened, starting at cluster %d\n", filename, current_rde.first_cluster);
            return 0;
        }
    }
    printf("File %s not found in root directory.\n", filename);
    return -1;
}

// FatRead
int fatRead(FileDescriptor *fd, char *buffer, int size) {
    // Total bytes read from the file
    int bytesRead = 0;
    // Start reading from the file's starting cluster
    int currentCluster = fd->startCluster;
    // Loop until the requested number of bytes is read or the end of the FAT chain is reached
    while (bytesRead < size && currentCluster < END_OF_CHAIN) {
        int bytesToRead = (size - bytesRead < CLUSTER_SIZE) ? size - bytesRead : CLUSTER_SIZE;
        // Read data from the current cluster into the buffer
        readCluster(currentCluster, buffer + bytesRead, bytesToRead);
        
        bytesRead += bytesToRead;
        // Move to the next cluster using the FAT table
        currentCluster = fatTable[currentCluster];
        // Check if the next cluster indicates the end of the file or an error
        if (currentCluster >= END_OF_CHAIN) break;
    }
    // Return the total number of bytes read
    return bytesRead;
}

