struct data {
	char file_data[1024];
	char file_name[1024];
};

program TRANSFER_PROG {
	version TRANSFER_VERS {
		int transfer(data) = 1;
	} = 1;
} = 0x23451111;
