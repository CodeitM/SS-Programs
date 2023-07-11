//To implememt a single pass macroprocessor;

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define TRUE 1
#define FALSE 0
#define size 10

struct ARGTAB{
    int pos;
    char args[10];
    
};

struct NAMTAB{
    char macro_name[10];
    int def_start;
    struct ARGTAB *arg;

}namtab[20];

int expanding = FALSE,def_line_no =0;

FILE *input, *output, *deftab;

int Search_in_Namtab(char opcode[10]);

void main(){

    char label[size],opcode[size],operand[30];
    input = fopen("input.txt","r");
    output= fopen("macro_output","w+");
    deftab = fopen("def_tab","w+");

    if(input == NULL){
        printf("\n Source file not found! Exiting");
        exit(0);
    }

    fscanf(input,"%s %s %s",label,opcode,operand);

    if(strcmp(opcode,"END") != 0){
        
        if(Search_in_Namtab(opcode) != -1){
            //Expand();
            printf("\n Found!");
        }
        else if(strcmp(opcode,"MACRO") == 0){
            Define(label,def_line_no);
            printf("\n define");
        }else{
            fprintf(output,"%s %s %s",label,opcode,operand);
        }

    }

    
}

int Search_in_Namtab(char opcode[10]){
     for(int i=0;i<20;i++){
         printf("\n opcode =%s namtab = %s",opcode,namtab[i].macro_name);
         if(strcmp(opcode,namtab[i].macro_name) == 0){
             return i;
         }
     }
    return -1;
}

void Define(char name[size],int def_line_no){

    char label[size],opcode[size],operand[30],*token;
     output= fopen("macro_output","w+");
    deftab = fopen("def_tab","w+");
     int level = 1;
     input = fopen("input.txt","r");
     fseek(input,0, SEEK_SET);
     fscanf(input,"%s %s %s",label,opcode,operand);
     while((strcmp(label,name)!=0)&&(strcmp(opcode,"MACRO")!=0)&&(feof(input))){
         fscanf(input,"%s %s %s",label,opcode,operand);
     }
     if((strcmp(label,name)==0)&&(strcmp(opcode,"MACRO")==0)){
         fprintf(deftab,"\n%d %s %s %s",def_line_no,label,opcode,operand);
         if(strstr(operand,"&")){
             struct ARGTAB new[10];
             int i = 0;
             new[i].pos =  0;
             
             token =strtok(operand,",");

             while(token != NULL){
                 new[i].pos += 1;
                strcpy(new[i].args,token);

                token = strtok(NULL,",");

            }

            name


             
         }
     }

    


}