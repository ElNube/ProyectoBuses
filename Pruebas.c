#include <stdio.h>
#include <string.h>

int main(){
    char a[20] = "hola", b[20];
    scanf("%20s", b);
    if (strcmp(a, b) == 0)
        printf("Son iguales");
    system("pause");
    return 0;
}