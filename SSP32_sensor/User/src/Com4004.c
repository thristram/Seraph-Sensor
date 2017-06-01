
/* Includes ------------------------------------------------------------------*/
#define _COM4004_GLOBAL
#include "includes.h"
#include <stdlib.h>

/**
  * @brief  产生随机数 1~255
  * @param  None.
  * @retval None
  */
uint8_t random(uint8_t xxx)  
{  
  uint8_t value,iii;  
  for(iii=0;iii<xxx;iii++)  
  {  
    value = rand() % (MAX + 1- MIN) + MIN; //产生随机数1~255
	}  
  return value;  
} 

/**
  * @brief  check sum(cumulative sum)
  * @param  u8 *sendbuf,u8 length.
  * @retval u8 result
  */
uint8_t Check_Sum(uint8_t *buf,uint8_t length)
{
	uint8_t i;
	uint8_t result = *buf++;
	for(i = 1;i < length;i++)
	{
		result ^= *buf++;
	}
	return result;
}



/* Parse text to JSON, then render back to text, and print! */
void doit(char *text)
{
	char *out;cJSON *json;
	
	json=cJSON_Parse(text);
	if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
	else
	{
		out=cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n",out);
		free(out);
	}
}

/* Read a file, parse, render back, etc. */
void dofile(char *filename)
{

	long len;
	char *data;
	FILE *f=fopen(filename,"rb");
	fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	data=(char*)malloc(len+1);
	
	fread(data,1,len,f);fclose(f);
	doit(data);
	free(data);
}

