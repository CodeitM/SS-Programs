//Implementing Linking Loader

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int Check_ESTAB(char name[10]);
int Find_Address_ESTAB(char name[10]);
int Find_New_Loc(char op, int m_addr,int cs_addr);

void main(){

    //First Pass of Linking Loader

    int prog_addr, cs_addr, cs_strt_add,cslth,addr,length = 0,execaddr,t_rec_addr;
    char record[100],cs_start_address[6],name[10]="",cs_length[6]="",symbol[10]="",address[6],text_record_address[6];

    FILE *obj_file, *est_tab;
    obj_file = fopen("object_file.txt","r");
    est_tab = fopen("external_symbol_table.dat","a");

    if(obj_file == NULL){
        printf("\n The Object File does not exist!");
        exit(0);
    }
      
      
    printf("\n Enter the location where the program has to be loaded: ");
    scanf("%x",&prog_addr);
      

    cs_addr = prog_addr;

    while(!feof(obj_file)){
        fscanf(obj_file,"%s",record);

        if(record[0] == 'H'){
            int i=2;

            strcpy(name,"");
            strcpy(cs_length,"");
            strcpy(cs_start_address,"");

               

            while(record[i]!='^'){
                strncat(name,&record[i],1);
                i++;
            }
            i++;
            while(record[i]!='^'){
                strncat(cs_start_address,&record[i],1);
                i++;
            }
    
            cs_strt_add = strtol(cs_start_address,NULL,16);

            cs_strt_add = cs_strt_add + cs_addr + length;
            i++;

            char p_name[10];
            strcpy(p_name,name);
   
            while(record[i]!='^'){
                strncat(cs_length,&record[i],1);
                i++;
            }
            
            cslth = strtol(cs_length,NULL,16);
            length = length + cslth;


            if(Check_ESTAB(p_name) == 1 ){
                fprintf(est_tab,"\n%s %s %x %x",p_name,"-",cs_strt_add,cslth);
            }
            else{
                printf("\n Duplicate external symbol");
                exit(0);
            }
            continue;
        }
        else if(record[0] == 'D'){
            int i= 2;
            
            while(record[i] != '\0'){
                strcpy(symbol,"");
                strcpy(address,"");
                while(record[i] != '^'){
                    strncat(symbol,&record[i],1);
                    i++;
                }

                i++;
                char p_symbol[10];
                strcpy(p_symbol,symbol);
                while(record[i]!='^'){
                    strncat(address,&record[i],1);
                    i++;
                }
                
                addr = strtol(address,NULL,16);
                addr = addr + cs_strt_add;

                if(Check_ESTAB(symbol) == 1){
                    fprintf(est_tab,"\n%s %s %x %x","-",p_symbol,addr,0);
                    
                }
                else{
                    printf("\n Duplicate external symbol");
                    exit(0);
                }
                i++;
            }
            continue;   
        } 
        continue;
    }
    fclose(est_tab);
    fclose(obj_file);


    //2nd Pass of Linking Loader

   FILE *lk_op;
   int count = 0, flag = 0;
    int trlth,byte_n,previous_address,m_addr,hl, new_location;
   char text_record_length[6],byte[2],m_address[6],m_name[10],half_length[2],op;

    est_tab = fopen("external_symbol_table.dat","r");
    obj_file = fopen("object_file.txt","r");
    lk_op = fopen("linked_output.dat","a");

    cs_addr = prog_addr;
    execaddr = prog_addr;
    previous_address = prog_addr;
    
    

    while(!feof(obj_file)){
        fscanf(obj_file,"%s",record);
        if(record[0] == 'H'){
            int i = 2;
            strcpy(name,"");
            strcpy(cs_length,"");
            strcpy(cs_start_address,"");
            while(record[i]!='^'){
                strncat(name,&record[i],1);
                i++;
            }
            char p_name[10];
            strcpy(p_name,name);
            i++;
            while(record[i]!='^'){
                strncat(cs_start_address,&record[i],1);
                i++;
            }
            cs_strt_add = strtol(cs_start_address,NULL,16);
            i++;
            while(record[i]!='^'){
                strncat(cs_length,&record[i],1);
                i++;
            }   
            cslth = strtol(cs_length,NULL,16);
            

            previous_address=Find_Address_ESTAB(p_name);
            printf("\n previous address =%x",previous_address);
            continue;
        }
        else if(record[0] == 'T'){
            int i = 2;
            strcpy(text_record_address,"");
            while(record[i]!='^'){
                strncat(text_record_address,&record[i],1);
                i++;
            }
            t_rec_addr = strtol(text_record_address,NULL,16);
            addr = t_rec_addr+cs_addr;
            i++;
            while(record[i]!='^'){
                strncat(text_record_length,&record[i],1);
                i++;
            }
            trlth = strtol(text_record_length,NULL,1);
            i++;
            
            if(previous_address < addr){
                printf("\n prevous =%x and addr =%x",previous_address,addr);
                while(previous_address < addr){
                    if(flag == 0){
                        fprintf(lk_op,"%x\t",previous_address);
   
                    }
                    if(count<4){
                        fprintf(lk_op,"%s","..");
                        
                        count++;
                        flag++;
                        previous_address++;
                    }
                    else{
                        fprintf(lk_op,"\t\t%s","..");
                        
                        count=1;
                        flag++;
                        previous_address++;
                    }

                    if(flag == 16){
                        fprintf(lk_op,"\n");
                        
                        flag=0;
                        count=0;
                    }
                    
                }
            }

            while(record[i] != '\0'){

                if(record[i] == '^'){
                    i++;
                    continue;
                }
                strcpy(byte,"");
                strncat(byte,&record[i],1);
                strncat(byte,&record[++i],1);

                if(flag == 0){
                    fprintf(lk_op,"%x\t",addr);
                    
                }
                if(count<4){
                    fprintf(lk_op,"%s",byte);
                    
                    count++;
                    flag++;
                    addr++;
                }
                else{
                    fprintf(lk_op,"\t\t%s",byte);
  
                    count=1;
                    flag++;
                    addr++;
                }

                if(flag == 16){
                    fprintf(lk_op,"\n");
                    
                    flag=0;
                    count=0;
                }

                i++;
            }
            previous_address = addr;
            
        }
        else if(record[0] == 'M'){
            int i = 2;
            strcpy(m_address,"");
            strcpy(m_name,"");
            strcpy(half_length,"");

            while(record[i] != '^'){
                strncat(m_address,&record[i],1);
                i++;
            }
            m_addr = strtol(m_address,NULL,16);
            i++;
            while(record[i] != '^'){
                strncat(half_length,&record[i],1);
                i++;
            }
            hl = strtol(half_length,NULL,16);
            i++;
            op = record[i];
            i++;
            while(record[i] != '^'){
                strncat(m_name,&record[i],1);
                i++;
            }

            m_addr =Find_Address_ESTAB(m_name);
            new_location = Find_New_Loc(op,m_addr,cs_addr);
            if(new_location == -1){
                printf("\n Undefined symbol!");
                exit(0);
            }
            previous_address = addr;
            
        }
        else if(record[0] == 'E'){
            int i = 2,e_addr;
            char end_address[6];

            strcpy(end_address,"");

            while(record[i] != '^'){
                strncpy(end_address,&record[i],1);
                i++;
            }
            e_addr =strtol(end_address,NULL,16);
            execaddr = cs_addr+e_addr;
            cs_addr = cs_addr + cslth;
            if(previous_address > addr){
                while(previous_address < execaddr){
                    if(flag == 0){
                        fprintf(lk_op,"%x\t",previous_address);
                        
                    }
                    if(count<4){
                        fprintf(lk_op,"%s","..");
                        count++;
                        flag++;
                        previous_address++;
                    }
                    else{
                        fprintf(lk_op,"\t\t%s","..");
                        count=1;
                        flag++;
                        previous_address++;
                    }

                    if(flag == 16){
                        fprintf(lk_op,"\n");
                        flag=0;
                        count=0;
                    }
                    
                }
            }
            else{
                while(addr < execaddr){
                    if(flag == 0){
                        fprintf(lk_op,"%x\t",addr);
                    }
                    if(count<4){
                        fprintf(lk_op,"%s","..");
                       
                        count++;
                        flag++;
                        addr++;
                    }
                    else{
                        fprintf(lk_op,"\t\t%s","..");
                        count=1;
                        flag++;
                        addr++;
                    }

                    if(flag == 16){
                        fprintf(lk_op,"\n");
                        flag=0;
                        count=0;
                    }
                    
                }
                previous_address = addr;
                
            }
        }
        
        
    }


        
}

int Check_ESTAB(char name[10]){

    FILE *estab;
    char cs[10],sym_name[10],add[6],len[6];
    int count = 0;

    estab = fopen("external_symbol_table.dat","r");

    while(!feof(estab)){
        
        fscanf(estab,"%s %s %s %s",cs,sym_name,add,len);
        //printf("\n%s %s %s %s %s",cs,sym_name,add,len,name);
        if(strcmp(name,sym_name) == 0)
            return 0;
        else 
            continue;

    }
    fclose(estab);
    return 1;
}

int Find_Address_ESTAB(char name[10]){

    FILE *estab;
    char cs[10],sym_name[10],add[6],len[6];
    int count = 0,addr;

    estab = fopen("external_symbol_table.dat","r");

    while(!feof(estab)){
        
        fscanf(estab,"%s %s %s %s",cs,sym_name,add,len);
        addr = strtol(add,NULL,16);
        if(strcmp(name,cs) == 0)
            return addr;
        else 
            continue;

    }
    fclose(estab);
    return 0;
}

int Find_New_Loc(char op, int m_addr, int cs_addr){
    if(op == '+'){
        return cs_addr+m_addr;
    }
    else if(op == '-'){
        return cs_addr-m_addr;
    }
    else return -1;
}

