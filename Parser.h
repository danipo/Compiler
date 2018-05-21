//COP3402 Project 3

//Daniel Powley, Ming Jiang



typedef struct
{
    int kind;      // cons = 1,
    char name[11];
    int val;       // number (ASCII value)
    int level;     // L level
    int addr;      // M address
} symbol;


//function prototypes

void program(int* index, int* cIndex, symbol* table,Instruction* code,int l);
int get();
void error(int n);
void block(int index,int* cIndex, symbol* table,Instruction* code,int l);
void statement(Instruction* code, symbol* table, int* index,int* cIndex, int l);
void condition(int* index, int*cIndex, Instruction* code, symbol* table, int l);
void expression(int* index, int* cIndex, Instruction* code, symbol* table, int l);
void term(int* index,int *cIndex, Instruction* code, symbol* table, int l);
void factor(int* index,int *cIndex, Instruction* code, symbol* table, int l);
void add(int* index,int* addr, symbol* table, int l);
int position(int*index, symbol* table, int l);
void emit(int op, int r, int l, int m, Instruction* code,int* cIndex);
void print_parser(int x);

// gloabal variables
char name[11];
int value,kind,token;
int numError = 0, lastGap  = 0, cIndex = 0, r = -1;
int CODE_SIZE = 1000;
FILE*ifp;
int print_flag = 0;


int parser ()
{
int* index;
int z = 0;
int* cIndex;
cIndex = &z;


int l = 0;
symbol table[1000] = {0};
Instruction code[3000] = {0};
ifp = fopen("LexiList.txt","r");
FILE *parseout = fopen("code.txt", "w");
int i;



program(index,cIndex, table, code,l);

if(numError == 0)
{
if(print_flag == 1)
 printf("No errors, program is syntactically correct.\n");


for(i = 0; i < *cIndex; i ++)
{
    fprintf(parseout,"%d\t",code[i].op);
    fprintf(parseout,"%d\t",code[i].r);
    fprintf(parseout,"%d\t",code[i].l);
    fprintf(parseout,"%d\n",code[i].m);
    if(print_flag == 1)
    {
        printf("%d\t",code[i].op);
        printf("%d\t",code[i].r);
        printf("%d\t",code[i].l);
        printf("%d\n",code[i].m);
    }

}


}
fclose(ifp);
fclose(parseout);
}


void program(int* index, int* cIndex, symbol* table,Instruction* code, int l)
{
//printf("haha\n");
token = get();
//printf("index: %d\n",index);
block(1,cIndex,table, code, l);
emit(9,0,0,3,code,cIndex);
if(token != periodsym)
error(9);
}

void block(int index,int* cIndex, symbol* table,Instruction* code,int l)
{
//printf("haha\n");
int space = 4;
int temp = index;

int addr;
addr = *cIndex;
//int x = *cIndex;

//table[temp].addr = *cIndex;
//table[temp].addr = addr;

emit(7,0,0,0,code,cIndex);

while(token == constsym || token == varsym || token == procsym)
{
    if(token == constsym)
    {
        kind = 1;
        while(1)
        {
            token = get();
            if(token != identsym)
            {
                error(4);
                return;
            }
            token = get();

            if (token!= eqsym)
            {
                error(3);
                return;
            }
            token = get();
            if (token != numbersym)
            {
                error(2);
                return;
            }
            token = get();

            add(&index,&space,table,l);


            if(token == semicolonsym)
            {
                token = get();
                break;
            }

            if(token != commasym)
            {
                error(5);
                return;
            }
        }
    }// token == const


    if(token == varsym)
    {
         kind = 2;
         while(1)
        {
            token = get();

            if(token != identsym)
            {
                error(4);
                return;
            }
            token = get();
            add(&index,&space,table,l);

            if(token == semicolonsym)
            {
                //printf("hello!!!\n");
                token = get();
                break;
            }


            if(token != commasym)
            {
                error(5);
                return;
            }

        }
    }// if token == var

    while (token == procsym)
    {
         kind = 3;

            token = get();

            if(token != identsym)
            {
                error(4);
                return;
            }
            token = get();

            if(token != semicolonsym)
            {
                error(5);
                return;
            }
            add(&index,&space, table,l);
            token = get();

            block(index, cIndex, table,code,l+1);

            if(token != semicolonsym)
            {
                error(5);
                return;
            }
            token = get();

    }
}

code[addr].m = *cIndex;
table[temp-1].addr = *cIndex;

//table[temp].addr = *cIndex;
emit(6,0,0,space,code,cIndex);


statement(code, table,&index,cIndex,l);

if(l != 0)
{
    emit(2,0,0,0,code,cIndex);
    l--;
}

}


