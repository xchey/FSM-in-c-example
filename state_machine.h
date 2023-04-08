//
// Created by chey on 2023/4/6.
//

#ifndef LINUX_KERNEL_DS_STATE_MACHINE_H
#define LINUX_KERNEL_DS_STATE_MACHINE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//! define events
typedef enum {
    E_IDLE = 0,     // idle event
    E_BELL,         // bell event
    E_WHITE_LIST,   // white list
    E_BLACK_LIST,   // black list
    E_FINISH,       // call complete event
    E_BUSY,         // call in busy event
    E_CONNECT,      // call connected
    E_DIAL,         // dial event
    E_TIME_OUT      // call timeout event
} event_t;

//! define states
typedef enum {
    S_IDLE = 0,
    S_BELL,
    S_TALK,
    S_HANGUP,
    S_DIAL,
    S_TIMEOUT
} state_t;

//! event action function
typedef void (*event_action_t)(event_t *ev, void *);

//! define a FSM structure
typedef struct {
    state_t cur_stat;               // current state
    event_t event;                  // event trigger
    event_action_t event_action;    // correspond event action
    state_t nxt_stat;               // next state
} fsm_table_t;

typedef struct FSM
{
    state_t         cur_stat;
    event_t         event;
    uint8_t         ns;
    fsm_table_t     *fsmtb;
} fsm_t;

extern void test_fsm(void);
#endif //LINUX_KERNEL_DS_STATE_MACHINE_H
