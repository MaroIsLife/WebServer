#pragma once

#include "library.hpp"
#include <limits.h>

struct Redirection
{
	int status;
	std::string url;
	Redirection();
};

struct ErrorPage
{
	int status;
	std::string path;
	ErrorPage();
};

struct Location
{
	std::string path;
	std::string root;
	std::vector<std::string> index;
	std::string upload;
	std::string cgi;
	bool auto_index;
	Redirection redirection;
};

struct Server
{
	std::string host;
	std::vector<std::string> port;
	std::vector<std::string> server_name;
	long long client_max_body_size;
	std::vector<Location> location;
	std::vector<ErrorPage> error_page;

	Server() : client_max_body_size(std::numeric_limits<long long>::max()) {};

};

class ConfigFile
{
private:
	std::ifstream file;
	std::string previous_line;
	bool is_line;
	bool is_eof;

	ConfigFile(void);
	ConfigFile(const ConfigFile &);
	ConfigFile &operator=(const ConfigFile &);

	void check_server(void);
	void check_location(void);
	void check_redirection(void);

	void set_server(void);
	void set_location(void);
	void set_redirection(void);
	void set_error_page(void);

	void duplicate_key(bool);

	bool read_line(std::string &line);

public:
	~ConfigFile(void);
	ConfigFile(const char *);
	std::vector<Server> configuration;
};