/* Used by some code below as an example datatype. */
struct record {const char *precision;double lat,lon;const char *address,*city,*state,*zip,*country; };
int test_cjson(void)
{
	//	const char *strings[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
//	int ids[4]={116,943,234,38793};
	struct record fields[2]={
		{"zip",37.7668,-1.223959e+2,"","SAN FRANCISCO","CA","94107","US"},
		{"zip",37.371991,-1.22026e+2,"","SUNNYVALE","CA","94085","US"}};
//	int numbers[3][3]={{0,-1,0},{1,0,0},{0,0,1}};	
	
	cJSON *root,*fmt,*img,*thm,*fld;char *out;int i;	/* declare a few. */
	cJSON *root2,*fmt2;char *out2;
//	char text1[]="{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\":       \"rect\", \n\"width\":      1920, \n\"height\":     1080, \n\"interlace\":  false,\"frame rate\": 24\n}\n}";	
//	char text2[]="[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
//	char text3[]="[\n    [0, -1, 0],\n    [1, 0, 0],\n    [0, 0, 1]\n	]\n";
//	char text4[]="{\n		\"Image\": {\n			\"Width\":  800,\n			\"Height\": 600,\n			\"Title\":  \"View from 15th Floor\",\n			\"Thumbnail\": {\n				\"Url\":    \"http:/*www.example.com/image/481989943\",\n				\"Height\": 125,\n				\"Width\":  \"100\"\n			},\n			\"IDs\": [116, 943, 234, 38793]\n		}\n	}";
//	char text5[]="[\n	 {\n	 \"precision\": \"zip\",\n	 \"Latitude\":  37.7668,\n	 \"Longitude\": -122.3959,\n	 \"Address\":   \"\",\n	 \"City\":      \"SAN FRANCISCO\",\n	 \"State\":     \"CA\",\n	 \"Zip\":       \"94107\",\n	 \"Country\":   \"US\"\n	 },\n	 {\n	 \"precision\": \"zip\",\n	 \"Latitude\":  37.371991,\n	 \"Longitude\": -122.026020,\n	 \"Address\":   \"\",\n	 \"City\":      \"SUNNYVALE\",\n	 \"State\":     \"CA\",\n	 \"Zip\":       \"94085\",\n	 \"Country\":   \"US\"\n	 }\n	 ]";
	mem_init();
	
	printf("\r\n this is a USART Interrupt demo \r\n");

	printf("\r\n***************串口实验--接受中断实验****************\r\n");
	printf("\r\n描述:使用超级终端来测试比较好,在超级终端敲入一系列字符，\r\n 单片机接收到自己之后直接将接收到的字符按原样打印出来\r\n");
	printf("\r\n请开始输入字符串:\r\n");
		
	root=cJSON_CreateArray();
	for (i=0;i<2;i++)
	{
		cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
		cJSON_AddStringToObject(fld, "precision", fields[i].precision);
		cJSON_AddNumberToObject(fld, "Latitude", fields[i].lat);
		cJSON_AddNumberToObject(fld, "Longitude", fields[i].lon);
		cJSON_AddStringToObject(fld, "Address", fields[i].address);
		cJSON_AddStringToObject(fld, "City", fields[i].city);
		cJSON_AddStringToObject(fld, "State", fields[i].state);
		cJSON_AddStringToObject(fld, "Zip", fields[i].zip);
		cJSON_AddStringToObject(fld, "Country", fields[i].country);
	}
	
/*	cJSON_ReplaceItemInObject(cJSON_GetArrayItem(root,1),"City",cJSON_CreateIntArray(ids,4)); */
	
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);
	    
	root2=cJSON_CreateObject();       
	cJSON_AddItemToObject(root2, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));     
	cJSON_AddItemToObject(root2, "format", fmt2=cJSON_CreateObject());     
	cJSON_AddStringToObject(fmt2,"type",     "rect");     
	cJSON_AddNumberToObject(fmt2,"width",        1920);     
	cJSON_AddNumberToObject(fmt2,"height",       1080);     
	//cJSON_AddFalseToObject (fmt2,"interlace");     
	cJSON_AddNumberToObject(fmt2,"frame rate",   24); 
	out2 =cJSON_Print(root2);  
	printf("%s\n",out2);   
	cJSON_Delete(root2);  
	free(out2);
	return 0;
}
//计算接收长度
u32 calc_length(u8 *buf)
{
	u32 length;
	if((*buf & 0x80) == 0x00)//buf没有后续字节存在
	{
		length = *buf;
		return (length+4);
	}
	else//buf有后续字节存在 
	{
		if((*(buf+1) & 0x80) == 0x00)//buf+1没有后续字节存在
		{
			length = (*buf & 0x7F)*128 + *(buf+1);
			return (length+5);
		}
		else//buf+1有后续字节存在
		{
			if((*(buf+2) & 0x80) == 0x00)//buf+2没有后续字节存在
			{
				length = (*buf & 0x7F)*16384 + (*(buf+1)&0x7F)*128 + *(buf+2);
				return (length+6);
			}
			else//buf+2有后续字节存在
			{
				length = (*buf & 0x7F)*2097125 + (*(buf+1)&0x7F)*16384 + (*(buf+2)&0x7F)*128 + *(buf+3);
				return (length+7);
			}
		}
	}
}

void analyze(void)
{
	if (rev_success)
	{
		rev_success = 0;
		if(ssp_parse(ssp_buf,ssp_length))
		{
			rev_analyze(Rxfr4004.rx_var_header.topic,Rxfr4004.rx_payload);
		}
	}
}

