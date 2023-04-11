/* gcc -o pcap libpcap.c -lpcap */
#include <stdio.h>
#include <pcap.h>
#include <time.h>

//回调函数
void pcap_callback(unsigned char * arg,const struct pcap_pkthdr *packet_header,const unsigned char *packet_content)
{
  //  int *id = (int *)arg;//记录包ID
  //  printf("id = %d\n", ++(*id));

    static int id = 1;
    printf("id = %d\n", id++);

    pcap_dump(arg, packet_header, packet_content); /* 写文件 */

    printf("Packet length : %d\n",packet_header->len);
    printf("Number of bytes : %d\n",packet_header->caplen);
    printf("Received time : %s\n",ctime((const time_t*)&packet_header->ts.tv_sec));
    int i;
    for(i = 0; i < packet_header->caplen; i++)
    {
	printf(" %02x",packet_content[i]);
	if((i + 1) % 16 == 0)
	{
	    printf("\n");
	}
    }
    printf("\n\n");
}

int main(int argc, char *argv[])
{
    char *dev = NULL;
    char errbuf[1024] = { 0 };

    /* Step 0 : 指定网络设备名 */
    if (2 == argc && argv[1][0])
    {
	dev = argv[1];
    }
    else
    {
	dev = pcap_lookupdev(errbuf);
    }

    if (NULL == dev)
    {
	printf("no device\n");

	return -1;
    }
    printf("Device : %s\n", dev);


    /* Step 1 : 网络设备 */
    /*
     * 网络设备
     * 每次抓取字节数
     * 混杂模式         1, 混杂模式; 0, 非混杂模式
     * 抓取时间         0, 一直等待
     */
    pcap_t *pcap_handle = pcap_open_live(dev, 65535, 1, 0, errbuf);
    if (NULL == pcap_handle)
    {
	printf("%s\n", errbuf);

	return -1;
    }

#if 0
    /* 获取 ip 所在网段 与 掩码 */
    struct in_addr addr;
    bpf_u_int32 ipaddress, ipmask;
    char *dev_ip,*dev_mask;

    if (-1 == pcap_lookupnet(dev, &ipaddress, &ipmask, errbuf))
    {
	printf("%s\n", errbuf);

	return -1;
    }

    //输出ip
    addr.s_addr = ipaddress;
    dev_ip = inet_ntoa(addr);
    printf("ip 网段 : %s\n",dev_ip);

    //输出掩码
    addr.s_addr = ipmask;
    dev_mask = inet_ntoa(addr);
    printf("netmask : %s\n",dev_mask);
#endif


    /* Step 2 : 过滤条件 */
    struct bpf_program filter;
    /**
     * 参数 1 :
     * 参数 2 : 存放规则编译结果
     * 参数 3 : 过滤规则
     * 参数 4 : 优化规则表达式
     * 参数 5 : 指定本地网络的网络掩码, 不需要时写 0
     **/
    pcap_compile(pcap_handle, &filter, "src host 192.168.1.85", 1, 0);
    /* 应用规则 */
    pcap_setfilter(pcap_handle, &filter);

    /* 规则举例 : 
       src host 192.168.1.177 : 只接收源ip地址是192.168.1.177的数据包

       dst port 80 : 只接收tcp、udp的目的端口是80的数据包

       not tcp : 只接收不使用tcp协议的数据包

       tcp[13] == 0x02 and (dst port 22 or dst port 23) : 只接收 SYN 标志位置位且目标端口是 22 或 23 的数据包（ tcp 首部开始的第 13 个字节）

       icmp[icmptype] == icmp-echoreply or icmp[icmptype] == icmp-echo :只接收 icmp 的 ping 请求和 ping 响应的数据包

       ehter dst 00:e0:09:c1:0e:82 : 只接收以太网 mac 地址是 00:e0:09:c1:0e:82 的数据包

       ip[8] == 5 : 只接收 ip 的 ttl=5 的数据包（ip首部开始的第8个字节）
     */


    /* Step 3 : 抓取数据包 */
    printf("----------packet------------\n");
#if 0   /* 方式一 */
    const char *p_packet_content;
    struct pcap_pkthdr protocol_header;
    /*
     * 抓取一个数据包立即返回, 返回值是接受到的数据的起始地址
     */
    p_packet_content = pcap_next(pcap_handle, &protocol_header);
    printf("capture time : %s", ctime((const time_t *)&protocol_header.ts.tv_sec));
    printf("packet length : %d, ",protocol_header.len); /* 数据包应有长度 */
    printf("%d\n", protocol_header.caplen); /* 实际抓取到的长度 */
#else   /* 方式二 */
//   int id = 0;

    /* 抓包写磁盘 */
    pcap_dumper_t *dumpfp = pcap_dump_open(pcap_handle, "/tmp/a.pcap");

    /**
     * 参数 1 : 
     * 参数 2 : 抓包数量
     * 参数 3 : 回调函数, 数据包处理
     * 参数 4 : 回调参数
     **/
    if (pcap_loop(pcap_handle, 10, pcap_callback, (unsigned char *)dumpfp))
    {
	    printf("pcap_loop error\n");
	    return -1;
    }

    pcap_dump_close(dumpfp);
#endif

    pcap_close(pcap_handle);

    return 0;
}
