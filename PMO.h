//COP3402 Project 1

//Daniel Powley, Ming Jiang



#define MAX_STACK_HEIGHT 2000
#define MAX_LEXICONOGRAHPICAL_LEVEL 3
#define MAX_CODE_LENGTH 500

/* Set up stack*/
int stack[MAX_STACK_HEIGHT];


/* function prototype*/
int base(int l, int base);
void print_pmo(int x);

int print_flag1 = 0;



void PMO()
{
    int run = 1;

    FILE *ifp;

    int pc = 0;
    int sp = 0;
    int bp = 1;

    int i, j;

    int reg[8];
    int flag = 0;
    int bar[20] = {1};



    for(i = 0; i<8; i++){
        reg[i] = 0;
    }

    for(i = 0; i < MAX_STACK_HEIGHT; i ++)
        stack[i] = 0;

    Instruction IR;

    Instruction code[MAX_CODE_LENGTH];

    ifp = fopen("code.txt","r");

    int counter = 0;

     while (!feof(ifp))                                 //gets the OP,R,L,M from the input file
     {
     fscanf(ifp,"%d",&code[counter].op);
      fscanf(ifp,"%d",&code[counter].r);
       fscanf(ifp,"%d",&code[counter].l);
        fscanf(ifp,"%d",&code[counter].m);
         counter++;
     }

if (print_flag1==1)
    printf("Initial Values\t\t\t\tpc\tbp\tsp\n");
    while(run == 1)
    {

    /* fetch*/
    IR = code[pc];
        if (print_flag1==1)
    printf("%d\t", pc);
    pc = pc + 1;

    switch(IR.op)
        {
            case 1:
                /* LIT R,0,M*/
                reg[IR.r] = IR.m;
                if (print_flag1==1)
                {

                printf("lit\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;

            case 2:
                /* RTN 0,0,0*/
                sp = bp - 1;
                bp = stack [sp + 3];
                pc = stack [sp + 4];
                if (print_flag1==1)
                {
                printf("rtn\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;

            case 3:
                /*LOD R,L,M*/
                reg[IR.r] = stack[base(IR.l,bp) + IR.m];
                if (print_flag1==1)
                {
                printf("lod\t");
                printf("%d\t",IR.r); //1
                printf("%d\t",IR.l); //1
                printf("%d\t",IR.m); //4
                }
                break;

            case 4:
                /*STO R,L,M*/
                stack[base(IR.l,bp) + IR.m] = reg[IR.r];
                if (print_flag1==1)
                {
                printf("sto\t");
                printf("%d\t",IR.r); //0
                printf("%d\t",IR.l); //1
                printf("%d\t",IR.m); //4
                }
                break;


            case 5:
                /*CAL 0,L,M*/
                stack[sp + 1] = 0;
                stack[sp + 2] = base(IR.l, bp);
                stack[sp + 3] = bp;
                stack[sp + 4] = pc;
                bp = sp + 1;
                pc = IR.m;

                if (print_flag1==1)
                {
                printf("cal\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;

            case 6:
                /* INC 0, 0, M*/
                sp = sp + IR.m;
                if (print_flag1==1)
                {
                printf("inc\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;

            case 7:
                /*JMP 0,0,M*/
                pc = IR.m;
                if (print_flag1==1)
                {
                printf("jmp\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;


            case 8:
                /*JPC R, 0 , M*/
                if (reg[IR.r] == 0)
                    pc = IR.m;
                if (print_flag1==1)
                {
                    printf("jpc\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                    break;

            case 9:
                /*SIO R, 0 , 1 || 2 || 3*/
                if (print_flag1==1)
                {
                printf("sio\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }

                if(IR.m == 3)
                run = 0;

                else if (IR.m == 2)
                scanf("%d", &reg[IR.r]);

                break;

            case 10:
                /* neg*/
                reg[IR.r] = -reg[IR.l];
                if (print_flag1==1)
                {
                printf("neg\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;

            case 11:
                /*ADD*/
                reg[IR.r] = reg[IR.l] + reg[IR.m];
                if (print_flag1==1)
                {
                printf("add\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;

            case 12:
                /*SUB*/
                reg[IR.r] = reg[IR.l] - reg[IR.m];
                if (print_flag1==1)
                {
                printf("sub\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;

            case 13:
                /*MUL*/
                 reg[IR.r] = reg[IR.l] * reg[IR.m];
                if (print_flag1==1)
                {
                printf("mul\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;

            case 14:
                /*DIV*/
                 reg[IR.r] = reg[IR.l] / reg[IR.m];
                if (print_flag1==1)
                {
                printf("div\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;

            case 15:
                /* ODD*/
                 reg[IR.r] = reg[IR.r] % 2;
                if (print_flag1==1)
                {
                 printf("odd\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                 break;

            case 16:
                /* MOD*/
                reg[IR.r] = reg[IR.l] % reg[IR.m];
                if (print_flag1==1)
                {
                printf("mod\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;

            case 17:
                /*EQL*/
                reg[IR.r] = (reg[IR.l] == reg[IR.m]) ? 1 : 0;
                if (print_flag1==1)
                {
                printf("eql\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;

            case 18:
                /*NEQ*/
                reg[IR.r] = (reg[IR.l] != reg[IR.m]) ? 1 : 0;
                if (print_flag1==1)
                {
                printf("neq\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;

            case 19:
                /*LSS*/
                reg[IR.r] = (reg[IR.l] < reg[IR.m]) ? 1 : 0;
                if (print_flag1==1)
                {
                printf("lss\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;

            case 20:
                /*LEQ*/
                reg[IR.r] = (reg[IR.l] <= reg[IR.m]) ? 1 : 0;
                if (print_flag1==1)
                {
                printf("leq\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;

            case 21:
                /*GTR*/
                reg[IR.r] = (reg[IR.l] > reg[IR.m]) ? 1 : 0;
                if (print_flag1==1)
                {
                printf("gtr\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;

            case 22:
                /*GEQ*/
                reg[IR.r] = (reg[IR.l] >= reg[IR.m]) ? 1 : 0;
                if (print_flag1==1)
                {
                printf("geq\t");
                printf("%d\t",IR.r);
                printf("%d\t",IR.l);
                printf("%d\t",IR.m);
                }
                break;
        }
        if (print_flag1==1)
        printf("%d\t%d\t%d\t",pc,bp,sp);

        if (bp != 0)                                        //determines if the bar should come after the current sp
        {

            if(bp > bar[flag])
            {
            bar[flag+1] = bp;
                    flag ++;
            }

            else if (bp < bar[flag])
            {
            bar[flag] = 1;
            flag--;
            }



            for (i =  1; i <= sp; i ++)
            {
            if (print_flag1==1)
             printf("%d  ",stack[i]);

             for ( j = 1 ; j <= flag; j ++)
             {

             if(bar[j]!= 1 && bar[j] == i +1 && bar[j] != sp+1)
             {
                 if (print_flag1==1)
                 printf("| ");
             }
             }

            }
            if (print_flag1==1)
            printf("\n");
        }

        /* Case 9,  SIO R, 0 , 1*/
        if (IR.op == 9 && IR.m == 1)
            {
                if (print_flag1==1)
            printf("\nOutput: %d\n\n", reg[IR.r]);  //prints the value requested by the program
            }

        if (print_flag1==1)
        printf("Reg: ");

        if (print_flag1==1)
        {
        for(j = 0; j<8; j++)                        //prints registers
        {
            printf("%d ", reg[j]);
        }
        }
        if (print_flag1==1)
        printf("\n");
    }

    //printf("\nFactorial Output:\n");
    //printf("%d", reg[0]);

    fclose(ifp);

}

int base(int l, int base)
{
    int b1;

    b1 = base;

    while (l > 0)
    {
        b1 = stack[b1 + 1];
        l --;
    }

return b1;
}

void print_pmo(int x)
{
    print_flag1 = x;
}

