#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char mask_bits[50];

void Expand_Mask(char value[20]);

void main()
{
    FILE *input,*output;
    char word[50],pg_name[20],start_addr[50],length[50],mask[20];
    int record_start_addr,i, load_addr;

    input=fopen("input1.txt","r");
    output=fopen("output.txt","w");

    printf("Enter address to which file is to be loaded: ");
    scanf("%x",&load_addr);
    
    fscanf(input,"%s",word);

    while(strcmp(word,"E")!=0)
    {
        if(strcmp(word,"H")==0)
        {
            fscanf(input,"%s%s%s",pg_name,start_addr,length);
            fscanf(input,"%s",word);
           continue;
        }
        else if(strcmp(word,"T")==0)
        {
            fscanf(input,"%s%s%s",start_addr,length,mask);

            record_start_addr=(int)strtol(start_addr, NULL, 16);
            record_start_addr+=load_addr;
            Expand_Mask(mask);
            fscanf(input,"%s",word);

            i=0;
            continue;
        }
        if(mask_bits[i]=='1')
        {

            char addr_bits[10],op_bits[5];
            strcpy(addr_bits,"");
            strcpy(op_bits,"");
            strncpy(addr_bits,&word[2],4);
            strncpy(op_bits,&word[0],2);
            int address=(int)strtol(addr_bits, NULL, 16);
            address+=load_addr;
            fprintf(output,"%04x\t%s%x\n",record_start_addr,op_bits,address);
            record_start_addr+=0x3;
        }
        else if(mask_bits[i]=='0')
        {
            fprintf(output,"%04x\t%s\n",record_start_addr,word);
            record_start_addr+=0x3;
        }i++;
        fscanf(input,"%s",word);
    
    }
    fclose(input);
    fclose(output);
    /*input=fopen("reoutput.dat","r");
    printf("\n------OUTPUT OF RELOCATION LOADING-------\n");
    char ch=fgetc(input);
     while(ch!=EOF)
     {
         printf("%c",ch);
         ch=fgetc(input);
     }*/

}

void Expand_Mask(char value[20])
{
    strcpy(mask_bits,"");
     int i=0;
    int length=strlen(value);
   
  
    while(i<length)
    {
        switch (value[i])
        {
        case '0':strcat(mask_bits,"0000");
            break;
        case '1':strcat(mask_bits,"0001");
            break;
         case '2':strcat(mask_bits,"0010");
            break;
        case '3':strcat(mask_bits,"0011");
            break;
        case '4':strcat(mask_bits,"0100");
            break;
        case '5':strcat(mask_bits,"0101");
            break;
        case '6':strcat(mask_bits,"0110");
            break;
        case '7':strcat(mask_bits,"0111");
            break;
        case '8':strcat(mask_bits,"1000");
            break;
        case '9':strcat(mask_bits,"1001");
            break;
        case 'A':strcat(mask_bits,"1010");
            break;
        case 'B':strcat(mask_bits,"1011");
            break;
        case 'C':strcat(mask_bits,"1100");
            break;
        case 'D':strcat(mask_bits,"1101");
            break;
        case 'E':strcat(mask_bits,"1110");
            break;
        case 'F':strcat(mask_bits,"1111");
            break;                                                       
        
        
        }
        i++;
    }
}