u8 ssp_parse(u8 *buf,u16 buf_len)
{
	u8 *temp_buf;
	u32 useful_data_len;
	u8  remaining_len;
	u8  topic_len;
	u16 payload_len;
	temp_buf = buf;
	if((temp_buf[0] == 0xBB) && (temp_buf[1] == 0xBB))
	{
		Rxfr4004.rx_fix_header.ch.first_ch_byte = temp_buf[2];
		if((temp_buf[3] & 0x80) == 0x00)
		{
			remaining_len = 1;
			useful_data_len = temp_buf[3];
			if(useful_data_len != buf_len - 4)//接收到的有效数据长度与实际收到的数据不等，错误，不接收本包数据
			{
				return 0;
			}
		}
		else if((temp_buf[4] & 0x80) == 0x00)
		{
			remaining_len = 2;
			useful_data_len = (temp_buf[3]& 0x7F)*128 + temp_buf[4];
			if(useful_data_len != buf_len - 5)//接收到的有效数据长度与实际收到的数据不等，错误，不接收本包数据
			{
				return 0;
			}
		}
		else if((temp_buf[5] & 0x80) == 0x00)
		{
			remaining_len = 3;
			useful_data_len = (temp_buf[3]& 0x7F)*16384 + (temp_buf[4]& 0x7F)*128 + temp_buf[5];
			if(useful_data_len != buf_len - 6)//接收到的有效数据长度与实际收到的数据不等，错误，不接收本包数据
			{
				return 0;
			}
		}
		else if((temp_buf[6] & 0x80) == 0x00)
		{
			remaining_len = 4;
			useful_data_len = (temp_buf[3]& 0x7F)*2097126 + (temp_buf[4]& 0x7F)*16384 + (temp_buf[5]& 0x7F)*128 + temp_buf[6];
			if(useful_data_len != buf_len - 7)//接收到的有效数据长度与实际收到的数据不等，错误，不接收本包数据
			{
				return 0;
			}
		}
		if(remaining_len)
		{
			Rxfr4004.rx_var_header.version = temp_buf[remaining_len+3];
			Rxfr4004.rx_var_header.topic_lengthH = temp_buf[remaining_len+4];
			topic_len = Rxfr4004.rx_var_header.topic_lengthL = temp_buf[remaining_len+5];
			payload_len = useful_data_len - 5 - topic_len;
			mymemcpy(Rxfr4004.rx_var_header.topic,(temp_buf+remaining_len+6),topic_len);
			Rxfr4004.rx_var_header.message_id_H = temp_buf[remaining_len+5+topic_len+1];
			Rxfr4004.rx_var_header.message_id_L = temp_buf[remaining_len+5+topic_len+2];
			mymemcpy(Rxfr4004.rx_payload,(temp_buf+remaining_len+8+topic_len),payload_len);
			return 1;
		}
		else
		{
			return 0;
		}
		
	}
	else
	{
		return 0;
	}

}

int mystrcmp(unsigned char *str1,const unsigned char *str2)
{
	int ret=0;
	while(!(ret=(*(unsigned char *)str2 - *(unsigned char *)str1)&&*str2))
	{
		str1++;
		str2++;
	}
	if(ret<0)
	ret=-1;
	else if(ret>0)
	ret=1;
	return ret;
}

void mystrncat(char *desc,char *src,u32 len)
{
	while(len--)
	{
		*desc++ = *src++;
	}
}


void rev_analyze(u8 *topic_buf,u8 *cjson_buf)
{
	u8 *temp_topicbuf,*temp_cjsonbuf;
	temp_topicbuf = topic_buf;
	temp_cjsonbuf	= cjson_buf;
	if (strncmp(temp_topicbuf,"/action/perform",Rxfr4004.rx_var_header.topic_lengthL) == 0)
	{
		deal_action_perform(temp_cjsonbuf);
		
	}
	else if (strncmp(temp_topicbuf,"/config/ss",Rxfr4004.rx_var_header.topic_lengthL) == 0)
	{
		deal_config_ss(temp_cjsonbuf);
		//success_receipt();
	}
	
}

