#pragma once

#include <string_view>

class Application;
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

class CmdPause : public Command {
public:
  CmdPause(Application& app);
  void execute() override;
private:
  Application& m_app;
};
