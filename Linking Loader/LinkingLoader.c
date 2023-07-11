//Implementing the linking Loader

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct ESTAB{
    char cs_name[10];
    char sym_name[10];
    int address;
    int cs_length;
}est[30];

struct OBJ{
    char b[2];
    int add;
}objcode[500];

int k = 0,c=0;

int Search_CS_ESTAB(char name[10]);
void Display();
int Search_SYM_ESTAB(char symbol[10]);

void main(){

    FILE *objfile;
    objfile = fopen("object_file.txt","r");

    if(objfile == NULL){
        printf("\n The file not found! Error!");
        exit(0);
    }

    int progaddr,csaddr,cslth;
    char record[100];

    printf("\n Enter the address where it should be loaded: ");
    scanf("%x",&progaddr);

    
    csaddr = progaddr;
    //printf("\n here00");

    while(!feof(objfile)){
        //printf("\n here00");
        fscanf(objfile,"%s",record);
        printf("\n REcord: %s",record);


        if(record[0] == 'H'){

            char ctrlsec_name[10]="",s_addr[6]="",s_length[6]="",temp_name[10]= "";
            int i = 2,ctrlsec_add,ctrlsec_len;

            strcpy(ctrlsec_name,"");
            strcpy(s_addr,"");
            strcpy(s_length,"");

            while(record[i] != '^'){
                strncat(ctrlsec_name,&record[i],1);
                i++;
            }
            i++;

            //printf("\n sec= %s",ctrlsec_name);
            strcpy(temp_name,ctrlsec_name);
            
            while(record[i] != '^'){
                strncat(s_addr,&record[i],1);
                i++;
            }
            
            ctrlsec_add = strtol(s_addr,NULL,16);
            //printf("\n csr add = %x",ctrlsec_add);
            i++;
            //printf("\n temp_name = %s",temp_name);
            
            while(record[i] != '^'){
                strncat(s_length,&record[i],1);
                i++;
            }
            printf("\n temp_name = %s",temp_name);
            
            ctrlsec_len = strtol(s_length,NULL,16);
            cslth = ctrlsec_len;
            i++;
            printf("\n progaddr change = %x",progaddr);
            
            if(Search_CS_ESTAB(temp_name) == 1){
                printf("\n Error! Duplicate Name!");
                exit(0);
            }
            else{
                strcpy(est[k].cs_name,temp_name);
                strcpy(est[k].sym_name,"-");
                est[k].address = csaddr;
                est[k].cs_length = ctrlsec_len;
                k++;
            }
            Display();
        }
        else if(record[0] == 'D'){
            int i = 2;
            char def_name[10], s_addr[6],temp_name[10];
            strcpy(def_name,"");
            //strcpy(s_addr,"");
            strcpy(temp_name,"");
            int def_addr;

            while(record[i] != '\0'){
                while (record[i] != '^'){
                    strncat(def_name,&record[i],1);
                    i++;
                }
                strcpy(temp_name,def_name);
                i++;
                strcpy(s_addr,"");
                while (record[i] != '^'){
                    strncat(s_addr,&record[i],1);
                    i++;
                }
                i++;
                //printf("\n saddr = %s",s_addr);

                def_addr = strtol(s_addr,NULL,16);
                //printf("\n defaddr = %x %d",def_addr,def_addr);
                printf("\n progaddr change = %x",progaddr);

                if(Search_SYM_ESTAB(temp_name) > -1){
                    printf("\n Error! Duplicate Symbol!");
                    exit(0);
                }
                else{
                    strcpy(est[k].cs_name,"-");
                    strcpy(est[k].sym_name,temp_name);
                    est[k].address = csaddr+def_addr;
                    est[k].cs_length = 0;
                    k++;
                }
                Display();
            }
        }
        else if(record[0] == 'E'){
            csaddr = cslth + csaddr;
        }
        
        continue;
        fclose(objfile);
    }
    
// End of Pass One
//************************************************************************************************************

    objfile = fopen("object_file.txt","r");
    csaddr = progaddr;
    //char objcode[500] = "";
    int previous_addr = progaddr;
    int  line = 0, tab = 0;

    while(!feof(objfile)){
        int cslth;
        fscanf(objfile,"%s",record);

        if(record[0] == 'H'){
            int i = 2;
            char ctrlsec_name[10],ctrlsec_len[10];
            strcpy(ctrlsec_name,"");
            while(record[i] != '^'){
                strncat(ctrlsec_name,&record[i],1);
                i++;
            }
            i++;
            while(record[i] != '^'){
                i++;
            }
            i++;
            while(record[i] != '^'){
                strncat(ctrlsec_len,&record[i],1);
                i++;
            }
            cslth = strtol(ctrlsec_len,NULL,16);

            i++;
        }
        else if(record[0] == 'T'){
            int i = 2,t_start_addr,t_length,byte_hex,start_addr;
            char t_rec_addr_s[6],t_rec_len_s[6],byte[2];
            strcpy(t_rec_addr_s,"");
            strcpy(t_rec_len_s,"");

            while (record[i] != '^'){
                    strncat(t_rec_addr_s,&record[i],1);
                    i++;
            }
            i++;
            t_start_addr = strtol(t_rec_addr_s,NULL,16);

            while (record[i] != '^'){
                    strncat(t_rec_len_s,&record[i],1);
                    i++;
            }
            i++;
            t_length = strtol(t_rec_len_s,NULL,16);
            start_addr = t_start_addr + csaddr;

            while(record[i] != '\0'){
                while(record[i] != '^'){
                    strcpy(byte,"");
                    strncat(byte,&record[i],1);
                    strncat(byte,&record[++i],1);

                    byte_hex = strtol(byte,NULL,16);
                    

                    //printf("\n previosadd = %x, t_start_add = %x",previous_addr,start_addr);

                    if(previous_addr < start_addr){
                        while(previous_addr < start_addr){

                                strcpy(objcode[c].b,"..");
                                objcode[c].add = previous_addr; 
                                //printf("\n %x  %s",objcode[c].add,objcode[c].b);
                                c++;
                                previous_addr++;
                            }

                    }

                    strcpy(objcode[c].b,byte);
                    objcode[c].add = start_addr; 
                    //printf("\n %x  %s",objcode[c].add,objcode[c].b);
                    c++;
                    start_addr++;
                    i++;
                    previous_addr = start_addr;
                }
                i++;
            }
            
            
        }
        else if(record[0] == 'M'){
            int i = 2, m_addr,half_bit;
            char m_addr_s[6] = "",hb_s[6]="",op[1],mod_name[6]="";
            while(record[i] != '^'){
                strncat(m_addr_s,&record[i],1);
                i++;
            }
            i++;
            m_addr = strtol(m_addr_s,NULL,16);
            while(record[i] != '^'){
                strncat(hb_s,&record[i],1);
                i++;
            }
            i++;
            half_bit = strtol(hb_s,NULL,16);
            strncpy(op,&record[i],1);
            i++;
            while(record[i] != '^'){
                strncat(mod_name,&record[i],1);
                i++;
            }
            i++;

            int sym_value = Search_SYM_ESTAB(mod_name);



            if(sym_value == -1){
                printf("\n Symbol Not Entered! Error!");
                exit(0);
            }
            else{

            }





        }
    }

}

