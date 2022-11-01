#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

void change_words(FILE* inp)
{
    int frst_pos = -1;
    int last_pos = -1;
    int readed;
    while((readed = getc(inp)) != EOF)
    {
        if(readed != ' ' && readed != '\n' && readed != '\t')
        {
            if(frst_pos == -1)
            {
                frst_pos = ftell(inp) - 1;
            }
            last_pos = ftell(inp) - 1;
        }
    }
    fseek(inp, last_pos, SEEK_SET);
    while((readed = getc(inp)) != ' ' && readed != '\n' && readed != '\t')
    {
        fseek(inp, -2, SEEK_CUR);
        if(ftell(inp) <= frst_pos)
        {
            break;
        }
    }
    last_pos = ftell(inp);
    if(last_pos <= frst_pos)
    {
        return;
    }
    char *frst;
    char *last;
    char *other;
    int i = -1;
    int j = -1;
    int k = -1;
    int middle_count = 0;
    fseek(inp, 0, SEEK_SET);
    while(ftell(inp) != frst_pos)
    {
        fread(&readed, sizeof(char), 1, inp);
        if (j == -1)
        {
            other = malloc(sizeof(char));
        }
        else
        {
            other = realloc(other, (j+2)*sizeof(char));
        }
        other[++j] = (char)readed;
    }
    fread(&readed, sizeof(char), 1, inp);
    while(readed != (int)' ' && readed != '\n' && readed != '\t')
    {
        if (i == -1)
        {
            frst = malloc(sizeof(char));
        }
        else
        {
            frst = realloc(frst, (i+2)*sizeof(char));
        }
        frst[++i] = (char)readed;
        fread(&readed, sizeof(char), 1, inp);
    }
    if (i == -1)
    {
        frst = malloc(sizeof(char));
    }
    else
    {
        frst = realloc(frst, (i+2)*sizeof(char));
    }
    frst[++i] = '\0';
    if (j == -1)
    {
        other = malloc(sizeof(char));
    }
    else
    {
        other = realloc(other, (j+2)*sizeof(char));
    }   
    other[++j] = (char)readed;
    middle_count++;
    while(ftell(inp) != last_pos)
    {
        fread(&readed, sizeof(char), 1, inp);
        if (j == -1)
        {
            other = malloc(sizeof(char));
        }
        else
        {
            other = realloc(other, (j+2)*sizeof(char));
        }
        other[++j] = (char)readed;
        middle_count++;
    }
    if (j == -1)
    {
        other = malloc(sizeof(char));
    }
    else
    {
        other = realloc(other, (j+2)*sizeof(char));
    }
    other[++j] = '\0';
    while((fread(&readed, sizeof(char), 1, inp) > 0) && readed != ' ' && readed != '\n' && readed != '\t')
    {
        if (k == -1)
        {
            last = malloc(sizeof(char));
        }
        else
        {
            last = realloc(last, (k+2)*sizeof(char));
        }
        last[++k] = (char)readed;
    }
    if (k == -1)
    {
        last = malloc(sizeof(char));
    }
    else
    {
        last = realloc(last, (k+2)*sizeof(char));
    }
    last[++k] = '\0';
    if (j == -1)
    {
        other = malloc(sizeof(char));
    }
    else
    {
        other = realloc(other, (j+2)*sizeof(char));
    }
    other[++j] = (char)readed;
    while((fread(&readed, sizeof(char), 1, inp)) > 0)
    {
        if (j == -1)
        {
            other = malloc(sizeof(char));
        }
        else
        {
            other = realloc(other, (j+2)*sizeof(char));
        }
        other[++j] = (char)readed;
    }
    fseek(inp, 0, SEEK_SET);
    int writed = 0;
    for(int ii = 0; ftell(inp) < frst_pos; ii++)
    {
        fwrite(&(other[ii]), sizeof(char), 1, inp);
        writed++;
    }
    for(int ii = 0; ii < strlen(last); ii++)
    {
        fwrite(&(last[ii]), sizeof(char), 1, inp);
    }
    for(int ii = 0; ii < middle_count ; ii++)
    {
        fwrite(&(other[ii + writed]), sizeof(char), 1, inp);
    }
    for(int ii = 0; ii < strlen(frst); ii++)
    {
        fwrite(&(frst[ii]), sizeof(char), 1, inp);
    }
    for(int ii = writed + middle_count; ii < strlen(other); ii++)
    {
        fwrite(&(other[ii]), sizeof(char), 1, inp);
    }
    free(frst);
    free(other);
    free(last);
}

void go_to_dir(char* dir_path, int deap)
{
    struct dirent* check;
    DIR* dir_rec;
    DIR* input = opendir(dir_path);
    char** dirs_arr;
    int size = 0;
    check = readdir(input);
    check = readdir(input);
    while(check = readdir(input))
    {
        char* adding = malloc(sizeof(dir_path)+sizeof(check->d_name) + 1);
        strcpy(adding, dir_path);
        strcat(adding, "/");
        strcat(adding, check->d_name);
        if((dir_rec = opendir(adding)) != NULL)
        {
            struct stat *st = malloc(sizeof(struct stat));
            stat(adding, st);
            time_t opening = st->st_atime;
            printf("%*c", deap + 1, ' '); //надо еще вывести время создания
            printf("dir: %s %s", adding, ctime(&opening));
            go_to_dir(adding, deap + 4);
            closedir(dir_rec);
            free(st);
        }
        else
        {
            struct stat *st = malloc(sizeof(struct stat));
            stat(adding, st);
            if((*st).st_mode & S_IXUSR)
            {
                printf("%*c", deap + 1, ' ');
                printf("%s\n", check->d_name);
                FILE* ch = fopen(adding, "r+");
                change_words(ch);
                fclose(ch);
            }
            free(st);
        }
        free(adding);
    }
    closedir(input);
}

int main (int argc, char** argv)
{
    if(argc!=2)
    {
        printf("error");
        return 1;
    }
    DIR* dp;
    if ((dp = opendir(argv[1])) == NULL)
    {
        printf("not cat");
    }

    struct dirent* rec;

    struct stat file_stat, file_stat_2;
    long long reg_size = 0;
    while(rec = readdir(dp))
    {
        lstat(rec->d_name, &file_stat);
        stat(rec->d_name, &file_stat_2);
        char* name = rec->d_name;
        if(S_ISLNK(file_stat.st_mode))
        {
            char name[260];
            for (int i = 0; rec->d_name[i] != '\0'; i++)
            {
                name[i] = rec->d_name[i];
                if(rec->d_name[i+1] == '\0')
                {
                    name[i+1] = '\0';
                }
            }
            DIR* back = dp;
            struct stat file_stat_check;
            rewinddir(back);
            struct dirent* ch;
            int pass = 0;
            while(ch = readdir(back))
            {
                stat(ch->d_name, &file_stat_check);
                if(file_stat.st_ino == file_stat_check.st_ino)
                {
                    pass++;
                }
            }
            if(pass == 0)
            {
                printf("unknow-file: %s ", name);
            }
            rewinddir(back);
            ch = readdir(back);
            while (ch->d_name != rec->d_name)
            {
                ch = readdir(back);
            }
        }
        else
        {
            reg_size += file_stat.st_size;
        }
    }
    printf("\n size of all files: %llu", reg_size);
    closedir(dp);
    struct stat *st = malloc(sizeof(struct stat));
    stat(argv[1], st);
    time_t opening = st->st_atime;
    printf("%s", argv[1], ctime(&opening));
    free(st);
    go_to_dir(argv[1], 1);
    return 0;
}