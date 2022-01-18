#include <stdio.h>
#include <string.h>
 
void encrypt(char password[],int key)
{
    int i;
    for(i=0;i<strlen(password);++i)
    {
        password[i] = password[i] - key;
    }
}
 
void decrypt(char password[],int key)
{
    int i;
    for(i=0;i<strlen(password);++i)
    {
        password[i] = password[i] + key;
    }
}
int main()
{
    char password[20] ;
    printf("Enter the password: \n ");
    scanf("%s",password);
    printf("Passwrod     = %s\n",password);
    encrypt(password,12);
    printf("Encrypted value = %s\n",password);
    decrypt(password,12);
    printf("Decrypted value = %s\n",password);
    return 0;
}
