#include "nrf24l01.h"
#include "stm32f1xx_hal.h"
nrf24l01_device nrf24l01_get_default_config(){
  nrf24l01_device new_device;
  new_device.spi = NULL;
  new_device.irq_port = NULL;
  new_device.ce_port = NULL;
  new_device.csn_port = NULL;
  new_device.irq_pin = 0;
  new_device.ce_pin = 0;
  new_device.csn_pin = 0;
  new_device.timer = NULL;
  new_device.power_up = 0;
  new_device.primary_rx = 0;
  new_device.dynamic_ack_enable = 0;
  new_device.dynamic_payload_length_enable = 0;
  new_device.payload_with_ack_enable = 0;
  new_device.address_width = nrf24l01_address_width_5bytes;
  new_device.auto_retransmit_delay = nrf24l01_auto_retransmit_delay_250us;
  new_device.auto_retransmit_count = nrf24l01_auto_retransmit_count_3;
  new_device.frequency_channel = 2;
  new_device.air_data_rate = nrf24l01_air_data_rate_2mbps;
  new_device.rf_output_power = nrf24l01_rf_output_power_0dbm;
  new_device.setup_lna_gain = 1;

  /* data pipe 0 */
  new_device.data_pipe[0].nrf24l01_data_pipe_enable = 1;
  new_device.data_pipe[0].nrf24l01_data_pipe_auto_ack = 1;
  new_device.data_pipe[0].nrf24l01_data_pipe_receive_address = (uint8_t*)malloc(5);
  memset(new_device.data_pipe[0].nrf24l01_data_pipe_receive_address, 0xe7, 5 * sizeof(uint8_t));
  new_device.data_pipe[0].nrf24l01_data_pipe_payload_width = 0;
  new_device.data_pipe[0].nrf24l01_data_pipe_payload = NULL;
  new_device.data_pipe[0].nrf24l01_data_pipe_dyn_payload_length_enable = 0;

  /* data pipe 1 */
  new_device.data_pipe[1].nrf24l01_data_pipe_enable = 1;
  new_device.data_pipe[1].nrf24l01_data_pipe_auto_ack = 1;
  new_device.data_pipe[1].nrf24l01_data_pipe_receive_address = (uint8_t*)malloc(5);
  memset(new_device.data_pipe[1].nrf24l01_data_pipe_receive_address, 0xc2, 5 * sizeof(uint8_t));
  new_device.data_pipe[1].nrf24l01_data_pipe_payload_width = 0;
  new_device.data_pipe[1].nrf24l01_data_pipe_payload = NULL;
  new_device.data_pipe[1].nrf24l01_data_pipe_dyn_payload_length_enable = 0;

  /* data pipe 2 */
  new_device.data_pipe[2].nrf24l01_data_pipe_enable = 0;
  new_device.data_pipe[2].nrf24l01_data_pipe_auto_ack = 1;
  new_device.data_pipe[2].nrf24l01_data_pipe_receive_address = (uint8_t*)malloc(1);
  memset(new_device.data_pipe[2].nrf24l01_data_pipe_receive_address, 0xc3, 1 * sizeof(uint8_t));
  new_device.data_pipe[2].nrf24l01_data_pipe_payload_width = 0;
  new_device.data_pipe[2].nrf24l01_data_pipe_payload = NULL;
  new_device.data_pipe[2].nrf24l01_data_pipe_dyn_payload_length_enable = 0;

  /* data pipe 3 */
  new_device.data_pipe[3].nrf24l01_data_pipe_enable = 0;
  new_device.data_pipe[3].nrf24l01_data_pipe_auto_ack = 1;
  new_device.data_pipe[3].nrf24l01_data_pipe_receive_address = (uint8_t*)malloc(1);
  memset(new_device.data_pipe[3].nrf24l01_data_pipe_receive_address, 0xc4, 1 * sizeof(uint8_t));
  new_device.data_pipe[3].nrf24l01_data_pipe_payload_width = 0;
  new_device.data_pipe[3].nrf24l01_data_pipe_payload = NULL;
  new_device.data_pipe[3].nrf24l01_data_pipe_dyn_payload_length_enable = 0;

  /* data pipe 4 */
  new_device.data_pipe[4].nrf24l01_data_pipe_enable = 0;
  new_device.data_pipe[4].nrf24l01_data_pipe_auto_ack = 1;
  new_device.data_pipe[4].nrf24l01_data_pipe_receive_address = (uint8_t*)malloc(1);
  memset(new_device.data_pipe[4].nrf24l01_data_pipe_receive_address, 0xc5, 1 * sizeof(uint8_t));
  new_device.data_pipe[4].nrf24l01_data_pipe_payload_width = 0;
  new_device.data_pipe[4].nrf24l01_data_pipe_payload = NULL;
  new_device.data_pipe[4].nrf24l01_data_pipe_dyn_payload_length_enable = 0;

  /* data pipe 5 */
  new_device.data_pipe[5].nrf24l01_data_pipe_enable = 0;
  new_device.data_pipe[5].nrf24l01_data_pipe_auto_ack = 1;
  new_device.data_pipe[5].nrf24l01_data_pipe_receive_address = (uint8_t*)malloc(1);
  memset(new_device.data_pipe[5].nrf24l01_data_pipe_receive_address, 0xc6, 1 * sizeof(uint8_t));
  new_device.data_pipe[5].nrf24l01_data_pipe_payload_width = 0;
  new_device.data_pipe[5].nrf24l01_data_pipe_payload = NULL;
  new_device.data_pipe[5].nrf24l01_data_pipe_dyn_payload_length_enable = 0;

  new_device.transmit_address = malloc(5) ;
  memset(new_device.transmit_address, 0xe7, 5 * sizeof(uint8_t));
  return new_device;
}

