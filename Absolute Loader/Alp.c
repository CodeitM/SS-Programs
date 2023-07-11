//Absolute loader practice

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void main(){
    FILE *objcode;
    int start_addr,prgm_length,previous_address;
    char record_type[100],name[6],address[6],length[6],rt[1];
    objcode = fopen("object_code.txt","r");
    printf("\n H0");

    while(!feof(objcode)){
    
        fscanf(objcode,"%s",record_type);
       if(record_type[0] == 'H'){
           int i = 2;
           while(record_type[i] != '^' ){
               strncat(name,&record_type[i],1);
               i++;
           }
           printf("\n %s",name);
           i++;
           while(record_type[i] != '^' ){
               strncat(address,&record_type[i],1);
               i++;
           }
           start_addr = strtol(address,NULL,16);
           i++;
            while(record_type[i] != '^' ){
               strncat(length,&record_type[i],1);
               i++;
           }
           i++;
           prgm_length = strtol(length,NULL,16);
           previous_address= start_addr;

       }
       else if(record)
    }
}