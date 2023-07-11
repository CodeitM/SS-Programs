//To implement a 2-pass SIC Assembler

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define LENGTH 10

int Search_Symtab(char *label);
int Search_Optab(char *opcode);
int Check_Index(char *operand);
int Change_Address(int address);

void main(){

    int i_operand,locctr,ins_length,start_address,end_address,length;

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

    /* First Pass Complete */

    /*2nd Pass Begin */
    FILE *second,*optab;

    first = fopen("first.txt","r");
    symtab = fopen("symtab.txt","r");
    optab = fopen("optab.txt","r");
    second = fopen("second.txt","a");

    int header = 0,index = 0,label_address,op,max_record_size=0, mem_break=0;
    char address[LENGTH],t_start[LENGTH]="",t_length[2],t_records[85]="",buffer[100]="",t[85],record_start_address[5]="";

    while(!feof(first)){
        fscanf(first,"%s %s %s",address,opcode,operand);

        //Header record is created
        if(header == 0){
            fprintf(second,"H^%s^00%s^0%x\n",name,address,length);
            strcpy(record_start_address,address);
            header++;
            continue;
        }

        if(strcmp(opcode,"END")!=0){            //Searches till END keyword is reached
            op = Search_Optab(opcode);
            if(op!= -1){                        //If opcode is found in OPTAB
                if(strcmp(operand,"-") != 0){   //If there exists a value in operand field
                    if(strstr(operand,",")){    //If indexing is present, set index flag and get operand
                        index = 1;
                        strcpy(operand,strtok(operand,",")); 
                    }
                    label_address = Search_Symtab(operand);
                    if(label_address!= -1){        //If the symbol in operand field is found on SYMTAB
                        if(index == 1){             //Change address if indexing exists
                            label_address = Change_Address(label_address);
                            index = 0;
                        }
                    }
                    else{                          //If symbol not found in symbol table
                        fprintf(second,"Error! Symbol Not Found in Symbol Table");
                        label_address=0x0000;
                    }
                }
                else                              //If operand not found in operand field
                    label_address=0x0000;
                
            }
            if((strcmp(opcode,"RESW") == 0)||(strcmp(opcode,"RESB") == 0)){ //If the opcode is RESW or RESB, allocate memory only
                mem_break++;
                continue;
            }
            else if((max_record_size > 28)||(mem_break > 0)){           //If record size will exceed 30 on adding current instruction,create new
                strcpy(t_start,"");                                         //text record
                strcat(t_start,"T^00");                                 //Set the starting part of text record
                strcat(t_start,record_start_address);
                strcat(t_start,"^");
                //printf("\n If 30 and above: %s",t_start);
                strcat(t,t_start);


                //Add size of text record
                strcpy(buffer,"");
                strcat(t,itoa(max_record_size,buffer,16));
                strcat(t,"^");

                //Add all instruction codes to text record
                strcat(t,t_records);
                

                //Print the text record to the object code
                fprintf(second,t);                    
                fprintf(second,"\n");

                //Reset array for next text record
                strcpy(t,"");
                strcpy(t_start,"");
                strcpy(t_records,"");
                strcpy(t_length,"");
                strcpy(record_start_address,address);
                
                max_record_size = 0;
                mem_break = 0;
                
                strcpy(t_start,"T^00");
                strcat(t_start,address);
                strcat(t_start,"^");
                
            }
            //If opcode is WORD , then add the direct value
            if(strcmp(opcode,"WORD")==0){
                label_address=strtol(operand,NULL,16);
                strcpy(buffer,"");
               
                for(int i=0;i<(6-strlen(itoa(label_address,buffer,16)));i++){
                    strcat(t_records,"0");
                } 
                strcpy(buffer,"");          
                strcat(t_records,itoa(label_address,buffer,16));
                strcat(t_records,"^");
                //printf("\n Word : %s ",t_records);
                max_record_size+=3;
                continue;
            }
            else if(strcmp(opcode,"BYTE")==0){
                label_address=strtol(operand,NULL,16);
                        //strcat(t_records,"0000");
                        
                strcpy(buffer,"");
                for(int i=0;i<(6-strlen(itoa(label_address,buffer,16)));i++){
                    strcat(t_records,"0");
                } 
                strcpy(buffer,""); 
                strcat(t_records,itoa(label_address,buffer,16));
                strcat(t_records,"^");
                
                max_record_size+=3;
                continue;
            }
            else{
                strcpy(buffer,"");
                if(strlen(itoa(op,buffer,16)) <2){
                    strcat(t_records,"0");
                }
                strcpy(buffer,"");
                strcat(t_records,itoa(op,buffer,16));
                strcpy(buffer,"");
                if(strlen(itoa(label_address,buffer,16))< 4){
                    strcpy(buffer,"");
                    strcat(t_records,itoa(label_address,buffer,16));
                    strcat(t_records,"00");
                }
                strcpy(buffer,"");
                strcat(t_records,itoa(label_address,buffer,16));
                strcat(t_records,"^");
                //printf("\n Word : %s ",t_records);
                max_record_size+=3;
               
                continue;
            }
        }
            
        else{

            strcat(t,t_start);
            strcpy(buffer,"");
            strcat(t,itoa(max_record_size,buffer,16));
            strcat(t,"^");
            strcat(t,t_records);
            fprintf(second,t);
            //printf("\n End last");
            fprintf(second,"\n");
            fprintf(second,"E^00");
            //printf("\n Start_address = %s",itoa(start_address,buffer,16));
            strcpy(buffer,"");
            fprintf(second,itoa(start_address,buffer,16));
            fprintf(second,"^");
            break;
        }
        continue;
    }
}

//Function to check for label in symbol table
int Search_Symtab(char *label){
    int add;
    char read[LENGTH], op[LENGTH];
    FILE *sym;
    sym = fopen("symtab.txt","r");
    while(!feof(sym)){
        fscanf(sym,"%s %s",read,op);
        add = strtol(op,NULL,16);
        if(strcmp(read,label)==0)
            return add;
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
int Change_Address(int address){
    char adder[LENGTH]="8000";
    int add = strtol(adder,NULL,16);
    address = address+add;
    return address;
}
