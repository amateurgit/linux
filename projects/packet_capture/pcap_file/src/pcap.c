/*
   读 PCAP 文件, 理解 PCAP 数据结构
 */
#include <stdio.h>
#include <stdlib.h>

#define PCAP_FILE "./a.pcap"
#define MAGIC (0xa1b2c3d4)

#define BUFFSZ (0xffff)

typedef unsigned int guint32;
typedef unsigned short guint16;
typedef int gint32;

typedef struct pcap_hdr_s {
    guint32 magic_number;       /* 字节序 */
    guint16 version_major;
    guint16 version_minor;
    gint32  thiszone;
    guint32 sigfigs;
    guint32 snaplen;            /* 最大存储长度 */
    guint32 network;            /* 链路类型 */
} pcap_hdr_t;

typedef struct pcaprec_hdr_s {
        guint32 ts_sec;         /* timestamp seconds */
        guint32 ts_usec;        /* timestamp microseconds */
        guint32 incl_len;       /* 抓取到的数据包长度 */
        guint32 orig_len;       /* 原始数据包长度 */
} pcaprec_hdr_t;

static void print_endian(unsigned int num)
{
    printf("---------------------字节序-----------------------\n");
    if (num == MAGIC)
    {
        printf("little endian\n");
    }
    else
    {
        printf("big endian\n");
    }

    return;
}

static void print_pcap_file_header(pcap_hdr_t *hdr)
{
    printf("--------------------文件头------------------------\n");
    printf("magic_number = 0x%x%x%x%x\n",
            (hdr->magic_number & 0xff000000) >> 24,
            (hdr->magic_number & 0xff0000) >> 16,
            (hdr->magic_number & 0xff00) >> 8,
            hdr->magic_number & 0xff);

    printf("version_major = %d\n", hdr->version_major);
    printf("version_minor = %d\n", hdr->version_minor);
    printf("thiszone = %d\n", hdr->thiszone);
    printf("sigfigs = %d\n", hdr->sigfigs);
    printf("snaplen = %d\n", hdr->snaplen);
    printf("network = %d\n", hdr->network);

    return;
}

static void print_pcap_packet_header(pcaprec_hdr_t *hdr, int n)
{
    printf("-------------------- [%d] Pcap Packet Header------------------------\n", n);
    printf("ts_sec = %d\n", hdr->ts_sec);
    printf("ts_usec = %d\n", hdr->ts_usec);
    printf("incl_len = %d\n", hdr->incl_len);
    printf("orig_len = %d\n", hdr->orig_len);

    return;
}

int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    pcap_hdr_t pcap_file_header;

    pcaprec_hdr_t pcap_packet_header;
    char buff[BUFFSZ] = { 0 };

    int total = 0;

    fp = fopen(PCAP_FILE, "rb");
    if (!fp)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    if (1 != fread(&pcap_file_header, sizeof(pcap_file_header), 1, fp))
    {
        perror("fread");

        fclose(fp);
        exit(EXIT_FAILURE);
    }

    /* 打印文件头 */
    print_pcap_file_header(&pcap_file_header);

    /* 打印字节序 */
    print_endian(pcap_file_header.magic_number);

    while (1)
    {
        if (1 != fread(&pcap_packet_header, sizeof(pcap_packet_header), 1, fp))
        {
            if (feof(fp))
            {
                break;
            }
            else
            {
                perror("fread");

                fclose(fp);
                exit(EXIT_FAILURE);
            }
        }

        total++;

        fseek(fp, pcap_packet_header.incl_len, SEEK_CUR);

        /* 打印 Packet Header */
        print_pcap_packet_header(&pcap_packet_header, total);
    }

    fclose(fp);

    printf("-------------------文件包含的数据包个数------------------\n");
    printf("%d\n", total);

    exit(EXIT_SUCCESS);
}
