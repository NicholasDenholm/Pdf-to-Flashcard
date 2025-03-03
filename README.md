# PDF Text Extractor

A simple flashcard making tool that can extract, clean, and save text content from PDF files without using external PDF libraries.

## Features

- Extract text content from PDF files using basic parsing techniques
- Clean extracted text (remove unwanted characters and convert to lowercase)
- Save processed text to output files
- Supports file and folder selection through a simple GUI interface (Windows)

## Requirements

- C compiler (GCC recommended)
- Windows system for GUI dialogs (or modify code for other platforms)

## Building

To compile and run the program:

```bash
./build.sh
```

To clean up generated files:

```bash
./build.sh clean
```

## File Structure

- `flashcards.c` - Main program file
- `loading.c` - Implementation of file/folder dialog functions
- `loading.h` - Header file for loading functions
- `build.sh` - Build script

## Usage

1. Run the program
2. Select a PDF file when prompted
3. Choose an output folder for the processed text
4. The program will extract and clean the text from the PDF and save it to a file in the specified output folder

## Limitations

This PDF text extractor uses a simplified approach to parse PDF content:
- Only extracts text between BT (Begin Text) and ET (End Text) markers
- May not handle complex PDF features like:
  - Compressed content streams
  - Images and other inserts
  - Non-standard text encodings
  - Custom fonts
  - Advanced text positioning

For more robust PDF processing, consider using dedicated PDF libraries.