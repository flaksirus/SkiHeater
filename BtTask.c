#include "BtTask.h"

static BtCommandTask btApp;

void InitBtTask()
{
    /* Set up task 1 handler */
    btApp.task.handler = BtTaskHandler;
        
    setSppState(sppDevInitialising);
    btApp.spp = 0;
        
    /* Init the Connection Manager */
    ConnectionInit(&btApp.task);
}

/* Task handler function */
void BtTaskHandler(Task task, MessageId id, Message message)
{
    sppDevState state = btApp.spp_state;
    
    switch(id)
    {
    case CL_INIT_CFM:
        DEBUG(("CL_INIT_CFM\n"));
        if(((CL_INIT_CFM_T*)message)->status == success)
        {
            /* Connection Library initialisation was a success */
            /*sppDevInit();   */
            spp_init_params init;

            init.client_recipe = 0;
            init.size_service_record = 0;
	        init.service_record = 0;
	        init.no_service_record = 0;
	
            /* Initialise the spp profile lib, stating that this is device B */ 
            SppInitLazy(&btApp.task, &btApp.task, &init);
        }
        /*else
            Some reset function*/
        break;
    case CL_DM_LINK_SUPERVISION_TIMEOUT_IND:
        DEBUG(("CL_DM_LINK_SUPERVISION_TIMEOUT_IND\n"));
        break;
    case CL_DM_SNIFF_SUB_RATING_IND:
        DEBUG(("CL_DM_SNIFF_SUB_RATING_IND\n"));
        break;
    case SPP_INIT_CFM:
        DEBUG(("SPP_INIT_CFM\n"));
        switch(state)
        {
        case sppDevInitialising:
            /* Check for spp_init_success. What do we do if it failed? */
            if (((SPP_INIT_CFM_T *) message)->status == spp_init_success)
            {
                setSppState(sppDevReady);
                /* Turn off security */
                ConnectionSmRegisterIncomingService(0x0000, 0x0001, 0x0000);
                /* Write class of device */
                ConnectionWriteClassOfDevice(0x1F00);
                /* Start Inquiry mode */
                setSppState(sppDevPairable);
                /* Set devB device to inquiry scan mode, waiting for discovery */
                ConnectionWriteInquiryscanActivity(0x400, 0x200);
                ConnectionSmSetSdpSecurityIn(TRUE);
                /* Make this device discoverable (inquiry scan), and connectable (page scan) */
                ConnectionWriteScanEnable(hci_scan_enable_inq_and_page);
            }
            break;
        case sppDevReady:
        case sppDevPairable:
        case sppDevConnecting:
        case sppDevConnected:
        default:
            DEBUG(("SPP_INIT_CFM unhandled: state = %d, id = %d\n", state, id));
            break;
        }
        break;
    case SPP_CONNECT_CFM:
        {        
            SPP_CONNECT_CFM_T *cfm = (SPP_CONNECT_CFM_T *) message;
            DEBUG(("SPP_CONNECT_CFM result = %d\n", cfm->status));
            switch(state)
            {   
            case sppDevConnecting:
                /* Connect cfm, but must check status as connection may have failed */
                if (cfm->status == rfcomm_connect_success)
                {
                    /* Connection Success */
                    DEBUG(("Device connected...\n"));
                    /* Connect Uart to Rfcomm */
                    /*(void) StreamConnect(StreamUartSource(), cfm->sink);
                    (void) StreamConnect(StreamSourceFromSink(cfm->sink), StreamUartSink());*/
					/* (void) StreamConnectDispose(StreamSourceFromSink(cfm->sink)); */
                    
                    btApp.spp = cfm->spp;                    
                    btApp.sink = cfm->sink;
                    setSppState(sppDevConnected);
                    ConnectionWriteScanEnable(hci_scan_enable_off);
                }
                else
                {
                    /* Connection failed */
                    setSppState(sppDevPairable);
                    DEBUG(("Connection failed\n"));
                }
                break;
            case sppDevPairable:
                /* Connect cfm, but must check status as connection may have failed */
                if (cfm->status == rfcomm_connect_success)
                {
                    /* Device has been reset to pairable mode. Disconnect from current device */
                    SppDisconnect(cfm->spp);
                }
                break;
            case sppDevInitialising:
            case sppDevReady:
            case sppDevConnected:
            default:
                DEBUG(("SPP_CONNECT_CFM unhandled: state = %d, id = %d\n", state, id));
                break;
            }
        }
        break;
    case SPP_CONNECT_IND:
        {
            DEBUG(("SPP_CONNECT_IND\n"));        
            /*CAST_TYPED_MSG(SPP_CONNECT_IND, ind, message);*/
            switch(state)
            {   
            case sppDevPairable:
                /* Received command that a device is trying to connect. Send response. */
                SppConnectResponseLazy(((SPP_CONNECT_IND_T*)message)->spp, TRUE, &((SPP_CONNECT_IND_T*)message)->addr, 1, FRAME_SIZE);
                /*sppDevAuthoriseConnectInd(btApp,(SPP_CONNECT_IND_T*)message);*/
                setSppState(sppDevConnecting);
                break;
            case sppDevInitialising:
            case sppDevConnecting:
            case sppDevReady:
            case sppDevConnected:
            default:
                DEBUG(("SPP_CONNECT_IND unhandled: state = %d, id = %d\n", state, id));
                break;
            }
        }
        break;
    case SPP_DISCONNECT_IND:
        DEBUG(("SPP_DISCONNECT_IND\n"));
        /* Disconnect message has arrived */
        switch(state)
        {
        case sppDevConnected:
            /* Turn off security */
            ConnectionSmRegisterIncomingService(0x0000, 0x0001, 0x0000);
            /* Write class of device */
            ConnectionWriteClassOfDevice(0x1F00);
            /* Start Inquiry mode */
            setSppState(sppDevPairable);
            /* Set devB device to inquiry scan mode, waiting for discovery */
            ConnectionWriteInquiryscanActivity(0x400, 0x200);
            ConnectionSmSetSdpSecurityIn(TRUE);
            /* Make this device discoverable (inquiry scan), and connectable (page scan) */
            ConnectionWriteScanEnable(hci_scan_enable_inq_and_page);
            break;
        case sppDevInitialising:
        case sppDevPairable:
        case sppDevConnecting:
        case sppDevReady:
        default:
            DEBUG(("SPP_DISCONNECT_IND unhandled: state = %d, id = %d\n", state, id));
            break;
        }
        break;      
    case CL_DM_ACL_OPENED_IND:
        DEBUG(("CL_DM_ACL_OPENED_IND\n"));
        break;
    case CL_DM_ACL_CLOSED_IND:
        DEBUG(("CL_DM_ACL_CLOSED_IND\n"));
        break;
    case CL_SM_PIN_CODE_IND:
        DEBUG(("CL_SM_PIN_CODE_IND\n"));
        ConnectionSmPinCodeResponse(&((CL_SM_PIN_CODE_IND_T*)message)->bd_addr, 4, (uint8*)"1234");
        /*sppDevHandlePinCodeRequest((CL_SM_PIN_CODE_IND_T *) message);*/
        break;
    case CL_SM_AUTHORISE_IND:  
        DEBUG(("CL_SM_PIN_CODE_IND\n"));
        /*sppDevAuthoriseResponse((CL_SM_AUTHORISE_IND_T*) message);*/
        ConnectionSmAuthoriseResponse(&((CL_SM_AUTHORISE_IND_T*)message)->bd_addr, 
                                          ((CL_SM_AUTHORISE_IND_T*)message)->protocol_id, 
                                          ((CL_SM_AUTHORISE_IND_T*)message)->channel, 
                                          ((CL_SM_AUTHORISE_IND_T*)message)->incoming, 
                                          TRUE);
        break;
    case CL_SM_AUTHENTICATE_CFM:
        DEBUG(("CL_SM_AUTHENTICATE_CFM\n"));
        /*sppDevSetTrustLevel((CL_SM_AUTHENTICATE_CFM_T*)message);   */
        /*CAST_TYPED_MSG(CL_SM_AUTHENTICATE_CFM, cfm, message);   */  
        if(((CL_SM_AUTHENTICATE_CFM_T*)message)->status == auth_status_success)
        {
            /* Pairing success, now set the trust level*/
            ConnectionSmSetTrustLevel(&((CL_SM_AUTHENTICATE_CFM_T*)message)->bd_addr, TRUE);
        }
        else if(((CL_SM_AUTHENTICATE_CFM_T*)message)->status == auth_status_fail)
        {
             DEBUG(("Pairing failed\n"));
        }
        break;
    case CL_SM_ENCRYPTION_KEY_REFRESH_IND:
        DEBUG(("CL_SM_ENCRYPTION_KEY_REFRESH_IND\n"));
        break;
    case CL_DM_LINK_POLICY_IND:
        DEBUG(("CL_DM_LINK_POLICY_IND\n"));
        break;
    case CL_SM_IO_CAPABILITY_REQ_IND:
        DEBUG(("CL_SM_IO_CAPABILITY_REQ_IND\n"));
        ConnectionSmIoCapabilityResponse( &btApp.bd_addr, 
                                          cl_sm_io_cap_no_input_no_output,
                                          FALSE,
                                          TRUE,
                                          FALSE,
                                          0,
                                          0 );
        break;
 
    case CL_SM_REMOTE_IO_CAPABILITY_IND:
        {
            CAST_TYPED_MSG(CL_SM_REMOTE_IO_CAPABILITY_IND, csricit, message);    

            DEBUG(("CL_SM_REMOTE_IO_CAPABILITY_REQ_IND\n"));
            
            DEBUG(("\t Remote Addr: nap %04x uap %02x lap %08lx\n",
                    csricit->bd_addr.nap,
                    csricit->bd_addr.uap,
                    csricit->bd_addr.lap ));
            btApp.bd_addr = csricit->bd_addr;
        }
        break;          
    case SPP_MESSAGE_MORE_DATA:
        DEBUG(("SPP_MESSAGE_MORE_DATA\n"));
        ParseSppData(task, ((SPP_MESSAGE_MORE_DATA_T*)message)->source);
        break;
    case SPP_MESSAGE_MORE_SPACE:
        DEBUG(("SPP_MESSAGE_MORE_SPACE\n"));
        break;
	/*case BUTTON_RESET_PRESS:
		DEBUG(("Button pressed\n"));
        switch(btApp.spp_state)
        {   
        case sppDevInitialising:
            return;
            break;
        case sppDevReady:
        case sppDevPairable:
        case sppDevConnecting:
            sppDevInquire(btApp);
            break;
        case sppDevConnected:
            SppDisconnect(btApp.spp);
            break;
        }
		break;*/
    case CL_RFCOMM_CONTROL_IND:
        DEBUG(("CL_RFCOMM_CONTROL_IND\n"));
        break;        
    default:
        /* An unexpected message has arrived - must handle it */
        DEBUG(("bt app - msg type  not yet handled 0x%x\n", id));
        break;
    }
}


