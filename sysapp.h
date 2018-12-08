#ifndef __SYSAPP_H__
#define __SYSAPP_H__

// this function checks the user password
// input:  
//      pass   - a null-terminated password string
// output:
//      0 if the password was incorrect
//      1 if the password was correct
//
int check_pass(char *pass);

// function to check if guessed password is correct
void hack_system(char *correct_pass);

#endif
