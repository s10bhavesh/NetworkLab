#include<stdio.h>
#include<string.h>
int main(){
char a[100000];
FILE *fp;
fp = fopen("input.txt","r");
a[0] = '-';
int i = 0;
while(a[i] != EOF){
i++;
fscanf(fp,"%c",&a[i]);
}
int flen = strlen(a);
printf("no of words : %d \n",flen);
printf("Enter Word : ");
char word[100];
scanf("%s",&word);
int len = strlen(word);
int count = 0;
for(i = 1; i <= flen-len+1;i++){

int j = 0;
char buff[100];
while( j < len ){
buff[j] = a[i+j];
j++;
}
int match = 1;
j = 0;
while(j < len){
if(buff[j] != word[j]) {
match = 0;
break;
}
j++;
}
if(match) count++;
}
printf("No of matches : %d\n",count); 
return 0;
}
