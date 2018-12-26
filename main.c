#include <libinput.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define LEFT_SWIPE "xdotool key alt+ctrl+Up"
#define RIGHT_SWIPE "xdotool key alt+ctrl+Down"
#define DISPLAY_ALL "xdotool key super"
#define abs(x) (x < 0 ? -x : x)


void handle_four_fingers(struct libinput_event_gesture *ev, struct libinput *li);

int scan_device (int ev) {
	// check the access permission
	if (access("/proc/bus/input/devices", R_OK) != 0) {
		printf("permission denied\n");
		exit(-1);
	}

	FILE *f = fopen("/proc/bus/input/devices", "r");
	char s[1024];
	char *pos = NULL;
	int x, y;
	while (fgets(s, 1024, f) != NULL) {
		if ( s[0] == 'H' ) {
			pos = strstr(s, "event");
			sscanf(pos, "event%d", &x);
		} 
		else if ( s[0] == 'B' ) {
			sscanf(s+3, "EV=%x", &y);
			if ( y == ev ) {
				return x;
			}
		}
	}
	return -1;
}

int dispatch (struct libinput *li) {
	struct epoll_event ep[32];
	epoll_wait(libinput_get_fd(li), ep, 32, -1);
	libinput_dispatch(li);
}

int open_restricted (const char *path, int flags, void *user_data) {
	int fd = open(path, O_RDONLY | O_NONBLOCK);
	return fd;
}

int close_restricted (int fd, void *user_data) {
	close(fd);
}

void handle_gesture(struct libinput_event_gesture *ev, struct libinput *li) {
	int finger_count = libinput_event_gesture_get_finger_count(ev);
	switch ( finger_count ) {
		case 3:
			// handle_three_fingers(ev, li);
			fprintf(stderr, "Have not implemented 3 fingers yet...\n");			
			break;
		case 4:
			handle_four_fingers(ev, li);
			break;
	}
}

void handle_three_fingers(struct libinput_event_gesture *ev, struct libinput *li) {

}

void handle_four_fingers(struct libinput_event_gesture *ev, struct libinput *li) {
	ev = libinput_get_event(li);
	double dy = libinput_event_gesture_get_dy(ev);
	double dx = libinput_event_gesture_get_dx(ev);
	if ( abs(dx) > abs(dy) ) { // swipe left or right
		if ( dx < 0 ) { // swipe left
			system(LEFT_SWIPE);
		}
		else { // swipe right
			system(RIGHT_SWIPE);
		}
	}
	else { // swipe up or down
		if ( dy < 0 ) { // swipe down
			system(DISPLAY_ALL);		
		}
		else { // swipe up
			system(DISPLAY_ALL);
		}
	}
}

int main () {
	int dev = scan_device(0x1b);
	if (dev == -1) {
		exit(-1);
	}
	char dev_str[50];
	sprintf(dev_str, "/dev/input/event%d", dev);

	struct libinput_interface interface = {
		open_restricted,
		close_restricted
	};
	struct libinput *li = libinput_path_create_context(&interface, NULL);
	struct libinput_device *device = libinput_path_add_device(li, dev_str);
	if (device == NULL) {
		printf("get device failed, please check your permissions\n");
		return -1;
	} 
	struct libinput_event *ev;
	unsigned long last_left = 0, last_right = 0, t = 0;
	libinput_device_ref(device);
	libinput_ref(li);
	dispatch(li);
	while ( 1 ) {
		ev = libinput_get_event(li);
		if (ev == NULL) {
			libinput_event_destroy(ev);
			dispatch(li);
			continue;
		}
		switch (libinput_event_get_type(ev)) {
			case LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN:
				handle_gesture(ev, li);
				break;
			case LIBINPUT_EVENT_GESTURE_PINCH_BEGIN:
				break;
		}
		dispatch(li);
	}
}
