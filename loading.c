#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char *file_path;
    char *output_folder;
} Loading;

// File open dialog using Win32 API
char* file_open_dialog(HWND parent) {
    OPENFILENAME ofn;       // Common dialog box structure
    static char file[260];  // Buffer for file name

    // Initialize OPENFILENAME structure
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = parent;
    ofn.lpstrFile = file;
    ofn.nMaxFile = sizeof(file);
    ofn.lpstrFilter = "Text Files\0*.TXT\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = "Choose your input text file";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Save As dialog box
    if (GetOpenFileName(&ofn) == TRUE) {
        return _strdup(ofn.lpstrFile);  // Return the selected file path
    } else {
        return NULL;  // No file selected
    }
}

// Folder open dialog using SHBrowseForFolder (Win32 API)
char* folder_open_dialog(HWND parent) {
    BROWSEINFO bi;
    PIDLIST_ABSOLUTE pidl;
    char path[MAX_PATH];

    // Initialize the BROWSEINFO structure
    bi.hwndOwner = parent;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = path;
    bi.lpszTitle = "Choose the output folder";
    bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
    bi.lpfn = NULL;
    bi.lParam = 0;

    pidl = SHBrowseForFolder(&bi);
    if (pidl != NULL) {
        // Get the folder path
        if (SHGetPathFromIDList(pidl, path)) {
            return _strdup(path);  // Return the folder path
        }
    }
    return NULL;  // No folder selected
}

void load_data_box(Loading *loading) {
    char *file_path = file_open_dialog(NULL);
    if (file_path != NULL) {
        loading->file_path = file_path;
        printf("File path selected: %s\n", file_path);
    } else {
        printf("No file selected. Exiting.\n");
        exit(0);
    }
}

void set_output_folder(Loading *loading) {
    char *output_folder = folder_open_dialog(NULL);
    if (output_folder != NULL) {
        loading->output_folder = output_folder;
        printf("Output folder selected: %s\n", output_folder);
    } else {
        printf("No folder selected. Exiting.\n");
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    Loading loading;
    loading.file_path = NULL;
    loading.output_folder = NULL;

    load_data_box(&loading);
    set_output_folder(&loading);

    // Here you can use loading.file_path and loading.output_folder as needed

    gtk_main();
    return 0;
}
