#include <stdio.h>  // Standard Input/Output library
#include <string.h> // String manipulation functions
#include <stdlib.h> // Standard library for utility functions

// Function prototypes
int romanToDecimal(const char *roman); // Converts Roman numerals to decimal
void decimalToWords(int number, char *words); // Converts a decimal number to words
int performOperation(int num1, int num2, char operator); // Performs arithmetic operations
void processInput(const char *input, char *output); // Processes the input and generates output
void sanitizeInput(const char *input, char *cleanedInput); // Sanitizes the input

// Roman numeral to decimal mapping
int romanValue(char c) {
    switch (c) { // Return decimal value for each Roman numeral
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0; // Return 0 for invalid characters
    }
}

// Convert Roman numeral to decimal
int romanToDecimal(const char *roman) {
    int total = 0; // Store the total decimal value
    for (int i = 0; i < strlen(roman); i++) {
        int current = romanValue(roman[i]); // Get value of current Roman numeral
        int next = romanValue(roman[i + 1]); // Get value of next Roman numeral
        if (current < next) { // If current numeral is smaller, subtract it
            total -= current;
        } else { // Otherwise, add it
            total += current;
        }
    }
    return total; // Return the total decimal value
}

// Convert decimal number to words
void decimalToWords(int number, char *words) {
    const char *units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"}; // Units mapping
    const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"}; // Teens mapping
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"}; // Tens mapping
    const char *thousands[] = {"", "Thousand", "Million"}; // Thousands mapping

    char buffer[100]; // Temporary buffer for words
    int chunkCount = 0; // Track the position of chunks (e.g., thousands, millions)
    
    words[0] = '\0'; // Initialize output as an empty string

    if (number == 0) { // Handle the case where the number is zero
        strcpy(words, "Zero");
        return;
    }

    while (number > 0) { // Process each chunk of 3 digits
        int chunk = number % 1000; // Get the last 3 digits
        number /= 1000; // Remove the last 3 digits from the number

        if (chunk > 0) { // If the chunk is not zero
            char chunkWords[50] = ""; // Temporary storage for chunk words

            if (chunk / 100 > 0) { // Handle hundreds place
                sprintf(buffer, "%s Hundred ", units[chunk / 100]); // Append "Hundred"
                strcat(chunkWords, buffer);
            }

            chunk %= 100; // Remove hundreds place
            if (chunk >= 10 && chunk < 20) { // Handle teens
                strcat(chunkWords, teens[chunk - 10]);
            } else { // Handle tens and units
                if (chunk / 10 > 0) { // Handle tens place
                    strcat(chunkWords, tens[chunk / 10]);
                    if (chunk % 10 > 0) strcat(chunkWords, " "); // Add space if units exist
                }
                if (chunk % 10 > 0) { // Handle units place
                    strcat(chunkWords, units[chunk % 10]);
                }
            }

            if (chunkCount > 0) { // Add thousands or millions suffix
                sprintf(buffer, " %s ", thousands[chunkCount]);
                strcat(chunkWords, buffer);
            }

            strcat(chunkWords, " "); // Add a space
            strcat(chunkWords, words); // Append previous words
            strcpy(words, chunkWords); // Update words with the current chunk
        }

        chunkCount++; // Move to the next chunk (thousands, millions, etc.)
    }

    // Remove trailing space
    size_t len = strlen(words);
    if (len > 0 && words[len - 1] == ' ') {
        words[len - 1] = '\0';
    }
}

// Perform arithmetic operation
int performOperation(int num1, int num2, char operator) {
    switch (operator) { // Perform operation based on the operator
        case '+': return num1 + num2; // Addition
        case '-': return num1 - num2; // Subtraction
        case '*': return num1 * num2; // Multiplication
        case '/': return num2 != 0 ? num1 / num2 : 0; // Division with zero check
        default: return 0; // Return 0 for invalid operator
    }
}

// Process input and generate output
void processInput(const char *input, char *output) {
    char roman1[50], roman2[50], operator; // Variables to store parts of the input
    int num1, num2, result; // Decimal values of the Roman numerals and result
    char resultWords[100]; // Words representation of the result

    // Parse input into two Roman numerals and an operator
    sscanf(input, "%s %c %s", roman1, &operator, roman2);

    // Convert Roman numerals to decimal
    num1 = romanToDecimal(roman1);
    num2 = romanToDecimal(roman2);

    // Perform the operation
    result = performOperation(num1, num2, operator);

    // Convert result to words
    decimalToWords(result, resultWords);

    // Format output
    sprintf(output, "%s", resultWords); // Store result words in output
}

int main() {
    char input[100], output[100]; // Buffers for input and output
    FILE *inputFile, *outputFile; // File pointers for input and output files

    // Open input and output files
    inputFile = fopen("input.txt", "r"); // Open input file in read mode
    outputFile = fopen("output.txt", "w"); // Open output file in write mode

    if (inputFile == NULL || outputFile == NULL) { // Check if files opened successfully
        printf("Error opening file!\n"); // Print error message if file opening fails
        return 1; // Exit with error code
    }

    // Process each line of the input file
    while (fgets(input, sizeof(input), inputFile)) { // Read a line from the input file
        input[strcspn(input, "\n")] = '\0'; // Remove newline character from input

        processInput(input, output); // Process the input line

        fprintf(outputFile, "%s\n", output); // Write the result to the output file
    }

    fclose(inputFile); // Close the input file
    fclose(outputFile); // Close the output file

    printf("Processing complete. Check output.txt for results.\n"); // Inform the user

    return 0; // Exit successfully
}
