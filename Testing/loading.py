import easygui as g
import sys

class Loading():

    def __init__(self):
        self.file_path = None
        self.output_folder = None


    def load_data_box(self):
        file_path = g.fileopenbox(msg="Choose your input text file")

        if file_path:
            #print("Input folder Path:", input_folder)
            self.file_path = file_path
            return file_path
        else:
            print("No file selected. Exiting.")
            sys.exit(0)

    def set_output_folder(self):
        
        output_folder = g.diropenbox(msg="Choose the output folder")

        if output_folder:
            #print("Output Folder:", output_folder)
            self.output_folder = output_folder
            return output_folder
        
        else:
            print("No folder selected. Exiting.")
            sys.exit(0)
