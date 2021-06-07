#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

int receiver_pid;
int value_sent;
bool can_send = true;

void receiver_signal_handler(int nsig) {
	can_send = true;
}

void send_value() {
	int bits_count = sizeof(int) * 8;
	for (int i = 0; i < bits_count; ++i) {
		while (!can_send);

		if ((value_sent & (1 << i)) != 0) {
			kill(receiver_pid, SIGUSR1);
		}
		else {
			kill(receiver_pid, SIGUSR2);
		}
		can_send = false;
	}

	kill(receiver_pid, SIGCHLD);
}

int main() {
	// Get the signal from receiver.
	signal(SIGUSR1, receiver_signal_handler);
	printf("Sender PID : %d\n", (int)getpid());

	// Read the PID.
	printf("Enter the PID for receiver:\n");
	if (scanf("%d", &receiver_pid) < 0) {
		printf("An error occurred while reading receiver PID.\n");
		exit(-1);
	}

	printf("Enter a valid integer:\n");
	while (scanf("%d", &value_sent) < 1) {
		printf("Incorrect input, shutting down\n");		
		exit(-1);
	}

	send_value();
	return 0;
}