void statement(Instruction* code, symbol* table, int* index,int* cIndex, int l)
{
int n, temp1, temp2,i;
//printf("are you here?\n");
    if(token == identsym)
    {
        n = position(index,table,l);
        //printf("n: %d\n",n);
        //printf("name: %s\n",table[0].name);
        if( n == 0)
        {
            error(11); //undeclared
            return;
        }

        if(table[n].kind != 2)
        {
            error(12); // only var allowed
            return;
        }
        token = get();

        if(token != becomessym)
        {
            error(13);
            return;
        }
        token = get();
        //("\nline: 261\n");
        // printf("Token: %d\n",token);
        //back
        expression(index,cIndex,code,table,l);
//printf("level: %d\n addr: %d\n",l - table[n].level,table[n].addr);
        emit(4,r,l - table[n].level,table[n].addr,code,cIndex);
        r--;
    }
    else if(token == callsym)
    {

        token = get();
        if(token != identsym)
        {
            error(14);
            return;
        }
        n = position(index,table,l);
        if(n == 0)
        {
            error(11);
            return;
        }
        if(table[n].kind != 3)
        {
            error(15);
            return;
        }
        //printf("\n\nadreesssafsfd %d\n\n",table[n].addr);
        //printf("n:%d\n",n);
    emit(5,0, l - table[n].level, table[n].addr,code,cIndex);
        token = get();
    }

    else if(token == beginsym)
    {
        //printf("!!!are you here?\n");
        token = get();
        statement(code,table,index,cIndex,l);

        //printf("\nLine 295, token: %d\n",token);
        while(token == semicolonsym)
        {
            token = get();
            //printf("inside the while token == semicolonsym\n");
            statement(code,table,index,cIndex,l);
        }
        if(token != endsym)
        {
            error(17);
            return;
        }
        token = get();
    }
else if(token == ifsym)
    {
        token = get();
        condition(index,cIndex,code,table,l);
        //printf("token :%d \n", token);
        if(token != thensym)
            {
                error(16);
                return;
            }
        token = get();
        temp1 = *cIndex;
        emit(8,r,0,0,code,cIndex);
        r--;
        statement(code,table,index,cIndex,l);

        if(token == elsesym)
        {
            token = get();
            code[temp1].m = *cIndex + 1;
            temp2 = *cIndex;
            emit(7,0,0,0,code,cIndex);
            statement(code,table,index,cIndex,l);
            code[temp2].m = *cIndex;

        }
        else
        code[temp1].m = *cIndex;
    }

    else if(token == whilesym)
    {
        temp1 = *cIndex;
        token = get();
        condition(index,cIndex,code,table,l);
        temp2 = *cIndex;
        emit(8,r,0,0,code,cIndex);
        r--;
        if(token != dosym)
        {
            error(18);
            return;
        }
        token = get(token);
        statement(code,table,index,cIndex,l);
        emit(7,0,0,temp1,code,cIndex);
        code[temp2].m = *cIndex;
    }

// else if write and read later
    else if(token == readsym)
    {
        token = get();
        if(token != identsym)
            error(4);
        token = get();
        n = position(index,table,l);

        r++;
        emit(9,r,0,2,code,cIndex);
        emit(4,r,l-table[n].level,table[n].addr,code,cIndex);
        r--;

    }

    else if(token == writesym)
    {
        token = get();
        expression(index,cIndex,code,table,l);
        emit(9,r,0,1,code,cIndex);
    }


}


