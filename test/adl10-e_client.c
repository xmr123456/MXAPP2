

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus.h>
#include <pthread.h>
#include "adl10-e.h"


const int EXCEPTION_RC = 2;

enum {
    TCP,
    TCP_PI,
    RTU
};


union current_total_clc{
	unsigned short n[2];
	float current;
};

int equal_dword(uint16_t *tab_reg, const uint32_t value);
int is_memory_equal(const void *s1, const void *s2, size_t size);

#define BUG_REPORT(_cond, _format, _args ...) \
    printf("\nLine %d: assertion error for '%s': " _format "\n", __LINE__, # _cond, ## _args)

#define ASSERT_TRUE(_cond, _format, __args...) {  \
    if (_cond) {                                  \
        printf("OK\n");                           \
    } else {                                      \
        BUG_REPORT(_cond, _format, ## __args);    \
    }                                             \
};

int is_memory_equal(const void *s1, const void *s2, size_t size)
{
    return (memcmp(s1, s2, size) == 0);
}

int equal_dword(uint16_t *tab_reg, const uint32_t value) {
    return ((tab_reg[0] == (value >> 16)) && (tab_reg[1] == (value & 0xFFFF)));
}

/************************************************* 
Function: adl10e_get_current_total_electricity
Description: 获取 电压
Input: ctx modbus实例
Output:	tab_rp_registers 获取输出总功率
	   
Return: 返回0代表成功 ， 非0代表失败
*************************************************/

int adl10e_get_current_total_electricity(modbus_t *ctx,uint16_t *tab_rp_registers)
{
	int rc;
	int i;
	float result=0;
	union current_total_clc m_current;
	
	if(ctx == NULL || tab_rp_registers == NULL)
		return 0;


	rc = modbus_read_registers(ctx, CURRENT_TOTOAL_ELECTRICITY_ADDRESS,
							CURRENT_TOTOAL_ELECTRICITY_NB, tab_rp_registers);

	if(rc == CURRENT_TOTOAL_ELECTRICITY_NB){
		printf("OK!\n");
		
		m_current.n[0]=tab_rp_registers[0];
		m_current.n[1]=tab_rp_registers[1];
		FILE *fd=fopen("total_electricity.txt","w+");
		if(fd==NULL){
			perror("file open failed\n");
			return -1;
		}
		char buf[32];
		sprintf(buf,"%f",m_current.current);
		fwrite(buf,strlen(buf),1,fd);
		fclose(fd);
		
		return 0;
	}else{
		printf("FAILED get voltage\n");
		return -1;
	}


}


/************************************************* 
Function: adl10e_set_voltage
Description: 设置 电流
Input: ctx modbus实例
	   voltage 要设置电流值
Return: 返回0代表成功 ， 非0代表失败
*************************************************/


int adl10e_set_voltage(modbus_t *ctx,uint16_t voltage)
{
	int rc;
	int i;
	int result=0;
	
	if(ctx == NULL )
		return 0;


	rc = modbus_write_registers(ctx, VOLTAGE_ADDRESS,
							VOLTAGE_NB, &voltage);
	ASSERT_TRUE(rc == VOLTAGE_NB, "FAILED (nb points %d)\n", rc);

	if(rc == VOLTAGE_NB){
		printf("OK!\n");
		return 0;
	}else{
		printf("FAILED set voltage\n");
		return -1;
	}

	

}


/************************************************* 
Function: adl10e_get_voltage
Description: 获取 电压
Input: ctx modbus实例
Output:	tab_rp_registers 获取输出电流buf
	   
Return: 返回0代表成功 ， 非0代表失败
*************************************************/



int adl10e_get_voltage(modbus_t *ctx,uint16_t *tab_rp_registers)
{
	int rc;
	int i;
	int result=0;
	
	if(ctx == NULL || tab_rp_registers == NULL)
		return 0;


	rc = modbus_read_registers(ctx, VOLTAGE_ADDRESS,
							VOLTAGE_NB, tab_rp_registers);

	if(rc == VOLTAGE_NB){
		printf("OK!\n");
		
		FILE *fd=fopen("voltage.txt","w+");
		if(fd==NULL){
			perror("file open failed\n");
			return -1;
		}
		char buf[16];
		sprintf(buf,"%hd",tab_rp_registers[0]);
		fwrite(buf,strlen(buf),1,fd);
		fclose(fd);
		
		return 0;
	}else{
		printf("FAILED get voltage\n");
		return -1;
	}


}


/************************************************* 
Function: adl10e_set_current
Description: 设置 电流
Input: ctx modbus实例
	   current 要设置电流值
Return: 返回0代表成功 ， 非0代表失败
*************************************************/
int adl10e_set_current(modbus_t *ctx,uint16_t current)
{
	int rc;
	int i;
	int result=0;
	
	if(ctx == NULL )
		return -1;


	rc = modbus_write_registers(ctx, CURRENT_ADDRESS,
							CURRENT_NB, &current);

	if(rc == CURRENT_NB){
		printf("OK!\n");
		return 0;
	}else{
		printf("FAILED set current\n");
		return -1;
	}
	

}

/************************************************* 
Function: adl10e_get_current
Description: 获取 电流
Input: ctx modbus实例
Output:tab_rp_registers 获取输出电流buf
	   
Return: 返回0代表成功 ， 非0代表失败
*************************************************/

int adl10e_get_current(modbus_t *ctx,uint16_t *tab_rp_registers)
{
	int rc;
	int i;
	int result=0;
	
	if(ctx == NULL || tab_rp_registers == NULL)
		return 0;


	rc = modbus_read_registers(ctx, CURRENT_ADDRESS,
							CURRENT_NB, tab_rp_registers);


	if(rc == CURRENT_NB){
		printf("OK!\n");
		FILE *fd=fopen("current.txt","w+");
		if(fd==NULL){
			perror("file open failed\n");
			return -1;
		}
		char buf[16];
		sprintf(buf,"%hd",tab_rp_registers[0]);
		fwrite(buf,strlen(buf),1,fd);
		fclose(fd);
		
		return 0;
	}else{
		printf("FAILED set current\n");
		FILE *fd=fopen("current.txt","w+");
		if(fd==NULL){
			perror("file open failed\n");
			return -1;
		}
		char buf[16];
		sprintf(buf,"%s","0");
		fwrite(buf,strlen(buf),1,fd);
		fclose(fd);
		return -1;
	}


}

int adl10e_start_charging(modbus_t *ctx)
{
	int rc;
	int i;
	float result=0;
	union current_total_clc m_current;
	
	if(ctx == NULL)
		return 0;


    /* Single */
    rc = modbus_write_bit(ctx, ADL10_E_BITS_ADDRESS, ON);
    ASSERT_TRUE(rc == 1, "");
	return 0;
}


int adl10e_stop_charging(modbus_t *ctx)
{
	int rc;
	int i;
	float result=0;
	union current_total_clc m_current;
	
	if(ctx == NULL )
		return 0;


    /* Single */
    rc = modbus_write_bit(ctx, ADL10_E_BITS_ADDRESS, OFF);
    ASSERT_TRUE(rc == 1, "");
	
	adl10e_set_current(ctx,0);


	return 0;

}


/************************************************* 
Function: adl10e_get_power_factory
Description: 获取 功率因子
Input: ctx modbus实例
Output:tab_rp_registers 获取功率因子buf
	   
Return: 返回0代表成功 ， 非0代表失败
*************************************************/

int adl10e_get_power_factory(modbus_t *ctx,uint16_t *tab_rp_registers)
{
	int rc;
	int i;
	int result=0;
	
	if(ctx == NULL || tab_rp_registers == NULL)
		return 0;


	rc = modbus_read_registers(ctx, POWER_FACTOR_ADDRESS,
							POWER_FACTOR_NB, tab_rp_registers);

	if(rc == POWER_FACTOR_NB){
		printf("OK!\n");
		
		FILE *fd=fopen("power_factory.txt","w+");
		if(fd==NULL){
			perror("file open failed\n");
			return -1;
		}
		char buf[16];
		sprintf(buf,"%hd",tab_rp_registers[0]);
		fwrite(buf,strlen(buf),1,fd);
		fclose(fd);
		
		return 0;
	}else{
		printf("FAILED get power_factory\n");
		return -1;
	}
}


/************************************************* 
Function: adl10e_get_address
Description: 获取 地址
Input: ctx modbus实例
Output:tab_rp_registers 获取地址（高8位)
	   
Return: 返回0代表成功 ， 非0代表失败
*************************************************/


int adl10e_get_address(modbus_t *ctx,uint16_t *tab_rp_registers)
{
	int rc;
	int i;
	int result=0;
	
	if(ctx == NULL || tab_rp_registers == NULL)
		return 0;


	rc = modbus_read_registers(ctx, ADDRESS_ADDRESS,
							ADDRESS_NB, tab_rp_registers);

	if(rc == ADDRESS_NB){
		printf("OK!\n");
		return 0;
	}else{
		printf("FAILED get power_factory\n");
		return -1;
	}

//	printf("%d\n",(unsigned char)((tab_rp_registers[0]&0xff00)>>8));
//	return (unsigned char)((tab_rp_registers[0]&0xff00)>>8);


}

/************************************************* 
Function: adl10e_get_baud_rate
Description: 获取 波特率
Input: ctx modbus实例
Output:tab_rp_registers 获取地址（低8位 0：9600 1：4800 2:2400 3:1200)
	   
Return: 返回0代表成功 ， 非0代表失败
*************************************************/

int adl10e_get_baud_rate(modbus_t *ctx,uint16_t *tab_rp_registers)
{
	int rc;
	int i;
	int result=0;
	
	if(ctx == NULL || tab_rp_registers == NULL)
		return 0;


	rc = modbus_read_registers(ctx, ADDRESS_ADDRESS,
							ADDRESS_NB, tab_rp_registers);
	ASSERT_TRUE(rc == ADDRESS_NB, "FAILED (nb points %d)\n", rc);
	for(i = 0;i<ADDRESS_NB;i++){
		printf("[%d][%x]\n",i,tab_rp_registers[i]);
	}



	if(rc == ADDRESS_NB){
		printf("OK!\n");
		return 0;
	}else{
		printf("FAILED get power_factory\n");
		return -1;
	}
/*
	switch((unsigned char)((tab_rp_registers[0]&0x00ff)))
	{
		case 0:
			printf("9600 !\n");
			return 9600;
		case 1:
			printf("4800 !\n");
			return 4800;
		case 2:
			printf("2400 !\n");
			return 2400;
		case 3:
			printf("1200 !\n");
			return 1200;
		default:
			return 0;
	}
*/
}

int get_quick_start_command()
{
	FILE *fd=fopen("quick_start.txt","r");
	if(fd==NULL){
		return 0;
	}
	fclose(fd);
	remove("quick_start.txt");
	return 1;
}

int get_slow_start_command()
{
	FILE *fd=fopen("slow_start.txt","r");
	if(fd==NULL){
		return 0;
	}
	fclose(fd);
	remove("slow_start.txt");
	return 1;
}

int get_stop_command()
{
	FILE *fd=fopen("stop.txt","r");
	if(fd==NULL){
		return 0;
	}
	fclose(fd);
	remove("stop.txt");
	return 1;
}


void *heart_activity(void *param)
{
	modbus_t *ctx = (modbus_t *)param;
	int ret;
	int nb_points;
	uint8_t *tab_rp_bits;
	union current_total_clc m_total;
	while(1){
		ret = adl10e_get_current_total_electricity(ctx,m_total.n);
		if(ret == 0){
			printf("total_electricity:%f\n",m_total.current);
		}
		
		ret = adl10e_get_current(ctx,&m_total.n[0]);
		if(ret == 0){
			printf("current:%d\n",m_total.n[0]);
		}
		
		ret = adl10e_get_voltage(ctx,&m_total.n[0]);
		if(ret == 0){
			printf("voltage:%d\n",m_total.n[0]);
		}
		
		ret = adl10e_get_power_factory(ctx,&m_total.n[0]);
		if(ret == 0){
			printf("power_factory:%d\n",m_total.n[0]);
		}
	 sleep(1);
	}
}

void *command_activity(void *arg)
{
	modbus_t *ctx = (modbus_t *)arg;
	int ret;
	union current_total_clc m_total;
	while(1){
		if(get_quick_start_command()==1){
			adl10e_start_charging(ctx);
			adl10e_set_current(ctx,100);
			continue;
		}
		else if(get_slow_start_command()==1){
			adl10e_start_charging(ctx);
			adl10e_set_current(ctx,10);
			continue;
		}
		else if(get_stop_command()==1){
			adl10e_stop_charging(ctx);
			continue;
		}
		else{
			continue;
		}
		usleep(500000);
	}
}

void *start_qml(void *arg)
{
	system("./Charge");
	while(1){
	}
}

int main(int argc, char *argv[])
{
    uint16_t *tab_rp_registers = NULL;
    modbus_t *ctx = NULL;
    int i;
    int nb_points;
    uint32_t old_response_to_sec;
    uint32_t old_response_to_usec;
    uint32_t new_response_to_sec;
    uint32_t new_response_to_usec;

    int use_backend;
    int success = FALSE;
	char *dev_path="/dev/ttyUSB1";
	int server_id = SERVER_ID;

	uint16_t m_current;
	uint16_t m_voltage;
	uint16_t m_powerfactory;

	pthread_t tid1,tid2,tid3;

    if (argc > 1) {
        if (strcmp(argv[1], "tcp") == 0) {
            use_backend = TCP;
        } else if (strcmp(argv[1], "tcppi") == 0) {
            use_backend = TCP_PI;
        } else if (strcmp(argv[1], "rtu") == 0) {
            use_backend = RTU;
			if(argc>2)
				dev_path = strdup(argv[2]);
			if(argc > 3)
				server_id = atoi(argv[3]);
        } else {
            printf("Usage:\n  %s [tcp|tcppi|rtu] - Modbus client for unit testing\n\n", argv[0]);
            exit(1);
        }
    } else {
        /* By default */
        use_backend = TCP;
    }

    if (use_backend == TCP) {
        ctx = modbus_new_tcp("127.0.0.1", 1502);
    } else if (use_backend == TCP_PI) {
        ctx = modbus_new_tcp_pi("::1", "1502");
    } else {
        ctx = modbus_new_rtu(dev_path, 115200, 'N', 8, 1);
    }
    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }
    modbus_set_debug(ctx, TRUE);
    modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);

    if (use_backend == RTU) {
        modbus_set_slave(ctx, server_id);
    }

    modbus_get_response_timeout(ctx, &old_response_to_sec, &old_response_to_usec);
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
    modbus_get_response_timeout(ctx, &new_response_to_sec, &new_response_to_usec);

    printf("** UNIT TESTING **\n");

    printf("1/1 No response timeout modification on connect: ");
    ASSERT_TRUE(old_response_to_sec == new_response_to_sec &&
                old_response_to_usec == new_response_to_usec, "");

	nb_points = ADL10_E_INPUT_REGISTERS_NB;
    tab_rp_registers = (uint16_t *) malloc(nb_points * sizeof(uint16_t));
    memset(tab_rp_registers, 0, nb_points * sizeof(uint16_t));
	
	adl10e_stop_charging(ctx);
	//pthread_create(&tid1,NULL,start_qml,NULL);
	pthread_create(&tid2,NULL,command_activity,(void *)ctx);
	pthread_create(&tid3,NULL,heart_activity,(void *)ctx);
	//pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);

    modbus_close(ctx);
    modbus_free(ctx);
    ctx = NULL;

    /* Test init functions */
    printf("\nTEST INVALID INITIALIZATION:\n");
    ctx = modbus_new_rtu(NULL, 1, 'A', 0, 0);
    ASSERT_TRUE(ctx == NULL && errno == EINVAL, "");

    ctx = modbus_new_rtu("/dev/dummy", 0, 'A', 0, 0);
    ASSERT_TRUE(ctx == NULL && errno == EINVAL, "");

    ctx = modbus_new_tcp_pi(NULL, NULL);
    ASSERT_TRUE(ctx == NULL && errno == EINVAL, "");

    printf("\nALL TESTS PASS WITH SUCCESS.\n");
    success = TRUE;

close:
    /* Free the memory */
    free(tab_rp_registers);

    /* Close the connection */
    modbus_close(ctx);
    modbus_free(ctx);

    return (success) ? 0 : -1;
}




