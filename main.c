#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    check if text string and search string match
*/
int match(char* text, int idx, int len, char* st, int n, int* ret)
{
    int i;
    *ret     = -1;
    int same = 1;
    
    for(i=n-1 ; i>=0 ; i--)
    {
        if(text[idx+i] != st[i]) same = 0;
        if(idx+n<len && text[idx+n] == st[i] && *ret == -1) *ret = i; 
    }
    
    return same;
}

// Sunday's algorithm
int Sunday(char* text, int len ,char* st)
{
    int *index = (int*)malloc(sizeof(int));
    int i      = 0;
    int cnt    = 0;
    int n      = strlen(st);

    while(i+n <= len)
    {
        if(match(text,i,len,st,n,index))
            cnt++;
        
        if(*index == -1)
            i += n + 1;
        else
            i += n - *index;
    }

    free(index);
    return cnt;
}

/*
    Print the result
*/
void Print(char* output, int text_length, int search_string_cnt)
{
    // open output file
    FILE *fo;
    if((fo = fopen(output, "w")) == NULL)
    {
        printf("Fail to open output file. Please check the output file is in current directory.\n");
        return ;
    }

    // catenate two string as a output string 
    char tmp1[100];
    sprintf(tmp1,"%d",text_length);
    // itoa(text_length,tmp1,10);
    char tmp2[100];
    sprintf(tmp2,"%d",search_string_cnt);
    // itoa(search_string_cnt,tmp2,10);
    char prefix1[] = "Size of file is ";
    char prefix2[] = "Number of matches = ";
    int space = strlen(prefix1) + strlen(prefix2) + strlen(tmp1) + strlen(tmp2) + 2;
    char *outcome = (char*)malloc(space * sizeof(char));
    strcpy(outcome,prefix1);
    strcat(outcome,tmp1);
    strcat(outcome,"\n");
    strcat(outcome,prefix2);
    strcat(outcome,tmp2);
    printf("%s\n", outcome);

    int outlen = strlen(outcome);
    fwrite(outcome,sizeof(char),outlen,fo);


    if(fclose(fo) != 0)
        printf("Fail to close output file");
}

int main(int argc, char* argv[])
{
    char* input         = argv[1];
    char* search_string = argv[2];
    char* output        = argv[3];

    if(argc != 4)
    {
        printf("Program parameter doesn't meet the requirement.\n");
        return 0;
    }       

    int n = strlen(search_string);

    if(sizeof(search_string) >= 20)
    {
        printf("The search string excesses the limit length.\n");
        return 0;        
    }


    // read in the file
    FILE *fi;
    if((fi = fopen(input, "rb")) == NULL)
    {
        printf("Fail to open input file. Please check the input file is in current directory.\n");
        return 0;
    }

    // get the length of the input file
    fseek(fi,0,SEEK_END);
    int text_length = ftell(fi);
    rewind(fi);

    // char text[80];

    int i;
    int accumulativeSize = 0;
    int bufferSize = 80;
    int SearchStringCnt = 0;
    char *text = (char*)malloc(80*sizeof(char));
    char *LastPart = (char*)malloc((n-1)*sizeof(char));
    char *tmp = (char*)malloc(100*sizeof(char));

    // put every 80 bytes into buffer, and process this 80 bytes content
    while(fread(text,sizeof(char),bufferSize,fi))
    {
        accumulativeSize += bufferSize;

        int last_len = strlen(LastPart);
        if(last_len < n-1)
            SearchStringCnt += Sunday(text, bufferSize, search_string);
        else
        {
            memset(tmp,0,100*sizeof(char));   
            strcat(tmp, LastPart);
            strcat(tmp, text);
            SearchStringCnt += Sunday(tmp, bufferSize+n-1, search_string);
        }
        int cur_len = strlen(text);

        int j;
        for(i=cur_len-n+1, j=0 ; i<cur_len ; i++, j++)
            LastPart[j] = text[i];
        
        if(text_length - accumulativeSize < 80) bufferSize = text_length - accumulativeSize;
    }
    free(tmp);
    free(LastPart);
    free(text);

    // close the file
    if(fclose(fi) != 0)
        printf("Fail to close input file");

    // printf("%d\n%d\n",text_length,SearchStringCnt);
    Print(output, text_length, SearchStringCnt);

    return 0;
}