int Search_CS_ESTAB(char name[10]){
    //printf("\n name = %s",name);
    for(int i = 0; i < 10;i++){
        if(strcmp(est[i].cs_name,name) == 0)
            return 1;
    }
    return 0;
}

void Display(){
    for(int i = 0; i <10 ;i++){
        printf("\n CS-name: %s Sym_name: %s Address: %x Length : %x",est[i].cs_name,est[i].sym_name,est[i].address,est[i].cs_length);
    }

}

int Search_SYM_ESTAB(char symbol[10]){
    //printf("\n symbol = %s",symbol);
    for(int i = 0; i < 30;i++){
        if(strcmp(est[i].sym_name,symbol) == 0)
            return i;
    }
    return -1;
}

/*void Modify_Address(char op,int index,int csaddr,int sp_add){
    switch(op){
        int sym_value = est[index].address;
        int change_address = Find_in_ObjectCode(csaddr+sp_add);
        char bytesfound[10]="";
        strcat(bytesfound,objcode[change_address].b);
        strcat(bytesfound,objcode[change_address+1].b);
        strcat(bytesfound,objcode[change_address+2].b);

        int bytes = strtol(bytesfound,NULL,16);

        case '+':
        {
            objcode
        }
    }

}*/

int Find_in_ObjectCode(int f_add){
    int length_of_objecode = sizeof(objcode)/(sizeof(char[2])+sizeof(int));
    for(int i = 0;i< length_of_objecode;i++){
        if(objcode[i].add == f_add)
            return i;
        
    }
    return -1;
}