uint8_t nrf24l01_init(nrf24l01_device * device){
  if (device == NULL) return -1; // null pointer
  HAL_Delay(11);
  // power
  if (device->power_up)
    nrf24l01_power_up(device);
  else
    nrf24l01_power_down(device);

  // primary rx
  uint8_t config_register;
  nrf24l01_read_register(device, CONFIG, &config_register, 1);
  if (device->primary_rx)
    config_register |= PRIM_RX;
  else
    config_register &= ~PRIM_RX;
  nrf24l01_write_register(device, CONFIG, &config_register, 1);

  // address_width
  uint8_t address_width = 3;
  switch (device->address_width) {
    case nrf24l01_address_width_3bytes:
      address_width = 1;
      nrf24l01_write_register(device, SETUP_AW, &address_width , 1);
      break;
    case nrf24l01_address_width_4bytes:
      address_width = 2;
      nrf24l01_write_register(device, SETUP_AW, &address_width , 1);
      break;
    case nrf24l01_address_width_5bytes:
      address_width = 3;
      nrf24l01_write_register(device, SETUP_AW, &address_width , 1);
      break;
    default:
      // default config: 5 bytes
      device->address_width = nrf24l01_address_width_5bytes;
      nrf24l01_write_register(device, SETUP_AW, &address_width , 1);
  }

  // auto retransmit delay and retransmit count
  uint8_t setup_retr_register = 0;
  setup_retr_register |= (device->auto_retransmit_delay << 4) | (device->auto_retransmit_count);
  nrf24l01_write_register(device, SETUP_RETR, &setup_retr_register, 1);

  // frequency channel
  if (device->frequency_channel > 125)
    // default config = 2
    device->frequency_channel = 2;
  nrf24l01_write_register(device, RF_CH, &device->frequency_channel, 1);

  // air data rate
  uint8_t rf_setup_register;
  nrf24l01_read_register(device, RF_SETUP, &rf_setup_register, 1);
  switch (device->air_data_rate) {
    case nrf24l01_air_data_rate_1mbps:
      CLEAR_BIT(rf_setup_register, RF_DR);
      break;
    case nrf24l01_air_data_rate_2mbps:
      SET_BIT(rf_setup_register, RF_DR);
      break;
    default:
      SET_BIT(rf_setup_register, RF_DR);
  }

  // rf_output_power
  switch (device->rf_output_power) {
    case nrf24l01_rf_output_power_minus18dbm:
      CLEAR_BIT(rf_setup_register, RF_PWR);
      break;
    case nrf24l01_rf_output_power_minus12dbm:
      SET_BIT(rf_setup_register, RF_PWR0);
      CLEAR_BIT(rf_setup_register, RF_PWR1);
      break;
    case nrf24l01_rf_output_power_minus6dbm:
      SET_BIT(rf_setup_register, RF_PWR1);
      CLEAR_BIT(rf_setup_register, RF_PWR0);
      break;
    case nrf24l01_rf_output_power_0dbm:
      SET_BIT(rf_setup_register, RF_PWR);
      break;
    default:
      SET_BIT(rf_setup_register, RF_PWR);
  }

  // setup lna gain
  if (device->setup_lna_gain)
    SET_BIT(rf_setup_register, LNA_HCURR);
  else
    CLEAR_BIT(rf_setup_register, LNA_HCURR);

  // write new configuration to rf_setup register
  nrf24l01_write_register(device, RF_SETUP, &rf_setup_register, 1);

  // data pipes
  nrf24l01_init_data_pipe(device, 0);
  nrf24l01_init_data_pipe(device, 1);
  nrf24l01_init_data_pipe(device, 2);
  nrf24l01_init_data_pipe(device, 3);
  nrf24l01_init_data_pipe(device, 4);
  nrf24l01_init_data_pipe(device, 5);

  // transmit address
  nrf24l01_write_register(device, TX_ADDR, device->transmit_address, device->address_width);

  // start timer
//  SET_BIT(device->timer->Instance->CR1, TIM_CR1_CEN);
  HAL_TIM_Base_Start(device->timer);
  return 0;
}

