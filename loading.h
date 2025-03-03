// loading.h
#ifndef LOADING_H   // Include guard to prevent multiple inclusions
#define LOADING_H

// Check if we're on Windows
#ifdef _WIN32
    #include <windows.h>
#else
    // Define a placeholder for non-Windows platforms
    typedef void* HWND;
#endif

// Structure to hold file path and output folder
typedef struct {
    char *file_path;
    char *output_folder;
} Loading;

// Function prototypes
char* file_open_dialog(HWND parent);      // Open a file selection dialog
char* folder_open_dialog(HWND parent);    // Open a folder selection dialog
void load_data_box(Loading *loading);     // Load file path
void set_output_folder(Loading *loading); // Set output folder

#endif // LOADING_H