#include "Command.hpp"
#include "Application.hpp"

Command::Command(std::string_view _name, std::string_view _help)
		: name(_name), help(_help) {}

CmdVersion::CmdVersion(Application& app)
    : Command("version", "Display firmware version"),  m_app(app) {}

void CmdVersion::execute() {
    m_app.CLIversion();
}

CmdBlink::CmdBlink(Application& app)
	: Command("blink", "Blinks LD4 at 10Hz"), m_app(app)  {}

void CmdBlink::execute()  {
    m_app.CLIblink();
}

CmdBreathe::CmdBreathe(Application& app)
	: Command("breathe", "Start the breathing cycle"), m_app(app)  {}

void CmdBreathe::execute()  {
    m_app.CLIbreathe();
}

CmdSelect::CmdSelect(Application& app)
	: Command("select", "Select a breathing pattern to run"), m_app(app)  {}

void CmdSelect::execute()  {
    m_app.CLIselect();
}

CmdPause::CmdPause(Application& app)
	: Command("pause", "Suspend the breathing cycle"), m_app(app)  {}

void CmdPause::execute()  {
    m_app.CLIpause();
}

CmdEndpos::CmdEndpos(Application& app)
	: Command("endpos", "Get the end positions by driving to the end switches"), m_app(app) {}

void CmdEndpos::execute() {
	m_app.m_getEndPositions();
}
