#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//
// Password to be guessed
//
static char *correct_pass = "IsNotSecret";

//
// Helper function to artificially introduce some timing delay to the password-check
// function. If this is not done, the timing difference between characters will be
// nearly imperceptible.
void delay() {
    int j, q;
    for (j = 0; j < 100; j++) {
        q = q + j;
    }
}

int check_pass(char *pass) {
    int i;
    for (i = 0; i <= strlen(correct_pass); i++) {
        delay();  // artificial delay added for timehack
        if (pass[i] != correct_pass[i])
            return 0;
    }
    return 1;
};

void hack_system(char *correct_pass) {
    if (check_pass(correct_pass)) {
        printf("OK: You have found correct password: '%s'\n", correct_pass);
        printf("OK: Congratulations!\n");
        exit(0);
    } else {
        printf("FAIL: The password is not correct! You have failed\n");
        exit(3);
    };
};