void nrf24l01_deinit(nrf24l01_device * device);

uint8_t nrf24l01_init_data_pipe(nrf24l01_device * device, uint8_t pipe_number)
{
  if (pipe_number > 5) return -1; // invalid pipe number
  if (device == NULL) return -1;
  nrf24l01_data_pipe* target_pipe = NULL;

  target_pipe = &device->data_pipe[pipe_number];

  // enable
  if (target_pipe->nrf24l01_data_pipe_enable)
    nrf24l01_data_pipe_enable(device, pipe_number, 1);
  else
    nrf24l01_data_pipe_enable(device, pipe_number, 0);

  // dynamic payload length
  if (target_pipe->nrf24l01_data_pipe_dyn_payload_length_enable)
    nrf24l01_data_pipe_dynamic_payload_length(device, pipe_number, 1);
  else
    nrf24l01_data_pipe_dynamic_payload_length(device, pipe_number, 0);

  // auto ack
  if (target_pipe->nrf24l01_data_pipe_auto_ack)
    nrf24l01_data_pipe_auto_ack(device, pipe_number, 1);
  else
    nrf24l01_data_pipe_auto_ack(device, pipe_number, 0);

  // receive address
  switch (pipe_number) {
    case 0:
    case 1:
      nrf24l01_data_pipe_address(device, pipe_number, target_pipe->nrf24l01_data_pipe_receive_address, device->address_width);
      break;
    default:
      nrf24l01_data_pipe_address(device, pipe_number, target_pipe->nrf24l01_data_pipe_receive_address, 1);
      break;
  }

  // payload width
  nrf24l01_data_pipe_payload_width(device, pipe_number, target_pipe->nrf24l01_data_pipe_payload_width);

  return 0;

}

void nrf24l01_delay(nrf24l01_device * device, uint16_t us){
  if (device == NULL || device->timer == NULL) return;
  __HAL_TIM_SET_COUNTER(device->timer, 0) ;
  while (__HAL_TIM_GET_COUNTER(device->timer) < us);
}

