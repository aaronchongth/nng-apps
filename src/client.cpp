#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string>

#include "addressbook.pb.h"

#include <nng/nng.h>
#include <nng/protocol/reqrep0/req.h>
#include <nng/supplemental/util/platform.h>

void
fatal(const char *func, int rv)
{
	fprintf(stderr, "%s: %s\n", func, nng_strerror(rv));
	exit(1);
}

/*  The client runs just once, and then returns. */
int
client(const char *url, const char *msecstr)
{
	nng_socket sock;
	int        rv;
	nng_msg *  msg;
	nng_time   start;
	nng_time   end;
	unsigned   msec;

	msec = atoi(msecstr);

	if ((rv = nng_req0_open(&sock)) != 0) {
		fatal("nng_req0_open", rv);
	}

	if ((rv = nng_dial(sock, url, NULL, 0)) != 0) {
		fatal("nng_dial", rv);
	}

	start = nng_clock();

	if ((rv = nng_msg_alloc(&msg, 0)) != 0) {
		fatal("nng_msg_alloc", rv);
	}
	if ((rv = nng_msg_append_u32(msg, msec)) != 0) {
		fatal("nng_msg_append_u32", rv);
	}

	if ((rv = nng_sendmsg(sock, msg, 0)) != 0) {
		fatal("nng_send", rv);
	}

	if ((rv = nng_recvmsg(sock, &msg, 0)) != 0) {
		fatal("nng_recvmsg", rv);
	}
	end = nng_clock();
	nng_msg_free(msg);
	nng_close(sock);

	printf("Request took %u milliseconds.\n", (uint32_t)(end - start));
	return (0);
}

int
main(int argc, char **argv)
{
	tutorial::AddressBook address_book;
	tutorial::Person* person = address_book.add_people();
	person->set_name("Aaron");
	person->set_id(0);

	std::string serialized_address_book;
	if (!address_book.SerializeToString(&serialized_address_book))
	{
		printf("Unable to serialize address book\n");
		return 1;
	}

	tutorial::AddressBook deserialized_address_book;
	if (!deserialized_address_book.ParseFromString(serialized_address_book))
	{
		printf("Unable to deserialize address book\n");
		return 1;
	}

	if (deserialized_address_book.people_size() != 1 ||
			!deserialized_address_book.people(0).has_name() ||
			deserialized_address_book.people(0).name() != std::string("Aaron"))
	{
		printf("Some values are wrong\n");
		return 1;
	}

	// int rc;

	// if (argc != 3) {
	// 	fprintf(stderr, "Usage: %s <url> <secs>\n", argv[0]);
	// 	exit(EXIT_FAILURE);
	// }
	// rc = client(argv[1], argv[2]);
	// exit(rc == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
	return 0;
}