void deal_config_ss(u8 *buf)
{
	cJSON *root,*tmp,*level1,*level2;
	root = cJSON_Parse((char*)buf);
	if (root)
	{
		level1 = cJSON_GetObjectItem(root,"system");
		if(level1)
		{
			tmp = cJSON_GetObjectItem(level1,"HPST");
			if(tmp)
			{
				printf("HPST: %d\n",tmp->valueint);
			}
			tmp = cJSON_GetObjectItem(level1,"REFR");
			if(tmp)
			{
				printf("REFR: %d\n",tmp->valueint);
			}
		}
		level1 = cJSON_GetObjectItem(root,"wifi");
		if(level1)
		{
			tmp = cJSON_GetObjectItem(level1,"ssid");
			if(tmp && tmp->valuestring)
			{
				printf("ssid: %s\n",tmp->valuestring);
			}
			tmp = cJSON_GetObjectItem(level1,"passwd");
			if(tmp && tmp->valuestring)
			{
				printf("passwd: %s\n",tmp->valuestring);
			}
		}
		tmp = cJSON_GetObjectItem(root,"time");
		if(tmp)
		{
			printf("time: %d\n",tmp->valueint);
		}
		level1 = cJSON_GetObjectItem(root,"location");
		if(level1)
		{
			tmp = cJSON_GetObjectItem(level1,"city");
			if(tmp && tmp->valuestring)
			{
				printf("city: %s\n",tmp->valuestring);
			}
			tmp = cJSON_GetObjectItem(level1,"country");
			if(tmp && tmp->valuestring)
			{
				printf("country: %s\n",tmp->valuestring);
			}
			tmp = cJSON_GetObjectItem(level1,"region");
			if(tmp && tmp->valuestring)
			{
				printf("region: %s\n",tmp->valuestring);
			}
		}
		level1 = cJSON_GetObjectItem(root,"weather");
		if(level1)
		{
			level2 = cJSON_GetObjectItem(level1,"astronomy");
			if (level2)
			{
				tmp = cJSON_GetObjectItem(level2,"sunrise");
				if(tmp && tmp->valuestring)
				{
					printf("sunrise: %s\n",tmp->valuestring);
				}
				tmp = cJSON_GetObjectItem(level2,"sunset");
				if(tmp && tmp->valuestring)
				{
					printf("sunset: %s\n",tmp->valuestring);
				}
			}
			level2 = cJSON_GetObjectItem(level1,"atmosphere");
			if(level2)
			{
				tmp = cJSON_GetObjectItem(level2,"HM");
				if(tmp && tmp->valuestring)
				{
					printf("HM: %s\n",tmp->valuestring);
				}
				tmp = cJSON_GetObjectItem(level2,"PR");
				if(tmp && tmp->valuestring)
				{
					printf("PR: %s\n",tmp->valuestring);
				}
				tmp = cJSON_GetObjectItem(level2,"visibility");
				if(tmp && tmp->valuestring)
				{
					printf("visibility: %s\n",tmp->valuestring);
				}
			}
			level2 = cJSON_GetObjectItem(level1,"condition");
			if (level2)
			{
				tmp = cJSON_GetObjectItem(level2,"code");
				if(tmp && tmp->valuestring)
				{
					printf("code: %s\n",tmp->valuestring);
				}
				tmp = cJSON_GetObjectItem(level2,"TP");
				if(tmp && tmp->valuestring)
				{
					printf("TP: %s\n",tmp->valuestring);
				}
			}
			level2 = cJSON_GetObjectItem(level1,"airquality");
			if (level2)
			{
				tmp = cJSON_GetObjectItem(level2,"index");
				if(tmp && tmp->valuestring)
				{
					printf("index: %s\n",tmp->valuestring);
				}
			}
		}			
		cJSON_Delete(root);
	}
}



