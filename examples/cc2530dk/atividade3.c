/* This is a very simple hello_world program.
 * It aims to demonstrate the co-existence of two processes:
 * One of them prints a hello world message and the other blinks the LEDs
 *
 * It is largely based on hello_world in $(CONTIKI)/examples/sensinode
 *
 * Author: George Oikonomou - <oikonomou@users.sourceforge.net>
 * Aletrado para a disciplina de Rede de Sensores Sem Fio
 */
// cocococococococo
#include "contiki.h"
#include "dev/leds.h"

#include <stdio.h> /* For printf() */

#define LED_PING_EVENT (44)
#define LED_PONG_EVENT (45)

/*---------------------------------------------------------------------------*/
static struct etimer et_hello;
static struct etimer et_blink;
static struct etimer et_proc3;
static struct etimer et_pong;
static uint16_t count;
static uint8_t blinksY;
static uint8_t blinksR;

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
PROCESS(blink_process, "Yellow LED blink process");
PROCESS(proc3_process, "Red LED blink process");
PROCESS(pong_process, "Pong process");

AUTOSTART_PROCESSES(&hello_world_process,&blink_process,&proc3_process,&pong_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();

  etimer_set(&et_hello, CLOCK_SECOND * 4);
  count = 0;

  printf("Hello World Online\n");
  while(1) {
    PROCESS_WAIT_EVENT();

    if(ev == PROCESS_EVENT_TIMER)
    {
		etimer_reset(&et_hello);
		//printf("Hello: Ping\n");
		//process_post(&pong_process,LED_PING_EVENT, (void*)(&hello_world_process));
    }
    if(ev == LED_PONG_EVENT)
    {
    	//printf("Hello: Pong\n\n\n");
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(blink_process, ev, data)
{
	PROCESS_BEGIN();

	etimer_set(&et_blink, CLOCK_SECOND * 1);
	blinksY = 0;

  	printf("Blink Online\n");
  	while(1) {
  	    PROCESS_WAIT_EVENT();

  	    if(ev == PROCESS_EVENT_TIMER)
  	    {
  	    	if(!blinksY)
  			{
  				leds_on(LEDS_YELLOW);
  				blinksY++;
  			}
  			else
  			{
  				leds_off(LEDS_YELLOW);
  				blinksY--;
  			}
  	    	etimer_reset(&et_blink);

  	        printf("Blink: Ping\n");
  	        process_post(&pong_process, LED_PING_EVENT, (void*)(&blink_process));
  	    }
  	    if(ev == LED_PONG_EVENT)
  	    {
  	    	if(!blinksY)
  			{
  				leds_on(LEDS_YELLOW);
  				blinksY++;
  			}
  			else
  			{
  				leds_off(LEDS_YELLOW);
  				blinksY--;
  			}
  	    	printf("Blink: Pong\n\n\n");
  	    }
  	  }

  	PROCESS_END();
}


/*---------------------------------------------------------------------------*/
PROCESS_THREAD(proc3_process, ev, data)
{
	PROCESS_BEGIN();

  	etimer_set(&et_proc3, CLOCK_SECOND * 5);
	blinksR = 0;

	printf("Proc3 Online\n");
	while(1) {
	    PROCESS_WAIT_EVENT();

	    if(ev == PROCESS_EVENT_TIMER)
	    {
	    	if(!blinksR)
			{
				leds_on(LEDS_RED);
				blinksR++;
			}
			else
			{
				leds_off(LEDS_RED);
				blinksR--;
			}
	    	etimer_reset(&et_proc3);

	        printf("Proc3: Ping\n");
	        process_post(&pong_process, LED_PING_EVENT, (void*)(&proc3_process));
	    }
	    if(ev == LED_PONG_EVENT)
	    {
	    	if(!blinksR)
			{
				leds_on(LEDS_RED);
				blinksR++;
			}
			else
			{
				leds_off(LEDS_RED);
				blinksR--;
			}
	    	printf("Proc3: Pong\n\n\n");
	    }
	  }

	PROCESS_END();
}

PROCESS_THREAD(pong_process, ev, data)
{
  PROCESS_BEGIN();
  //struct process *aux2;
  printf("Pong Process Online\n");
  while(1) {
    PROCESS_WAIT_EVENT();

    if(ev == LED_PING_EVENT) {
		printf("pong: LED Ping! ponging...\n", count);
		leds_on(LEDS_BLUE);
		etimer_set(&et_pong, CLOCK_SECOND * 0.2);

		process_post((struct process*)data, LED_PONG_EVENT, NULL);

//		if((struct process*)data == &blink_process)
//		{
//			process_post(&blink_process, LED_PONG_EVENT, NULL);
//		}
		//aux2 = (struct process*)data;
    }
    if(ev == PROCESS_EVENT_TIMER){
    	leds_off(LEDS_BLUE);
    	etimer_reset(&et_hello);
    }
  }

  PROCESS_END();
}
