#ifndef _BT_TASK_H_
#define _BT_TASK_H_

#include <spp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <source.h>
#include <sink.h>
#include <stream.h>
#include <vm.h>

#include "global.h"
#include "PioTask.h"

#define FRAME_SIZE		0

typedef enum
{
    sppDevInitialising,
    sppDevReady,
    sppDevPairable,
    sppDevConnecting,
    sppDevConnected
} sppDevState;

typedef struct BtCommandTask {
    TaskData            task;
    Sink                sink;
    SPP*                spp;
    bdaddr              bd_addr;
    sppDevState         spp_state;
} BtCommandTask;

void InitBtTask(void);
void BtTaskHandler(Task task, MessageId id, Message message);

void ParseSppData(Task task, Source src);


/*************************************************************************
NAME    
    setSppState    
DESCRIPTION
    Set the SPP State to the specified state
RETURNS*/
void setSppState(const sppDevState state);

void sink_write(Sink sink, const char *buf, int size);
void sink_write_str(Sink sink, const char *str);

#endif /* _BT_TASK_H_ */
