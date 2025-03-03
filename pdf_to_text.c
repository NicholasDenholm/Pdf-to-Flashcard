#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

#include "loading.h"

// Function to clean text: remove unwanted characters and convert to lowercase
char* clean_text(const char* text) {
    char* cleaned_text = (char*)malloc(strlen(text) + 1);
    int j = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalnum(text[i]) || isspace(text[i]) || strchr(".,!?;:'\"-", text[i])) {
            cleaned_text[j++] = tolower(text[i]);
        }
    }

    cleaned_text[j] = '\0';
    return cleaned_text;
}

// Function to save cleaned text to a file
void save_text_to_file(const char* text, const char* output_path) {
    FILE* file = fopen(output_path, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file, "%s", text);
    fclose(file);
    printf("Text saved to %s\n", output_path);
}

// Function to extract the note name from the file path
char* extract_note_name(const char* file_path) {
    regex_t regex;
    regmatch_t matches[1];
    char* note_name = (char*)malloc(256);
    
    // Regular expression to extract the last part before extension (note name)
    regcomp(&regex, "[^/\\]+(?=\\.[^/\\]+$|$)", REG_EXTENDED);
    if (regexec(&regex, file_path, 1, matches, 0) == 0) {
        strncpy(note_name, file_path + matches[0].rm_so, matches[0].rm_eo - matches[0].rm_so);
        note_name[matches[0].rm_eo - matches[0].rm_so] = '\0'; // Null-terminate
        printf("note name: %s\n", note_name);
    } else {
        printf("No match found\n");
        free(note_name);
        note_name = NULL;
    }

    regfree(&regex);
    return note_name;
}

// PDF text extraction without using external libraries
char* pdf_to_text(const char* pdf_path) {
    FILE* file = fopen(pdf_path, "rb");
    if (!file) {
        perror("Error opening PDF file");
        return NULL;
    }
    
    // Buffer for storing the extracted text
    char* extracted_text = (char*)malloc(100000); // Allocate a reasonably large buffer
    if (!extracted_text) {
        fclose(file);
        perror("Memory allocation failed");
        return NULL;
    }
    extracted_text[0] = '\0';
    
    // Read the PDF header to verify it's a PDF file
    char header[8];
    if (fread(header, 1, 8, file) != 8 || strncmp(header, "%PDF-1.", 7) != 0) {
        fclose(file);
        free(extracted_text);
        printf("Invalid PDF file or unsupported version\n");
        return NULL;
    }
    
    // Buffer for reading the file content
    char buffer[4096];
    size_t bytes_read;
    size_t total_size = 0;
    char* full_content = (char*)malloc(1);
    full_content[0] = '\0';
    
    // Read the entire file content
    rewind(file);
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        char* temp = (char*)realloc(full_content, total_size + bytes_read + 1);
        if (!temp) {
            fclose(file);
            free(full_content);
            free(extracted_text);
            perror("Memory reallocation failed");
            return NULL;
        }
        full_content = temp;
        memcpy(full_content + total_size, buffer, bytes_read);
        total_size += bytes_read;
        full_content[total_size] = '\0';
    }
    
    // Simple text extraction - look for text between BT and ET tags
    // This is a simplified approach that won't handle all PDF text encoding methods
    char* text_ptr = strstr(full_content, "BT");
    int extracted_length = 0;
    
    while (text_ptr != NULL) {
        // Find the end of this text object
        char* end_ptr = strstr(text_ptr, "ET");
        if (end_ptr == NULL) break;
        
        // Extract text between brackets (Tj operator)
        char* bracket_start = text_ptr;
        while (bracket_start < end_ptr) {
            bracket_start = strchr(bracket_start, '(');
            if (bracket_start == NULL || bracket_start > end_ptr) break;
            
            char* bracket_end = strchr(bracket_start, ')');
            if (bracket_end == NULL || bracket_end > end_ptr) break;
            
            // Copy text between brackets
            int len = bracket_end - bracket_start - 1;
            if (len > 0) {
                strncat(extracted_text, bracket_start + 1, len);
                strcat(extracted_text, " ");
                extracted_length += (len + 1);
            }
            
            bracket_start = bracket_end + 1;
        }
        
        // Look for the next text object
        text_ptr = strstr(end_ptr, "BT");
    }
    
    // Clean up
    fclose(file);
    free(full_content);
    
    // If we didn't extract any text, return a placeholder
    if (extracted_length == 0) {
        strcpy(extracted_text, "PDF text extraction failed. This PDF may use complex encoding or be protected.");
    }
    
    return extracted_text;
}


int main() {

    Loading loading;
    loading.file_path = NULL;
    loading.output_folder = NULL;

    load_data_box(&loading); // Load the file path
    set_output_folder(&loading); // Set the output folder

    // Use the loaded file path instead of a hardcoded one
    if (loading.file_path == NULL) {
        printf("No file path provided. Exiting.\n");
        return 1;
    }
    
    // Extract text from the PDF
    char* extracted_text = pdf_to_text(loading.file_path);
    if (extracted_text == NULL) {
        printf("Text extraction failed. Exiting.\n");
        if (loading.file_path) free(loading.file_path);
        if (loading.output_folder) free(loading.output_folder);
        return 1;
    }
    
    // Clean the extracted text
    char* cleaned_text = clean_text(extracted_text);
    
    // Extract note name from the file path
    char* note_name = extract_note_name(loading.file_path);
    
    // Create output file path using the output folder and note name
    char* output_path = NULL;
    if (loading.output_folder && note_name) {
        output_path = (char*)malloc(strlen(loading.output_folder) + strlen(note_name) + 10);
        sprintf(output_path, "%s/%s_text.txt", loading.output_folder, note_name);
    } else {
        output_path = strdup("output.txt");
    }
    
    // Save the cleaned text to the file
    save_text_to_file(cleaned_text, output_path);


    // Sample PDF path
    const char* pdf_path = "example.pdf";
    char* extracted_text = pdf_to_text(pdf_path);

    // Clean the extracted text
    char* cleaned_text = clean_text(extracted_text);

    // Define output file path
    const char* output_path = "output.txt";

    // Save the cleaned text to a file
    save_text_to_file(cleaned_text, output_path);

    // Extract and print the note name from the file path
    char* note_name = extract_note_name(pdf_path);

    // Cleanup and free dynamic alloc'ed memory
    if (cleaned_text) free(cleaned_text);
    if (note_name != NULL) free(note_name);    
    if (loading.file_path) free(loading.file_path);
    if (loading.output_folder) free(loading.output_folder);

    return 0;
}
