#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define LENGTH 10

int Search_Symtab(char *label);
int Search_Optab(char *opcode);

int length ,start_address;
void main(){

    int i_operand,locctr,ins_length,end_address;
    

    FILE *input, *first, *symtab;
    input = fopen("input_code.txt","r");
    first = fopen("first.txt","a");
    symtab = fopen("symtab.txt","a");

    char label[LENGTH], opcode[LENGTH], operand[LENGTH], name[LENGTH];

    if(input == NULL){
        printf("\n The Code does not Exist!");
        exit(0);
    }

    /*Begginning First Pass */
    while(!feof(input)){
        //printf("\n *");
        fscanf(input,"%s %s %s",label,opcode,operand);
        //printf("\n %s %s %s",label,opcode,operand);
       
        //Reading the start of program
        if(strcmp(opcode,"START") == 0){
             
            if(strcmp(operand,"-") != 0)        //If start address is given, location counter is updated
               locctr= strtol(operand,NULL,16);
            else
                locctr = strtol(0,NULL,16);     //Start address not given, so location counter ipdated as 0

            if(strcmp(label,"-") != 0){
                strncpy(name,label,LENGTH);
            }
            else{
                strncpy(name," ",LENGTH);
            }
            fprintf(symtab," %s %x \n",label,locctr);
            fprintf(first,"%x %s %s \n",locctr,opcode,operand);
            start_address = locctr;
            continue;
        }
        //Checking remaining lines of code until the end
        if(strcmp(opcode,"END") != 0){
            //printf("\n Are we here?");
            if(strcmp(label,"-") != 0){                         //If there is a value in label fiels
                //printf("\n Got through the label -");
                if(Search_Symtab(label) != -1)                 //Checking SYMTAB if label already exists
                    fprintf(first,"Error: Duplicate Symbol!");    
                else
                    fprintf(symtab," %s %x \n",label,locctr);     
            }
            //printf("\n Over herer!");
            //int hi = Search_Optab(opcode);
            //printf("\n the hi =%d",hi);
            if(Search_Optab(opcode) != -1){                  //Checks whether opcode is valid
                //printf("\n Checks opcode");
                ins_length=3;
                locctr+=ins_length;
            }
            else if(strcmp(opcode,"WORD") == 0){
                ins_length=3;
                locctr+=ins_length;
            }
            else if(strcmp(opcode,"RESW") == 0){
                i_operand = strtol(operand,NULL,16);
                ins_length=3*i_operand;
                locctr+=ins_length;
            }
            else if(strcmp(opcode,"RESB") == 0){
                i_operand= strtol(operand,NULL,16);
                ins_length=i_operand;
                locctr+=ins_length;
            }
            else if(strcmp(opcode,"BYTE") == 0){
                ins_length=strlen(operand);
                locctr+=ins_length;
            }
            else{
                fprintf(first,"Error: Invalid Opcode!"); 
            }
            fprintf(first,"%x %s %s \n",(locctr-ins_length),opcode,operand);
            continue;
        }
        else{
            fprintf(first,"%x %s %s \n",(locctr-ins_length),opcode,operand);
            end_address = locctr;
            break;
        }
  
    }
    length = end_address - start_address;
    printf("\n The length of the program is: %x",length);

    fclose(first);
    fclose(input);
    fclose(symtab);
}

int Search_Symtab(char *label){
    int add;
    char read[LENGTH], op[LENGTH];
    FILE *sym;
    sym = fopen("symtab.txt","r");
    while(!feof(sym)){
        fscanf(sym,"%s %s",read,op);
        add = strtol(op,NULL,16);
        if(strcmp(read,label)==0){
            printf("\n REad = %s,label=%s",read,label);
            printf("\n Address returned = %d",add);
            return add;
    }
        }
            
    fclose(sym);
    return -1;
}

//Function to check for opcode in OPCODE TABLE
int Search_Optab(char *opcode){
    FILE *optab;
    int hexa;
    char code[LENGTH],hex[LENGTH];
    //printf("\n optab search");
    optab = fopen("optab.txt","r");
    while(!feof(optab)){    
        fscanf(optab,"%s %s",code,hex);
        hexa= strtol(hex,NULL,16);
        //printf("\n Opcode: %s Hex:%x ",code,hexa);
        if(strcmp(code,opcode)==0)
            return hexa;
        else
            continue;
    }
    fclose(optab);
    return -1;
}
