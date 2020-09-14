#include <stdio.h>

void zing(void) {
char *getlogin();
char *username = getlogin();
printf("Hello %s. You're the best!\n", username);
}
