#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_schema(FILE* fptr){
	fprintf(fptr,"CREATE TABLE users(username TEXT, password TEXT);");
	return;
}

int main(){
	printf("-----------------------------\n");
	printf("WELCOME TO THE SIGN-UP Page ! \n");
	printf("-----------------------------\n");
	FILE* fptr = fopen("signup.sql","w");
	if(!fptr){
		printf("File not found \n");
		exit(1);
	}
	char choice[10];
	load_schema(fptr);
	while(1){
		char username[100];
		char password[100];
		printf("Username:");
		fgets(username,sizeof(username),stdin);
		username[strcspn(username,"\n")] = '\0';
		printf("Password:");
		fgets(password,sizeof(password),stdin);
		password[strcspn(password,"\n")] = '\0';
		fprintf(fptr,"INSERT INTO users VALUES('%s','%s');",username,password);
		fflush(fptr);
		printf("\nUser successfully registered \n");
		printf("Do you want to continue registration [y/n] ?:");
		fgets(choice,sizeof(choice),stdin);	
		if(choice[0] == 'y'){
			continue;
		}else{
			break;
		}
	}
	fclose(fptr);
	return 0;
}
