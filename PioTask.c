#include "PioTask.h"
#include "Heater.h"

static PioCommandTask pioApp;
uint8 patterns[] = { PWM0, PWM10, PWM20, PWM30, PWM40, PWM50, PWM60, PWM70, PWM80, PWM90, PWM100 };

void InitPioTask()
{
    /* Set up task 1 handler */
    pioApp.task.handler = PioTaskHandler;
                
    pioApp.new_heater_state = Heater0;
    
    MessagePioTask(&pioApp.task);
    PioSetDir(0x03, 0x03);
    PioSet(0x03, 0);
    MessageSend( &pioApp.task, NewHeaterState, 0);
}

/* Task handler function */
void PioTaskHandler(Task task, MessageId id, Message message)
{
    switch(id)
    {
        case NewHeaterState:
            {
                ledsPlay(patterns[(int)pioApp.new_heater_state]);
                pioApp.heater_state = pioApp.new_heater_state; 
                DEBUG(("NewHeaterState state = %d\n", (int) pioApp.new_heater_state));
            }
           break;
        case RequestTemperature:
           break;
        default:
            /* An unexpected message has arrived - must handle it */
            DEBUG(("pio app - msg type  not yet handled 0x%x\n", id));
            break;
    }
}

int16 GetTemperature()
{
    return 0;
}

HeaterState GetBootHeater()
{
    return pioApp.heater_state;
}

void SetBootHeater(uint16 newState)
{
    pioApp.new_heater_state = patterns[newState/10];
    /*Message* msg = (Message*) newState;*/
    MessageSend( &pioApp.task, NewHeaterState, 0);
}
