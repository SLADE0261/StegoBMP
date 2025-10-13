# StegoBMP

A robust C implementation of LSB (Least Significant Bit) steganography for hiding secret text files within BMP images.

## 🔒 Overview

StegoBMP allows you to securely hide confidential text files inside BMP images without any visible changes to the image. The program uses LSB steganography to encode data into the least significant bits of image pixels, making the hidden data virtually undetectable to the human eye.

## ✨ Features

- **Encode**: Hide any text file inside a 24-bit BMP image
- **Decode**: Extract hidden files from stego images
- **Magic String Protection**: Use custom passwords to secure your hidden data
- **Automatic Extension Preservation**: Original file extensions are maintained during decoding
- **Capacity Validation**: Checks if the image can hold the secret file before encoding
- **Error Handling**: Comprehensive validation and error reporting

## 🛠️ Technical Details

- **Algorithm**: LSB (Least Significant Bit) Steganography
- **Supported Format**: 24-bit BMP images only
- **Language**: C
- **Platform**: Cross-platform (Linux, macOS, Windows with appropriate compiler)

### How It Works

The program modifies the least significant bit of each pixel's color values (RGB) to encode data. Since changes to LSBs produce minimal visual differences, the modifications are imperceptible to human eyes.

**Encoding Structure:**
1. BMP Header (54 bytes) - Copied unchanged
2. Magic String - Custom password for verification
3. File Extension Length (32 bits)
4. File Extension (e.g., ".txt")
5. Secret File Size (32 bits)
6. Secret File Data (encoded bit by bit)
7. Remaining Image Data (copied unchanged)

## 📋 Prerequisites

- GCC compiler or any C compiler
- Basic knowledge of command line operations
- BMP image files (24-bit)
- Text files for encoding

## 🚀 Installation

1. Clone the repository:
```bash
git clone https://github.com/yourusername/StegoBMP.git
cd StegoBMP
```

2. Compile the project:
```bash
gcc main.c encode.c decode.c -o stegobmp
```

Or use the provided Makefile (if available):
```bash
make
```

## 📖 Usage

### Encoding (Hiding a File)

**Basic Syntax:**
```bash
./stegobmp -e <input_image.bmp> <secret_file.txt> <magic_string>
```

**With Custom Output Name:**
```bash
./stegobmp -e <input_image.bmp> <secret_file.txt> <output_image.bmp> <magic_string>
```

**Examples:**
```bash
# Encode secret.txt into input.bmp with magic string "#*"
# Output will be saved as output_image.bmp (default)
./stegobmp -e input.bmp secret.txt "#*"

# Encode with custom output filename
./stegobmp -e input.bmp secret.txt stego.bmp "myPassword123"
```

### Decoding (Extracting Hidden File)

**Basic Syntax:**
```bash
./stegobmp -d <stego_image.bmp> <magic_string>
```

**With Custom Output Name:**
```bash
./stegobmp -d <stego_image.bmp> <output_name> <magic_string>
```

**Examples:**
```bash
# Decode from stego.bmp using magic string "#*"
# Output will be saved as output_text.txt (default)
./stegobmp -d stego.bmp "#*"

# Decode with custom output filename (extension auto-added)
./stegobmp -d stego.bmp decoded "myPassword123"
# Creates: decoded.txt
```

## 📁 Project Structure

```
StegoBMP/
├── main.c              # Main program entry point
├── encode.c            # Encoding implementation
├── encode.h            # Encoding function declarations
├── decode.c            # Decoding implementation
├── decode.h            # Decoding function declarations
├── types.h             # Custom type definitions
└── README.md           # This file
```

## ⚠️ Important Notes

1. **Image Format**: Only 24-bit BMP images are supported
2. **File Type**: Currently optimized for text files, but can handle any file type
3. **Magic String**: Must be identical for encoding and decoding
4. **Capacity**: The image must have sufficient capacity to hold the secret file
5. **Output Extension**: When decoding, do not include file extension in output name

## 🔢 Capacity Calculation

The image must have enough pixels to store:
- Magic string
- File extension length (4 bytes)
- File extension
- Secret file size (4 bytes)
- Secret file content

**Formula:**
```
Required Capacity = (Magic String Length + Extension Length + 8 + Secret File Size) × 8 bits
Available Capacity = Image Width × Image Height × 3 bytes
```

## 🎯 Example Workflow

```bash
# 1. Prepare files
# - input.bmp (source BMP image)
# - secret.txt (file to hide)

# 2. Encode the secret
./stegobmp -e input.bmp secret.txt output.bmp "SecurePass"
# Output: SUCCESS: do_encoding function completed ✅

# 3. Share output.bmp (appears as normal image)

# 4. Decode the secret
./stegobmp -d output.bmp extracted "SecurePass"
# Output: SUCCESS: do_decoding function completed ✅
# Creates: extracted.txt (with original content)
```

## 🐛 Error Messages

| Error | Meaning | Solution |
|-------|---------|----------|
| Not enough arguments | Missing required parameters | Check command syntax |
| Incorrect input file type | File is not .bmp | Use 24-bit BMP images only |
| Incorrect secret file type | File is not .txt | Use text files |
| Unable to open file | File not found or no permission | Check file path and permissions |
| Magic String not found | Wrong magic string used | Use the correct magic string from encoding |
| Capacity check failed | Image too small for secret | Use a larger image |

## 🔐 Security Considerations

- The magic string acts as a password - keep it secure
- LSB steganography is detectable through statistical analysis
- For higher security, encrypt your secret file before encoding
- Use high-resolution images for better hiding capacity
- Avoid using the same magic string repeatedly

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## 📝 License

This project is open source and available under the [MIT License](LICENSE).

## 👨‍💻 Author

**Krishnakant C. Pore**
- Project Date: May 26, 2025

## 🙏 Acknowledgments

- LSB Steganography technique
- BMP file format specification
- C programming community

## 📚 Future Enhancements

- [ ] Support for PNG and JPEG formats
- [ ] GUI interface
- [ ] Encryption integration
- [ ] Multi-file encoding
- [ ] Password strength validation
- [ ] Compression before encoding
- [ ] Progress indicators for large files

---

**⭐ If you find this project useful, please consider giving it a star!**