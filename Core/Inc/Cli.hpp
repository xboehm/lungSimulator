#pragma once

#include "GlobalConf.hpp"
#include "Command.hpp"
#include "Application.hpp"
#include "Uart.hpp"
#include <cstddef>
#include <array>
#include <string_view>

template<std::size_t N>
class Cli{
public:
	Cli(std::array<Command*, N>& commands, Uart& uart, uint16_t& uartSize, Application& appl)
	        : m_commands(commands), m_uart(uart), m_uartSize(uartSize), m_application(appl) {}

private:
	std::array<Command*, N>& m_commands;
	Uart& m_uart;
	uint16_t& m_uartSize;
	Application& m_application;
	uint8_t m_rxBuf[constants::RxBufSize] {};
	char m_readBuf[constants::RxBufSize] {};
	std::array<char, 8> m_payload {};
	bool m_cmdFound {false};


public:
	std::array<char, 8>& getPayload(){
	    return m_payload;
	  }

	void listen()  {
		 m_uart.send(std::as_bytes(std::span{"<Enter command> "}));
		 m_uart.send(std::as_bytes(std::span{m_application.m_printState()}));
		 m_uart.receiveToIdleDMA(m_rxBuf, std::size(m_rxBuf));
	}

	//decode received bytes, start to listen again immediately after m_rxBuf is read and execute command
	void decode() {
		//copy m_rxBuf to readBuf to ensure that input string_view views a string that won't change
		std::copy(m_rxBuf, m_rxBuf+m_uartSize, m_readBuf);
		//don't change m_readBuf anymore!! treat it as const
		std::string_view input {m_readBuf};
		//'help macro' prints all available commands with their description
		if(input == "help"){
			m_cmdFound = true;
			m_uart.send(std::as_bytes(std::span{"Available commands:\r\n"}));
			for(auto cmd: m_commands) {
				m_uart.send(std::as_bytes(std::span{"Name: "}));
				m_uart.send(std::as_bytes(std::span{cmd->name}));
				m_uart.send(std::as_bytes(std::span{" ("}));
				m_uart.send(std::as_bytes(std::span{cmd->help}));
				m_uart.send(std::as_bytes(std::span{")\r\n"}));
			}
		}
		else{
			int hyphenPos = input.find('-');
			//if Command has a payload
			if(hyphenPos != -1){
				std::copy(input.begin()+hyphenPos+1, input.end(), m_payload.data());
				input.remove_suffix(input.size()-hyphenPos);
			}
			//check if the input matches a known command name. If so, execute it
			for(auto cmd: m_commands){
				if(input == cmd->name){
					m_cmdFound = true;
					cmd->execute();
					break;
				}
			}
		}
		if(!m_cmdFound) {
			m_uart.send(std::as_bytes(std::span{"Command not found.\r\n"}));
		}
		else {
			m_cmdFound = false;
		}
		//start listening again
		listen();
		//clear buffers
		for(std::size_t i{0}; i < m_uartSize; ++i) {
			m_readBuf[i] = '\0';
		}
		m_payload.fill(0);
	}
};

