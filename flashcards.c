#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "loading.h"

#define max_line_length 1024
#define exit_command "exit"
#define clear() printf("\033[h\033[j")

// function to print the question and wait for user input
int get_input(char *question)
{
        // printf("%s", question); // prints cool symbols

        char user_input[max_line_length];
        fgets(user_input, sizeof(user_input), stdin);

        // check if the user wants to exit
        if (strncmp(user_input, exit_command, strlen(exit_command)) == 0)
        {
                return 1; // indicate that the user wants to exit
        }

        return 0; // indicate to continue
}

// function to read a complete question that may span multiple lines
void read_question(FILE *file)
{
        puts("press enter to continue or type exit to stop\n\n");
        // getchar();
        // strcpy(question, "1"); // initialize the question buffer
        char question_line[max_line_length];

        if (fgets(question_line, sizeof(question_line), file) != NULL)
        {

                question_line[strcspn(question_line, "\n")] = '\0';
                printf("%s\n", question_line);

                // read lines until we encounter "a:"
                while (fgets(question_line, sizeof(question_line), file) != NULL)
                {
                        // trim newline escape characters off the end

                        question_line[strcspn(question_line, "\n")] = '\0';

                        // check for "a:" to stop reading the question
                        if (strncmp(question_line, "a:", 2) == 0)
                        {
                                // we've reached the answer line, stop reading
                                break;
                        }
                        // append the line to the question buffer
                        // strcat(question, line);
                        // strcat(question, "\n"); // add newline for formatting
                        printf("%s\n", question_line);
                }
        }
}

// function to read the answer
void read_answer(FILE *file)
{
        char answer_line[max_line_length];
        // read the answer line
        if (fgets(answer_line, sizeof(answer_line), file) != NULL)
        {
                // trim the answer line
                answer_line[strcspn(answer_line, "\n")] = '\0';
                printf("answer: %s\n", answer_line + 2); // print answer without "a:"

                // print additional lines for the answer
                while (fgets(answer_line, sizeof(answer_line), file) != NULL)
                {
                        // trim the answer line
                        answer_line[strcspn(answer_line, "\n")] = '\0';
                        // check if the next line is a question
                        if (strncmp(answer_line, "q:", 2) == 0)
                        {
                                // we reached the next question, so stop reading answers
                                break;
                        }
                        printf("%s\n", answer_line); // print additional answer lines
                }
        }
}

int flash_cards(const char *filename)
{
        FILE *file = fopen(filename, "r");
        if (file == NULL)
        {
                perror("error opening file");
                return 1;
        }

        char question[max_line_length]; // buffer to hold the question

        while (1)
        {
                clear();
                // read the question, which may span multiple lines
                read_question(file);

                // print_question(question);

                // if the question is empty, we've reached the end of the file
                if (feof(file))
                {
                        puts("end of file reached.\n");
                        break;
                }

                // print the question and check if the user wants to exit
                if (get_input(question))
                {
                        printf("exiting the flashcard session.\n");
                        exit(1); // exit if the user requested to exit
                }

                read_answer(file);
                printf("\n");

                if (get_input(question))
                {
                        printf("exiting the flashcard session.\n");
                        exit(1); // exit if the user requested to exit
                }
                clear();
        }
        fclose(file);
        return 0;
}

int main()
{

        Loading loading;
        loading.file_path = NULL;
        //loading.output_folder = NULL;

        load_data_box(&loading); // Load the file path

        int code;
        do
        {
                const char *filename1 = "questionslec1.txt"; // change this to your filename
                code = flash_cards(filename1);

                const char *filename2 = "questionslec2.txt"; // change this to your filename
                code = flash_cards(filename2);

                const char *filename3 = "questionslec3.txt"; // change this to your filename
                code = flash_cards(filename3);

                const char *filename4 = "questionslec4.txt"; // change this to your filename
                code = flash_cards(filename4);

                const char *filename5 = "questionslec6.txt"; // Change this to your filename
                code = flash_cards(filename5);
        } while (code != 1); // this exit condition part can be removed

        return 0;
}