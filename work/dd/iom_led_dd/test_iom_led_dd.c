/* filename: test_iom_led_dd.c
 * Test program for LED device driver
 *
 */

#include <stdio.h>
#include <unistd.h> //open()
#include <fcntl.h> // O_WRONLU()

#define DEVNAME "/dev/iom_led"
#define ON 1
#define OFF 0

int main(int argc, char* argv[]){


	int fd = -1;
	unsigned char led_signali = OFF;

	fd = open(DEV_NAME, O_WRONLU);
	if(fd < 0){
		printf("Device open error\n");
		return -1;
	}

	while(1){
		//toggle led depends on current led state
		led_signal = led_signal ? OFF : ON; //toggle
		write(fd, &led_signal, sizeof(led_signal));
		sleep(1);
	}

	close(fd);
	return 0;

}
