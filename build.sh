#!/bin/bash

gcc -Wall -o flashcards flashcards.c loading.c

if [ $? -eq 0 ]; then
        ./flashcards
fi

cleanup() {
    echo "Cleaning up..."
    rm -f flashcards.exe
    rm -f *.o
    rm -f *~
    rm -f .*.swp
    echo "Cleanup complete."
}

if [ "$1" == "clean" ]; then
    cleanup
fi