void condition(int* index, int*cIndex, Instruction* code, symbol* table, int l)
{
    int n;

    if(token == oddsym)
    {
        token = get();
        expression(index,cIndex,code,table,l);
        //printf("token :%d \n", token);
        emit(15,r,0,0,code,cIndex);
    }
    else
    {
        expression(index,cIndex,code,table,l);
        if(token != eqsym && token != neqsym && token != lessym && token != leqsym && token != gtrsym && token != geqsym)
        {
            error(20);
            return;
        }

    n = token;
    token = get();
    expression(index,cIndex,code,table,l);

    switch(n)
    {
    case 9:
        emit(17,r-1,r-1,r,code,cIndex);
        r--;
        break;

    case 10:
        emit(18,r-1,r-1,r,code,cIndex);
        r--;
        break;

    case 11:
        emit(19,r-1,r-1,r,code,cIndex);
        r--;
        break;

    case 12:
        emit(20,r-1,r-1,r,code,cIndex);
        r--;
        break;

    case 13:
        emit(21,r-1,r-1,r,code,cIndex);
        r --;
        break;

    case 14:
        emit(22,r-1,r-1,r,code,cIndex);
        r--;
        break;
    }
    }
}

void expression(int* index, int* cIndex, Instruction* code, symbol* table, int l)
{
    //printf("\nLine 401\n");
   // printf("token: %d\n",token);
    int addop;
    if(token == plussym || token == minussym)
    {
        addop = token;
        token = get();
        term(index,cIndex,code,table,l);
        if(addop == minussym)
        {
            emit(10,r,0,r,code,cIndex);
            r--;
        }
    }
    else{
        term(index,cIndex,code,table,l);
       // printf("\nline 414, token: %d\n",token);
        }
    while(token == plussym || token ==minussym)
    {
        addop = token;
        token = get();
        term(index,cIndex,code,table,l);
        if(addop == plussym)
        {
            emit(11,r-1,r-1,r,code,cIndex);
            r--;
        }
        else
        {
            emit(12,r-1,r-1,r,code,cIndex);
            r--;
        }
    }
}

void term(int* index,int *cIndex, Instruction* code, symbol* table, int l)
{
    int temp;
    factor(index,cIndex,code,table,l);

    while(token == multsym || token == slashsym)
    {
        temp = token;
        token = get();
        factor(index,cIndex,code,table,l);
        if(temp == multsym)
        {
            emit(13,r-1,r-1,r,code,cIndex);
            r--;
        }
        else
        {
            emit(14,r-1,r-1,r,code,cIndex);
            r--;
        }
    }
}

void factor(int* index,int *cIndex, Instruction* code, symbol* table, int l)
{
    int i,level, adr, val;
    while(token == identsym || token == numbersym || token == lparentsym)
    {
        if(token == identsym)
        {
            i = position(index,table,l);
            if(i == 0)
            {
                error(11);
                return;
            }
            else
            {
                kind = table[i].kind;
                level = table[i].level;
                adr = table[i].addr;
                val = table[i].val;

                if(kind == 1)
                {
                    r++;
                    //emit(1,i-1,0,val,code,cIndex);
                    emit(1,r,0,val,code,cIndex);
                }
                else if (kind == 2)
                {
                    r++;
                   // printf("\n\n%d\n\n",adr);
                    emit(3,r,l - level, adr,code,cIndex);
                }
                else
                {
                    error(21);
                    return;
                }
            }
            token = get();
            //printf("token :%d \n", token);
        }

        else if(token == numbersym)
        {
            if (value > 99999)
            {
                error(25);
                return;
            }
          //  printf("here?!\n");
            token = get();
           // printf("\nline 479, token: %d\n",token);
           r++;
            emit(1,r,0,value,code,cIndex);
        }
        else if (token == lparentsym)
        {
            token = get();
            expression(index,cIndex,code,table,l);
            if(token == rparentsym)
                token = get();
            else
            {
                error(22);
                return;
            }
        }

    }
}

int get()
{
//printf("WTFF!!!\n");
int token;
    fscanf(ifp, "%d", &token);


//printf("%d\t",token);

if(token == 2)
{
    fscanf(ifp, "%s", name);
    //printf("%s\t",name);
    //printf("%s\t",name);
}

else if(token == 3)
{
    fscanf(ifp,"%d", &value);
//printf("%d\t",value);
}

return token;
}




