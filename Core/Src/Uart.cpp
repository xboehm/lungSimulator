#include "Uart.hpp"

Uart::Uart(UART_HandleTypeDef& handle)
	: m_handle (handle)
{
}

void Uart::send(std::span<const std::byte> const buf) {
	HAL_UART_Transmit(std::addressof(m_handle), reinterpret_cast<const uint8_t *>(buf.data()), buf.size(), HAL_MAX_DELAY);
}