inline uint8_t nrf24l01_chip_select(nrf24l01_device * device){
  if (device == NULL) return -1;
  HAL_GPIO_WritePin(device->csn_port, device->csn_pin, (uint8_t )RESET);
  return 0;
}

inline uint8_t nrf24l01_chip_deselect(nrf24l01_device * device){
  if (device == NULL) return -1;
  HAL_GPIO_WritePin(device->csn_port, device->csn_pin, (uint8_t)SET);
  return 0;
}

inline uint8_t nrf24l01_chip_enable(nrf24l01_device * device){
  if (device == NULL) return -1;
  HAL_GPIO_WritePin(device->ce_port, device->ce_pin, (uint8_t)SET) ;
  return 0;
}

inline uint8_t nrf24l01_chip_disable(nrf24l01_device * device){
  if (device == NULL) return -1;
  HAL_GPIO_WritePin(device->ce_port, device->ce_pin, (uint8_t)RESET);
  return 0;
}

uint8_t nrf24l01_send_command(nrf24l01_device* device, uint8_t command){
  /* prepare data */
  if (device == NULL) return -1;
  uint8_t status_register = 0;

  uint8_t  spi_tx_payload = command;
  uint8_t  spi_rx_payload = 0;
  spi_tx_payload = command;

  /* perform spi transmit */
  nrf24l01_chip_select(device);
  HAL_SPI_TransmitReceive(device->spi, &spi_tx_payload, &spi_rx_payload, 1, 100) ;
  nrf24l01_chip_deselect(device);

  status_register = spi_rx_payload;

  return status_register;
}

uint8_t nrf24l01_read_register(nrf24l01_device * device, uint8_t reg, uint8_t* data, uint16_t length){
  if (reg > 0x1d) return -1; // invalid register address
  if (data == NULL) return -1; // invalid pointer
  if (device == NULL) return -1;

  uint8_t status_register = 0;
  uint8_t command = reg | R_REGISTER;

  uint8_t* spi_tx_payload = (uint8_t*) malloc(length + 1);
  uint8_t* spi_rx_payload = (uint8_t*) malloc(length + 1);
  spi_tx_payload[0] = command;
  memset(spi_tx_payload + 1, 0xff, length);

  nrf24l01_chip_select(device);
  HAL_SPI_TransmitReceive(device->spi, spi_tx_payload, spi_rx_payload, length + 1, 100);
  nrf24l01_chip_deselect(device);

  status_register = spi_rx_payload[0];
  memcpy(data, spi_rx_payload + 1, length);

  free(spi_rx_payload);
  free(spi_tx_payload);

  return status_register;
}

uint8_t nrf24l01_write_register(nrf24l01_device * device, uint8_t reg, uint8_t* data, uint16_t length){
  if (reg > 0x1d) return -1; // invalid register address
  else if (HAL_GPIO_ReadPin(device->ce_port, device->ce_pin)) return -1; // invalid mode
  else if (data == NULL) return  -1; // invalid pointer
  else if (device == NULL) return -1;


  uint8_t status_register = 0 ;
  uint8_t command = reg | W_REGISTER;

  uint8_t* spi_tx_payload =(uint8_t *) malloc(length + 1);
  uint8_t* spi_rx_payload =(uint8_t *) malloc(length + 1);
  spi_tx_payload[0] = command;
  memcpy(spi_tx_payload + 1, data, length);

  nrf24l01_chip_select(device);
  HAL_SPI_TransmitReceive(device->spi, spi_tx_payload, spi_rx_payload, length + 1, 100);
  nrf24l01_chip_deselect(device);

  status_register = spi_rx_payload[0];

  free(spi_rx_payload);
  free(spi_tx_payload);

  return status_register;
}

