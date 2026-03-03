/*
 * protocol.h
 *
 *  Created on: Nov 27, 2025
 *      Author: Vy Khang
 */

#ifndef INC_PROTOCOL_H_
#define INC_PROTOCOL_H_

#include <stdint.h>
#define SLAVE_PACKET_SIZE 17
#define MASTER_PACKET_SIZE 8

typedef enum {
		idle = 0,
		processing,
		success,
		error,
}slave_status;

typedef enum {
		get_status = 0,
		motor,
		measure,
}master_cmd;

typedef enum {
		motor_off = 0,
		motor_on,
}motor_param;

typedef struct {
		slave_status status;
		float temperature;
		float humidity;
		float moisture;
		uint8_t motor_status;
}slave_packet;

typedef struct {
		master_cmd cmd;
		motor_param param;
}master_packet;

typedef struct{

}master;

typedef struct{
		slave_status status;
		void (*get_status_handler) (void);
		void (*motor_handler)(motor_param);
		void (*measure_handler)(void);
}slave;

void slave_init(slave* s, void (*get)(void), void (*motor)(motor_param), void(*measure)(void));
void assemble_master_packet(master_packet*, const master_cmd, const motor_param);
void assemble_slave_packet(slave_packet*, const slave_status ,const float ,const float ,const float ,const uint8_t);
void serialize_master_packet(const master_packet*, uint8_t*);
void deserialize_master_packet(master_packet*, const uint8_t*);
void serialize_slave_packet(const slave_packet*, uint8_t*);
void deserialize_slave_packet(slave_packet*, const uint8_t*);

#endif /* INC_PROTOCOL_H_ */
