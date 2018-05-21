//COP3402 Project 2

//Daniel Powley, Ming Jiang




/* token type
nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5, multsym = 6,
slashsym = 7, oddsym = 8, eqlsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17,
semicolonsym = 18, periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22,
ifsym = 23, thensym = 24, whilesym = 25, dosym = 26, callsym = 27, constsym = 28,
 varsym = 29, procsym = 30, writesym = 31, readsym = 32, elsesym = 33.*/


int print_flag2 = 0;

#define norw 15 // # of reserved words
#define imax 32767 //maximum integer value
#define cmax 11 //maximum number of chars for idents*/
#define nestmax 256 //maximum length of strings

//structure
typedef struct
{
    int token;
    char name[11];
    int value;
} namerecord;


//function prototypes
void wordToken(namerecord* list, char s[12],char*word[],int index);
int symbolToken(namerecord* list,char sSymbol[], int index, FILE* ifp);
void print(namerecord* list, int index, char* representation[]);
char c;


//main function
int analyzer()
{



// add word instruction to string list
    char *word[] =
    {
    "null","begin","call","const","do","else","end","if",
    "odd","procedure","read","then","var","while","write"
    };

// add special symbols to array
    char sSymbol[] =
    {
    '+', '-', '*', '/', '(', ')', '=', ',', '.', '<', '>', ';',':'
    };

// file point
    FILE * ifp;
    ifp = fopen("input.txt","r");

char *representation[] = {
"","nulsym","identsym","numbersym","plussym","minussym","multsym",
"slashsym","oddsym","eqlsym","neqsym","lessym","leqsym","gtrsym",
"geqsym","lparentsym","rparentsym","commasym","semicolonsym","periodsym",
"becomesym","beginsym","endsym","ifsym","thensym","whilesym","dosym",
"callsym","constsym","varsym","procsym","writesym","readsym","elsesym",
};


// declare a string
    char s[12] = "";

//declare variables
    int counter = 0, num = 0, flag = 0, error = 0, index = 0;
    int i;


// list of struct to save instructions, up to 3000, can increase the number later on
    namerecord list[3000];

// set each token to 0
    for(i = 0 ; i < 3000; i ++)
        list[i].token = 0;

//get a c in the file
    c = fgetc(ifp);


// while the file is no end keep loop
    while(!feof(ifp)){

// reset vaules, flag, counter and error to zero
        flag = 0;
        counter = 0;
        error = 0;



        // check if the c is a letter
        if(isalpha(c))
        {
            // read c in to string
            s[counter] = c;
            counter ++;

            // get next c
             c = fgetc(ifp);


             //while the c is letter or digit keep the loop
             while(isalpha(c) || isdigit(c))
             {
                 // we don't want any var name to be larger than 11
                 if (counter < 11)
                 {
                // add c to string
                 s[counter] = c;
                 counter ++;
                 }

                 //variable name too long
                else
                    error = 1;

                // get next c
                 c = fgetc(ifp);

             }

            // if error is 1, means variable name is too lonh
            if(error == 1)
                printf("error: variable too long\n");

            // call wordtoken function
            wordToken(list,s,word,index);
            //set the string to 0
            memset(s,0,sizeof(s));

            // increase the index and flag
            index++;
            flag =1;

        } // end of if (isalpha(c))


        // if the c is dighit
        else if(isdigit(c))
        {
            //set num to zero
            num = 0;
            //add c to string
            s[counter] = c;

            // while loop
            while(flag == 0)
            {
                // get c
                c = fgetc(ifp);


                // if c is digit, add c to string
                if(isdigit(c)){
                counter ++;
                s[counter] = c;
                }

                //if c is not a digit
                else
                {
                    // add convert string to integer
                    for(i = 0; i <= counter; i ++)
                    {
                    num += (s[i]-'0') * pow(10,counter-i);
                    }

                    // if string is too long, print error
                    if (counter > 4)
                    {
                        printf("\nerror: integer should be less than 5 digits in length\n");
                        memset(s,0,sizeof(s));
                        break;
                    }

                    // add copy string to list
                    strcpy(list[index].name, s);
                    //reset string
                    memset(s,0,sizeof(s));
                    //add num to value, we don;t need value in this program, i did so just in case
                    //we need it in next program
                    list[index].value = num;

                    //token of number is always 3
                    list[index].token = 3;

                    index++;
                    flag = 1;
                }
            }
        }

        // do nothing if the c is white space just scan next c
        else if(c == '\n' || c == '\t' || c == ' ' || c == '\r');

                // special symbols
                else
                {
                // call the symbol token function
                flag = symbolToken(list,sSymbol,index,ifp);
                // flag == 2 when it's a comment
                if (flag < 2)
                {
                index++;
                }
                }

            //if flag is not 1, keep reading until end of file.
        if ( flag != 1)
        {
            c = fgetc(ifp);
            if (c == EOF)
                break;

        }
        //printf("%c",c);
    }

// call print function
    print(list,index,representation);
fclose(ifp);

}

