//Shane Bennett
//CSCI 304 - Computer Organization
//Project 6
//Due 03/27/24
//This project allows the user to input numbers in decimal, binary, octal, or hex form and perform various operations
//to them. These include addition, subtraction, negation, complement, bitwise OR, bitwise XOR, bitwise AND, and
//shifts. It displays a menu that gives the user a visualization of the numbers in all of the forms converted.

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "calc.h"

unsigned short get_binary_op(bin_str bin) { // convert bin_str to short; return value
    unsigned short result = 0;
    int length = strlen(bin);
    for(int i = 0; i < length; i++){
        if(bin[i] == '1'){
            result += (1 << (length - 1 - i));
        }
    }
    return result;
}

void convert_to_binary(short val, bin_str bin) {
    char string[17] = "0000000000000000"; // 16 bits plus null terminator
    short value = val;

    //positive number
    if (value >= 0) {
        value = val;

        // Convert value to binary
        int i = 15;
        while (value > 0 && i >= 1) {
            if(value % 2 == 1){
                string[i] = '1';
            } else {
                string[i] = '0';
            }
            value /= 2;
            i--;
        }
    }

    //negative number
    if (value < 0){
        int total = -32768; //-2^15
        string[0] = '1';
        for(int k = 14; k >= 0; k --){
            int power = 1 << k; //2^k
            if(total + power <= value){
                string[15 - k] = '1';
                total += power;
            }
        }
    }

    // Adding spaces
    char str_spaced[20] = "                    ";
    int j = 0;
    for (int l = 0; l < 16; l++) {
        str_spaced[j++] = string[l];
        if ((l + 1) % 4 == 0) {
            str_spaced[j++] = ' ';
        }
    }
    str_spaced[19] = '\0'; //add string termination

    strcpy(bin, str_spaced); // Copy spaced string to bin_str
}

short get_operand(char mode){ //read in numeric value in mode; return value
    short value;
    bin_str temp_bin_val;
    switch(mode) {
        case 'O': // octal
            printf("Enter octal value: ");
            scanf("%ho", &value); // gets octal value
            printf("%o\n", (unsigned short) value); //echo printing
            break;
        case 'H': // hex
            printf("Enter hex value: ");
            scanf("%hx", &value); // gets hex value
            printf("%X\n", (unsigned short) value); //echo printing
            break;
        case 'D': // decimal
            printf("Enter decimal value: ");
            scanf("%hd", &value); // gets decimal value
            printf("%d\n", value); // echo printing
            break;
        case 'B': // binary
            printf("Enter binary value: ");
            scanf("%s", temp_bin_val); //gets binary value
            printf("%s", temp_bin_val);

            for (int i = 0; i < strlen(temp_bin_val); i++) { //checks for validity of user entry
                if (temp_bin_val[i] != '1' && temp_bin_val[i] != '0') {
                    get_operand('B'); //asks user to retry if entry not valid
                }
            }
            value = get_binary_op(temp_bin_val); //calls binary_op method
            break;
    }
    return value;
}

void add(short *val, char mode){ //call get_operand to get value in mode to add to val; detect pos/neg overflow
    short other_op = get_operand(mode);
    short new_val = *val;
    new_val += other_op;
    //checking for positive overflow
    if(val > 0 && other_op > 0 && new_val < 0){
        printf("Positive Overflow\n");
    }
    //checking for negative overflow
    if(val < 0 && other_op < 0 && new_val > 0){
        printf("Negative Overflow\n");
    }
    *val = new_val;
}

void subtract(short *val, char mode){ //similar to add, but with subtraction
    short other_op = get_operand(mode);
    short new_val = *val;
    new_val -= other_op;
    //checking for negative overflow
    if(val < 0 && other_op > 0 && new_val > 0){
        printf("Negative Overflow\n");
    }
    //checking for positive overflow
    if(val > 0 && other_op < 0 && new_val <0){
        printf("Positive Overflow\n");
    }
    *val = new_val;
}

