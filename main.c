#include "main.h"

int main(void)
{    
    DEBUG(("Main Started...\n"));
    
    /* Start BT task */
    InitBtTask();

    /* Start PIO task*/ 
    InitPioTask();
    
    /* Start the message scheduler loop */
    MessageLoop();
    
    /* Will never get here! */
    DEBUG(("Main Ended!\n"));
    
    return 0;
}