uint8_t nrf24l01_read_rx_payload(nrf24l01_device * device, uint8_t* data, uint16_t length){
  uint8_t config_register = 0, status_register = 0;
  if (device == NULL) return -1;
  nrf24l01_read_register(device, CONFIG, &config_register, 1);
  if (length < 1 || length > 32) return -1; // invalid payload length
  if (data == NULL) return  -1; // invalid pointer

  uint8_t* spi_tx_payload = (uint8_t*)malloc(length + 1);
  uint8_t* spi_rx_payload = (uint8_t*)malloc(length + 1);
  spi_tx_payload[0] = R_RX_PAYLOAD;
  memset(spi_tx_payload + 1, 0xff, length);

  nrf24l01_chip_select(device);
  HAL_SPI_TransmitReceive(device->spi, spi_tx_payload, spi_rx_payload, length + 1, 100);
  nrf24l01_chip_deselect(device);

  status_register = spi_rx_payload[0];
  memcpy(data, spi_rx_payload + 1, length);

  free(spi_rx_payload);
  free(spi_tx_payload);

  return status_register;
}

uint8_t nrf24l01_write_tx_payload(nrf24l01_device * device, uint8_t* data , uint16_t length){
  uint8_t status_register = 0;

  if (length < 1 || length > 32) return -1; // invalid payload length
  if (data == NULL) return  -1; // invalid pointer;
  if (device == NULL) return -1;

  uint8_t* spi_tx_payload = (uint8_t*)malloc(length + 1);
  uint8_t* spi_rx_payload = (uint8_t*)malloc(length + 1);
  spi_tx_payload[0] = W_TX_PAYLOAD;
  memcpy(spi_tx_payload + 1, data, length);

  nrf24l01_chip_select(device);
  HAL_SPI_TransmitReceive(device->spi, spi_tx_payload, spi_rx_payload, length + 1, 100);
  nrf24l01_chip_deselect(device);

  status_register = spi_rx_payload[0];

  free(spi_tx_payload);
  free(spi_rx_payload);

  return status_register;
}

uint8_t nrf24l01_flush_tx(nrf24l01_device * device){
  uint8_t status_register = 0;
  if (device == NULL) return -1;
  status_register = nrf24l01_send_command(device, FLUSH_TX);
  return status_register;
}

uint8_t nrf24l01_flush_rx(nrf24l01_device * device)
{
  uint8_t status_register = 0;
  if (device == NULL) return -1;
  status_register = nrf24l01_send_command(device, FLUSH_RX);
  return status_register;
}

uint8_t nrf24l01_reuse_tx_payload(nrf24l01_device * device){
  uint8_t config_register = 0, status_register = 0;
  if (device == NULL) return -1;
  nrf24l01_read_register(device, CONFIG, &config_register, 1);

  if (config_register & PRIM_RX) return  -1 ; // invalid device

  status_register = nrf24l01_send_command(device, REUSE_TX_PL);

  return status_register;
}

uint8_t nrf24l01_activate_extra_features(nrf24l01_device * device){
  uint8_t status_register = 0;
  if (device == NULL) return -1;

  uint8_t spi_tx_payload[2] = {ACTIVATE, 0x73};
  uint8_t spi_rx_payload[2];

  nrf24l01_chip_select(device);
  HAL_SPI_TransmitReceive(device->spi, spi_tx_payload, spi_rx_payload, 2, 100);
  nrf24l01_chip_deselect(device);

  status_register = spi_rx_payload[0];
  return status_register;
}

uint8_t nrf24l01_read_rx_payload_width(nrf24l01_device * device, uint8_t* data){
  uint8_t status_register;
  if (device == NULL) return -1;

  uint8_t spi_tx_payload[2] = {R_RX_PL_WID, 0xff};
  uint8_t spi_rx_payload[2] = {0, 0};
  spi_tx_payload[0] = R_RX_PL_WID;
  spi_tx_payload[1] = 0xff;

  nrf24l01_chip_select(device);
  HAL_SPI_TransmitReceive(device->spi, spi_tx_payload, spi_rx_payload, 2, 100);
  nrf24l01_chip_deselect(device);

  status_register = spi_rx_payload[0];
  data[0] = spi_rx_payload[1];

  return status_register;
}