void deal_action_perform(u8 *buf)
{
	cJSON *root,*tmp,*level1,*level2,*level3;
	int cmd_size;
	u8 i;
	int qos,seq_num,sep_type;//sep_type为1时是SL，为2时是SC
	SL_AP *sl;
	SC_AP *sc;
	root = cJSON_Parse((char*)buf);
	if (root)
	{
		tmp = cJSON_GetObjectItem(root,"qos");
		if(tmp)
		{
			qos = tmp->valueint;
			//printf("qos: %d\n",tmp->valueint);
		}
		level1 = cJSON_GetObjectItem(root,"cmd");	
		if (level1)
		{
			if(level1->type == cJSON_Array)
			{
				seq_num = cmd_size = cJSON_GetArraySize(level1);
				if(cmd_size)
				{
					for(i = 0;i < cmd_size;i++)
					{
						level2 = cJSON_GetArrayItem(level1,i);
						if(level2)
						{
							tmp = cJSON_GetObjectItem(level2,"sepid");
							if(tmp && tmp->valuestring)
							{
								//printf("seqid: %s\n",tmp->valuestring);
								if (*tmp->valuestring == 'S')
									if(*tmp->valuestring+1 == 'L')
									{sep_type = 1;mymemcpy(sl->sepid,tmp->valuestring,strlen(tmp->valuestring));}
									else
									{sep_type = 2;mymemcpy(sc->sepid,tmp->valuestring,strlen(tmp->valuestring));}
							}
							tmp = cJSON_GetObjectItem(level2,"seqid");
							if(tmp)
							{
								if(sep_type == 1)	sl->seqid = tmp->valueint;
								else if(sep_type == 2)	sc->seqid = tmp->valueint;
								//printf("seqid: %d\n",tmp->valueint);
							}
							tmp = cJSON_GetObjectItem(level2,"CH");
							if(tmp)
							{
								if(sep_type == 1)	sl->ch = tmp->valueint;
								else if(sep_type == 2)	sc->ch = tmp->valueint;
								//printf("CH: %d\n",tmp->valueint);
							}
							tmp = cJSON_GetObjectItem(level2,"action");
							if(tmp && tmp->valuestring)
							{
								if(sep_type == 1)	mymemcpy(sl->action,tmp->valuestring,strlen(tmp->valuestring));
								else if(sep_type == 2)	mymemcpy(sc->action,tmp->valuestring,strlen(tmp->valuestring));
								//printf("action: %s\n",tmp->valuestring);
							}
							tmp = cJSON_GetObjectItem(level2,"topos");
							if(tmp && tmp->valuestring)
							{
								if(sep_type == 1)	mymemcpy(sl->topos,tmp->valuestring,strlen(tmp->valuestring));
								else if(sep_type == 2)	mymemcpy(sc->topos,tmp->valuestring,strlen(tmp->valuestring));
								//printf("toppos: %d\n",tmp->valueint);
							}
							level3 = cJSON_GetObjectItem(level2,"option");
							if(level3)
							{
								tmp = cJSON_GetObjectItem(level3,"durations");
								if(tmp)
								{
									sl->option.duration = tmp->valueint;
									//printf("durations: %d\n",tmp->valueint);
								}
								tmp = cJSON_GetObjectItem(level3,"erase");
								if(tmp)
								{
									sl->option.erase = tmp->valueint;
									//printf("erase: %d\n",tmp->valueint);
								}
							}
							tmp = cJSON_GetObjectItem(level2,"stseq");
							if(tmp)
							{
								if(sep_type == 1)	sl->stseq = tmp->valueint;
								else if(sep_type == 2)	sc->stseq = tmp->valueint;
								//printf("stseq: %d\n",tmp->valueint);
							}
							tmp = cJSON_GetObjectItem(level2,"timeout");
							if(tmp)
							{
								if(sep_type == 1)	sl->stseq = tmp->valueint;
								else if(sep_type == 2)	sc->stseq = tmp->valueint;
								//printf("timeout: %d\n",tmp->valueint);
							}
						}
					}
				}
				//reveive success;
				//send_message_without_payload(0x85,0x01,Rxfr4004.rx_var_header.message_id_H,Rxfr4004.rx_var_header.message_id_L,0x00);
			}	
		}
		cJSON_Delete(root);
	}
}



