/*
 * swtimer.c
 *
 *  Created on: 2020. 11. 19.
 *      Author: MCES
 */


#include "swtimer.h"

typedef struct{
  uint8_t Timer_En;         // Timer Enable Signal
  uint8_t Timer_Mode;       // Timer Mode
  uint32_t Timer_Ctn;       // Timer Current Value
  uint32_t Timer_Init;      // When the timer is initialized, the Counter's value.
  void (*TmrFnct)(void *);  // The Function will be executed at the end.
  void *TmrFnctArg;         // Arguments of the function.
}swtimer_t;

static volatile uint32_t sw_timer_counter      = 0;     // SW Timer Counter
static volatile uint16_t sw_timer_handle_index = 0;
static swtimer_t swtimer_tbl[_HW_DEF_SW_TIMER_MAX];

bool swtimerInit(void){
  uint8_t i;
  static uint8_t execute = 0;

  if(execute == 1){  // if you have already run it,
    return false;    // return false.
  }

  for(i =0; i<_HW_DEF_SW_TIMER_MAX; i++){   // Initialize the struct of the Timer
    swtimer_tbl[i].Timer_En = OFF;
    swtimer_tbl[i].Timer_Ctn = 0;
    swtimer_tbl[i].Timer_Init = 0;
    swtimer_tbl[i].TmrFnct = NULL;
  }

  execute = 1;
  return true;
}


void swtimerISR(void){    // = Software Timer Interrupt Service Request = SW Timer Interrupt Handler
  uint8_t i;
  sw_timer_counter++;     // count up

  for(i=0; i<_HW_DEF_SW_TIMER_MAX && i<sw_timer_handle_index; i++){    // iterate as the Number of Timer
    if( swtimer_tbl[i].Timer_En == ON ){                               // If the Timer is enable
      swtimer_tbl[i].Timer_Ctn--;                                      // Decount the Timer counter

      if(swtimer_tbl[i].Timer_Ctn == 0){                               // If the overflow happens ( because of the short of the timer )
        if(swtimer_tbl[i].Timer_Mode == ONE_TIME){                     // If the Mode is ONE TIME
          swtimer_tbl[i].Timer_En = OFF;                               // Timer is Unable
        }
        swtimer_tbl[i].Timer_Ctn = swtimer_tbl[i].Timer_Init;          // Initialize the Timer
        (*swtimer_tbl[i].TmrFnct)(swtimer_tbl[i].TmrFnctArg);          // Run the Timer function
      }
    }
  }
}


void swtimerSet(uint8_t TmrNum, uint32_t TmrData, uint8_t TmrMode, void (*Fnct)(void *), void *arg){
  swtimer_tbl[TmrNum].Timer_Mode = TmrMode;
  swtimer_tbl[TmrNum].TmrFnct    = Fnct;
  swtimer_tbl[TmrNum].TmrFnctArg = arg;
  swtimer_tbl[TmrNum].Timer_Ctn  = TmrData;
  swtimer_tbl[TmrNum].Timer_Init = TmrData;
}



void swtimerStart(uint8_t TmrNum){
  if(TmrNum < _HW_DEF_SW_TIMER_MAX){
    swtimer_tbl[TmrNum].Timer_Ctn = swtimer_tbl[TmrNum].Timer_Init;
    swtimer_tbl[TmrNum].Timer_En  = ON;
  }
}

void swtimerStop(uint8_t TmrNum){
  if( TmrNum < _HW_DEF_SW_TIMER_MAX ){
    swtimer_tbl[TmrNum].Timer_En = OFF;
  }
}

void swtimerReset(uint8_t TmrNum){
  swtimer_tbl[TmrNum].Timer_En  = OFF;
  swtimer_tbl[TmrNum].Timer_Ctn = swtimer_tbl[TmrNum].Timer_Init;
}

swtimer_handle_t swtimerGetHandle(void){
  swtimer_handle_t TmrIndex = sw_timer_handle_index;
  sw_timer_handle_index++;
  return TmrIndex;
}


uint32_t swtimerGetCounter(void){
  return sw_timer_counter;
}
