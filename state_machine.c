//
// Created by chey on 2023/4/6.
//

#include "state_machine.h"

/// finite state machine:
/// 4 key components: event, current state, response action, next state
/// encapsulate the components into a structure, define an structure array
/// loop the array, execute correspond event
/// /
void idle_func(event_t *event, void *args)
{
    int op = 0;
    printf("mobile phone in idle state... "
           "[input event]: 1. Bell, 2. Dail, 3. Idle: ");
    while (1)
    {
        scanf("%d", &op);
        if ((op <= 0) || (op > 3))
        {
            printf("unknown option %d, re-input: ", op);
        }
        else
        {
            break;
        }
    }
    printf("\n");
    switch (op)
    {
        case 1: *event = E_BELL; break;
        case 2: *event = E_DIAL; break;
        case 3: *event = E_IDLE; break;
        default: *event = E_IDLE; break;
    }
}

void bell_func(event_t *event, void *args)
{
    int op = 0;
    printf("mobile phone ringing..."
           "[input event]: 1. TimeOut, 2. WhiteList, 3. BlackList: ");
    while(1)
    {
        scanf("%d", &op);
        if ((op <= 0) || (op > 3))
        {
            printf("unknown option %d, re-input: ", op);
        }
        else
        {
            break;
        }
    }
    printf("\n");
    switch (op)
    {
        case 1: *event = E_TIME_OUT; break;
        case 2: *event = E_WHITE_LIST; break;
        case 3: *event = E_BLACK_LIST; break;
        default: *event = E_IDLE; break;
    }
}

void dial_func(event_t *event, void *args)
{
    int op = 0;
    printf("mobile phone dailing..."
           "[input event]: 1. Busy, 2. Connected, 3. TimeOut: ");
    while(1)
    {
        scanf("%d", &op);
        if ((op <= 0) || (op > 3))
        {
            printf("unknown option %d, re-input: ", op);
        }
        else
        {
            break;
        }
    }
    printf("\n");
    switch (op)
    {
        case 1: *event = E_BUSY; break;
        case 2: *event = E_CONNECT; break;
        case 3: *event = E_TIME_OUT; break;
        default: *event = E_IDLE; break;
    }
}

void timeout_func(event_t *event, void *args)
{
    printf("> call timeout (event %d)\n", *event);
    *event = E_IDLE;
}

void hangup_func(event_t *event, void *args)
{
    printf("> call hangup (event %d)\n", *event);
    *event = E_IDLE;
}

void talk_func(event_t *event, void *args)
{
    #define END_OF_TALKING      (5)
    int ret=0;
    printf("Talking ... "
           "[input 5 to end talking]: ");
    while (ret != END_OF_TALKING)
    {
        scanf("%d", &ret);
    }
    *event = E_FINISH;
}

// create a finite state machine table
fsm_table_t fsmtb[16] = {
        {S_IDLE,    E_IDLE,       idle_func,    S_IDLE},
        {S_IDLE,    E_BELL,       bell_func,    S_BELL},
        {S_IDLE,    E_DIAL,       dial_func,    S_DIAL},
        {S_DIAL,    E_TIME_OUT,   timeout_func, S_TIMEOUT},
        {S_DIAL,    E_BUSY,       hangup_func,  S_HANGUP},
        {S_DIAL,    E_CONNECT,    talk_func,    S_TALK},
        {S_BELL,    E_TIME_OUT,   timeout_func, S_TIMEOUT},
        {S_BELL,    E_WHITE_LIST, talk_func,    S_TALK},
        {S_BELL,    E_BLACK_LIST, hangup_func,  S_HANGUP},
        {S_TALK,    E_FINISH,     hangup_func,  S_HANGUP},
        {S_HANGUP,  E_IDLE,       idle_func,    S_IDLE},
        {S_TIMEOUT, E_IDLE,       idle_func,    S_IDLE}
};


static int fsm_action(fsm_t *fsm, void *args)
{
    if ((fsm == NULL) || (fsm->fsmtb == NULL) || (fsm->ns == 0))
    {
        return -1;
    }
    state_t cur_stat = fsm->cur_stat;
    fsm_table_t *pfsmtb = fsm->fsmtb;

    for (int i = 0; i < fsm->ns; ++i)
    {
        if (pfsmtb[i].cur_stat == cur_stat && pfsmtb[i].event == fsm->event)
        {
            pfsmtb[i].event_action(&fsm->event, args);
            fsm->cur_stat = pfsmtb[i].nxt_stat;
            break;
        }
    }
    return 0;
}


static fsm_t *create_fsm(fsm_table_t *fsmtbl, const state_t state, const event_t event, const int32_t ns)
{
    fsm_t *fsm = (fsm_t *) malloc(sizeof(fsm_t));
    if (fsm != NULL)
    {
        fsm->cur_stat = state;
        fsm->event = event;
        fsm->fsmtb = fsmtbl;
        fsm->ns = ns;
    }
    return fsm;
}

void test_fsm(void)
{
    uint8_t n = sizeof(fsmtb) / sizeof(fsmtb[0]);
    fsm_t *fsm = create_fsm(fsmtb, S_IDLE, E_IDLE, n);
    while(1)
    {
        fsm_action(fsm, NULL);
    }
}
