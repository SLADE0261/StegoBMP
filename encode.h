#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/*
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo
{
    /* Source Image info */
    char src_image_fname[20];
    FILE *fptr_src_image;
    uint image_capacity;
    char usr_migc_str[10];  // Input magic string
    uint size_usr_migc_str; // Length of input magic string

    /* Secret File Info */
    char secret_fname[20];
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    uint size_extn_secret_file;
    uint size_secret_file;

    /* Stego Image Info */
    char stego_image_fname[20];
    FILE *fptr_stego_image;

} EncodeInfo;

/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(int argc, char *argv[]);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Encode a character and write */
Status encode_8(char *en_char, EncodeInfo *encInfo);

/* Encode an integer and write */
Status encode_32(int *en_int, EncodeInfo *encInfo);

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

/* Encode secret file extenstion lenth and file extenstion */
Status encode_secret_file_extn(EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif
