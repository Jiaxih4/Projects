/**
 * deepfried_dd
 * CS 341 - Fall 2023
 */
#include "format.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>
#include <math.h>
#include <ctype.h>

extern char* optarg;

int main(int argc, char **argv) {
    char* input_name = NULL;
    char* output_name = NULL;
    FILE* input = stdin;
    FILE* output = stdout;

    int leverin = 0;
    int leverout = 0;

    int opt;
    int block_size = 512;
    int block_count = -1;
    int in_block_skip = 0;
    int out_block_skip = 0;

    size_t blocks_copied = 0;

    size_t full_blocks_in = 0;
    size_t partial_blocks_in = 0;
    size_t full_blocks_out = 0;
    size_t partial_blocks_out = 0;
    size_t total_bytes_copied = 0;

    double time_elapsed = 0;
    clock_t start;
    clock_t end;
    start = clock();

    while ((opt = getopt(argc, argv, "i:o:b:c:p:k:")) != -1) {
        switch(opt) {
            case 'i':
                input_name = optarg;
                break;
            case 'o':
                output_name = optarg;
                break;
            case 'b':
                block_size = atoi(optarg);
                break;
            case 'c':
                block_count = atoi(optarg);
                break;
            case 'p':
                in_block_skip = atoi(optarg);
                break;
            case 'k':
                out_block_skip = atoi(optarg);
                break;
            default:
                break;
        }
    }
    if (input_name != NULL) {
        input = fopen(input_name, "r");
        leverin = 1;
        if (input == NULL) {
            print_invalid_input(input_name);
            exit(1);
        }
    }
    if (output_name != NULL) {
        if (access(output_name, F_OK) == 0) {
            output = fopen(output_name, "r+");
            if (output == NULL) {
                print_invalid_output(output_name);
                exit(1);
            }
        } else if (access(output_name, F_OK) == -1) {
            output = fopen(output_name, "w+");
            if (output == NULL) {
                print_invalid_output(output_name);
                exit(1);
            }
        }
        leverout = 1;
    }
    if (leverin && leverout) {
        int fseeki = 0;
        if ((fseeki = fseek(input, in_block_skip * block_size, SEEK_SET)) != 0) {
            fclose(input);
            fclose(output);
            end = clock();
            time_elapsed = ((double)(end - start))/CLOCKS_PER_SEC;
            print_status_report(full_blocks_in, partial_blocks_in, full_blocks_out, partial_blocks_out, total_bytes_copied, time_elapsed);
            exit(1);
        }

        if (fseek(output, out_block_skip * block_size, SEEK_SET) != 0) {
            fclose(input);
            fclose(output);
            end = clock();
            time_elapsed = ((double)(end - start))/CLOCKS_PER_SEC;
            print_status_report(full_blocks_in, partial_blocks_in, full_blocks_out, partial_blocks_out, total_bytes_copied, time_elapsed);
            exit(1);
        }
    }
    char buf[block_size];
    
    while (1) {
        if (feof(input)) {
            break;
        }
        if (blocks_copied < (size_t)block_count || block_count == -1) {
            int bytes_read = fread(buf, 1, block_size, input);
            if (bytes_read == 0) {
                break;
            } 
            if (bytes_read < block_size) {
                partial_blocks_in++;
            } else {
                full_blocks_in++;
            }
            int bytes_written = fwrite(buf, 1, bytes_read, output);
            if (bytes_written < block_size) {
                partial_blocks_out++;
            } else {
                full_blocks_out++;
            }
            total_bytes_copied += bytes_written;
            blocks_copied++;
        } else {
            break;
        }
    }
    end = clock();
    time_elapsed = ((double)(end - start))/CLOCKS_PER_SEC;
    print_status_report(full_blocks_in, partial_blocks_in, full_blocks_out, partial_blocks_out, total_bytes_copied, time_elapsed);
    fclose(input);
    fclose(output);
    return 0;
}

