#ifndef _PIO_TASK_H_
#define _PIO_TASK_H_

#include <message.h>
#include <pio.h>
#include <stdio.h>
#include <stream.h>
#include <vm.h>

#include "global.h"

enum {
    NewHeaterState = 0,
    RequestTemperature
};

typedef enum 
{
    Heater0,
    Heater10,
    Heater20,
    Heater30,
    Heater40,
    Heater50,
    Heater60,
    Heater70,
    Heater80,
    Heater90,
    Heater100
} HeaterState;

typedef struct PioCommandTask {
    TaskData            task;
    HeaterState         heater_state;
    HeaterState         new_heater_state;            				 
} PioCommandTask;

void InitPioTask(void);
void PioTaskHandler(Task task, MessageId id, Message message);

void SetBootHeater(uint16 newState); /*new state in percents*/
HeaterState GetBootHeater(void);
int16 GetTemperature(void);


#endif /* _PIO_TASK_H_ */
