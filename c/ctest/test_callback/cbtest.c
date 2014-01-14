
/** @file cbtest.c
  * @note HangZhou Hikvision Digital Technology Co., Ltd. All Right Reserved.
  * @brief    Simple C program , the function is inside same file. the function can later be put into external library like a shared object
  *
  * @author   liuboyf1
  * @date     2012-11-1
  * @version  V1.0.0
  *
  * @note ///Description here
  * @note History:
  * @note     <author>   <time>    <version >   <desc>
  * @note
  * @warning
  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _MyMsg
{
    int appId;
    char msgbody[32];
} MyMsg;

void myfunc( MyMsg *msg)
{
    if (strlen(msg->msgbody)  > 0 )
    {
        printf(" App Id = %d \n Msg = %s \n", msg->appId, msg->msgbody);
    }
    else
    {
        printf(" App Id = %d \n Msg =  No Msg\n", msg->appId);
    }
}

void (*callback)(void *); // Prototype definition

int main()
{
    MyMsg msg1;
    msg1.appId = 100;
    strcpy(msg1.msgbody, "This is a test\n");
    //Register
    callback = (void *) myfunc ;
    callback((MyMsg *) &msg1);
	return 0;
}

