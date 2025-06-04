#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100
#define MAX_KEYWORDS 32

// Token types
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_DELIMITER,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
    int line_number;
} Token;

// C Keywords list
const char* keywords[MAX_KEYWORDS] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

// Operators list
const char* operators[] = {
    "++", "--", "+=", "-=", "*=", "/=", "%=", "==", "!=", "<=", ">=", 
    "&&", "||", "<<", ">>", "->",
    "+", "-", "*", "/", "%", "=", "<", ">", "!", "&", "|", "^", "~", "?", ":"
};

// Delimiters
const char delimiters[] = "();{}[],";

// Global variables
FILE *input_file;
int current_line = 1;
char current_char;
int eof_reached = 0;

// Function prototypes
void get_next_char();
void skip_whitespace();
void skip_comment();
int is_keyword(const char* str);
int is_operator(const char* str);
int is_delimiter(char ch);
Token get_next_token();
void print_token(Token token);
void analyze_file(const char* filename);

// Get next character from file
void get_next_char() {
    if (!eof_reached) {
        current_char = fgetc(input_file);
        if (current_char == EOF) {
            eof_reached = 1;
        }
        if (current_char == '\n') {
            current_line++;
        }
    }
}

// Skip whitespace characters
void skip_whitespace() {
    while (!eof_reached && isspace(current_char)) {
        get_next_char();
    }
}

// Skip comments (both single line // and multi-line /* */)
void skip_comment() {
    if (current_char == '/') {
        get_next_char();
        if (current_char == '/') {
            // Single line comment
            while (!eof_reached && current_char != '\n') {
                get_next_char();
            }
        } else if (current_char == '*') {
            // Multi-line comment
            get_next_char();
            while (!eof_reached) {
                if (current_char == '*') {
                    get_next_char();
                    if (current_char == '/') {
                        get_next_char();
                        break;
                    }
                } else {
                    get_next_char();
                }
            }
        } else {
            // It's just a division operator, step back
            fseek(input_file, -1, SEEK_CUR);
            current_char = '/';
        }
    }
}

