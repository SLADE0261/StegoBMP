
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

Status read_and_validate_encode_args(int argc, char *argv[])
{
    // Check for correct number of arguments
    if ((argc < 5) || (6 < argc))
    {
        printf("ERROR: Not enough arguments.\n");
        return e_failure;
    }

    // Check input file type is .bmp
    if (strstr(argv[2], ".bmp") == NULL)
    {
        printf("ERROR: Incorrect input file type.\n");
        return e_failure;
    }

    // Check secret file type is .txt
    // if (strstr(argv[3], ".txt") == NULL)
    // {
    //     printf("ERROR: Incorrect secret file type.\n");
    //     return e_failure;
    // }

    // Check output file type is .bmp (if provided)
    if (argc == 6)
    {
        if (strstr(argv[4], ".bmp") == NULL)
        {
            printf("ERROR: Incorrect output file type.\n");
            return e_failure;
        }
    }

    return e_success;
}

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    fseek(fptr_image, 18, SEEK_SET);            // Seek to width in BMP header
    fread(&width, sizeof(int), 1, fptr_image);  // Read width
    fread(&height, sizeof(int), 1, fptr_image); // Read height
    fseek(fptr_image, 0, SEEK_SET);             // Reset file pointer
    return width * height * 3;                  // Return image capacity in bytes
}

Status open_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Fail if source image cannot be opened
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
        return e_failure;
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Fail if secret file cannot be opened
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
        return e_failure;
    }

    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Fail if stego image cannot be opened
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
        return e_failure;
    }

    return e_success;
}

OperationType check_operation_type(char *argv[])
{
    // Determine operation type from argument
    if (strcmp(argv[1], "-e") == 0)
        return e_encode;
    else if (strcmp(argv[1], "-d") == 0)
        return e_decode;
    else
        return e_unsupported;
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);     // Go to end of file
    long file_size = ftell(fptr); // Get file size
    fseek(fptr, 0, SEEK_SET);     // Reset file pointer
    return file_size;
}

Status check_capacity(EncodeInfo *encInfo)
{
    // Check if image can hold secret file
    if ((encInfo->size_secret_file + (encInfo->size_usr_migc_str) + (encInfo->size_extn_secret_file) + ((uint)4) + ((uint)4)) > encInfo->image_capacity)
    {
        return e_failure;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "check_capacity");
        return e_success;
    }
}

Status do_encoding(EncodeInfo *encInfo)
{
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
    {
        return e_failure;
    }

    encode_magic_string(encInfo->usr_migc_str, encInfo); // Encode magic string

    encInfo->extn_secret_file[0] = '.';
    sscanf(encInfo->secret_fname, "%*[^.].%s", &encInfo->extn_secret_file[1]); // Extract extension
    encode_secret_file_extn(encInfo);                                          // Encode extension

    encode_secret_file_size(encInfo->size_secret_file, encInfo); // Encode secret file size

    encode_secret_file_data(encInfo); // Encode secret file data

    copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image); // Copy rest of image

    return e_success;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    // Copy BMP header (first 54 bytes)
    char ch;
    for (int i = 0; i < 54; i++)
    {
        ch = fgetc(fptr_src_image);
        putc(ch, fptr_stego_image);
    }

    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    // Encode each character of magic string
    for (int i = 0; i < encInfo->size_usr_migc_str; i++)
    {
        char magic_char = magic_string[i];
        if (encode_8(&magic_char, encInfo) != e_success)
        {
            printf("ERROR: %s function failed\n", "encode_magic_string");
        }
    }

    return e_success;
}

Status encode_secret_file_extn(EncodeInfo *encInfo)
{
    encInfo->size_extn_secret_file = strlen(encInfo->extn_secret_file);
    encode_32(&encInfo->size_extn_secret_file, encInfo); // Encode extension length

    // Encode extension characters
    for (int i = 0; i < encInfo->size_extn_secret_file; i++)
    {
        if (encode_8(&encInfo->extn_secret_file[i], encInfo) != e_success)
        {
            printf("ERROR: %s function failed\n", "encode_secret_file_extn");
        }
    }

    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    int file_size_int = (int)encInfo->size_secret_file;
    // Encode secret file size as 32 bits
    if (encode_32(&file_size_int, encInfo) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    // Encode each character of secret file
    char a_char;
    for (int i = 0; i < encInfo->size_secret_file; i++)
    {
        a_char = fgetc(encInfo->fptr_secret);
        if (encode_8(&a_char, encInfo) != e_success)
        {
            printf("ERROR: %s function failed\n", "encode_secret_file_data");
            return e_failure;
        }
    }

    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    // Copy remaining image data after secret is encoded
    char a_char;
    while (fread(&a_char, 1, 1, fptr_src) > 0)
    {
        fputc(a_char, fptr_dest);
    }

    return e_success;
}

Status encode_8(char *en_char, EncodeInfo *encInfo)
{
    // Encode 8 bits of a character into image
    for (int i = 0; i < 8; i++)
    {
        int bit = (*en_char >> (7 - i)) & 1; // Get bit (MSB first)
        char file_bit = fgetc(encInfo->fptr_src_image);

        if (bit == 1)
        {
            file_bit = file_bit | 1; // Set LSB
        }
        else
        {
            file_bit = file_bit & ~1; // Clear LSB
        }

        fputc(file_bit, encInfo->fptr_stego_image);
    }

    return e_success;
}

Status encode_32(int *en_int, EncodeInfo *encInfo)
{
    // Encode 32 bits of an integer into image
    for (int i = 0; i < 32; i++)
    {
        int bit = (*en_int >> (31 - i)) & 1; // Get bit (MSB first)
        char file_bit = fgetc(encInfo->fptr_src_image);

        if (bit == 1)
        {
            file_bit = file_bit | 1;
        }
        else if (bit == 0)
        {
            file_bit = file_bit & ~1;
        }
        else
        {
            printf("ERROR: %s function failed\n", "encode_32");
            return e_failure;
        }

        fputc(file_bit, encInfo->fptr_stego_image);
    }

    return e_success;
}
