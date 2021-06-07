#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

int sender_pid;
int current_bit_index = 0;
int result_number = 0;
bool received_value = false;

// All handlers should notify the sender.
void sigusr1_handler_bit(int nsig) {
	result_number |= (1 << current_bit_index++);
	kill(sender_pid, SIGUSR1);
}

void sigusr2_handler_nobit(int nsig) {
	current_bit_index++;
	kill(sender_pid, SIGUSR1);
}

void sigchld_handler(int nsig) {
	received_value = true;
}

int main() {
	printf("Reciever PID: %d\n", (int)getpid());

	printf("Enter the PID for sender:\n");
	// Read sender program PID from console.
	if (scanf("%d", &sender_pid) < 0) {
		printf("An error occurred while reading sender PID.\n");
		exit(-1);
	}

	printf("Reciever: waiting for message\n");

	
	signal(SIGUSR1, sigusr1_handler_bit);
	signal(SIGUSR2, sigusr2_handler_nobit);
	signal(SIGCHLD, sigchld_handler);

	
	while (!received_value);

	printf("Reciever: received value: %d\n", result_number);
	return 0;
}