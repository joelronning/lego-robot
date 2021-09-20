#include <stdio.h>
#include "brick.h"
#include <unistd.h>

#define Sleep( msec ) usleep(( msec ) * 1000 ) /* Definerar sleep, Sleep(1000)= 1 sekund */

#define MOTOR_RIGHT    	OUTA
#define MOTOR_LEFT    	OUTB
#define MOTOR_C    		OUTC
#define MOTOR_D    		OUTD
#define SENSOR_TOUCH	IN1
#define SENSOR_US		IN2
#define SENSOR_3		IN3
#define SENSOR_4		IN4

#define MOTOR_BOTH     	( MOTOR_LEFT | MOTOR_RIGHT ) /* Bitvis ELLER ger att båda motorerna styrs samtidigt */

int max_hastighet;         /* variabel för max hastighet på motorn */
POOL_T touchSensor, us_sensor;

int main( void )
{  
	
	if ( !brick_init()) return ( 1 ); /* Initialiserar EV3-klossen */
	printf( "*** ( EV3 ) Hello! ***\n" );
	Sleep( 2000 );
	
	if ( tacho_is_plugged( MOTOR_BOTH, TACHO_TYPE__NONE_ )) {  /* TACHO_TYPE__NONE_ = Alla typer av motorer */
        max_hastighet = tacho_get_max_speed( MOTOR_LEFT, 0 );	/* Kollar maxhastigheten som motorn kan ha */
        tacho_reset( MOTOR_BOTH );
    } else {
        printf( "Anslut vänster motor i port A,\n"
        "Anslut höger motor i port B.\n"
        );
      	brick_uninit();
        return ( 0 );  /* Stänger av sig om motorer ej är inkopplade */
    }
	
	touchSensor = sensor_search( LEGO_EV3_TOUCH ); // Registrerar en touch sensor på touchSensor-variabeln
	touch_set_mode_touch(touchSensor); // anger vilken "mode" sensorn skall ha

	us_sensor=sensor_search( LEGO_EV3_US );
	us_set_mode_us_dist_cm(us_sensor);
	int distance=sensor_get_value(0, us_sensor,0);
	printf("%d\n", distance);
	

	tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * 0.1 );  // Sätter hastigheten på båda motorerna till 50% av maxhastigheten
	/* Om man vill köra bakåt anger man negativ hastighet, till exempel max_hastighet * (-0.5) */
	
	
	printf("turning\n");
	
	tacho_set_position_sp (MOTOR_LEFT, 50);
	tacho_set_position_sp (MOTOR_RIGHT, -50);
	tacho_run_to_rel_pos (MOTOR_BOTH);
	Sleep(1000);
	
	tacho_set_position_sp (MOTOR_BOTH, 1000);
	tacho_run_to_rel_pos(MOTOR_BOTH);
	Sleep(1000);
	
	printf("något\n");
	distance=sensor_get_value(0, us_sensor,0);
	
	if(distance<600){
	printf("stopp!\n");
	tacho_set_position_sp (MOTOR_BOTH, 0);
	tacho_run_to_rel_pos(MOTOR_BOTH);
	}
	

	
	

	brick_uninit();
	printf( "dying...\n" );
    return ( 0 );

}
