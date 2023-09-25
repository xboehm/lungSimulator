#include "Uart.hpp"
#include "charconv"

Uart::Uart(UART_HandleTypeDef* handle, DMA_HandleTypeDef* dma)
	: m_handle {handle}, m_dma {dma}
{
}

void Uart::send(std::span<const std::byte> const buf) {
	HAL_UART_Transmit(m_handle, reinterpret_cast<const uint8_t *>(buf.data()), buf.size(), HAL_MAX_DELAY);
}

void Uart::send1(char* buf, uint16_t size){
	HAL_UART_Transmit(m_handle, (uint8_t*)buf, size, 100);
}

void Uart::receive(uint8_t* buf, uint16_t size) {
	HAL_UART_Receive(m_handle, buf, size, 3000);
}

void Uart::receiveIT(uint8_t* buf, uint16_t size) {
	HAL_UART_Receive_IT(m_handle, buf, size);
}

void Uart::receiveToIdleDMA(uint8_t* buf, uint16_t size){
	HAL_UARTEx_ReceiveToIdle_DMA(m_handle, buf, size);
	//interrupt for half transfer complete must be disabled every single time
	__HAL_DMA_DISABLE_IT(m_dma, DMA_IT_HT);
}

Uart& operator<< (Uart& out, const int number){
	char buf[10] {};
	auto result {std::to_chars(buf, &buf[std::size(buf)], number)};
	if(result.ec == std::errc()){
		out.send(std::as_bytes(std::span{buf, result.ptr}));
	}
	return out;
}

Uart& operator<< (Uart& out, const float number){
	char buf[10] {};
	auto result {std::to_chars(buf, &buf[std::size(buf)], number)};
	if(result.ec == std::errc()){
			out.send(std::as_bytes(std::span{buf, result.ptr}));
	}
	return out;
}