void print_bases (short val, char mode){ //print out base values and str for mode
    char mode_str[4];
    bin_str bin_string;
    convert_to_binary(val, bin_string);

    switch (mode) { //changes mode_str to requisite string
        case 'B': {
            strcpy (mode_str, "Bin");
            break;
        }
        case 'D': {
            strcpy(mode_str, "Dec");
            break;
        }
        case 'H': {
            strcpy(mode_str, "Hex");
            break;
        }
        case 'O': {
            strcpy(mode_str, "Oct");
            break;
        }
    }

    // prints out interface
    printf("\n");
    printf("****************************************\n");
    printf("* Base Values:         Input Mode: %s *\n", mode_str);
    printf("*   Binary  :  %s     *\n", bin_string);
    printf("*   Hex     :  %04X                    *\n", (unsigned short)val);
    printf("*   Octal   :  %06o                  *\n", (unsigned short)val);
    printf("*   Decimal :  %-6d                  *\n", val);
    printf("****************************************\n");
    printf("\n");
}

char print_menu(void) { //print menu; get option until valid; return option
    int valid_option = 0;
    char opt;
    while (valid_option == 0) { //menu
        printf("Please select one of the following options: \n\n");
        printf("B  Binary Mode            &  AND with Value           +  Add to Value\n");
        printf("O  Octal Mode             |  OR  with Value           -  Subtract from Value\n");
        printf("H  Hexadecimal Mode       ^  XOR with Value           N  Negate Value\n");
        printf("D  Decimal Mode           ~  Complement Value\n\n");
        printf("C  Clear Value            <  Shift Value Left\n");
        printf("S  Set Value              >  Shift Value Right\n\n");
        printf("Q  Quit\n");
        printf("\n");
        printf("Option: ");
        char option[10];
        scanf("%10s", option); // user input
        if (strlen(option) != 1) {
            printf("\nInvalid option: %s\n\n", option);
            continue;
        }
        if (islower(option[0]) != 0) { //Making Option Uppercase
            opt = toupper(option[0]);
        } else { //grabbing first character
            opt = option[0];
        }
        printf("%s\n", option);// echo printing
        if (opt == 'O' || opt == 'H' || opt == 'D' || opt == 'C' || opt == 'S' || opt == 'Q' || opt == 'B' ||
            opt == '&' || opt == '|' || opt == '^' || opt == '~' || opt == '<' || opt == '>' || opt == '+' ||
            opt == '-' || opt == 'N') { //Checking option value
            valid_option = 1;
            switch (opt) {
                case 'O': {
                    printf("Mode: Octal\n");
                    break;
                }
                case 'H': {
                    printf("Mode: Hexadecimal\n");
                    break;
                }
                case 'D': {
                    printf("Mode: Decimal\n");
                    break;
                }
                case 'B': {
                    printf("Mode: Binary\n");
                    break;
                }
            }
            return opt;
        } else { //letting user know response was invalid
            printf("\nInvalid option: %s\n\n", option);
        }
    }
}

int main(void){ //main menu loop; execute option or call
    char mode = 'D'; //default mode
    short val = 0; //default value
    int quit_option = 1;
    while(quit_option){ //endless loop until quit option is chosen
        print_bases(val, mode); //print bases with default values
        char opt = print_menu();
        switch(opt){
            case 'C': //sets value to 0
                val = 0;
                break;
            case 'S': // lets user set value
                val = get_operand(mode);
                break;
            case 'Q': // lets user quit program
                quit_option = 0;
                break;
            case 'O': //  octal mode
            case 'H': // hex mode
            case 'D': // decimal mode
            case 'B': // binary mode
                mode = opt;
                break;
            case '&': {// bitwise and
                short other_op = get_operand(mode);
                val &= other_op;
                break;
            }
            case '|': {// bitwise or
                short other_op = get_operand(mode);
                val |= other_op;
                break;
            }
            case '^': {// bitwise XOR
                short other_op = get_operand(mode);
                val ^= other_op;
                break;
            }
            case '~': {// bitwise complement
                val = ~val;
                break;
            }
            case 'N': { // negate
                val = ~val + 1;
                break;
            }
            case '+': {// addition
                add(&val, mode);
                break;
            }
            case '-': {// subtraction
                subtract(&val, mode);
                break;
            }
            case '<': { //left shift
                int shift;
                printf("Enter number of positions to left shift value: ");
                scanf("%d", &shift);
                printf("%hd\n", shift);
                val <<= shift;
                break;
            }
            case '>': { //right shift
                int shift;
                printf("Enter number of positions to right shift value: ");
                scanf("%d", &shift);
                printf("%hd\n", shift);
                val >>= shift;
                break;
            }
        }
    }
}