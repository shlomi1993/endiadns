// 311408264 Shlomi Ben-Shushan

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Const flags.
const char* UNX_FLAG = "-unix";
const char* MAC_FLAG = "-mac";
const char* WIN_FLAG = "-win";
const char* KEEP = "-keep";
const char* SWAP = "-swap";

// Const new line marks.
const char UNX_NL[1] = {0x000a};
const char MAC_NL[1] = {0x000d};
const char WIN_NL[2] = {0x000d, 0x000a};

/********************************************************************************
* Function:     swap                                                            *
* Input:        Two-bytes sting                                                 *
* Output:       Void.                                                           *
* Operation:    Simple function that swap the two bytes.                        *
********************************************************************************/
void swap(char* buffer) {
    char temp = buffer[0];
    buffer[0] = buffer[1];
    buffer[1] = temp;
}

/********************************************************************************
* Function:     checkArgs                                                       *
* Input:        Main's arguments - argc and argv.                               *
* Output:       an int, 1 or 0.                                                 *
* Operation:    Look for an illegal argument. Returns 0 if found one, 1 o.w.    *
********************************************************************************/
int checkArgs(int argc, char *argv[]) {

    // Check argc.
    if (argc != 3 && argc != 5 && argc != 6)
        return 0;

    // Check for identical source and destination files.
    if (!strcmp(argv[1], argv[2]))
        return 0;

    // Verify OS-flags arguments.
    if (argc > 3) {
        if (strcmp(argv[3], UNX_FLAG) && strcmp(argv[3], MAC_FLAG) && strcmp(argv[3], WIN_FLAG))
            return 0;
        if (strcmp(argv[4], UNX_FLAG) && strcmp(argv[4], MAC_FLAG) && strcmp(argv[4], WIN_FLAG))
            return 0;
    }

    // Verify endian-flag arguments.
    if (argc == 6) {
        if (strcmp(argv[5], KEEP) && strcmp(argv[5], SWAP))
            return 0;
    }

    // If all pass, return 1
    return 1;

}