void error (int n)
{
numError ++;

switch(n)
{
    case 1:
        printf("Error number %d, ",n);
        printf("Use = instead of :=.\n");
        break;
    case 2:
        printf("Error number %d, ",n);
        printf("= must be followed by a number.\n");
        break;
    case 3:
        printf("Error number %d, ",n);
        printf("Identifier must be followed by =.\n");
        break;
    case 4:
        printf("Error number %d, ",n);
        printf("const, var, procedure must be followed by identifier.\n");
        break;
    case 5:
        printf("Error number %d, ",n);
        printf("Semicolon or comma missing.\n");
        break;
    case 6:
        printf("Error number %d, ",n);
        printf("Incorrect symbol after procedure declaration.\n");
        break;
    case 7:
        printf("Error number %d, ",n);
        printf("Statement expected\n");
        break;
    case 8:
        printf("Error number %d, ",n);
        printf("Incorrect symbol after statement part in block.\n");
        break;
    case 9:
        printf("Error number %d, ",n);
        printf("Period expected.\n");
        break;
    case 10:
        printf("Error number %d, ",n);
        printf("Semicolon between statements missing.\n");
        break;
    case 11:
        printf("Error number %d, ",n);
        printf("Undeclared identifier.\n");
        break;
    case 12:
        printf("Error number %d, ",n);
        printf("Assignment to constant or procedure is not allowed.\n");
        break;
    case 13:
        printf("Error number %d, ",n);
        printf("Assignment operator expected.\n");
        break;
    case 14:
        printf("Error number %d, ",n);
        printf("call must be followed by an identifier\n");
        break;
    case 15:
        printf("Error number %d, ",n);
        printf("Call of a constant or variable is meaningless.\n");
        break;
    case 16:
        printf("Error number %d, ",n);
        printf("then expected\n");
        break;
    case 17:
        printf("Error number %d, ",n);
        printf("Semicolon or } expected\n");
        break;
    case 18:
        printf("Error number %d, ",n);
        printf("do expected\n");
        break;
    case 19:
        printf("Error number %d, ",n);
        printf("Incorrect symbol following statement.\n");
        break;
    case 20:
        printf("Error number %d, ",n);
        printf("Relational operator expected.\n");
        break;
    case 21:
        printf("Error number %d, ",n);
        printf("Expression must not contain a procedure identifier.\n");
        break;
    case 22:
        printf("Error number %d, ",n);
        printf("Right parenthesis missing.\n");
        break;
    case 23:
        printf("Error number %d, ",n);
        printf("The preceding factor cannot begin with this symbol.\n");
        break;
    case 24:
        printf("Error number %d, ",n);
        printf("An expression cannot begin with this symbol.\n");
        break;
    case 25:
        printf("Error number %d, ",n);
        printf("This number is too large.\n");
        break;


break;
}

fclose(ifp);
exit(1);
}

void add(int* index,int* addr, symbol* table, int l)
{
int x = *index;
//printf("\n\ninside the add, index = %d\n\n",x);

table[x].kind = kind;
strcpy(table[x].name,name);
memset(name,0,sizeof(name));

if(kind == 1)
    table[x].val = value;

if(kind == 2)
{
    table[x].level = l;
    table[x].addr = *addr;
    (*addr)++;

}

if(kind == 3){
    table[x].level = l;
     //printf("Here!\n");
}
(*index)++;

}

int position(int*index, symbol* table, int l)
{
    int i, c,gap;
    int counter = 0;

    for(i = 0; i <= *index; i ++)
    {
        if(strcmp(table[i].name, name) == 0)
           {
               if(table[i].level <= l)
                {

                    if(counter != 0)
                        lastGap = gap;

                    gap = l - table[i].level;

                    if(counter == 0)
                        c = i;

                    if(gap < lastGap)
                        c = i;

                    counter++;
                }
           }
           //return i;
    }
    //printf("c::::%d\n",c);
    return c;
}



void emit(int op,int reg, int l, int m, Instruction* code,int* cIndex)
{
if(*cIndex > CODE_SIZE)
    error(25);


    int x = *cIndex;
    code[x].op = op;
    code[x].r = reg;
    code[x].l = l;
    code[x].m = m;
    (*cIndex)++;
}

void print_parser(int x)
{
    print_flag = x;
}