// Check if string is a keyword
int is_keyword(const char* str) {
    for (int i = 0; i < MAX_KEYWORDS && keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if string is an operator
int is_operator(const char* str) {
    int num_operators = sizeof(operators) / sizeof(operators[0]);
    for (int i = 0; i < num_operators; i++) {
        if (strcmp(str, operators[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if character is a delimiter
int is_delimiter(char ch) {
    return strchr(delimiters, ch) != NULL;
}

// Get next token from input
Token get_next_token() {
    Token token;
    token.line_number = current_line;
    token.value[0] = '\0';
    
    skip_whitespace();
    
    if (eof_reached) {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }
    
    // Skip comments
    if (current_char == '/') {
        long pos = ftell(input_file);
        skip_comment();
        if (ftell(input_file) != pos) {
            return get_next_token(); // Recursively get next token after comment
        }
    }
    
    // String literals
    if (current_char == '"') {
        token.type = TOKEN_STRING;
        int i = 0;
        token.value[i++] = current_char;
        get_next_char();
        
        while (!eof_reached && current_char != '"' && i < MAX_TOKEN_LENGTH - 2) {
            if (current_char == '\\') {
                token.value[i++] = current_char;
                get_next_char();
            }
            token.value[i++] = current_char;
            get_next_char();
        }
        
        if (current_char == '"') {
            token.value[i++] = current_char;
            get_next_char();
        }
        
        token.value[i] = '\0';
        return token;
    }
    
    // Character literals
    if (current_char == '\'') {
        token.type = TOKEN_STRING;
        int i = 0;
        token.value[i++] = current_char;
        get_next_char();
        
        while (!eof_reached && current_char != '\'' && i < MAX_TOKEN_LENGTH - 2) {
            if (current_char == '\\') {
                token.value[i++] = current_char;
                get_next_char();
            }
            token.value[i++] = current_char;
            get_next_char();
        }
        
        if (current_char == '\'') {
            token.value[i++] = current_char;
            get_next_char();
        }
        
        token.value[i] = '\0';
        return token;
    }
    
    // Numbers
    if (isdigit(current_char)) {
        token.type = TOKEN_NUMBER;
        int i = 0;
        
        while (!eof_reached && (isdigit(current_char) || current_char == '.') && i < MAX_TOKEN_LENGTH - 1) {
            token.value[i++] = current_char;
            get_next_char();
        }
        
        token.value[i] = '\0';
        return token;
    }
    
    // Identifiers and Keywords
    if (isalpha(current_char) || current_char == '_') {
        int i = 0;
        
        while (!eof_reached && (isalnum(current_char) || current_char == '_') && i < MAX_TOKEN_LENGTH - 1) {
            token.value[i++] = current_char;
            get_next_char();
        }
        
        token.value[i] = '\0';
        
        if (is_keyword(token.value)) {
            token.type = TOKEN_KEYWORD;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
        
        return token;
    }
    
    // Delimiters
    if (is_delimiter(current_char)) {
        token.type = TOKEN_DELIMITER;
        token.value[0] = current_char;
        token.value[1] = '\0';
        get_next_char();
        return token;
    }
    
    // Operators (check multi-character first)
    char op_buffer[3] = {0};
    op_buffer[0] = current_char;
    
    // Check for two-character operators
    long pos = ftell(input_file);
    get_next_char();
    if (!eof_reached) {
        op_buffer[1] = current_char;
        if (is_operator(op_buffer)) {
            token.type = TOKEN_OPERATOR;
            strcpy(token.value, op_buffer);
            get_next_char();
            return token;
        }
    }
    
    // Reset position and check single character operator
    fseek(input_file, pos, SEEK_SET);
    current_char = op_buffer[0];
    op_buffer[1] = '\0';
    
    if (is_operator(op_buffer)) {
        token.type = TOKEN_OPERATOR;
        strcpy(token.value, op_buffer);
        get_next_char();
        return token;
    }
    
    // Unknown character
    token.type = TOKEN_UNKNOWN;
    token.value[0] = current_char;
    token.value[1] = '\0';
    get_next_char();
    return token;
}

// Print token information
void print_token(Token token) {
    const char* type_names[] = {
        "KEYWORD", "IDENTIFIER", "OPERATOR", "NUMBER", "STRING", "DELIMITER", "UNKNOWN", "EOF"
    };
    
    printf("Line %d: %-12s -> '%s'\n", token.line_number, type_names[token.type], token.value);
}

// Analyze input file
void analyze_file(const char* filename) {
    input_file = fopen(filename, "r");
    if (!input_file) {
        printf("Error: Cannot open file '%s'\n", filename);
        return;
    }
    
    printf("\n========== LEXICAL ANALYSIS RESULTS ==========\n");
    printf("File: %s\n", filename);
    printf("Line:  Token Type    -> Value\n");
    printf("===============================================\n");
    
    current_line = 1;
    eof_reached = 0;
    get_next_char();
    
    Token token;
    do {
        token = get_next_token();
        if (token.type != TOKEN_EOF) {
            print_token(token);
        }
    } while (token.type != TOKEN_EOF);
    
    printf("===============================================\n");
    printf("Analysis complete. Total lines processed: %d\n\n", current_line - 1);
    
    fclose(input_file);
}

// Create sample input file
void create_sample_file() {
    FILE *sample = fopen("sample_input.c", "w");
    if (!sample) {
        printf("Error creating sample file\n");
        return;
    }
    
    fprintf(sample, "// Sample C program for lexical analysis\n");
    fprintf(sample, "#include <stdio.h>\n\n");
    fprintf(sample, "int main() {\n");
    fprintf(sample, "    int count = 0;\n");
    fprintf(sample, "    float average = 3.14;\n");
    fprintf(sample, "    char name[] = \"Hello World\";\n");
    fprintf(sample, "    \n");
    fprintf(sample, "    for (int i = 0; i < 10; i++) {\n");
    fprintf(sample, "        count += i;\n");
    fprintf(sample, "    }\n");
    fprintf(sample, "    \n");
    fprintf(sample, "    if (count >= 45) {\n");
    fprintf(sample, "        printf(\"Sum is: %%d\\n\", count);\n");
    fprintf(sample, "    } else {\n");
    fprintf(sample, "        printf(\"Sum is less than 45\\n\");\n");
    fprintf(sample, "    }\n");
    fprintf(sample, "    \n");
    fprintf(sample, "    return 0;\n");
    fprintf(sample, "}\n");
    
    fclose(sample);
    printf("Sample input file 'sample_input.c' created successfully!\n");
}

int main() {
    printf("=== LEXICAL ANALYZER FOR C LANGUAGE ===\n");
    printf("This program demonstrates lexical analysis in compiler design\n");
    
    // Create sample file
    create_sample_file();
    
    // Analyze the sample file
    analyze_file("sample_input.c");
    
    // Display statistics
    printf("=== TOKEN CATEGORIES RECOGNIZED ===\n");
    printf("1. KEYWORDS: C language reserved words\n");
    printf("2. IDENTIFIERS: Variable and function names\n");
    printf("3. OPERATORS: Arithmetic, logical, assignment operators\n");
    printf("4. NUMBERS: Integer and floating-point literals\n");
    printf("5. STRINGS: String and character literals\n");
    printf("6. DELIMITERS: Punctuation marks like (), {}, []\n");
    printf("7. UNKNOWN: Unrecognized characters\n\n");
    
    printf("=== FEATURES IMPLEMENTED ===\n");
    printf("• Multi-character operator recognition (++, --, +=, etc.)\n");
    printf("• Comment skipping (both // and /* */ style)\n");
    printf("• String and character literal handling\n");
    printf("• Line number tracking\n");
    printf("• Comprehensive C keyword recognition\n");
    printf("• Error handling for unknown tokens\n\n");
    
    // Option to analyze custom file
    char filename[256];
    printf("Enter filename to analyze (or press Enter to exit): ");
    if (fgets(filename, sizeof(filename), stdin) && strlen(filename) > 1) {
        filename[strlen(filename) - 1] = '\0'; // Remove newline
        analyze_file(filename);
    }
    
    return 0;
}