uint8_t nrf24l01_write_ack_payload(nrf24l01_device * device, uint8_t* data, uint8_t pipe, uint16_t length){
  uint8_t config_register = 0, status_register = 0;
  if (device == NULL) return -1;
  nrf24l01_read_register(device, CONFIG, &config_register, 1);

  if (!(config_register & PWR_UP && config_register & PRIM_RX && HAL_GPIO_ReadPin(device->ce_port, device->ce_pin))) return -1; // invalid mode
  if (pipe > 5) return -1; // invalid pipe number

  uint8_t* spi_tx_payload = (uint8_t*)malloc(length + 1);
  uint8_t* spi_rx_payload = (uint8_t*)malloc(length + 1);
  spi_tx_payload[0] = W_ACK_PAYLOAD | pipe;
  memcpy(spi_tx_payload + 1, data, length);

  nrf24l01_chip_select(device);
  HAL_SPI_TransmitReceive(device->spi, spi_tx_payload, spi_rx_payload, length+1, 100);
  nrf24l01_chip_deselect(device);

  status_register = spi_rx_payload[0];

  free(spi_rx_payload);
  free(spi_tx_payload);

  return status_register;
}

uint8_t nrf24l01_write_tx_payload_no_ack(nrf24l01_device * device, uint8_t * data, uint16_t length){
  uint8_t config_register = 0, status_register = 0;
  if (device == NULL) return -1;
  nrf24l01_read_register(device, CONFIG, &config_register, 1);

  if (!(config_register & PWR_UP && ~config_register & PRIM_RX && HAL_GPIO_ReadPin(device->ce_port, device->ce_pin))) return -1; // invalid mode
  if (length < 1 || length > 32) return -1; // invalid length

  uint8_t* spi_tx_payload = (uint8_t*)malloc(length + 1);
  uint8_t* spi_rx_payload = (uint8_t*)malloc(length + 1);
  spi_tx_payload[0] = W_TX_PAYLOAD_NOACK;
  memcpy(spi_tx_payload + 1, data, length);

  nrf24l01_chip_select(device);
  HAL_SPI_TransmitReceive(device->spi, spi_tx_payload, spi_rx_payload, length + 1, 100);
  nrf24l01_chip_deselect(device);

  status_register = spi_rx_payload[0];

  free(spi_tx_payload);
  free(spi_rx_payload);

  return status_register;
}

uint8_t nrf24l01_nop(nrf24l01_device * device){
  if (device == NULL) return -1;
  return nrf24l01_send_command(device, NOP);
}

uint8_t nrf24l01_power_up(nrf24l01_device * device){
  uint8_t config_register = 0, status_register = 0;
  if (device == NULL) return -1;
  nrf24l01_read_register(device, CONFIG, &config_register, 1);
  config_register |= PWR_UP;
  status_register = nrf24l01_write_register(device, CONFIG, &config_register, 1);

  device->power_up = 1;

  HAL_Delay(2);

  return status_register;
}

uint8_t nrf24l01_power_down(nrf24l01_device * device){
  uint8_t config_register = 0, status_register = 0;
  if (device == NULL) return -1;
  nrf24l01_read_register(device, CONFIG, &config_register, 1);
  config_register &= ~PWR_UP;
  status_register = nrf24l01_write_register(device, CONFIG, &config_register, 1);

  device->power_up = 0;

  return status_register;
}

uint8_t nrf24l01_transmit(nrf24l01_device * device){
  uint8_t config_register = 0, status_register = 0, fifo_status_register = 0;
  if (device == NULL) return -1;
  nrf24l01_read_register(device, CONFIG, &config_register, 1);
  nrf24l01_read_register(device, FIFO_STATUS, &fifo_status_register, 1);

  if (config_register & PRIM_RX) return -1; // invalid configuration
  if (fifo_status_register & TX_EMPTY) return -1; // tx fifo is empty

  nrf24l01_chip_enable(device);
  nrf24l01_delay(device, 130);
  nrf24l01_chip_disable(device);

  status_register = nrf24l01_nop(device);

  return status_register;
}

