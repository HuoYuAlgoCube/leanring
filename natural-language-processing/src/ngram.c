#include <stdio.h>
#include <string.h>

int is2Bype(int c){
    return (c >= 0x81) && (c <= 0x9F) || (c >= 0xe0) && (c <= 0xef) ? 1 : 0;
}

int main(int argc,char* argv[]){
    printf("Hello N Grame!\n\n");
    char buf[100];
    int n;
    if (scanf("%d\n",&n)!=1){
        fprintf(stderr,"读入非数");
    }
    if (fgets(buf,sizeof buf,stdin) != NULL){
        buf[strcspn(buf, "\n")] = '\0';
    }
    for (int i=0;i<100;i++){
        if (buf[i] == '\0')break;
        for (int j=0;j<n;j++){
            if (i+j >= 100)break;
            putchar(buf[i+j]);
        }
        printf("\n");
    }
    return 0;
}
