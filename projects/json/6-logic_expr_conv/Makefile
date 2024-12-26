CC=gcc
INC=/usr/local/include/json-c/
LIB=json-c
LIB_PATH=/usr/local/lib/

logic_expr_conv:logic_expr_conv.c
	$(CC) -o $@ $< -I$(INC) -L$(LIB_PATH) -l$(LIB)

.PHONY clean:
	-rm -f logic_expr_conv
