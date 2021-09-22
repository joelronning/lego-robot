#include <stdio.h>
#include "brick.h"
#include <unistd.h>

#define sleep( msec ) usleep(( msec ) * 1000 ) /* Definerar sleep, sleep(1000)= 1 sekund */

#define MOTOR_RIGHT    	OUTA
#define MOTOR_LEFT    	OUTB
#define MOTOR_C    		OUTC
#define MOTOR_D    		OUTD
#define SENSOR_TOUCH	IN1
#define SENSOR_2		IN2
#define SENSOR_3		IN3
#define SENSOR_4		IN4

#define MOTOR_BOTH     	( MOTOR_LEFT | MOTOR_RIGHT ) /* Bitvis ELLER ger att båda motorerna styrs samtidigt */

int max_hastighet, distance;         /* variabel för max hastighet på motorn */
POOL_T touchSensor, us_sensor, gyro_sensor;

int main( void )
{  
	
	if ( !brick_init()) return ( 1 ); /* Initialiserar EV3-klossen */
	printf( "*** ( EV3 ) Hello! ***\n" );
	sleep( 2000 );
	
        max_hastighet = tacho_get_max_speed( MOTOR_LEFT, 0 );
	distance=sensor_get_value(0, us_sensor,0);
	
	touchSensor = sensor_search( LEGO_EV3_TOUCH ); // Registrerar en touch sensor på touchSensor-variabeln
	touch_set_mode_touch(touchSensor); // anger vilken "mode" sensorn skall ha
	
	us_sensor=sensor_search( LEGO_EV3_US );
	us_set_mode_us_dist_cm(us_sensor);

	gyro_sensor = sensor_search(LEGO_EV3_GYRO);
	gyro_set_mode_gyro_ang(gyro_sensor);


	tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * 0.2 );  // Sätter hastigheten på båda motorerna till x% av maxhastigheten
	tacho_set_ramp_up_sp(MOTOR_BOTH, 500); //Accelerar upp över 500 ms
	tacho_set_ramp_down_sp(MOTOR_BOTH, 500); //Accelerar ner över 500 ms
	
	while(distance > 500)
	{
		tacho_set_position_sp (MOTOR_BOTH, 100);
		tacho_run_to_rel_pos (MOTOR_BOTH);
		distance = sensor_get_value(0, us_sensor, 0);
		printf("distance:%d\n", distance);
		Sleep(100);
	}

	printf("turning\n");
	tacho_set_position_sp (MOTOR_LEFT, 150);
	tacho_set_position_sp (MOTOR_RIGHT, -150);
	tacho_run_to_rel_pos (MOTOR_BOTH);
	sleep(2000)
	
	printf("forward\n");
	tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * 0.4);
	tacho_set_position_sp (MOTOR_BOTH, 1920);
	tacho_run_to_rel_pos (MOTOR_BOTH);
	sleep(5000);
	
	printf("turning\n");
	tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * 0.2);
	tacho_set_position_sp (MOTOR_LEFT, 150);
	tacho_set_position_sp (MOTOR_RIGHT, -150);
	tacho_run_to_rel_pos (MOTOR_BOTH);
	sleep(2000)

	while(!sensor_get_value(0, touchSensor, 0)); //Så länge touch-sensorn inte är intryckt kommer while-loopen köras
	
	
	

	brick_uninit();
	printf( "dying...\n" );
    return ( 0 );

}