void success_receipt(void)
{
	u8 *send_buf,*temp_send_buf;
	u16 send_payload_len;
	char *send_payload_buf;
	cJSON *root;
	root=cJSON_CreateObject();       
	cJSON_AddItemToObject(root, "code", cJSON_CreateString("0x0200000"));     
	cJSON_AddItemToObject(root, "msg", cJSON_CreateString("operation are Successfully Performed")); 
	send_payload_buf =cJSON_PrintUnformatted(root);
	cJSON_Delete(root); 
	send_payload_len = strlen(send_payload_buf);
	mymemcpy(Txto4004.tx_payload,send_payload_buf,send_payload_len);
	temp_send_buf = send_buf = mymalloc(send_payload_len+5);
	*send_buf++ = Txto4004.tx_fix_header.ch.first_ch_byte = 0x83;
	*send_buf++ = (3 + send_payload_len);
	*send_buf++	= Txto4004.tx_var_header.version = 0x01;
	*send_buf++	= Txto4004.tx_var_header.message_id_H = Rxfr4004.rx_var_header.message_id_H;
	*send_buf++	= Txto4004.tx_var_header.message_id_L = Rxfr4004.rx_var_header.message_id_L;
	mystrncat(send_buf,Txto4004.tx_payload,send_payload_len);
	Usart2_Send(temp_send_buf,(5 + send_payload_len));
	mymemset(Txto4004.tx_payload,0,send_payload_len);
	free(send_payload_buf);
	myfree(send_buf);
}

void error_recepit(void)
{
	u8 *send_buf,*temp_send_buf;
	u16 send_payload_len;
	char *send_payload_buf;
	cJSON *root;
	root=cJSON_CreateObject();       
	cJSON_AddItemToObject(root, "code", cJSON_CreateString("0x0401F08"));     
	cJSON_AddItemToObject(root, "msg", cJSON_CreateString("Unauthorized Seraph Sense")); 
	send_payload_buf =cJSON_PrintUnformatted(root);
	cJSON_Delete(root); 
	send_payload_len = strlen(send_payload_buf);
	mymemcpy(Txto4004.tx_payload,send_payload_buf,send_payload_len);
	temp_send_buf = send_buf = mymalloc(send_payload_len+5);
	*send_buf++ = Txto4004.tx_fix_header.ch.first_ch_byte = 0x85;
	*send_buf++ = (3 + send_payload_len);
	*send_buf++	= Txto4004.tx_var_header.version = 0x01;
	*send_buf++	= Txto4004.tx_var_header.message_id_H = Rxfr4004.rx_var_header.message_id_H;
	*send_buf++	= Txto4004.tx_var_header.message_id_L = Rxfr4004.rx_var_header.message_id_L;
	mystrncat(send_buf,Txto4004.tx_payload,send_payload_len);
	Usart2_Send(temp_send_buf,(5 + send_payload_len));
	mymemset(Txto4004.tx_payload,0,send_payload_len);
	free(send_payload_buf);
	myfree(send_buf);
}

void calc_send_r_length(u8 *buf,Txmessage *tx)
{
	u32 padload_len;
	padload_len = strlen(buf);
	if(tx->tx_var_header.ext_message_id){
		padload_len += 4;
	}
	else{
		padload_len += 3;
	}
	if ((padload_len / 2097126) > 0)//有4个字节长的r_length
	{	
		tx->tx_fix_header.r_length_len = 4;
		tx->tx_fix_header.r_length[0] = (u8)(padload_len / 2097126);
		padload_len = padload_len % 2097126;
		if((padload_len / 16384) > 0)
		{
			tx->tx_fix_header.r_length[1] = (u8)(padload_len / 16384);
			padload_len = padload_len % 16384;
			if ((padload_len / 128) > 0)
			{
				tx->tx_fix_header.r_length[2] = (u8)(padload_len / 128);
				padload_len = (padload_len % 128);
				
				tx->tx_fix_header.r_length[3] = (u8)padload_len;
			}
		}
	}
	else if((padload_len / 16384) > 0)//有3个字节的r_length
	{
			tx->tx_fix_header.r_length_len = 3;
			tx->tx_fix_header.r_length[0] = (u8)(padload_len / 16384);
			padload_len = padload_len % 16384;
			if ((padload_len / 128) > 0)
			{
				tx->tx_fix_header.r_length[1] = (u8)(padload_len / 128);
				padload_len = (padload_len % 128);
				tx->tx_fix_header.r_length[2] = (u8)padload_len;
			}
	}
	else if((padload_len / 128) > 0)//有2个字节的r_length
	{
		tx->tx_fix_header.r_length_len = 2;
		tx->tx_fix_header.r_length[0] = (u8)(padload_len / 128);
		padload_len = (u8)padload_len % 128;
		
		tx->tx_fix_header.r_length[1] = (u8)padload_len;
	}
	else
	{
		tx->tx_fix_header.r_length_len = 1;
		tx->tx_fix_header.r_length[0] = (u8)padload_len;
	}
}