/********************************************************************************
* Function:     main                                                            *
* Input:        Main's arguments - argc and argv.                               *
* Output:       Int.                                                            *
* Operation:    The entry point of the program.                                 *
********************************************************************************/
int main(int argc, char *argv[]) {

    // Verifies arguments.
    if (checkArgs(argc, argv) == 0)
        return 0;
    
    // Open files and verify existence.
    FILE *src = fopen(argv[1], "r");
    if (src == NULL)
        return 0;
    FILE *dst = fopen(argv[2], "w");
    if (dst == NULL)
        return 0;
    fclose(dst);
    dst = fopen(argv[2], "a");
    if (dst == NULL)
        return 0;

    // Set buffer and Initialize n = number of read bytes.
    char buffer[2];
    int n;

    // Option 1: arguments for source file name and destination file name.
    if (argc == 3) {
        do {
            n = fread(&buffer, sizeof(char), 2, src);
            fwrite(buffer, sizeof(char), n, dst);
        } while(n > 0);
    }
    
    // Option 2: as before plus flags arguments for OS.
    else if (argc == 5) {

        // Retrieve additioal arguments.
        const char* src_os = argv[3];
        const char* dst_os = argv[4];    
        
        // If source os and destination os are the same, act like in case 1.
        if (!strcmp(src_os, dst_os)) {
            do {
                n = fread(&buffer, sizeof(char), 2, src);
                fwrite(buffer, sizeof(char), n, dst);
            } while (n > 0);
        }

        // Handle sub-case Unix -> Mac.
        else if (!strcmp(src_os, UNX_FLAG) && !strcmp(dst_os, MAC_FLAG)) {
            do {
                n = fread(&buffer, sizeof(char), 2, src);
                if (buffer[0] == UNX_NL[0]) {
                    buffer[0] = MAC_NL[0];
                } else if (buffer[1] == UNX_NL[0]) {
                    buffer[1] = MAC_NL[0];
                }
                fwrite(buffer, sizeof(char), n, dst);
            } while (n > 0);
        }

        // Handle sub-case Unix -> Windows.
        else if (!strcmp(src_os, UNX_FLAG) && !strcmp(dst_os, WIN_FLAG)) {
            do {
                n = fread(&buffer, sizeof(char), 2, src);
                if (buffer[0] == UNX_NL[0]) {
                    buffer[0] = WIN_NL[0];
                    fwrite(buffer, sizeof(char), n, dst);
                    buffer[0] = WIN_NL[1];
                    fwrite(buffer, sizeof(char), n, dst); 
                } else if (buffer[1] == UNX_NL[0]) {
                    buffer[1] = WIN_NL[0];
                    fwrite(buffer, sizeof(char), n, dst);
                    buffer[1] = WIN_NL[1];
                    fwrite(buffer, sizeof(char), n, dst); 
                }
                else {
                    fwrite(buffer, sizeof(char), n, dst); 
                }
            } while (n > 0);
        }

        // Handle sub-case Mac -> Unix.
        else if (!strcmp(src_os, MAC_FLAG) && !strcmp(dst_os, UNX_FLAG)) {
            do {
                n = fread(&buffer, sizeof(char), 2, src);
                if (buffer[0] == MAC_NL[0]) {
                    buffer[0] = UNX_NL[0];
                } else if (buffer[1] == MAC_NL[0]) {
                    buffer[1] = UNX_NL[0];
                }
                fwrite(buffer, sizeof(char), n, dst);
            } while (n > 0);
        }

        // Handle sub-case Mac -> Windows.
        else if (!strcmp(src_os, MAC_FLAG) && !strcmp(dst_os, WIN_FLAG)) {
            do {
                n = fread(&buffer, sizeof(char), 2, src);
                if (buffer[0] == MAC_NL[0]) {
                    buffer[0] = WIN_NL[0];
                    fwrite(buffer, sizeof(char), n, dst);
                    buffer[0] = WIN_NL[1];
                    fwrite(buffer, sizeof(char), n, dst); 
                } else if (buffer[1] == MAC_NL[0]) {
                    buffer[1] = WIN_NL[0];
                    fwrite(buffer, sizeof(char), n, dst);
                    buffer[1] = WIN_NL[1];
                    fwrite(buffer, sizeof(char), n, dst); 
                }
                else {
                    fwrite(buffer, sizeof(char), n, dst); 
                }       
            } while (n > 0);      
        }

        // Handle sub-case Windows -> Unix.
        else if (!strcmp(src_os, WIN_FLAG) && !strcmp(dst_os, UNX_FLAG)) {
            do {
                n = fread(&buffer, sizeof(char), 2, src);
                if (buffer[0] == WIN_NL[0]) {
                    n = fread(&buffer, sizeof(char), 2, src);
                    if (buffer[0] == WIN_NL[1]) {
                        buffer[0] = UNX_NL[0];
                    }
                } else if (buffer[1] == WIN_NL[0]) {
                    n = fread(&buffer, sizeof(char), 2, src);
                    if (buffer[1] == WIN_NL[1]) {
                        buffer[1] = UNX_NL[0];
                    }
                }
                fwrite(buffer, sizeof(char), n, dst);
            } while (n > 0);
        }

        // Handle sub-case Windows -> Mac.
        else if (!strcmp(src_os, WIN_FLAG) && !strcmp(dst_os, MAC_FLAG)) {
            do {
                n = fread(&buffer, sizeof(char), 2, src);
                if (buffer[0] == WIN_NL[0]) {
                    n = fread(&buffer, sizeof(char), 2, src);
                    if (buffer[0] == WIN_NL[1]) {
                        buffer[0] = MAC_NL[0];
                    }
                } else if (buffer[1] == WIN_NL[0]) {
                    n = fread(&buffer, sizeof(char), 2, src);
                    if (buffer[1] == WIN_NL[1]) {
                        buffer[1] = MAC_NL[0];
                    }
                }
                fwrite(buffer, sizeof(char), n, dst);
            } while (n > 0);
        }

    }
    
    // Option 3: as before plus endian switching flag.
    else if (argc == 6) {
        
        // Retrieve additioal arguments.
        const char* src_os = argv[3];
        const char* dst_os = argv[4];
        const char* endian = argv[5];

        // Switch endians case.
        if (!strcmp(endian, SWAP)) {

            // If source os and destination os are the same, just switch endians.
            if (!strcmp(src_os, dst_os)) {
                do {
                    n = fread(&buffer, sizeof(char), 2, src);
                    swap(buffer);
                    fwrite(buffer, sizeof(char), n, dst);
                } while (n > 0);
            }

            // Handle sub-case Unix -> Mac.
            else if (!strcmp(src_os, UNX_FLAG) && !strcmp(dst_os, MAC_FLAG)) {
                do {
                    n = fread(&buffer, sizeof(char), 2, src);
                    swap(buffer);
                    if (buffer[0] == UNX_NL[0]) {
                        buffer[0] = MAC_NL[0];
                    } else if (buffer[1] == UNX_NL[0]) {
                        buffer[1] = MAC_NL[0];
                    }
                    fwrite(buffer, sizeof(char), n, dst);
                } while (n > 0);
            }

            // Handle sub-case Unix -> Windows.
            else if (!strcmp(src_os, UNX_FLAG) && !strcmp(dst_os, WIN_FLAG)) {
                do {
                    n = fread(&buffer, sizeof(char), 2, src);
                    swap(buffer);
                    if (buffer[0] == UNX_NL[0]) {
                        buffer[0] = WIN_NL[0];
                        fwrite(buffer, sizeof(char), n, dst);
                        buffer[0] = WIN_NL[1];
                        fwrite(buffer, sizeof(char), n, dst); 
                    } else if (buffer[1] == UNX_NL[0]) {
                        buffer[1] = WIN_NL[0];
                        fwrite(buffer, sizeof(char), n, dst);
                        buffer[1] = WIN_NL[1];
                        fwrite(buffer, sizeof(char), n, dst); 
                    }
                    else {
                        fwrite(buffer, sizeof(char), n, dst); 
                    }
                } while (n > 0);
            }

            // Handle sub-case Mac -> Unix.
            else if (!strcmp(src_os, MAC_FLAG) && !strcmp(dst_os, UNX_FLAG)) {
                do {
                    n = fread(&buffer, sizeof(char), 2, src);
                    swap(buffer);
                    if (buffer[0] == MAC_NL[0]) {
                        buffer[0] = UNX_NL[0];
                    } else if (buffer[1] == MAC_NL[0]) {
                        buffer[1] = UNX_NL[0];
                    }
                    fwrite(buffer, sizeof(char), n, dst);
                } while (n > 0);
            }

            // Handle sub-case Mac -> Windows.
            else if (!strcmp(src_os, MAC_FLAG) && !strcmp(dst_os, WIN_FLAG)) {
                do {
                    n = fread(&buffer, sizeof(char), 2, src);
                    swap(buffer);
                    if (buffer[0] == MAC_NL[0]) {
                        buffer[0] = WIN_NL[0];
                        fwrite(buffer, sizeof(char), n, dst);
                        buffer[0] = WIN_NL[1];
                        fwrite(buffer, sizeof(char), n, dst); 
                    } else if (buffer[1] == MAC_NL[0]) {
                        buffer[1] = WIN_NL[0];
                        fwrite(buffer, sizeof(char), n, dst);
                        buffer[1] = WIN_NL[1];
                        fwrite(buffer, sizeof(char), n, dst); 
                    } else {
                        fwrite(buffer, sizeof(char), n, dst); 
                    }       
                } while (n > 0);      
            }

            // Handle sub-case Windows -> Unix.
            else if (!strcmp(src_os, WIN_FLAG) && !strcmp(dst_os, UNX_FLAG)) {
                do {
                    n = fread(&buffer, sizeof(char), 2, src);
                    swap(buffer);
                    if (buffer[0] == WIN_NL[0]) {
                        n = fread(&buffer, sizeof(char), 2, src);
                        swap(buffer);
                        if (buffer[0] == WIN_NL[1]) {
                            buffer[0] = UNX_NL[0];
                        }
                    } else if (buffer[1] == WIN_NL[0]) {
                        n = fread(&buffer, sizeof(char), 2, src);
                        swap(buffer);
                        if (buffer[1] == WIN_NL[1]) {
                            buffer[1] = UNX_NL[0];
                        }
                    }
                    fwrite(buffer, sizeof(char), n, dst);
                } while (n > 0);
            }

            // Handle sub-case Windows -> Mac.
            else if (!strcmp(src_os, WIN_FLAG) && !strcmp(dst_os, MAC_FLAG)) {
                do {
                    n = fread(&buffer, sizeof(char), 2, src);
                    swap(buffer);
                    if (buffer[0] == WIN_NL[0]) {
                        n = fread(&buffer, sizeof(char), 2, src);
                        swap(buffer);
                        if (buffer[0] == WIN_NL[1]) {
                            buffer[0] = MAC_NL[0];
                        }
                    } else if (buffer[1] == WIN_NL[0]) {
                        n = fread(&buffer, sizeof(char), 2, src);
                        swap(buffer);
                        if (buffer[1] == WIN_NL[1]) {
                            buffer[1] = MAC_NL[0];
                        }
                    }
                    fwrite(buffer, sizeof(char), n, dst);
                } while (n > 0);
            }

        }
        
        // Keep endians case.
        else if (!strcmp(endian, KEEP)) {
            char* arr[5] = {argv[0], argv[1], argv[2], argv[3], argv[4]};
            main(5, arr);
        }

    }
    
    // Close files and finish program.
    fclose(src);
    fclose(dst);
	return (0);
    
}