uint8_t nrf24l01_listen(nrf24l01_device * device){
  uint8_t config_register = 0, status_register = 0, fifo_status_register = 0;
  if (device == NULL) return -1;
  nrf24l01_read_register(device, CONFIG, &config_register, 1);
  nrf24l01_read_register(device, FIFO_STATUS, &fifo_status_register, 1);

  if (!(config_register & PRIM_RX)) return -1 ; // invalid configuration
  if (fifo_status_register & RX_FULL) return -1; // rx fifo is full

  nrf24l01_chip_enable(device);

  status_register = nrf24l01_nop(device);

  return status_register;
}

uint8_t nrf24l01_dynamic_payload_length(nrf24l01_device * device, uint8_t enable){
  uint8_t status_register = 0, feature_register = 0;
  if (device == NULL) return -1;
  nrf24l01_read_register(device, FEATURE, &feature_register, 1);

  if (enable)
    feature_register |= EN_DPL;
  else
    feature_register &= ~EN_DPL;

  status_register = nrf24l01_write_register(device, FEATURE, &feature_register, 1);

  return status_register;

}

uint8_t nrf24l01_payload_with_ack(nrf24l01_device * device, uint8_t enable){
  uint8_t status_register = 0, feature_register = 0;
  if (device == NULL) return -1;
  nrf24l01_read_register(device, FEATURE, &feature_register, 1);

  if (enable)
    feature_register |= EN_ACK_PAY;
  else
    feature_register &= ~EN_ACK_PAY;

  status_register = nrf24l01_write_register(device, FEATURE, &feature_register, 1);

  return status_register;
}

uint8_t nrf24l01_dynamic_ack(nrf24l01_device * device, uint8_t enable){
  uint8_t status_register = 0, feature_register = 0;
  if (device == NULL) return -1;
  nrf24l01_read_register(device, FEATURE, &feature_register, 1);

  if (enable)
    feature_register |= EN_DYN_ACK;
  else
    feature_register &= ~EN_DYN_ACK;

  status_register = nrf24l01_write_register(device, FEATURE, &feature_register, 1);

  return status_register;
}

uint8_t nrf24l01_data_pipe_dynamic_payload_length(nrf24l01_device * device, uint8_t pipe_number, uint8_t enable){
  if (pipe_number > 5) return -1; // invalid pipe number
  uint8_t dynpd_register = 0;
  if (device == NULL) return -1;
  nrf24l01_read_register(device, DYNPD, &dynpd_register, 1);

  device->data_pipe[pipe_number].nrf24l01_data_pipe_dyn_payload_length_enable = enable;
  if (enable){
    SET_BIT(dynpd_register, (1 << pipe_number));
  }
  else {
    CLEAR_BIT(dynpd_register, (1 << pipe_number));
  }

  nrf24l01_write_register(device, DYNPD, &dynpd_register, 1);

  return 0;
}

uint8_t nrf24l01_data_pipe_enable(nrf24l01_device * device, uint8_t pipe_number, uint8_t enable){
  if (pipe_number > 5) return -1; // invalid pipe number
  uint8_t en_rxaddr_register;
  if (device == NULL) return -1;
  nrf24l01_read_register(device, EN_RXADDR, &en_rxaddr_register, 1);

  device->data_pipe[pipe_number].nrf24l01_data_pipe_enable = enable;

  if (enable)
    SET_BIT(en_rxaddr_register, (1<< pipe_number));
  else
    CLEAR_BIT(en_rxaddr_register, (1 << pipe_number));

  nrf24l01_write_register(device, EN_RXADDR, &en_rxaddr_register, 1);

  return 0;
}

