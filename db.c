#include <stdio.h>
#include <stdlib.h>
#include <db_error.h>
#include <db_memory.h>
#include <db_lexer.h>
#include <db_parser.h>
#include <db_executor.h>
#include <string.h>
#include <ctype.h>
#include <db_dot_command.h>

void print_on_startup(){
    printf("Mini RDBMS v0.0.0 \n");
    printf("Type .help for help \n");
}

static int is_comment(char* line){
	int i =0;
	int n = strlen(line);
	while(isspace(line[i])) i++;
	if(i + 2 < n && line[i] == '-' && line[i+1] == '-'){
		return 1;
	}
	return 0;
}
void run_statement(char* line){
        if(is_comment(line)) return;
	line_lexer(line);
        current = lexer_output;
        astNode* statement_node = parseStatement();
        execute(statement_node);
        // reset
        current = NULL;
        lexer_output = NULL;
        lexer_tail = NULL;
}

int main(int argc, char** argv){
    if(argc > 1){
	char* fileName = argv[1];
	FILE* fptr = fopen(fileName,"rb");
	if(!fptr){
		log_error("Cannot access target query file",RUNTIME_ERROR);
	}
	char query[1024];
	size_t query_len = 0;
	int c;
	while((c = fgetc(fptr)) != EOF){
		if(query_len >= sizeof(query) - 1){
			log_error("Query too large", RUNTIME_ERROR);
		}
		query[query_len++] = c;
		if(c == ';'){
			query[query_len] = '\0';
			run_statement(query);
			query_len = 0;
		}
	}
	return 0;
    }
    char line[1024];
    print_on_startup();
    while(1){
        printf(">> ");
        if(fgets(line,sizeof(line),stdin) == NULL){
            break;
        }
        if(line[0] == '\0' || line[0] == '\n') continue;
        // check for Dot commands
        line[strcspn(line,"\n")] = '\0';
        if(strcmp(line,".exit") == 0 || strcmp(line,".quit") == 0){
            break;
        }
        if(line[0] == '.'){
            execute_dot_command(line);
            continue;
        }
    	run_statement(line);
    }
    clean_up();
    return 0;
}
