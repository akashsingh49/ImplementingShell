#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void error(int);
void initialize_shell();
void read_input(char*);
int check_pipe(char*,int*);
int checksize(char*);
void convert_into_arguments_space(char*,char**);


void print_current_directory(void)
{
    char string[100];
    getcwd(string,sizeof(string));
    printf("\n Dir:%s%s",string," $");
}
void error(int x)
{
    //1 : memory not allocated properly.
    //2 : Forking error
    //3 : execvp error
}
void initialize_shell()
{
    printf("Welcome to the shell");
    // Give all the necessary commands here.
}
void read_input(char* inputdata)
{
    int space=20;
    //char *inputdata=malloc(space* sizeof(char));
    if(!inputdata)
    {
        error(1);
    }
    int data;
    int pointer=0;
    do{
        data=getchar();
        if(data!=EOF&&data!='\n')
        {
            inputdata[pointer]=data;
            pointer++;
        }
        else
        {
            inputdata[pointer]='\0';
            break;

            //return inputdata;
        }


    }while(1);


}
int check_pipe(char* line,int* indexes)
{

    char* pointer=line;
    int count=0;
    while(*pointer!='\0')
    {
        if(*pointer=='|')
        {
            count++;
        }
        pointer++;
    }

    //int indexes[count];
    pointer=line;
    int position=0;
    for(int i=0;i<10000000;i++)
    {
        if(*pointer=='|')
        {
            indexes[position]=i;
            position++;
        }
        if(*pointer=='\0')
        {
            break;
        }
        pointer++;
    }
    indexes[position]=-1;
    return count;

}
int checksize(char* line)// assuming that there are no 2 spaces continuosly.
{
    int flag1=0;
    int flag2=0;
    int currflag=0;
    int count=0;
    char* pointer;
    pointer=line;
    if(*pointer==' ')
    {
        flag1=1;
    }
    while(*pointer!='\0')
    {
        if(*pointer==' ')
        {
            count++;
        }
        pointer++;
    }
    pointer--;
    if(*pointer==' ')
    {
        flag2=1;
    }
    count=count-flag1-flag2+1;
    //printf("%d",count);
    return count;
}
void convert_into_arguments_space(char* line,char** arguments)// this will produce error if the no. of tokens will be greater than 20.
{
    char* dupl_line;
    dupl_line=line;
    arguments[0]=strtok(dupl_line," ");
    int position=1;
    while(1)
    {
        arguments[position]=strtok(NULL," ");
        position++;
        if(arguments[position]==NULL)
        {
            break;
        }
    }
}

void convert_into_arguments_pipe(char* line,char** arguments)// this will produce error if the no. of tokens will be greater than 20.
{
    char* dupl_line;
    dupl_line=line;
    arguments[0]=strtok(dupl_line,"|");
    int position=1;
    while(1)
    {
        arguments[position]=strtok(NULL,"|");
        position++;
        if(arguments[position]==NULL)
        {
            break;
        }
    }
}

	
void convert_into_arguments_redirection(char* line,char** arguments)// this will produce error if the no. of tokens will be greater than 20.
{
    char* dupl_line;
    dupl_line=line;
    arguments[0]=strtok(dupl_line,">");
    int position=1;
    while(1)
    {
        arguments[position]=strtok(NULL,">");
        position++;
        if(arguments[position]==NULL)
        {
            break;
        }
    }
}

void convert_into_arguments_INredirection(char* line,char** arguments)// this will produce error if the no. of tokens will be greater than 20.
{
    char* dupl_line;
    dupl_line=line;
    arguments[0]=strtok(dupl_line,"<");
    int position=1;
    while(1)
    {
        arguments[position]=strtok(NULL,"<");
        position++;
        if(arguments[position]==NULL)
        {
            break;
        }
    }
}




int check_basic_commands(char **s)// the first character in s must be exactly same as cd/help/exit to return int>0.
{
    /*
     * this function tests wether 1st arg is a basic command.It returns
     * 0: not a basic command
     * 1: basic command : cd
     * 2: basic command : help
     * 3: basic command : exit
     * */

    if((strcmp(s[0],"cd")== 0))
        return 1;
    else if((strcmp(s[0],"help")== 0))
        return 2;
    else if((strcmp(s[0],"exit")== 0))
        return 3;
    else
        return 0;

}

void  help (void)
{
    printf("%s"," *HELP* ");
	printf("%s","\n You can do the following \n1. command \n2.command>filename \n3.command>>filename \n4.1>filename \n5.2>filename \n6.2>&1 \n7.| for pipelining. \n8.quit : to end shell \n9. Multiple pipelining is supported. ");
}

void quit(void)
{
    
    exit(0);
}

