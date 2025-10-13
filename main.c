
/*
———————————————————————————————————————————————————————————————————————
Project: BMP Steganography Encoder/Decoder
Author : Krishnakant C. Pore
Date   : 26 May 2025
Language: C

Description:
  This C program allows users to hide (encode) a secret text file within a BMP image file
  and to extract (decode) the hidden text file from a stego BMP image.
  It uses LSB (Least Significant Bit) steganography to embed and retrieve data.

Features:
  - Encode a .txt secret file into a .bmp image, producing a new stego image.
  - Decode and extract the hidden secret file from a stego .bmp image.
  - Validates file types and checks if the image has enough capacity for the secret.
  - Provides error messages for invalid usage or unsupported operations.

Sample User Inputs:
  ./a.out -e input.bmp secret.txt "#*"
    → Encodes secret.txt into input.bmp using magic string "#*", and saves result as output_image.bmp (default output file)

  ./a.out -e input.bmp secret.txt output.bmp "#*"
    → Encodes secret.txt into input.bmp using magic string "#*", and saves result as output.bmp

  ./a.out -d stego.bmp "#*"
    → Decodes the hidden file from stego.bmp using magic string "#*", and saves it as output_text.txt (default output name)

  ./a.out -d stego.bmp output "#*"
    → Decodes the hidden file from stego.bmp using magic string "#*", and saves it as output.txt

File Info:
  - Only supports 24-bit BMP images for encoding/decoding.
  - Secret file must be a .txt file.
  - Output file for decoding should not include an extension; it is restored automatically.

———————————————————————————————————————————————————————————————————————
*/

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("ERROR: Not enough arguments.\n");
        return 0;
    }

    OperationType user_operation = check_operation_type(argv); // Determine operation type

    if (user_operation == e_encode) // Encode operation
    {
        EncodeInfo encInfo;

        if (read_and_validate_encode_args(argc, argv) != e_success) // Validate encode args
        {
            printf("ERROR: %s function failed\n", "read_and_validate_encode_args");
            return 0;
        }

        strcpy(encInfo.src_image_fname, argv[2]);
        strcpy(encInfo.secret_fname, argv[3]);
        if (argc == 6)
        {
            strcpy(encInfo.stego_image_fname, argv[4]);
            strcpy(encInfo.usr_migc_str, argv[5]);
        }
        else // argc == 5
        {
            strcpy(encInfo.stego_image_fname, "output_image.bmp");
            strcpy(encInfo.usr_migc_str, argv[4]);
        }

        encInfo.size_usr_migc_str = strlen(encInfo.usr_migc_str);

        if (open_files(&encInfo) == e_failure) // Open files for encoding
        {
            printf("ERROR: %s function failed\n", "open_files");
            return 0;
        }

        encInfo.image_capacity = get_image_size_for_bmp(encInfo.fptr_src_image); // Get image capacity
        encInfo.size_secret_file = get_file_size(encInfo.fptr_secret);           // Get secret file size

        Status capacity_status = check_capacity(&encInfo); // Check if image can hold secret

        if (capacity_status == e_success)
        {
            if (do_encoding(&encInfo) != e_success) // Perform encoding
                printf("ERROR: %s function failed\n", "do_encoding");
            else
                printf("SUCCESS: %s function completed ✅\n", "do_encoding");
        }
        else if (capacity_status == e_failure)
        {
            printf("ERROR: %s function failed\n", "capacity_status");
            return 0;
        }

        fclose(encInfo.fptr_src_image);
        fclose(encInfo.fptr_secret);
        fclose(encInfo.fptr_stego_image);
    }
    if (user_operation == e_decode) // Decode operation
    {
        DecodeInfo decInfo;

        if (read_and_validate_decode_args(argc, argv) != e_success) // Validate decode args
        {
            printf("ERROR: %s function failed\n", "read_and_validate_decode_args");
            return 0;
        }

        strcpy(decInfo.inp_image_fname, argv[2]);
        if (argc == 5)
        {
            strcpy(decInfo.out_fname, argv[3]);
            strcpy(decInfo.usr_migc_str, argv[4]);
        }
        else // argc == 4
        {
            strcpy(decInfo.out_fname, "output_text");
            strcpy(decInfo.usr_migc_str, argv[3]);
        }

        decInfo.size_usr_migc_str = strlen(decInfo.usr_migc_str);

        if (open_files_dec(&decInfo) == e_failure) // Open files for decoding
        {
            printf("ERROR: %s function failed\n", "open_files");
            return 1;
        }

        if (do_decoding(&decInfo) != e_success) // Perform decoding
            return 0;
        else
            printf("SUCCESS: %s function completed ✅\n", "do_decoding");

        fclose(decInfo.fptr_inp_image);
        fclose(decInfo.fptr_out);
    }
    if (user_operation == e_unsupported)
        printf("ERROR: Unsupported Operation.\n"); // Unsupported operation

    return 0;
}