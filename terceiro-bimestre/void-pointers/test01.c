#include <stdio.h>
#include <stdlib.h>

void printStuff(void *ptr, int numberOfElements) {
    for(int i = 0; i < numberOfElements; i++) {
        printf("%x\n", ((char*)ptr)[i]);
    }
}

int main(int arg, char *argv[]) {
    int numberArray[] = {1, 3, 5, 7};

    printStuff(numberArray, sizeof(numberArray));

    return 0;
}