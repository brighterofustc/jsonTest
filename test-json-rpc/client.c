/********************************************************************/
/* Copyright (C) SSE-USTC, 2013-2014                                */
/*                                                                  */
/*  FILE NAME             :  json_server.c                          */
/*  PRINCIPAL AUTHOR      :  ZhangFuqaing                           */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  ANY                                    */
/*  DATE OF FIRST RELEASE :  2015/01/07                             */
/*  DESCRIPTION           :  test file for json                     */
/********************************************************************/

/*
 * Revision log:
 *
 * Created by Zhang Fuqiang,2015/01/07
 *
 */

#include<stdio.h> 			
#include<stdlib.h>			
#include<sys/types.h>		
#include<sys/wait.h>		
#include<string.h>			

#include "stream.h"
#include "json.h"
#include "jsonrpc.h"


int send_reuqest(struct jsonrpc *rpc)
{
	struct jsonrpc_msg   *msg;
    struct json   *params;
    struct json   **idp;


    msg = jsonrpc_create_request("echo",params,idp);

    return jsonrpc_send(rpc,msg);
}

int send_notify(struct jsonrpc *rpc)
{
	struct jsonrpc_msg   *msg;
    struct json   *params;
    struct json   **idp;


    msg = jsonrpc_create_notify("shutdown",params,idp);

    return jsonrpc_send(rpc,msg);
}

int send_reply(struct jsonrpc *rpc)
{
	struct jsonrpc_msg   *msg;
    struct json   *params;
    struct json   **idp;


    msg = jsonrpc_create_reply("reply",params,idp);

    return jsonrpc_send(rpc,msg);
}

void print_jsonrpc_msg(struct jsonrpc_msg *msg)
{

	struct json* jm = jsonrpc_msg_to_json(msg);

	char *string = json_to_string(json,JSSF_SORT);

	puts(string);

	free(jm);
	free(string);
}


int main(int argc, char **argv)
{
    struct stream* p_stream;
    const char* stream_name = "tcp:127.0.0.1:1234";
    char pnum[4];
    uint8_t dscp = 1;
    int actualsend = 0;
    int index = 0;

    int error;
    error = jsonrpc_stream_open(stream_name,&p_stream,dscp);

    if(error)
    {
        printf("jsonrpc stream open failure!\n");
    }
    else
    {
        printf("jsonrpc stream open sucessed!\n");

        if(stream_connect(p_stream))
        {
            printf("jsonrpc stream connect failure!\n");
        }
        else
        {
            printf("jsonrpc stream connect successed!\n");
            
            struct jsonrpc_msg *msg;
            struct jsonrpc *rpc;

            rpc = jsonrpc_open(p_stream); 

            int ok;

            ok = send_reuqest(rpc);
            if(!ok)
            {
            	printf("Send Request Error\n");
            }
            else
            {
            	ok = jsonrpc_recv(rpc,&msg);

                if(!ok)
                {
                    print_jsonrpc_msg(msg); 
                }
                else
                {
                    printf("recv Request reply error\n");
                }    
            }


            ok = send_notify(rpc);
            if(!ok)
            {
            	printf("Send notify Error\n");
            }
            else
            {
            	ok = jsonrpc_recv(rpc,&msg);

                if(!ok)
                {
                    print_jsonrpc_msg(msg); 
                }
                else
                {
                    printf("recv notify reply error\n");
                }    
            }


             ok = send_reply(rpc);
            if(!ok)
            {
            	printf("Send Reply Error\n");
            }
            else
            {
            	ok = jsonrpc_recv(rpc,&msg);

                if(!ok)
                {
                    print_jsonrpc_msg(msg); 
                }
                else
                {
                    printf("recv reply error\n");
                }    
            }


        }
    }

    stream_close(p_stream);
    sleep(1);

	return 0;
}