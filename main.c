#include <stdlib.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>

const char *WELCOME_MSG = "Welcome to DB Commander\nType !h for available commands\nType !q to quit\n";
const char *COMMANDS[7] = {"!h", "!q", "!s", "!c", "!i", "!u", "!d"};
const char OPERATIONS[5] = {'>', '<', '=', '!', '-'};


void help() {
    const char *helpTitle = "Commands and examples below.\n\n";
    const char *selectCommand = "1. SELECT. Ex: !s person WHERE id=1\n";
    const char *createCommand = "2. CREATE. Ex: !c person(id,name)\n";
    const char *insertCommand = "3. INSERT. Ex: !i person(1,Nodar)\n";
    const char *updateCommand = "4. UPDATE. Ex: !u person name=Giorgi WHERE id=1\n";
    const char *deleteCommand = "5. DELETE. Ex: !d person WHERE id=1\n";
    printf("%s\n%s\n%s\n%s\n%s\n%s\n", helpTitle, selectCommand, createCommand, insertCommand, updateCommand,
           deleteCommand);
}

void welcomeMsgs() {
    printf("%s", WELCOME_MSG);
}

int getConditionFieldIndex(const char *fields, const char *field) {
    char currentField[100];
    int fieldIndex = 0;
    int currentFieldIndex = 0;
    for (int i = 0; i < strlen(fields); i++) {
        if (fields[i] != ' ') {
            currentField[currentFieldIndex++] = fields[i];
        } else {
            if (strcmp(currentField, field) == 0) {
                return fieldIndex;
            } else {
                fieldIndex++;
            }
        }
    }
    return -1;
}

int getConditionOperationIndex(const char conditionOperation) {
    for (int i = 0; i < 5; i++) {
        if (OPERATIONS[i] == conditionOperation) {
            return 0;
        }
    }
    return -1;
}

int getConditionOperation(const char *query) {
    for (int i = (int) (strlen(query) - 1); i >= 0; i--) {
        if (query[i] == '>' || query[i] == '<' || query[i] == '=' || query[i] == '!' || query[i] == '-') {
            int conditionIndex = getConditionOperationIndex(query[i]);
            if (conditionIndex != -1) {
                return conditionIndex;
            } else {
                return -1;
            }
        }
    }
    return -1;
}

char *getConditionValue(const char *query) {
    char value[100];

    int valueCounter = 0;
    for (int i = (int) (strlen(query) - 1); i >= 0; i--) {
        if (query[i] == '>' || query[i] == '<' || query[i] == '=' || query[i] == '!' || query[i] == '-') {
            char *condValue = malloc(100000);
            strcpy(condValue, value);
            return condValue;
        } else {
            value[valueCounter++] = query[i];
        }
    }
    return NULL;
}

int contains(const char *first, const char *second) {
    return strstr(first, second) != NULL;
}

char *getConditionField(const char *query, const char *fields) {
    char *copyFields = malloc(1000000);
    strcpy(copyFields, fields);
    char *field;
    field = strtok(copyFields, " ");
    while (field != NULL) {
        if (contains(query, field)) {
            return field;
        }
        field = strtok(copyFields, " ");
    }
    return NULL;
}


int isConditional(const char *query) {
    return contains(query, "WHERE");
}


void processSelect(const char *query) {
    char *fileName;
    char nameFromQuery[100];
    for (int i = 3; i < strlen(query); i++) {
        if (query[i] != ' ' && query[i] != '\n') {
            nameFromQuery[i - 3] = query[i];
        } else {
            break;
        }
    }

    fileName = malloc(10000000);
    strcat(fileName, "/home/nika/docDB/tables/");
    strcat(fileName, nameFromQuery);
    FILE *file = fopen(fileName, "r");

    if (file != NULL) {
        char c = fgetc(file);
        char fields[1000];
        int n = 0;
        while (c != '\n' && c != EOF) {
            fields[n++] = c;
            c = fgetc(file);
        }

        if (isConditional(query)) {
            char *conditionField = getConditionField(query, fields);
            if (conditionField != NULL) {
                char *conditionValue = getConditionValue(query);
                if (conditionValue != NULL) {
                    int conditionFieldIndex = getConditionFieldIndex(fields, conditionField);
                    if (conditionFieldIndex != -1) {
                        int conditionOperation = getConditionOperation(query);
                        if(conditionOperation != -1){
                            switch (conditionOperation){
                                case 0:
                                    printf("%c\n", OPERATIONS[0]);
                                    break;
                                default:
                                    printf("hellololo\n");
                                    break;
                            }
                        } else{
                            printf("Invalid condition operation\n");
                            return;
                        }
                    } else {
                        printf("Field %s not found in table %s\n", conditionField, nameFromQuery);
                    }
                } else{
                    printf("Condition not found\n");
                    return;
                }
            } else{
                printf("Condition field not found\n");
                return;
            }
        } else {
            printf("%s\n", fields);
            char c = fgetc(file);
            while (c != EOF) {
                printf("%c", c);
                c = fgetc(file);
            }
        }

    } else {
        printf("Table not found\n");
    }


}

void processCommand(const char *command, const char *query) {

    if (strcmp(command, COMMANDS[0]) == 0) {
        help();
    } else if (strcmp(command, COMMANDS[1]) == 0) {
        exit(0);
    } else if (strcmp(command, COMMANDS[2]) == 0) {
        processSelect(query);
    }
}

void processInput(char *input) {
    if (strlen(input) < 2) {
        printf("Unrecognizable command\n");
        return;
    }
    char command[2];
    for (int i = 0; i < 2; i++) {
        command[i] = input[i];
    }

    processCommand(command, input);
}

int main() {
    welcomeMsgs();
    char *input = malloc(1000 * sizeof(char));
    while (strcmp(input, COMMANDS[1]) != 0) {
        fgets(input, 1000, stdin);
        processInput(input);
        memset(input, 0, sizeof(&input));
    }
    printf("Session finished.\n");

    return 0;
}
