/*
        fsm.c

        Created: 2019-03-05

        Mario Dellaviola

        fsm
*/
#include "fsm.h"

sStateMachine FSM_Init()
{
    sStateMachine a = {
        {State_1, Event_1, Event_1_Handler},
        {State_2, Event_2, Event_2_Handler},
        {State_3, Event_3, Event_3_Handler}
    };
}

eStates Event_1_Handler()
{
    return State_2;
}

eStates Event_2_Handler()
{
    return State_3;
}

eStates Event_3_Handler()
{
    return State_1;
}

/*
    eState eNextState = State_3;

    eEvent eNewEvent = some read;

    if(    (eNextState < end_state ) 
        && (eNewEvent < lend_event ) 
        && (fsmHandle[eNextState].eStateMachineEvent == eNewEvent) 
        && (fsmHandle[eNextState].pfStateMachineEventHandler != NULL))
    {
        eNextState = *fsmHandle[eNextState].pfStateMachineEventHandler();
    }












*/