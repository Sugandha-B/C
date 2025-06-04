#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
void createFile();
void writeToFile();
void readFromFile();
void appendToFile();
void displayMenu();

int main() {
    int choice;
    
    printf("=== File Operations Program ===\n");
    
    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear the newline character from input buffer
        
        switch(choice) {
            case 1:
                createFile();
                break;
            case 2:
                writeToFile();
                break;
            case 3:
                readFromFile();
                break;
            case 4:
                appendToFile();
                break;
            case 5:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
        
        if(choice != 5) {
            printf("\nPress Enter to continue...");
            getchar();
        }
        
    } while(choice != 5);
    
    return 0;
}

void displayMenu() {
    printf("\n--- File Operations Menu ---\n");
    printf("1. Create a new file\n");
    printf("2. Write to file\n");
    printf("3. Read from file\n");
    printf("4. Append to file\n");
    printf("5. Exit\n");
}

void createFile() {
    char filename[100];
    FILE *file;
    
    printf("\nEnter filename to create: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline
    
    file = fopen(filename, "w");
    
    if(file == NULL) {
        printf("Error: Could not create file '%s'\n", filename);
        return;
    }
    
    printf("File '%s' created successfully!\n", filename);
    fclose(file);
}

void writeToFile() {
    char filename[100];
    char content[1000];
    FILE *file;
    
    printf("\nEnter filename to write to: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline
    
    file = fopen(filename, "w");
    
    if(file == NULL) {
        printf("Error: Could not open file '%s' for writing\n", filename);
        return;
    }
    
    printf("Enter content to write (press Enter twice to finish):\n");
    
    while(1) {
        fgets(content, sizeof(content), stdin);
        
        // Check if user pressed Enter twice (empty line)
        if(strlen(content) == 1 && content[0] == '\n') {
            break;
        }
        
        fprintf(file, "%s", content);
    }
    
    fclose(file);
    printf("Content written to file '%s' successfully!\n", filename);
}

void readFromFile() {
    char filename[100];
    char buffer[1000];
    FILE *file;
    
    printf("\nEnter filename to read from: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline
    
    file = fopen(filename, "r");
    
    if(file == NULL) {
        printf("Error: Could not open file '%s' for reading\n", filename);
        printf("Make sure the file exists!\n");
        return;
    }
    
    printf("\n--- Content of file '%s' ---\n", filename);
    
    while(fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }
    
    fclose(file);
    printf("\n--- End of file ---\n");
}

void appendToFile() {
    char filename[100];
    char content[1000];
    FILE *file;
    
    printf("\nEnter filename to append to: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0; // Remove newline
    
    file = fopen(filename, "a");
    
    if(file == NULL) {
        printf("Error: Could not open file '%s' for appending\n", filename);
        return;
    }
    
    printf("Enter content to append (press Enter twice to finish):\n");
    
    while(1) {
        fgets(content, sizeof(content), stdin);
        
        // Check if user pressed Enter twice (empty line)
        if(strlen(content) == 1 && content[0] == '\n') {
            break;
        }
        
        fprintf(file, "%s", content);
    }
    
    fclose(file);
    printf("Content appended to file '%s' successfully!\n", filename);
}