int handle_basic_commands(char** array,int flag)
{
    if(flag==1)
    {
        chdir(array[1]);
        print_current_directory();
        return 1;
    }
    if(flag==2)
    {
        help();
        return 1;
    }
    if(flag==3)
    {
        quit();

        return 1;
    }
    return 0;
}

int execute(char** string)
{
    int processid=fork();
    if(processid<0)
    {
        printf("%s","Error");// to remove
        error(2);
    }
    if(processid==0)
    {
        //child process
        int res=execvp(string[0],string);
        if(res<0)
        {
            printf("%s","error");//to remove
            error(3);

        }
        exit(0);// child process exiting

    }
    if(processid>0)
    {
        wait(NULL);
        return 1;
    }
    return 0;
}


int execute_with_pipe(char** string)
{
    int flag=0;
    int processid=fork();
    if(processid<0)
    {
        printf("%s","Error");// to remove
        error(2);
    }
    if(processid==0)
    {
        //child process
        int res=execvp(string[0],string);
        if(res<0)
        {
            printf("%s","error");//to remove
            error(3);
            flag=-1;

        }
        exit(0);// child process exiting

    }
    if(processid>0)
    {
        wait(NULL);
        return flag;
    }
    return flag;


}

int check_for_redirection(char* line,int* indexes)
{
    char* pointer=line;
    int count=0;
    while(*pointer!='\0')
    {
        if(*pointer=='>')
        {
            count++;
        }
        pointer++;
    }

    pointer=line;
    int position=0;
    for(int i=0;i<10000000;i++)
    {
        if(*pointer=='>')
        {
            indexes[position]=i;
            position++;
        }
        if(*pointer=='\0')
        {
            break;
        }
        pointer++;
    }
    indexes[position]=-1;
    return count;
}

int check_type_of_redirection(int* indexes,int length)//length of indexes is 1 + no. of >'s
{
    /*
     * This returns 1 if we have just > , 2 if we have >>.And -1 in case of multiples.-2 in case of error.
     * */
    if(length>3)
    {
        return -1;
    }
    if(length==2&&indexes[1]==-1)
    {
        return 1;
    }
    if(length==3&&indexes[1]==indexes[0]+1&&indexes[2]==-1)
    {
        return 2;
    }
    if(length==3&&indexes[1]-indexes[0]>1&&indexes[2]==-1)
    {
        return -1;
    }
    return -2;//error

}

int check_for_input_redirection(char* line)
{
    char* pointer=line;
    int count=0;
    while(*pointer!='\0')
    {
        if(*pointer=='<')
        {
            count++;
        }
        pointer++;
    }
    return count;
}

