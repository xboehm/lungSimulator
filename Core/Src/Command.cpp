#include "Command.hpp"

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

CmdFreq::CmdFreq(Application& app, std::array<char, 10>& payload)
    : Command("freq", "Scale pattern to different breathing frequency"), m_app(app), m_payload(payload) {}

void CmdFreq::execute() {
    m_app.CLIfreq(m_payload);
}

CmdVol::CmdVol(Application& app, std::array<char, 10>& payload)
    : Command("vol", "Scale pattern to different volume"), m_app(app), m_payload(payload) {}

void CmdVol::execute() {
    m_app.CLIvol(m_payload);
}

CmdChange::CmdChange(Application& app, std::array<char, 10>& payload)
	: Command("change", "Change the cylinder of the simulator"), m_app(app), m_payload(payload) {}

void CmdChange::execute() {
	m_app.CLIchange(m_payload);
}
