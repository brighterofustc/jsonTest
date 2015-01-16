/********************************************************************/
/* Copyright (C) SSE-USTC, 2014-2015                                */
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
#include<pthread.h>

#include "stream.h"
#include "json.h"
#include "jsonrpc.h"

int handle_rpc(struct jsonrpc *rpc, struct jsonrpc_msg *msg, int *done)
{
    if(msg->type == JSONRPC_REQUEST) 
    {
        struct jsonrpc_msg *reply = NULL;
        if(!strcmp(msg->method, "echo")) 
        {
            reply = jsonrpc_create_reply(json_clone(msg->params), msg->id);
        } 
        else 
        {
            struct json *error = json_object_create();
            json_object_put_string(error, "error", "unknown method");
            reply = jsonrpc_create_error(error, msg->id);
            printf("unknown request %s\n", msg->method);
        }
        jsonrpc_send(rpc, reply);
        return 0;
    } 
    else if(msg->type == JSONRPC_NOTIFY) 
    {
        if(!strcmp(msg->method, "shutdown")) 
        {
            *done = 1;
            return 0;
        } 
        else 
        {
            printf("unknown notification %s\n", msg->method);
            return 0;
        }
    } 
    else 
    {
        printf("unsolicited JSON-RPC reply or error\n");
        return 1;
    }
} 

int main(int argc, char **argv)
{
    const char* pstream_name = "ptcp:1234";
    char buffer[50];
    uint8_t dscp = 1;
    int actualrecv = 0;
    int error;

    struct pstream *p_pstream;
    error = jsonrpc_pstream_open(pstream_name,&p_pstream,dscp);
    if(error)
    {
        printf("jsonrpc pstream open failure!\n");
        exit(1);
    }

    struct jsonrpc  **rpcs;
    size_t n_rpcs =0, allocated_rpcs = 0;
    struct stream *p_stream; 
    while(1)
    {  
        
	while(pstream_accept(p_pstream, &p_stream));
	    
	printf("\nget one steam connect success!\n");

        printf("p_pstream name:%s\n", pstream_get_name(p_pstream));
        printf("p_pstream bound_port:%d\n", pstream_get_bound_port(p_pstream));
        printf("p_stream name:%s\n", stream_get_name(p_stream));
        printf("<remote_ip,remote_port> : <%d,%d>\n", stream_get_remote_ip(p_stream),stream_get_remote_port(p_stream));
        printf("<local_ip,local_port> : <%d,%d>\n", stream_get_local_ip(p_stream),stream_get_local_port(p_stream));

        if(n_rpcs >= allocated_rpcs)
        {
            rpcs = x2nrealloc(rpcs,&allocated_rpcs,sizeof *rpcs);	
        }	
        rpcs[n_rpcs ++] = jsonrpc_open(p_stream); 
         

        /* service existing connections */   
        int i;
        int done = 0;
        for(i = 0 ; i < n_rpcs;)
        {
        	struct jsonrpc *rpc = rpcs[i];
        	struct jsonrpc_msg *msg;

        	jsonrpc_run(rpc);

        	if(!jsonrpc_get_backlog(rpc))
        	{
        		error = jsonrpc_recv(rpc,&msg);

        		if(!error)
        		{
                            error = handle_rpc(rpc,msg,&done);
                            jsonrpc_msg_destroy(msg);
        		}
        		else if(error == 1)
        		{
                            error = 0;
        		}	
        	}	
            
            if(!error)
            {
            	error = jsonrpc_get_status(rpc);
            }

            if (error) {
                jsonrpc_close(rpc);
                printf("connection closed\n");
                memmove(&rpcs[i], &rpcs[i + 1],
                        (n_rpcs - i - 1) * sizeof *rpcs);
                n_rpcs--;
            } else {
                i++;
            }
        }	       
    }   

    stream_close(p_stream);
    pstream_close(p_pstream);

    return 0;  
}