uint8_t nrf24l01_data_pipe_auto_ack(nrf24l01_device * device, uint8_t pipe_number, uint8_t enable){
  if (pipe_number > 5) return -1; // invalid pipe number
  uint8_t en_aa_register;
  if (device == NULL) return -1;
  nrf24l01_read_register(device, EN_AA, &en_aa_register, 1);

  device->data_pipe[pipe_number].nrf24l01_data_pipe_auto_ack = enable;

  if (enable)
    SET_BIT(en_aa_register, (1<< pipe_number));
  else
    CLEAR_BIT(en_aa_register, (1 << pipe_number));

  nrf24l01_write_register(device, EN_AA, &en_aa_register, 1);

  return 0;
}

uint8_t nrf24l01_data_pipe_address(nrf24l01_device * device, uint8_t pipe_number, uint8_t * address, uint8_t length){
  if (pipe_number > 5) return -1; // invalid pipe number
  else if ( (length > 1 && pipe_number > 1) || (length < 3 && pipe_number <= 1) ) return -1; // invalid address length
  else if (length > 1 && length != device->address_width) return -1; // invalid address length
  if (device == NULL || address == NULL)  return  -1;

  uint8_t *temp = NULL;
  if (device->data_pipe[pipe_number].nrf24l01_data_pipe_receive_address != NULL){
    temp = device->data_pipe[pipe_number].nrf24l01_data_pipe_receive_address;
  }
  // allocate new address array
  device->data_pipe[pipe_number].nrf24l01_data_pipe_receive_address = (uint8_t*)malloc(length);
  memcpy(device->data_pipe[pipe_number].nrf24l01_data_pipe_receive_address, address, length);
  nrf24l01_write_register(device, RX_ADDR_P0 + pipe_number, device->data_pipe[pipe_number].nrf24l01_data_pipe_receive_address, length);

  if (temp != NULL) {
    free(temp);
    temp = NULL;
  }
  return 0;
}

uint8_t nrf24l01_data_pipe_payload_width(nrf24l01_device * device, uint8_t pipe_number, uint8_t width){
  if (pipe_number > 5) return -1; // invalid pipe number
  if (width > 32) return -1; // invalid payload width
  if (device == NULL) return -1;
  nrf24l01_write_register(device, RX_PW_P0 + pipe_number, &width, 1);

  return 0;
}

uint8_t nrf24l01_interrupt(nrf24l01_device * device, nrf24l01_irq irq, uint8_t enable){
  uint8_t status_register = 0, config_register = 0;
  uint8_t interrupt = 0;
  if (device == NULL) return -1;

  switch (irq) {
    case nrf24l01_irq_rx_data_ready:
      interrupt = MASK_RX_DR;
      break;
    case nrf24l01_irq_tx_data_sent:
      interrupt = MASK_TX_DS;
      break;
    case nrf24l01_maximum_retransmitted:
      interrupt = MASK_MAX_RT;
      break;
    default:
      return -1; // invalid irq number
  }

  nrf24l01_read_register(device, CONFIG, &config_register, 1);

  if (enable)
    CLEAR_BIT(config_register, interrupt);
  else
    SET_BIT(config_register, interrupt);

  status_register = nrf24l01_write_register(device, CONFIG, &config_register, 1);

  return status_register;
}

uint8_t nrf24l01_clear_interrupt_flag(nrf24l01_device * device, nrf24l01_irq irq){
  if (device == NULL)  return -1;
  uint8_t status_register = 0, eraser = 0;

  if (HAL_GPIO_ReadPin(NRF24L01_CE_GPIO_Port, NRF24L01_CE_Pin)){
    /* disable module before clearing inerruput flag */
    nrf24l01_chip_disable(device);
  }

  switch (irq) {
    case nrf24l01_irq_tx_data_sent:
      eraser = TX_DS;
      break;
    case nrf24l01_irq_rx_data_ready:
      eraser = RX_DR;
      break;
    case nrf24l01_maximum_retransmitted:
      eraser = MAX_RT;
      break;
    default:
      return -1; /* invalid interrupt number */
  }

  status_register = nrf24l01_write_register(device, STATUS, &eraser, 1);

  return status_register;
}
