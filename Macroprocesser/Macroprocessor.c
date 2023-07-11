//Program to implement microprocessor

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
    FILE *input, *namtab, *deftab, *argtab, *output;
    int len, i, pos = 1;
    char arg[20], code_op[20], opnd[20], label[20], name[20], def_code_op[20], opnd1[20], pos1[10], pos2[10];
    input = fopen("input.txt", "r");
    namtab = fopen("namtab.txt", "w+");
    deftab = fopen("deftab.txt", "w+");
    argtab = fopen("argtab.txt", "w+");
    output = fopen("op.txt", "w+");
    fscanf(input, "%s%s%s", label, code_op, opnd);
    while (strcmp(code_op, "END") != 0)
    {
        if (strcmp(code_op, "MACRO") == 0)
        {
            fprintf(namtab, "%s\n", label);
            fseek(namtab, SEEK_SET, 0);
            fprintf(deftab, "%s\t%s\n", label, opnd);
            fscanf(input, "%s%s%s", label, code_op, opnd);
            while (strcmp(code_op, "MEND") != 0)
            {
                if (opnd[0] == '&')
                {
                    sprintf(pos1, "%d", pos);
                    strcpy(pos2, "?");
                    strcpy(opnd, strcat(pos2, pos1));
                    pos = pos + 1;
                }
                fprintf(deftab, "%s\t%s\n", code_op, opnd);
                fscanf(input, "%s%s%s", label, code_op, opnd);
            }
            fprintf(deftab, "%s", code_op);
        }
        else
        {
            fscanf(namtab, "%s", name);
            if (strcmp(code_op, name) == 0)
            {
                len = strlen(opnd);
                for (i = 0; i < len; i++)
                {
                    if (opnd[i] != ',')
                        fprintf(argtab, "%c", opnd[i]);
                    else
                        fprintf(argtab, "\n");
                }
                fseek(deftab, SEEK_SET, 0);
                fseek(argtab, SEEK_SET, 0);
                fscanf(deftab, "%s%s", def_code_op, opnd1);
                fprintf(output, "//\t%s\t%s\n", def_code_op, opnd);
                fscanf(deftab, "%s%s", def_code_op, opnd1);
                while (strcmp(def_code_op, "MEND") != 0)
                {
                    if ((opnd[0] = '?'))
                    {
                        fscanf(argtab, "%s", arg);
                        fprintf(output, "-\t%s\t%s\n", def_code_op, arg);
                    }
                    else
                        fprintf(output, "-\t%s\t%s\n", def_code_op, opnd1);
                    fscanf(deftab, "%s%s", def_code_op, opnd1);
                }
            }
            else
                fprintf(output, "%s\t%s\t%s\n", label, code_op, opnd);
        }
        fscanf(input, "%s%s%s", label, code_op, opnd);
    }
    fprintf(output, "%s\t%s\t%s", label, code_op, opnd);

    fclose(input);
    fclose(namtab);
    fclose(deftab);
    fclose(argtab);
    fclose(output);
}