/*************************************************************************
NAME    
    setSppState    
DESCRIPTION
    Set the SPP State to the specified state
RETURNS  */
void setSppState(const sppDevState state)
{
    DEBUG(("SPP State - C=%d N=%d\n", btApp.spp_state, state));
    btApp.spp_state = state;
	
	/* Update led flash pattern 
    switch (state)
    {
    case sppDevConnected:
        ledsPlay(RED_FLASH);
        break;
    default:
		ledsPlay(RED_BLUE_ALT);
        break;
    }*/
}

#include "bt_control_parse.h"

void ParseSppData(Task task, Source src)
{
    parseSource(src, task);
}

void sink_write(Sink sink, const char *buf, int size)
{
    int offset = SinkClaim(sink, size);
    uint8 *dest = SinkMap(sink);
    memcpy(dest + offset, buf, size);
    SinkFlush(sink, size);
}

void sink_write_str(Sink sink, const char *str)
{
    sink_write(sink, str, strlen(str));
}


void command(Task task)
{
    BtCommandTask *self = (BtCommandTask*)task;
    sink_write_str(self->sink, "OK\r\n");
}

void command_heat_get(Task task)
{
    BtCommandTask *self = (BtCommandTask*)task;
    uint8 result = 10 * GetBootHeater();
    char buf[20];
    sprintf(buf, "OK %d\r\n", result);
    sink_write_str(self->sink, buf);
}

void command_heat_set(Task task, const struct command_heat_set* args)
{
    BtCommandTask *self = (BtCommandTask*)task;
    if(args->value<=100)
    {
        SetBootHeater(args->value);     
        sink_write_str(self->sink, "OK\r\n");
    }
    else 
        sink_write_str(self->sink, "ERROR: Value must be from 0 to 100\r\n");
}

void command_temp_get(Task task)
{
}

void handleUnrecognised(const uint8 *data, uint16 length, Task task)
{
    BtCommandTask *self = (BtCommandTask*)task;
    sink_write_str(self->sink, "ERROR\r\n");
    DEBUG(("handleUnrecognised\n"));   
}
