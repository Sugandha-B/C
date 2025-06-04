#include <stdio.h>
#include <stdlib.h>

// Compress the input file and write to output file
void compress(const char *input, const char *output) {
    FILE *in = fopen(input, "r");
    FILE *out = fopen(output, "w");

    if (!in || !out) {
        printf("File error.\n");
        return;
    }

    char current, previous;
    int count = 1;

    previous = fgetc(in);
    while ((current = fgetc(in)) != EOF) {
        if (current == previous) {
            count++;
        } else {
            fprintf(out, "%d%c", count, previous);
            count = 1;
            previous = current;
        }
    }
    // Write the last sequence
    if (previous != EOF) {
        fprintf(out, "%d%c", count, previous);
    }

    fclose(in);
    fclose(out);
    printf("File compressed successfully.\n");
}

// Decompress the input file and write to output file
void decompress(const char *input, const char *output) {
    FILE *in = fopen(input, "r");
    FILE *out = fopen(output, "w");

    if (!in || !out) {
        printf("File error.\n");
        return;
    }

    int count;
    char ch;

    while (fscanf(in, "%d%c", &count, &ch) == 2) {
        for (int i = 0; i < count; i++) {
            fputc(ch, out);
        }
    }

    fclose(in);
    fclose(out);
    printf("File decompressed successfully.\n");
}

// Main function to choose mode
int main() {
    int choice;
    char inputFile[100], outputFile[100];

    printf("1. Compress a file\n");
    printf("2. Decompress a file\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

    printf("Enter input file name: ");
    scanf("%s", inputFile);
    printf("Enter output file name: ");
    scanf("%s", outputFile);

    if (choice == 1) {
        compress(inputFile, outputFile);
    } else if (choice == 2) {
        decompress(inputFile, outputFile);
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}
