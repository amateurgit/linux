CC=gcc
INC=/usr/local/include/json-c/
LIB=json-c
LIB_PATH=/usr/local/lib/

json:json.c
	$(CC) -o $@ $< -I$(INC) -L$(LIB_PATH) -l$(LIB)

.PHONY clean:
	-rm -f json
