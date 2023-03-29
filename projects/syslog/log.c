#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "log.h"

typedef enum 
{
    X_DATE = 0,
    X_TIME,
    X_DEV_NAME,
    X_DEV_ID,
    X_LOG_ID,
    X_TYPE,
    X_SUBTYPE,
    X_PRI,
    X_SEVERITY,
    X_USER,
    X_UI,
    X_VD,
    X_ICP_EVENT_NAME,
    X_ICP_DEVICE_ID,
    X_ICP_FUNC_ID,
    X_SRC,
    X_IP,
    X_ICP_SERVIP,
    X_SRC_PORT,
    X_ICP_SERVPORT,
    X_PORT,
    X_DST,
    X_ICP_VISIP,
    X_DST_PORT,
    X_ICP_VIPORT,
    X_PROTO,
    X_SERVICE,
    X_ACTION,
    X_STATUS,
    X_DURATION,
    X_SENT,
    X_RCVD,
    X_SENT_PKT,
    X_RCVD_PKT,
    X_ATTACK_NAME,
    X_FUNC_ID,
    X_APP_LIST,
    X_APP,
    X_APP_TYPE,
    X_HOST_NAME,
    X_RULE_NAME,
    X_PROFILE,
    X_BANWORD,
    X_URL,
    X_DIR,
    X_FILE,
    X_FILE_TYPE,
    X_VIRUS,
    X_COMMAND,
    X_OS,
    X_NAME,
    X_POLICY,
    X_MSG,
    X_POLICY_SEQ,
    X_POLICY_ACT,
    X_POLICY_SVR,
    X_POLICY_SADDR,
    X_POLICY_DADDR,
    X_SYS_CPU,
    X_SYS_MEM,
    X_REASON,

    X_MAX_TYPE
} key_index_t;


static char *key_str[X_MAX_TYPE] = 
{
    "date",
    "time",
    "devname",
    "device_id",
    "log_id",
    "type",
    "subtype",
    "pri",
    "severity",
    "user",
    "ui",
    "vd",
    "icp_eventname",
    "icp_devid",
    "icp_funcid",
    "src",
    "ip",
    "icp_servip",
    "src_port",
    "icp_servport",
    "port",
    "dst",
    "icp_visip",
    "dst_port",
    "icp_viport",
    "proto",
    "service",
    "action",
    "status",
    "duration",
    "sent",
    "rcvd",
    "sent_pkt",
    "rcvd_pkt",
    "attack_name",
    "func_id",
    "app_list",
    "app",
    "app_type",
    "hostname",
    "rulename",
    "profile",
    "banword",
    "url",
    "dir",
    "file",
    "filetype",
    "virus",
    "command",
    "os",
    "name",
    "policyid",
    "msg",
    "seq",
    "act",
    "svr",
    "saddr",
    "daddr",
    "cpu",
    "mem",
    "reason"
};

enum 
{
    INIT = 0,
    NOP,
    KEY,
    VAL,
};

key_index_t key_index(const char *origin, int pos, int offset)
{
    const char *key = origin + pos;

    for (int i = 0; i < X_MAX_TYPE; i++ )
    {
	if (offset == strlen(key_str[i]) && 
		!strncasecmp(key_str[i], key, offset))
	{
	    return i;
	}
    }

    /* 未找到 */
    return -1;
}

int parse_attr(syslog_attr_t attr[], const char *origin, int len)
{
    printf("%s\n", origin);

    int i = 0;
    int state = INIT;
    int pos = 0, offset = 0;
    key_index_t index; 

    for ( i = 0; i < len; i++ )
    {
	switch(state)
	{
	    case INIT:
		if ('<' == origin[i])
		{
		    state = NOP;
		}
		break;

	    case NOP:
		if ('>' == origin[i])
		{
		    state = KEY;
		}
		break;

	    case KEY:
		if (!isspace(origin[i])) // 跳过空格
		{
		    if ('=' == origin[i])
		    {
			index = key_index(origin, pos, offset);
			state = VAL;
			pos = 0;
			offset = 0;
		    }
		    else
		    {
			if (!pos)
			{
			    pos = i;
			    offset = 1;
			}
			else
			{
			    offset++;
			}
		    }
		}
		break;

	    case VAL:
		if (isspace(origin[i]) || !origin[i]) // 空格或结尾
		{
		    if (pos && (index != -1))
		    {
			attr[index].pos = pos;
			attr[index].len = offset;
		    }

		    state = KEY;
		    pos = 0;
		    offset = 0;
		}
		else
		{
		    if (!pos)
		    {
			pos = i;
			offset = 1;
		    }
		    else
		    {
			offset++;
		    }
		}
		break;
	}
    }

    return 0;
}

int main(int argc, char *argv[])
{
    const char *origin = "<190>date=2022-05-20 time=12:36:31 devname=APW1KMB011002495 device_id=APW1KMB001000001 log_id=32001 type=event subtype=admin pri=information vd=root user=\"admin\" ui=https(192.168.1.85) action=login status=success reason=none profile=\"super_admin\" msg=\"admin|https(192.168.1.85)\"";

    syslog_attr_t attr[X_MAX_TYPE] = { 0 };

    char buf[1024] = { 0 };

    /* 解析 */
    parse_attr(attr, origin, strlen(origin) + 1);
    for ( int i = 0; i < X_MAX_TYPE; i++ )
    {
	if (attr[i].len)
	{
	    snprintf(buf, attr[i].len + 1, "%s", origin + attr[i].pos);
	    printf("%s = %s\n", key_str[i], buf);
	}
    }

    /* 业务逻辑 */

    return 0;
}
