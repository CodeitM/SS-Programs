//Implementing an absolute loader

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ins_length 6
#define record_length 100
#define byte_length 2


void main(){

    int strt_add,strt_add_r, length_prgm,record_len, byte_n,previous_address;
    FILE *obj_code, *mem_code;
    char prgm_name[10], record[record_length], name[10]="", start_address[ins_length]="", length_str[3]="",r_length[byte_length]="",byte[byte_length]="",buffer[10],record_start_address[ins_length]="";

    obj_code = fopen("object_code.txt","r"); 
    mem_code = fopen("memory_allocation.txt","a");

    if(obj_code == NULL){
        printf("\n The object code does not exist!");
        exit(0);
    }

    printf("\n Enter the name of the program: ");
    scanf("%s",&prgm_name);

    while(!feof(obj_code)){
        
        fscanf(obj_code,"%s",record);
        if(record[0] == 'H'){
            int i = 2;

            while(record[i] != '^'){
                strncat(name,&record[i],1);
                i++;
            }

            if(strcmp(name,prgm_name) != 0){
                printf("\n Program name is not verified!");
                exit(0);
            }
            else{
                i++;

                while(record[i] != '^'){
                    strncat(start_address,&record[i],1);
                    i++;
                }
                strt_add = strtol(start_address,NULL,16);
                
                i++;
                while(record[i] != '^'){
                    strncat(length_str,&record[i],1);
                    i++;
                }
                length_prgm = strtol(length_str,NULL,16);
            }
            
            fprintf(mem_code,"Memory Allocation Starting from location %x onwards",strt_add);
            previous_address = strt_add;
            continue;
        }
        else if(record[0] == 'T'){
            int i = 2;
            strcpy(record_start_address,"");
            strcpy(r_length,"");

            while(record[i] != '^'){
                    strncat(record_start_address,&record[i],1);
                    i++;
                }
            strt_add_r = strtol(record_start_address,NULL,16);

            i++;
            while(record[i] != '^'){
                strncat(r_length,&record[i],1);
                i++;
            }

            record_len = strtol(r_length,NULL,16);

            

            if(strt_add_r != previous_address){
                while(previous_address != strt_add_r){
                    fprintf(mem_code,"\n %x xx",previous_address);
                    previous_address++;
                }
            }

            int count = 0;
            i++;
            int address = strt_add_r;
            while(count < record_len){
                
                if(record[i] == '^'){
                    i++;
                    continue;
                } 
                else{
                    strcpy(byte,"");
                    strncat(byte,&record[i],1);
                    
                    strncat(byte,&record[++i],1);
                    strcpy(buffer,"");
                    fprintf(mem_code,"\n %s %s",itoa(address,buffer,16),byte);                   
                    address = address + 1;
                    i++;
                    count++;
                
                    continue;
                }             
            }
            previous_address = address;
            continue;
        }
        continue;
    }

    if((previous_address - 1 - strt_add)!= length_prgm){
        while((previous_address - 1 - strt_add)!= length_prgm){
            fprintf(mem_code,"\n %x xx",previous_address);
            previous_address++;
        }
    }

}