//初始化Txto4004数据，根据_REPLY_ type做不同初始化
void init_send_Txmessage(_REPLY_ type)
{
	Txmessage tx;
	cJSON *cs;
	switch(type)
	{
		case CONTINUE:
			cs = cJSON_CreateObject();    
			cJSON_AddItemToObject(cs, "code", cJSON_CreateString("0x0401F08"));     
			cJSON_AddItemToObject(cs, "msg", cJSON_CreateString("Unauthorized Seraph Sense")); 
			tx.tx_payload = cJSON_PrintUnformatted(cs);
			if(tx.tx_payload)	cJSON_Delete(cs);
			tx.tx_fix_header.ch.first_ch_byte = 0x83;
			tx.tx_var_header.version = 0x01;
			tx.tx_var_header.message_id_H = Rxfr4004.rx_var_header.message_id_H = 0x89;
			tx.tx_var_header.message_id_L = Rxfr4004.rx_var_header.message_id_L = 0x02;
			tx.tx_var_header.ext_message_id = 0x01;
			calc_send_r_length(tx.tx_payload,&tx);
			break;
		default:
			break;
	}
		send_message(&tx);
}
//发送Txto4004内容
void send_message(Txmessage *tx)
{
	u8 *send_buf,*temp_send_buf;
	u16 txlen = 0;
	u16 payloadlen;
	
	
	temp_send_buf = mymalloc(4096);
	mymemset(temp_send_buf,0,4096);
	send_buf = temp_send_buf;
	temp_send_buf = send_buf;
	*send_buf++ = (tx->tx_fix_header.ch.first_ch_byte);
	txlen++;
	while(tx->tx_fix_header.r_length_len--){
		*send_buf++ = tx->tx_fix_header.r_length[tx->tx_fix_header.r_length_len];
		txlen++;
	}
	*send_buf++ = tx->tx_var_header.version;
	*send_buf++ = (tx->tx_var_header.message_id_H);
	*send_buf++ = (tx->tx_var_header.message_id_L);
	txlen+=3;
	if(tx->tx_var_header.ext_message_id){
		*send_buf++ = (tx->tx_var_header.ext_message_id);
		txlen++;
	}
	if(tx->tx_payload){
		payloadlen = strlen(tx->tx_payload);
		mymemcpy(send_buf,tx->tx_payload,payloadlen);
		txlen+=payloadlen;
		myfree(tx->tx_payload);
	}
	//Usart2_Send(temp_send_buf,txlen);
	Usart1_Send(temp_send_buf,txlen);
	myfree(temp_send_buf);
}


void send_message_without_payload(u8 fix1,u8 version,u8 message_id_h,u8 message_id_l,u8 ex_message_id)
{
	u8 send_buf[6];
	u8 r_length;
	if(ex_message_id)
		r_length = 0x04;
	else
		r_length = 0x03;
	send_buf[0] = fix1;
	send_buf[1] = r_length;
	send_buf[2] = version;
	send_buf[3] = message_id_h;
	send_buf[4] = message_id_l;
	if(ex_message_id)
	{	
		send_buf[5] = ex_message_id;
	}
	Usart2_Send(send_buf,(r_length + 2));
}


