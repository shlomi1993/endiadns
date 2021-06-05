// Shlomi Ben-Shushan

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Const flags.
const char *UNX = "-unix";
const char *MAC = "-mac";
const char *WIN = "-win";
const char *KEEP = "-keep";
const char *SWAP = "-swap";

// Const new-line marks.
const char LF = 0x000a;
const char CR = 0x000d;
const char CRLF[2] = {0x000d, 0x000a};

/********************************************************************************
* Function:     isValidArgs                                                     *
* Input:        Main's arguments - argc and argv.                               *
* Output:       an int, 1 or 0.                                                 *
* Operation:    Look for an illegal argument. Returns 0 if found one, 1 o.w.    *
********************************************************************************/
int isValidArgs(int argc, char *argv[]) {
    // Check argc.
    if (argc != 3 && argc != 5 && argc != 6)
        return 0;

    // Check for identical source and destination files.
    if (!strcmp(argv[1], argv[2]))
        return 0;

    // Verify OS-flags arguments.
    if (argc > 3) {
        if (strcmp(argv[3], UNX) && strcmp(argv[3], MAC) && strcmp(argv[3], WIN))
            return 0;
        if (strcmp(argv[4], UNX) && strcmp(argv[4], MAC) && strcmp(argv[4], WIN))
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
* Function:     isBigEndian                                                     *
* Input:        Two-bytes string                                                *
* Output:       int - 1 for BE, 0 for LE.                                       *
* Operation:    Uses BOM to check file endianity. Returns 1 for BE, 0 O.W.      *
********************************************************************************/
int isBigEndian(char *buffer) {
    if (buffer[0] == (char)0xFF)
        return 0;
    return 1;
}

/********************************************************************************
* Function:     swap                                                            *
* Input:        Two-bytes string                                                *
* Output:       Void.                                                           *
* Operation:    Simple function that swap the two bytes.                        *
********************************************************************************/
void swap(char *buffer) {
    char temp = buffer[0];
    buffer[0] = buffer[1];
    buffer[1] = temp;
}

/********************************************************************************
* Function:     main                                                            *
* Input:        Main's arguments - argc and argv.                               *
* Output:       Int.                                                            *
* Operation:    The entry point of the program.                                 *
********************************************************************************/
int main(int argc, char *argv[]) {
    // Verifies arguments.
    if (!isValidArgs(argc, argv))
        return 0;

    // Open files and verify existence.
    FILE *src = fopen(argv[1], "rb");
    if (src == NULL)
        return 0;
    FILE *dst = fopen(argv[2], "wb");
    if (dst == NULL)
        return 0;

    // Set buffer and save char and buffer sizes.
    char buffer[2];
    int char_size = sizeof(char);
    int buffer_size = sizeof(buffer);

    // Option 1: arguments for source file name and destination file name.
    if (argc == 3) {
        while (fread(&buffer, buffer_size, char_size, src))
            fwrite(buffer, buffer_size, char_size, dst);
    }

    // Option 2: as before plus flags arguments for OS.
    else if (argc == 5) {
        // Retrieve additioal arguments.
        const char *src_os = argv[3];
        const char *dst_os = argv[4];

        // Check endianity.
        fread(&buffer, buffer_size, char_size, src);
        int i = isBigEndian(buffer);
        fwrite(buffer, buffer_size, char_size, dst);

        // If source os and destination os are the same, act like in case 1.
        if (!strcmp(src_os, dst_os)) {
            while (fread(&buffer, buffer_size, char_size, src))
                fwrite(buffer, buffer_size, char_size, dst);
        }
        // Handle sub-case Unix -> Mac.
        else if (!strcmp(src_os, UNX) && !strcmp(dst_os, MAC)) {
            while (fread(&buffer, buffer_size, char_size, src)) {
                if (buffer[i] == LF && buffer[1 - i] == (char)0x0)
                    buffer[i] = CR;
                fwrite(buffer, buffer_size, char_size, dst);
            }
        }
        // Handle sub-case Unix -> Windows.
        else if (!strcmp(src_os, UNX) && !strcmp(dst_os, WIN)) {
            while (fread(&buffer, buffer_size, char_size, src)) {
                if (buffer[i] == LF && buffer[1 - i] == (char)0x0) {
                    buffer[i] = CRLF[0];
                    fwrite(buffer, buffer_size, char_size, dst);
                    buffer[i] = CRLF[1];
                    fwrite(buffer, buffer_size, char_size, dst);
                } else
                    fwrite(buffer, buffer_size, char_size, dst);
            }
        }
        // Handle sub-case Mac -> Unix.
        else if (!strcmp(src_os, MAC) && !strcmp(dst_os, UNX)) {
            while (fread(&buffer, buffer_size, char_size, src)) {
                if (buffer[i] == CR && buffer[1 - i] == (char)0x0)
                    buffer[i] = LF;
                fwrite(buffer, buffer_size, char_size, dst);
            }
        }
        // Handle sub-case Mac -> Windows.
        else if (!strcmp(src_os, MAC) && !strcmp(dst_os, WIN)) {
            while (fread(&buffer, buffer_size, char_size, src)) {
                if (buffer[i] == CR && buffer[1 - i] == (char)0x0) {
                    buffer[i] = CRLF[0];
                    fwrite(buffer, buffer_size, char_size, dst);
                    buffer[i] = CRLF[1];
                    fwrite(buffer, buffer_size, char_size, dst);
                } else
                    fwrite(buffer, buffer_size, char_size, dst);
            }
        }
        // Handle sub-case Windows -> Unix.
        else if (!strcmp(src_os, WIN) && !strcmp(dst_os, UNX)) {
            while (fread(&buffer, buffer_size, char_size, src)) {
                if (buffer[i] == CRLF[0] && buffer[1 - i] == (char)0x0) {
                    fread(&buffer, buffer_size, char_size, src);
                    if (buffer[i] == CRLF[1] && buffer[1 - i] == (char)0x0)
                        buffer[i] = LF;
                }
                fwrite(buffer, buffer_size, char_size, dst);
            }
        }
        // Handle sub-case Windows -> Mac.
        else if (!strcmp(src_os, WIN) && !strcmp(dst_os, MAC)) {
            while (fread(&buffer, buffer_size, char_size, src)) {
                if (buffer[i] == CRLF[0] && buffer[1 - i] == (char)0x0) {
                    fread(&buffer, buffer_size, char_size, src);
                    if (buffer[i] == CRLF[1] && buffer[1 - i] == (char)0x0)
                        buffer[i] = CR;
                }
                fwrite(buffer, buffer_size, char_size, dst);
            }
        }
    }

    // Option 3: as before plus endian switching flag.
    else if (argc == 6) {
        // Retrieve additioal arguments.
        const char *src_os = argv[3];
        const char *dst_os = argv[4];

        // Keep endians case.
        if (!strcmp(argv[5], KEEP)) {
            char *arr[5] = {argv[0], argv[1], argv[2], argv[3], argv[4]};
            main(5, arr);
        }
        // Switch endians case.
        else if (!strcmp(argv[5], SWAP)) {
            // Check endianity.
            fread(&buffer, buffer_size, char_size, src);
            int i = 1 - isBigEndian(buffer);
            swap(buffer);
            fwrite(buffer, buffer_size, char_size, dst);

            // If source os and destination os are the same, just switch endians.
            if (!strcmp(src_os, dst_os)) {
                while (fread(&buffer, buffer_size, char_size, src)) {
                    swap(buffer);
                    fwrite(buffer, buffer_size, char_size, dst);
                }
            }
            // Handle sub-case Unix -> Mac.
            else if (!strcmp(src_os, UNX) && !strcmp(dst_os, MAC)) {
                while (fread(&buffer, buffer_size, char_size, src)) {
                    swap(buffer);
                    if (buffer[i] == LF && buffer[1 - i] == (char)0x0)
                        buffer[i] = CR;
                    fwrite(buffer, buffer_size, char_size, dst);
                }
            }
            // Handle sub-case Unix -> Windows.
            else if (!strcmp(src_os, UNX) && !strcmp(dst_os, WIN)) {
                while (fread(&buffer, buffer_size, char_size, src)) {
                    swap(buffer);
                    if (buffer[i] == LF && buffer[1 - i] == (char)0x0) {
                        buffer[i] = CRLF[0];
                        fwrite(buffer, buffer_size, char_size, dst);
                        buffer[i] = CRLF[1];
                        fwrite(buffer, buffer_size, char_size, dst);
                    } else
                        fwrite(buffer, buffer_size, char_size, dst);
                }
            }
            // Handle sub-case Mac -> Unix.
            else if (!strcmp(src_os, MAC) && !strcmp(dst_os, UNX)) {
                while (fread(&buffer, buffer_size, char_size, src)) {
                    swap(buffer);
                    if (buffer[i] == CR && buffer[1 - i] == (char)0x0)
                        buffer[i] = LF;
                    fwrite(buffer, buffer_size, char_size, dst);
                }
            }
            // Handle sub-case Mac -> Windows.
            else if (!strcmp(src_os, MAC) && !strcmp(dst_os, WIN)) {
                while (fread(&buffer, buffer_size, char_size, src)) {
                    swap(buffer);
                    if (buffer[i] == CR && buffer[1 - i] == (char)0x0) {
                        buffer[i] = CRLF[0];
                        fwrite(buffer, buffer_size, char_size, dst);
                        buffer[i] = CRLF[1];
                        fwrite(buffer, buffer_size, char_size, dst);
                    } else
                        fwrite(buffer, buffer_size, char_size, dst);
                }
            }
            // Handle sub-case Windows -> Unix.
            else if (!strcmp(src_os, WIN) && !strcmp(dst_os, UNX)) {
                while (fread(&buffer, buffer_size, char_size, src)) {
                    swap(buffer);
                    if (buffer[i] == CRLF[0] && buffer[1 - i] == (char)0x0) {
                        fread(&buffer, buffer_size, char_size, src);
                        swap(buffer);
                        if (buffer[i] == CRLF[1] && buffer[1 - i] == (char)0x0)
                            buffer[i] = LF;
                    }
                    fwrite(buffer, buffer_size, char_size, dst);
                }
            }
            // Handle sub-case Windows -> Mac.
            else if (!strcmp(src_os, WIN) && !strcmp(dst_os, MAC)) {
                while (fread(&buffer, buffer_size, char_size, src)) {
                    swap(buffer);
                    if (buffer[i] == CRLF[0] && buffer[1 - i] == (char)0x0) {
                        fread(&buffer, buffer_size, char_size, src);
                        swap(buffer);
                        if (buffer[i] == CRLF[1] && buffer[1 - i] == (char)0x0)
                            buffer[i] = CR;
                    }
                    fwrite(buffer, buffer_size, char_size, dst);
                }
            }
        }
        // Close files and finish program.
        fclose(src);
        fclose(dst);
        return 0;
    }
}