#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 255

// Function to encrypt the line using the provided encryption scheme
void encryptLine(char line[]) {
    char encryptedLine[MAXLINE * 2 + 1] = ""; // accommodate extra space for the encrypted characters
    char encryptedChar[3]; // two characters for the hex value and one for the null terminator

    for (int i = 0, j = 0; line[i] != '\0' && line[i] != '\n'; ++i) {
        if (line[i] == '\t') {
            strcat(encryptedLine, "TT");
        } else if (line[i] != '\r') {
            int outChar = line[i] - 16;
            if (outChar < 32) {
                outChar = (outChar - 32) + 144;
            }
            sprintf(encryptedChar, "%02X", outChar);
            strcat(encryptedLine, encryptedChar);
        }
    }
    printf("%s\n", encryptedLine);
}

// Function to decrypt the line using the provided decryption scheme
void decryptLine(char line[]) {
    char decryptedLine[MAXLINE + 1] = "";

    for (int i = 0, j = 0; line[i] != '\0' && line[i + 1] != '\0'; i += 2) {
        if (line[i] == 'T' && line[i + 1] == 'T') {
            strcat(decryptedLine, "\t");
        } else {
            int outChar = strtol(&line[i], NULL, 16);
            outChar += 16;
            if (outChar > 127) {
                outChar = (outChar - 144) + 32;
            }
            decryptedLine[j++] = (char)outChar;
        }
    }
    printf("%s\n", decryptedLine);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Insufficient arguments. Please provide a filename.\n");
        return 1;
    }

    FILE *file;
    char *filename = argv[argc - 1];
    char *outputFilename;
    char line[MAXLINE];

    if (argc == 2 || (argc == 3 && strcmp(argv[1], "-E") == 0)) { // Encrypt
        outputFilename = strcat(filename, ".crp");
        file = fopen(filename, "r");
        FILE *outputFile = fopen(outputFilename, "w");

        while (fgets(line, sizeof(line), file)) {
            encryptLine(line);
        }
        fclose(file);
        fclose(outputFile);
    } else if (argc == 3 && strcmp(argv[1], "-D") == 0) { // Decrypt
        int len = strlen(filename);
        if (len > 4 && strcmp(filename + len - 4, ".crp") == 0) {
            filename[len - 4] = '\0';
        }
        outputFilename = strcat(filename, ".txt");
        file = fopen(filename, "r");
        FILE *outputFile = fopen(outputFilename, "w");

        while (fgets(line, sizeof(line), file)) {
            decryptLine(line);
        }
        fclose(file);
        fclose(outputFile);
    } else {
        printf("Invalid command-line argument. Please use -E or -D.\n");
        return 1;
    }
    return 0;
}
