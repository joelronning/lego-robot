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
POOL_T touchSensor, us_sensor, gyro_sensor;

void rotate(int degrees);
void forward(int degrees);
void reverse(int degrees);
int turn_left(int degrees, int resolution);
int turn_right(int degrees, int resultion);

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

	gyro_sensor = sensor_search(LEGO_EV3_GYRO);
	gyro_set_mode_gyro_ang(gyro_sensor);

	int distance;

	tacho_set_speed_sp( MOTOR_BOTH, max_hastighet * 0.2 );  // Sätter hastigheten på båda motorerna till 50% av maxhastigheten
	/* Om man vill köra bakåt anger man negativ hastighet, till exempel max_hastighet * (-0.5) */
	
	distance=sensor_get_value(0, us_sensor,0);
/*
	while(distance > 500)
	{
		rotate(10);
		distance = sensor_get_value(0, us_sensor, 0);
		Sleep(150);
	}

	Sleep(500);

	while(distance > 300)
	{
		reverse(10);
		distance = sensor_get_value(0, us_sensor, 0);
		Sleep(150);
	}

	Sleep(800);

	rotate(150);

	Sleep(800);

	tacho_set_speed_sp(MOTOR_BOTH, max_hastighet * 0.4);
	reverse(4800);

	Sleep(4000);

	tacho_set_speed_sp(MOTOR_BOTH, max_hastighet * 0.2);
	rotate(-150);

	Sleep(2000);
*/	
	/*if(distance<600){
	printf("stopp!\n");
	tacho_set_position_sp (MOTOR_BOTH, 0);
	tacho_run_to_rel_pos(MOTOR_BOTH);
	}*/

	/*int gyro_val1, gyro_val2;

	while(!sensor_get_value(0, touchSensor, 0))
	{
		gyro_val1 = sensor_get_value(0, gyro_sensor, 0);	
		gyro_val2 = sensor_get_value(1, gyro_sensor, 0);	
		printf("Gyro, value 1: %d, value2: %d\n", gyro_val1, gyro_val2);
		Sleep(200);
	}*/

	int oversteer = turn_left(80, 10);
	turn_left(10 - oversteer, 1);

	tacho_set_position_sp (MOTOR_BOTH, 0);
	tacho_run_to_rel_pos(MOTOR_BOTH);

	printf("Leave book!\n");

	brick_uninit();
	printf( "dying...\n" );
    return ( 0 );

}


void rotate(int degree)
{
	tacho_set_position_sp(MOTOR_LEFT, degree);
	tacho_set_position_sp(MOTOR_RIGHT, degree * -1);
	tacho_run_to_rel_pos(MOTOR_BOTH);
}

void forward(int degree)
{
	tacho_set_position_sp(MOTOR_BOTH, degree);
	tacho_run_to_rel_pos(MOTOR_BOTH);
}


void reverse(int degree)
{
	tacho_set_position_sp(MOTOR_BOTH, -degree);
	tacho_run_to_rel_pos(MOTOR_BOTH);
}

int turn_left(int degrees, int resolution)
{
	rotate(-1);
	int angle = sensor_get_value(0, gyro_sensor, 0);
	int new_angle = angle - degrees;
	int diff;

	printf("\n%d", new_angle);
	
	while(angle > new_angle)
	{
		rotate(resolution);
		angle = sensor_get_value(0, gyro_sensor, 0);
		printf("angle: %d", angle);
		Sleep(100);
	}

	Sleep(100);

	diff = angle * -1 - degrees;

	return diff;
}

int turn_right(int degrees, int resolution)
{
	rotate(1);
	int angle = sensor_get_value(0, gyro_sensor, 0);
	int new_angle = angle + degrees;
	int diff;
	
	printf("\n%d", new_angle);

	while(angle < new_angle)
	{
		rotate(resolution * -1);
		angle = sensor_get_value(0, gyro_sensor, 0);
		printf("angle: %d", angle);
		Sleep(100);
	}

	Sleep(100);

	diff = angle - degrees;

	return diff;
}
