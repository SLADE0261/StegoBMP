
#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/*
 * Structure to store information required for
 * decoding secret file from stego image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Input Image info */
    char inp_image_fname[20]; // Input stego image filename
    FILE *fptr_inp_image;     // File pointer for input stego image
    char usr_migc_str[10];    // Input magic string
    uint size_usr_migc_str;   // Length of input magic string

    /* Output File Info */
    char out_fname[20];                  // Output filename for decoded secret
    FILE *fptr_out;                      // File pointer for output file
    long size_extn_out_file;             // Size of secret file extension
    char extn_out_file[MAX_FILE_SUFFIX]; // Secret file extension
    long size_out_file;                  // Size of decoded secret file

} DecodeInfo;

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(int argc, char *argv[]); // Validate decode arguments

/* Get File pointers for input and output files */
Status open_files_dec(DecodeInfo *decInfo); // Open files for decoding

/* Check magic string in stego image */
Status magic_string_status(const char *magic_string, DecodeInfo *decInfo); // Verify magic string

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo); // Main decoding function

/* Get size of secret file extension */
Status get_size_extn_out_file(DecodeInfo *decInfo); // Get extension size

/* Get secret file extension */
Status get_extn_out_file(DecodeInfo *decInfo); // Get extension

/* Get size of secret file */
Status get_size_out_file(DecodeInfo *decInfo); // Get secret file size

/* Write decoded data to output file */
Status write_out_file(DecodeInfo *decInfo); // Write secret data to file

/* Decode a character from image */
Status decode_8(DecodeInfo *decInfo); // Decode 8 bits to a char

#endif