#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus.h>
#include <sys/socket.h>
#include <pthread.h>

#include "adl10-e.h"

enum {
    TCP,
    TCP_PI,
    RTU
};

union current_total_clc{
	unsigned short n[2];
	float current;
};


int start_flag=0;
int heart_time=0;
static int init_adl10_e(modbus_mapping_t *mb_mapping)
{
	int i;


	if(mb_mapping == NULL){
		printf("mb_mapping is null!\n");
		return -1;
	}

	/* Initialize values of  BITS */
    for (i=0; i < ADL10_E_BITS_NB; i++) {
        mb_mapping->tab_bits[i] = 0x0;
    }

    /* Initialize values of  REGISTERS */
    for (i=0; i < ADL10_E_REGISTERS_NB_MAX; i++) {
        mb_mapping->tab_registers[i] = 0x0;
    }

	
	/* current_totoal_electricity */
	for (i=0; i < CURRENT_TOTOAL_ELECTRICITY_NB; i++) {
		mb_mapping->tab_registers[CURRENT_TOTOAL_ELECTRICITY_ADDRESS+i] = CURRENT_TOTOAL_ELECTRICITY_REGISTERS_TAB[i];
	}

	/* voltage */
	for (i=0; i < VOLTAGE_NB; i++) {
		mb_mapping->tab_registers[VOLTAGE_ADDRESS+i] = VOLTAGE_REGISTERS_TAB[i];
	}

	/* current */
	for (i=0; i < CURRENT_NB; i++) {
		mb_mapping->tab_registers[CURRENT_ADDRESS+i] = CURRENT_REGISTERS_TAB[i];
	}

	/* power factory */
	for (i=0; i < POWER_FACTOR_NB; i++) {
		mb_mapping->tab_registers[POWER_FACTOR_ADDRESS+i] = POWER_FACTOR_REGISTERS_TAB[i];
	}

	/* power factory */
	for (i=0; i < ADDRESS_NB; i++) {
		mb_mapping->tab_registers[ADDRESS_ADDRESS+i] = ADDRESS_REGISTERS_TAB[i];
	}


	return 0;
}


void *adl10_e(void *param)
{
	modbus_mapping_t * mb_mapping= (modbus_mapping_t *)param;
	uint16_t m_current,m_valtage;
	union current_total_clc m_total;
	int m_sleep_time=1;
	float m_temp;
	while(1){
		if(start_flag == 1){
			m_current = mb_mapping->tab_registers[CURRENT_ADDRESS];
			m_valtage = mb_mapping->tab_registers[VOLTAGE_ADDRESS];
			m_total.n[0] = mb_mapping->tab_registers[CURRENT_TOTOAL_ELECTRICITY_ADDRESS];
			m_total.n[1] = mb_mapping->tab_registers[CURRENT_TOTOAL_ELECTRICITY_ADDRESS+1];
			m_sleep_time = 1;
			
			if( heart_time == 0)
				m_current = 0;
			

			m_temp = (float)((double)m_current * m_valtage *m_sleep_time / 3600/1000);
			m_total.current  += m_temp;
			printf("total.current [%f] !\n",m_total.current);
			mb_mapping->tab_registers[CURRENT_TOTOAL_ELECTRICITY_ADDRESS] = m_total.n[0];
			mb_mapping->tab_registers[CURRENT_TOTOAL_ELECTRICITY_ADDRESS+1] = m_total.n[1];

			if(heart_time > 0)
				heart_time--;
		}else{
			printf("stop !\n");
		}

		sleep(1);
		
	}

}

int main(int argc, char *argv[])
{
	int s = -1;
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
	char *dev_path="/dev/ttyUSB0";
    int rc;
    int i;
    int use_backend;
    uint8_t *query;
    int header_length;
	int server_id=SERVER_ID;
	int nb_points;
	pthread_t tid;

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
            printf("Usage:\n  %s [tcp|tcppi|rtu] - Modbus server for adl10-e testing\n\n", argv[0]);
            return -1;
        }
    } else {
        /* By default */
        use_backend = TCP;
    }

    if (use_backend == TCP) {
        ctx = modbus_new_tcp("127.0.0.1", 1502);
        query = malloc(MODBUS_TCP_MAX_ADU_LENGTH);
    } else if (use_backend == TCP_PI) {
        ctx = modbus_new_tcp_pi("::0", "1502");
        query = malloc(MODBUS_TCP_MAX_ADU_LENGTH);
    } else {
        ctx = modbus_new_rtu(dev_path, 115200, 'N', 8, 1);
        modbus_set_slave(ctx, server_id);
        query = malloc(MODBUS_RTU_MAX_ADU_LENGTH);
    }
	
    header_length = modbus_get_header_length(ctx);

    modbus_set_debug(ctx, TRUE);


    mb_mapping = modbus_mapping_new_start_address(
    ADL10_E_BITS_ADDRESS, ADL10_E_BITS_NB,
    ADL10_E_INPUT_BITS_ADDRESS, ADL10_E_INPUT_BITS_NB,
    ADL10_E_REGISTERS_ADDRESS, ADL10_E_REGISTERS_NB_MAX,
    ADL10_E_INPUT_REGISTERS_ADDRESS, ADL10_E_INPUT_REGISTERS_NB);
    if (mb_mapping == NULL) {
        fprintf(stderr, "Failed to allocate the mapping: %s\n",
                modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }




	init_adl10_e(mb_mapping);
	pthread_create(&tid,NULL,adl10_e,(void *)mb_mapping);



	if (use_backend == TCP) {
        s = modbus_tcp_listen(ctx, 1);
        modbus_tcp_accept(ctx, &s);
    } else if (use_backend == TCP_PI) {
        s = modbus_tcp_pi_listen(ctx, 1);
        modbus_tcp_pi_accept(ctx, &s);
    } else {
        rc = modbus_connect(ctx);
        if (rc == -1) {
            fprintf(stderr, "Unable to connect %s\n", modbus_strerror(errno));
            modbus_free(ctx);
            return -1;
        }
    }



    for (;;) {

        rc = modbus_receive(ctx, query);
		heart_time=3;
		printf("rc [%d] recive\n",rc);
		printf("query header_length[%x]\n",query[header_length]);
        if (rc > 0) {
            /* rc is the query size */
            modbus_reply(ctx, query, rc, mb_mapping);
			if(mb_mapping->tab_bits[0] &0x01){
				/* start calc */
				start_flag=1;

			}else {
				/* stop clac */
				start_flag=0;
			}
        } else if (rc == -1) {
            /* Connection closed by the client or error */
            break;
        }
    }


	printf("Quit the loop: %s\n", modbus_strerror(errno));

	if (use_backend == TCP) {
		if (s != -1) {
			close(s);
		}
	}
	modbus_mapping_free(mb_mapping);
	free(query);
	/* For RTU */
	modbus_close(ctx);
	modbus_free(ctx);

	return 0;


}