int do_main_work(void)
{
	print_current_directory();    
	char line[100];                                         // the actual input
    read_input(line);                                       // this function takes input
    if(*line=='\0')
    {
        return 1;
    }
    int indexes[18];                                        // 18 pipes allowed.This array stores the indexes at which the pipe is present in the input
    int flag=check_pipe(line,indexes);                      // checks wether there are pipes in the input. flags=no. of pipes present
    char *arguments[50];                                    // this has the arguments in case of only spaces
    int count;                                              // don't know what it has //IMPORTANT : change this
    //printf("%d%s",flag,"indexes are");
    /*for(int i=0;i<flag;i++)
    {
		printf("%d",indexes[i]);
	}*/
    int indexes2[100];                                      //indexes at which the '>' are present.
    int redctd=check_for_redirection(line,indexes2);        // no. of >'s present.
    int type=check_type_of_redirection(indexes2,redctd+1);  //types of redirection -2:error -1:multiple 1:> 2:>>
    int check_in_rd=check_for_input_redirection(line);
    char *red_commands[3];                                  // have the commands around >. eg {command1 command2 NULL} for command1>(or >>)command2
    char *red_command1_args[50];
    if(flag==0&&redctd>0&&type>0&&check_in_rd==0)
    {
        convert_into_arguments_redirection(line,red_commands);
        if((strcmp(red_commands[0],"1")!=0)&&(strcmp(red_commands[0],"2")!=0))//red_commands[0]!="1"&&red_commands[0]!="2")
        {
            if(type==1)
            {
                convert_into_arguments_space(red_commands[0],red_command1_args);
                freopen(red_commands[1], "w", stdout);
                execute(red_command1_args);
                freopen("/dev/tty", "w", stdout);
                return 1;
            }
            if(type==2)
            {
                convert_into_arguments_space(red_commands[0],red_command1_args);
                freopen(red_commands[1], "a+", stdout);
                execute(red_command1_args);
                freopen("/dev/tty", "w", stdout);
                return 1;
            }

        }
        if((strcmp(red_commands[0],"1")==0))
        {
            //printf("%s","here at:  1>filename\n");
			printf("rest of things will be printed int the mentioned file\n");
            freopen(red_commands[1],"w",stdout);
            return 1;
        }
        if((strcmp(red_commands[0],"2")==0)&&(strcmp(red_commands[1],"&1")!=0))//red_commands[0]=="2"&&red_commands[1]!="&1")
        {
            freopen(red_commands[1],"w",stderr);
            return 1;
        }
        if((strcmp(red_commands[0],"2")==0)&&(strcmp(red_commands[1],"&1")==0))
        {
            dup2(1,2);
            return 1;
        }
    }
    if(check_in_rd==1)
    {

        convert_into_arguments_INredirection(line,red_commands);
        convert_into_arguments_space(line,red_command1_args);
        freopen(red_commands[1],"r",stdin);
        execute(red_command1_args);
        freopen("/dev/tty", "r", stdin);
        return 1;

    }

    if(flag==0&&redctd==0)
    {
        count=checksize(line);
        if(count>0)
        {
            convert_into_arguments_space(line,arguments);
            /*
	    for(int i=0;i<count+1;i++)//for(int=0;i<count+1;i++)
            {
                printf("%s",arguments[i]);
            }*/
            int bc=check_basic_commands(arguments);
            //printf("%d",bc);
            if(bc>0)
            {
                handle_basic_commands(arguments,bc);
                return 1;
            }
            else
            {
                execute(arguments);
                return 1;
            }
        }
    }
    char **listofargs[20];// 18  (have to decide exact no.)pipes allowed
    char* temp0[20];listofargs[0]=temp0;char* temp1[20];listofargs[1]=temp1;char* temp2[20];listofargs[2]=temp2;char* temp3[20];listofargs[3]=temp3;char* temp4[20];
    listofargs[4]=temp4;char* temp5[20];listofargs[5]=temp5;char* temp6[20];listofargs[6]=temp6;char* temp7[20];listofargs[7]=temp7;char* temp8[20];listofargs[8]=temp8;
    char* temp9[20];listofargs[9]=temp9;char* temp10[20];listofargs[10]=temp10;char* temp11[20];listofargs[11]=temp11;char* temp12[20];listofargs[12]=temp12;char* temp13[20];
    listofargs[13]=temp13;char* temp14[20];listofargs[14]=temp14;char* temp15[20];listofargs[15]=temp15;char* temp16[20];listofargs[16]=temp16;char* temp17[20];
    listofargs[17]=temp17;char* temp18[20];listofargs[18]=temp18;char* temp19[20];listofargs[19]=temp19;



    flag++;// flag denotes the number of args which is 1 + no. of pipes
    char *bulkargs[20];
    convert_into_arguments_pipe(line,bulkargs);
    int commands=0;
    for(int i=0;i<20;i++)
    {
        if(bulkargs[i]==NULL)
        {
            commands=i;
            break;
        }
        convert_into_arguments_space(bulkargs[i],listofargs[i]);
    }
    /*
    for(int i=0;i<commands;i++)
    {
        if(listofargs[i]==NULL)
        {
            break;
        }
        for(int j=0;j<20;j++)
        {
            if(listofargs[i][j]==NULL)
                break;
            puts(listofargs[i][j]);
        }
    }
    */
    //printf("%i",commands);

    for(int i=0;i<commands;i++)
    {
        if(i==0)
        {

            freopen("b.txt", "w", stdout);
            execute_with_pipe(listofargs[i]);
            freopen("a.txt","r",stdin);
        }
        if(i>0&&i<commands-1)
        {
            if(i%2==0)
            {
                freopen("a.txt","r",stdin);
                freopen("b.txt", "w", stdout);
            }
            else
            {

                //freopen("/dev/tty", "w", stdout);
                //freopen("/dev/tty", "r", stdin);
                freopen("b.txt","r",stdin);
                freopen("a.txt", "w", stdout);
            }

            execute_with_pipe(listofargs[i]);
        }
        if(i==commands-1)
        {

            if(i%2==0)
            {
                freopen("a.txt","r",stdin);
                freopen("b.txt", "w", stdout);
            }
            else
            {

                //freopen("/dev/tty", "w", stdout);
                //freopen("/dev/tty", "r", stdin);
                freopen("b.txt","r",stdin);
                freopen("a.txt", "w", stdout);
            }
            freopen("/dev/tty", "w", stdout);
            execute_with_pipe(listofargs[i]);
            freopen("/dev/tty", "r", stdin);
        }

    }
    return 1;
}


void handler_for_ctrl_plus_c()
{
do_main_work();
}
int main()
{
    initialize_shell();
    int a=2;
    do {
	signal(SIGINT,handler_for_ctrl_plus_c);
        a=do_main_work();// this will run in loop

    }while(a==1);
    //terminate();// end the shell

}