// This function is to check if the string is one of the word list
void wordToken(namerecord* list, char s[12], char* word[],int index)
{
// add string to the name list
    strcpy(list[index].name, s);

// compare the string to all 15 words in the list
    int i, temp = 15;

    for(i = 0; i < 15 ; i ++)
    {
        if(strcmp(s,word[i]) == 0)
        {
            temp = i;
        }

    }

// switch is to set the token of matched word
    switch (temp)
    {
        //null
        case 0:
            list[index].token = nulsym;
            break;

        //begin
        case 1:
            list[index].token = beginsym;
            break;

        //call
        case 2:
            list[index].token = callsym;
            break;

        //cons
        case 3:
            list[index].token = constsym;
            break;

        //do
        case 4:
            list[index].token = dosym;
            break;

        //else
        case 5:
            list[index].token = elsesym;
            break;

        //end
        case 6:
            list[index].token = endsym;
            break;

        //if
        case 7:
            list[index].token = ifsym;
            break;

        //odd
        case 8:
            list[index].token = oddsym;
            break;

        //procedure
        case 9:
            list[index].token =procsym;
            break;

        //read
        case 10:
            list[index].token = readsym;
            break;

        //then
        case 11:
            list[index].token = thensym;
            break;

        //var
        case 12:
            list[index].token = varsym;
            break;

        //while
        case 13:
            list[index].token = whilesym;
            break;

        //write
        case 14:
            list[index].token = writesym;
            break;

        //ident
        case 15:
            list[index].token = identsym;
            break;
    }
}


// this function is to determind the token of special symbols
int symbolToken(namerecord* list,char sSymbol[], int index,FILE* ifp)
{
    int i, temp = 13;
    int flag = 0;
    char s[2] = "";

// compare every c to each special symbol
    for(i = 0; i < 13; i ++)
    {
        if(c == sSymbol[i])
            temp = i;
    }

// set the token of matched symbol
    switch(temp)
    {

    // +
        case 0:
            list[index].token = plussym;
            list[index].name[0] = c;
            break;

        // -
        case 1:
            list[index].token = minussym;
            list[index].name[0] = c;
            break;

        // *
        case 2:
            list[index].token = multsym;
            list[index].name[0] = c;
            break;

        // /
        case 3:
        c = fgetc(ifp);
        //printf("%c",c);

        // if c == '*' means that it's a comment so keep reading until the end
        if(c == '*')
        {
            flag = 1;

            while(flag == 1)
            {
                if(c != '/')
                {
                    c = fgetc(ifp);
                    //printf("%c",c);
                }
                else
                flag--;
            }
            // return 2 for comments
            return 2;
        }
        // every else the c is just a slash
        else
        {
            list[index].token = slashsym;
            strcpy(list[index].name,"/");
            return 1;
        }

            break;

        // (
        case 4:
            list[index].token = lparentsym;
            list[index].name[0] = c;
            break;

        // )
        case 5:
            list[index].token = rparentsym;
            list[index].name[0] = c;
            break;

        // =
        case 6:
            list[index].token = eqsym;
            list[index].name[0] = c;
            break;

        // ,
        case 7:
            list[index].token = commasym;
            list[index].name[0] = c;
            break;

        // .
        case 8:
            list[index].token = periodsym;
            list[index].name[0] = c;
            break;

        // <
        case 9:
            c = fgetc(ifp);
            //printf("%c",c);
            // check if it's less than or equal sign
            if (c == '=')
            {
                list[index].token = leqsym;
                strcpy(list[index].name,"<=");
            }
            // check if it's no equal sign
            else if (c == '>')
            {
                list[index].token = neqsym;
                strcpy(list[index].name,"<>");
            }
            // everything else, it's just a less than sign
            else
            {
                list[index].token = lessym;
                strcpy(list[index].name,"<");
                return 1;
            }
            break;

        // >
        case 10:
            c = fgetc(ifp);
            //printf("%c",c);

            //check if it's greater than or equal sign
            if (c == '='){
                list[index].token = geqsym;
                strcpy(list[index].name,">=");
            }
            // everything else it just a greater than sign
            else
            {
                list[index].token = gtrsym;
                strcpy(list[index].name,">");
                return 1;
            }
            break;

        // ;
        case 11:
            list[index].token = semicolonsym;
            list[index].name[0] = c;
            break;

        // :
        case 12:
            c = fgetc(ifp);
           // printf("%c",c);
            // check if it's a define sign
            if(c == '=')
            {
                list[index].token = becomessym;
                strcpy(list[index].name,":=");
            }
            // everything else is just a invaild sign
            else
                //printf("\nerror: invaild symbol\n");

            break;

        // everything else
        case 13:
            // printf("\n error: invaild symbol\n");
            break;
    }
return 0;
}

// this function is to print


void print(namerecord* list, int index,char* representation[])
{
    FILE*fp = fopen("LexiList.txt","w");
    //printf("\n\nLexeme Table:\n");
    int i;
    // print each instruction is var name and token form
    /*
    for(i = 0; i < index ; i ++)
    {
        // if token is 0, means it's special symbol
        if(list[i].token != 0)

        {

            printf("%s\t",list[i].name);
            printf("%d\n",list[i].token);
            //fprintf(fp,"%d\n",list[i].token);

        }


    }*/

    if (print_flag2 == 1)
    printf("\nLexeme List:\n");



    for(i = 0; i < index; i++)
    {

        if(list[i].token != 0)

        {

            if (print_flag2 == 1)
            printf("%d ",list[i].token);


            fprintf(fp,"%d\n",list[i].token);
            // print the name if token is 2 or 3
            if(list[i].token == 2|| list[i].token == 3)
               {
                   if (print_flag2 == 1)
                   printf("%s ",list[i].name);

                    fprintf(fp,"%s\n",list[i].name);
               }

        }
    }
    if (print_flag2 == 1)
     printf("\nSymbolic Representation:\n");

 for(i = 0; i < index; i++)
    {

        if(list[i].token != 0)
        {
            if (print_flag2 == 1)
            printf("%s ",representation[list[i].token]);

            // print the name if token is 2 or 3

            if(list[i].token == 2|| list[i].token == 3)
               {
                   if (print_flag2 == 1)
                   printf("%s ",list[i].name);
               }

        }
    }
    if (print_flag2 == 1)
     printf("\n");

    fclose(fp);
}


void print_analyzer(int x)
{
    print_flag2 = x;
}


