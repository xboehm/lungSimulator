#pragma once

#include "Application.hpp"
#include "CustomTypes.hpp"
#include <string_view>

//class Application;
class Command{
public:
	Command(std::string_view _name, std::string_view _help);
	const std::string_view name {};
	const std::string_view help {};
    virtual void execute()  =  0;
};

class CmdVersion : public Command {
public:
  CmdVersion(Application& app);
  void execute() override;
private:
  Application& m_app;
};

class CmdBlink : public Command {
public:
  CmdBlink(Application& app);
  void execute() override;
private:
  Application& m_app;
};

class CmdBreathe : public Command {
public:
  CmdBreathe(Application& app);
  void execute() override;
private:
  Application& m_app;
};

class CmdSelect : public Command {
public:
	CmdSelect(Application& app);
	  void execute() override;
	private:
	  Application& m_app;
};

class CmdPause : public Command {
public:
  CmdPause(Application& app);
  void execute() override;
private:
  Application& m_app;
};

class CmdEndpos : public Command {
public:
  CmdEndpos(Application& app);
  void execute() override;
private:
  Application& m_app;
};

class CmdFreq : public Command{
public:
  CmdFreq(Application& app, CommandPayload& payload);
  void execute() override;
private:
  Application& m_app;
  CommandPayload& m_payload;
};

class CmdVol : public Command{
public:
  CmdVol(Application& app, CommandPayload& payload);
  void execute() override;
private:
  Application& m_app;
  CommandPayload& m_payload;
};

class CmdChange : public Command{
public:
	CmdChange(Application& app, CommandPayload& payload);
	void execute() override;
private:
	Application& m_app;
	CommandPayload& m_payload;
};
