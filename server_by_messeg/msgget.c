#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#define MSGSZ 128

typedef struct msgbuf {
         long mtype;
         char mtext[MSGSZ];
         } message_buf;
key_t key1 = -1;
int que_id = -1;
message_buf msgg;
message_buf prnt_count;

void IPC_for_getting_id(char* str, char simb)
{
    key1 = ftok(str, simb);
    if (key1 == -1)
    {
        perror("key error");
        exit(1);
    }
    que_id = msgget(key1, IPC_CREAT | 0666);
    if (que_id == -1)
    {
        perror("que error");
        exit(2);
    }
}

int main(int argc, char** argv)
{
    IPC_for_getting_id(argv[1], argv[2][0]);
    while(1)
    {
        msgg.mtype = 1;
        if(msgrcv(que_id, &msgg, sizeof(message_buf) - sizeof(long), 1, 0) < 0)
        {
            exit(1);
        }
        int count = 0;
        for(int i = 0; msgg.mtext[i] != '\0'; i++)
        {
            if(msgg.mtext[i] >= '0' && msgg.mtext[i] <= '9')
            {
                count++;
            }
        }
        prnt_count.mtype = 2;
        char str[MSGSZ];
        if (count == 0)
        {
            str[0] = '0';
        }
        else
        {
            int i = 0;
            int buf = count;
            int len = 0;
            while(buf != 0)
            {
                buf /= 10;
                len++;
            }
            str[len + 1] = '\0';
            while(count != 0)
            {
                str[len - 1] = count % 10 + '0';
                count /= 10;
            }
        }
        strcpy(prnt_count.mtext, str);
        msgsnd(que_id, &prnt_count, sizeof(message_buf) - sizeof(long), IPC_NOWAIT);
    }
    return 0;
}