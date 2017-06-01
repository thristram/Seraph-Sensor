//*************************************************************
#ifndef  COM4004_H
#define  COM4004_H
//*************************************************************

#ifndef  _COM4004_GLOBAL
#define  COM4004_EXT  extern 
#else
#define  COM4004_EXT 
#endif
#include "sys.h"
#include "type.h"

#define MAX              255            //随机数最大值
#define MIN              1              //随机数最小值
#define MAX_BUF_LEN			 1024
typedef struct
{
	union 
	{
		struct
		{
		u8 message_type	:4;
		u8 dup_flag			:1;
		u8 qos_level		:2;
		u8 message_source:1;
		}first_ch_union;
		u8 first_ch_byte;
	}ch;
	u8 r_length[4];
	u8 r_length_len;//r_length[]数组实际有值的个数
}Fixheader;

typedef struct
{
	u8 version;
	u8 topic_lengthH;
	u8 topic_lengthL;
	u8 topic[30];
	u8 message_id_H;
	u8 message_id_L;
	u8 ext_message_id;
}Variableheader;

typedef enum 
{
	RESERCED,
	CONNETC,
	GET,
	POST,
	PUT,
	DELETE,
	OTA,
}_REQUEST_;

typedef enum 
{
	CONTINUE,
	SWITCHING_PROTOCOLS,
	OK,
	CREATED,
	ACCEPTED,
	PATRICAL_CONTENT,
	BAD_REQUEST,
	UNAUTHORIZED,
	FORBIDDEN,
	NOT_FOUND,
	REQUEST_TIMEOUT,
	INTERNAL_SERVER_ERROR,
	SERVICE_UNAVAILABLE,
	VERSION_NOT_SUPPORTED,
}_REPLY_;


typedef struct
{
	Fixheader 			tx_fix_header;
	Variableheader 	tx_var_header;	
	char 						*tx_payload;
}Txmessage;

typedef struct
{
	Fixheader 			rx_fix_header;
	Variableheader 	rx_var_header;	
	u8 							rx_payload[500];
}Rxmessage;

typedef struct
{
	int erase;
	int duration;
}DM;
typedef struct
{
	int type;
	char *code;
	char *address;
	char *other;
	char *raw;
}UR;

typedef struct
{
	int  seqid;
	char *sepid;
	int  ch;
	char *action;
	char *topos;
	DM   option;
	int  stseq;
	int  timeout;
}SL_AP;//SL action/persorm data struct
typedef struct
{
	int  seqid;
	char *sepid;
	int  ch;
	char *action;
	char *topos;
	int  stseq;
	int  timeout;
}SC_AP;//SC action/perform data struct




COM4004_EXT Txmessage Txto4004;
COM4004_EXT Rxmessage Rxfr4004;

COM4004_EXT union  FLAG 		UART1Flag1_;
#define UART1Flag 					UART1Flag1_._flag_byte
#define rev_success					UART1Flag1_._flag_bit.bit0	//用于接收数据分析
#define rev_success2				UART1Flag1_._flag_bit.bit1	//用于接收回复

COM4004_EXT u8 send_buf[100];
COM4004_EXT u8 rev_buf[MAX_BUF_LEN];
COM4004_EXT u8 ssp_buf[MAX_BUF_LEN];
COM4004_EXT u16 ssp_length;


COM4004_EXT u8 Check_Sum(uint8_t *buf,uint8_t length);
COM4004_EXT u8 random(uint8_t xxx);
COM4004_EXT void UART2_Send_Data_Init(void);
COM4004_EXT void UART2_Send_Data_Start(void);
COM4004_EXT int test_cjson(void);
COM4004_EXT int mystrcmp(unsigned char *str1,const unsigned char *str2);
COM4004_EXT void rev_analyze(u8 *topic_buf,u8 *cjson_buf);
COM4004_EXT u32 calc_length(u8 *buf);
COM4004_EXT void mystrncat(char *desc,char *src,u32 len);
COM4004_EXT u8 ssp_parse(u8 *buf,u16 buf_len);
COM4004_EXT void deal_action_perform(u8 *buf);
COM4004_EXT void deal_config_ss(u8 *buf);
COM4004_EXT void analyze(void);
COM4004_EXT void success_receipt(void);
COM4004_EXT void send_message_without_payload(u8 fix1,u8 version,u8 message_id_h,u8 message_id_l,u8 ex_message_id);
COM4004_EXT void send_message_with_payload(u8 fix1,u8 version,u8 message_id_h,u8 message_id_l,u8 *payload);
COM4004_EXT void send_message(Txmessage *tx);
COM4004_EXT void init_send_Txmessage(_REPLY_ type);


#endif


