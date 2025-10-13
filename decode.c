
#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"

/* Function Definitions */

Status read_and_validate_decode_args(int argc, char *argv[])
{
    // Check for correct number of arguments
    if ((argc < 4) || (5 < argc))
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

    // Check output file name does not have extension
    if (argc == 5)
    {
        if (strstr(argv[3], ".") != NULL)
        {
            printf("ERROR: Output file name should not contain extension.\n");
            return e_failure;
        }
    }

    return e_success;
}

Status open_files_dec(DecodeInfo *decInfo)
{
    decInfo->fptr_inp_image = fopen(decInfo->inp_image_fname, "r");
    // Fail if input image cannot be opened
    if (decInfo->fptr_inp_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->inp_image_fname);
        return e_failure;
    }

    decInfo->fptr_out = fopen(decInfo->out_fname, "w");
    // Fail if output file cannot be opened
    if (decInfo->fptr_out == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->out_fname);
        return e_failure;
    }

    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_inp_image, 54, SEEK_SET); // Skip BMP header

    // Abort if magic string not found
    if (magic_string_status(decInfo->usr_migc_str, decInfo) != e_success)
    {
        printf("ERROR: Magic Sting not found.\n");
        return e_failure;
    }

    get_size_extn_out_file(decInfo); // Decode extension size
    get_extn_out_file(decInfo);      // Decode extension
    get_size_out_file(decInfo);      // Decode secret file size
    write_out_file(decInfo);         // Decode and write secret data

    // Rename output file to include extension
    char new_name[20];
    strcpy(new_name, decInfo->out_fname);
    strcat(new_name, decInfo->extn_out_file);
    rename(decInfo->out_fname, new_name);

    return e_success;
}

Status magic_string_status(const char *magic_string, DecodeInfo *decInfo)
{
    char dec_char[decInfo->size_usr_migc_str];

    // Decode 2 characters from image LSBs
    for (int i = 0; i < decInfo->size_usr_migc_str; i++)
    {
        char decoded_char = 0;
        for (int j = 0; j < 8; j++)
        {
            char temp_char = fgetc(decInfo->fptr_inp_image);
            decoded_char = decoded_char << 1;
            if ((temp_char & 1) == 1)
            {
                decoded_char = decoded_char | 1;
            }
        }
        dec_char[i] = decoded_char;
    }

    // Compare decoded string with expected magic string
    if (strcmp(dec_char, magic_string) == 0)
        return e_success;
    else
        return e_failure;
}

Status get_size_extn_out_file(DecodeInfo *decInfo)
{
    int decoded_int = 0;

    // Decode 32 bits for extension size
    for (int j = 0; j < 32; j++)
    {
        int temp_int = fgetc(decInfo->fptr_inp_image);
        decoded_int = decoded_int << 1;
        if ((temp_int & 1) == 1)
        {
            decoded_int = decoded_int | 1;
        }
    }

    decInfo->size_extn_out_file = (long)decoded_int;
    return e_success;
}

Status get_extn_out_file(DecodeInfo *decInfo)
{
    int size = decInfo->size_extn_out_file;
    char dec_char[size + 1];

    // Decode extension characters from image
    for (int i = 0; i < size; i++)
    {
        char decoded_char = 0;
        for (int j = 0; j < 8; j++)
        {
            char temp_char = fgetc(decInfo->fptr_inp_image);
            decoded_char = decoded_char << 1;
            if ((temp_char & 1) == 1)
            {
                decoded_char = decoded_char | 1;
            }
        }
        dec_char[i] = decoded_char;
    }

    dec_char[size] = '\0';
    strcpy(decInfo->extn_out_file, dec_char);
    return e_success;
}

Status get_size_out_file(DecodeInfo *decInfo)
{
    int decoded_int = 0;

    // Decode 32 bits for secret file size
    for (int j = 0; j < 32; j++)
    {
        int temp_int = fgetc(decInfo->fptr_inp_image);
        decoded_int = decoded_int << 1;
        if ((temp_int & 1) == 1)
        {
            decoded_int = decoded_int | 1;
        }
    }

    decInfo->size_out_file = (long)decoded_int;
    return e_success;
}

Status write_out_file(DecodeInfo *decInfo)
{
    // Decode and write each character of secret file
    for (int i = 0; i < decInfo->size_out_file; i++)
    {
        decode_8(decInfo);
    }
    return e_success;
}

Status decode_8(DecodeInfo *decInfo)
{
    char decoded_char = 0;

    // Decode 8 bits to get one character
    for (int i = 0; i < 8; i++)
    {
        char temp_char = fgetc(decInfo->fptr_inp_image);
        decoded_char = decoded_char << 1;
        if ((temp_char & 1) == 1)
        {
            decoded_char = decoded_char | 1;
        }
    }

    fputc(decoded_char, decInfo->fptr_out); // Write decoded character to output
}
