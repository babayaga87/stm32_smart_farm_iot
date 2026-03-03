/*
 * protocol.c
 *
 *  Created on: Nov 27, 2025
 *      Author: Vy Khang
 */
#include "protocol.h"
#include "nrf24l01.h"
#include "string.h"

void assemble_master_packet(master_packet* master_pkt, const master_cmd cmd, const motor_param param){
  master_pkt->cmd = cmd;
  master_pkt->param = param;
}

void assemble_slave_packet(slave_packet* slave_pkt,
                           const slave_status status,
                           const float temp,
                           const float humid ,
                           const float moist,
                           const uint8_t motor_status){

  slave_pkt->status = status;
  slave_pkt->temperature = temp;
  slave_pkt->humidity = humid;
  slave_pkt->moisture = moist;
  slave_pkt->motor_status = motor_status;
}

void serialize_master_packet(const master_packet* master_pkt, uint8_t* buffer){
  memcpy(buffer,  &master_pkt->cmd, sizeof(master_cmd));
  memcpy(buffer + sizeof(master_cmd), &master_pkt->param, sizeof(motor_param));
}

void deserialize_master_packet(master_packet* master_pkt, const uint8_t* buffer){
  memcpy(&master_pkt->cmd, buffer, sizeof(master_cmd));
  memcpy(&master_pkt->param, buffer + sizeof(master_cmd), sizeof(motor_param));
}

void serialize_slave_packet(const slave_packet* slave_pkt, uint8_t* buffer){
  memcpy(buffer, &slave_pkt->status, sizeof(slave_status));
  memcpy(buffer + sizeof(slave_status), &slave_pkt->temperature, sizeof(float));
  memcpy(buffer +
         sizeof(slave_status) +
         sizeof(float),
         &slave_pkt->humidity, sizeof(float ));
  memcpy(buffer +
         sizeof(slave_status) +
         sizeof(float)+
         sizeof(float),
         &slave_pkt->moisture, sizeof(float));
  memcpy(buffer +
         sizeof(slave_status) +
         sizeof(float)+
         sizeof(float) +
         sizeof(float),
         &slave_pkt->motor_status, sizeof(uint8_t));
}

void deserialize_slave_packet(slave_packet* slave_pkt, const uint8_t* buffer){
  memcpy(&slave_pkt->status, buffer, sizeof(slave_status));
  memcpy(&slave_pkt->temperature, buffer + sizeof(slave_status), sizeof(float));
  memcpy(&slave_pkt->humidity, buffer + sizeof(slave_status) + sizeof(float), sizeof(float ));
  memcpy(&slave_pkt->moisture, buffer + sizeof(slave_status) + sizeof(float) * 2, sizeof (float));
  memcpy(&slave_pkt->motor_status, buffer + sizeof(slave_status) + sizeof(float) * 3, sizeof(uint8_t));
}

void slave_init(slave* s, void (*get)(void), void (*motor)(motor_param), void(*measure)(void)){
  s->status = idle;
  s->get_status_handler = get;
  s->motor_handler = motor;
  s->measure_handler = measure;
}

