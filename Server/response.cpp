#include "response.hpp"


Response::Response()
{


}

void Response::set_map()
{
	map_status["200"] = " 200 OK";
    map_status["201"] = " 201 Created";
    map_status["202"] = " 202 Accepted";
    map_status["204"] = " 204 No Content";
    map_status["206"] = " 206 Partial Content";
    map_status["301"] = " 301 Moved Permanently";
    map_status["302"] = " 302 Found";
    map_status["304"] = " 304 Not Modified";
    map_status["400"] = " 400 Bad Request";
    map_status["401"] = " 401 Unauthorized";
    map_status["403"] = " 403 Forbidden";
    map_status["404"] = " 404 Not Found";
    map_status["405"] = " 405 Method Not Allowed";
    map_status["406"] = " 406 Not Acceptable";
    map_status["408"] = " 408 Request Timeout";
    map_status["411"] = " 411 Length Required";
    map_status["413"] = " 413 Payload Too Large";
    map_status["414"] = " 414 URI Too Long";
    map_status["415"] = " 415 Unsupported Media Type";
    map_status["429"] = " 429 Too Many Requests";
    map_status["431"] = " 431 Request Header Fields Too Large";
    map_status["500"] = " 500 Internal Server Error";
	map_status["501"] = " Not Implemented";
    map_status["502"] = " 502 Bad Gateway";
    map_status["503"] = " 503 Service Unavailable";
    map_status["504"] = " 504 Gateway Timeout";
    map_status["505"] = " 505 HTTP Version Not Supported";
}

std::string Response::getStatus(std::string const &code)
{
	return (map_status[code]);
}

void Response::response_error(Request &req)
{
	s_content_type = get_content_type("public/index.html") + "\n";
	s_content = "<html><head><link rel=\"stylesheet\" href=\"styles.css\"></head><body><div id=\"main\"><div class=\"fof\"><h1>Error " + req.header.status + "</h1><h2>" + getStatus(req.header.status) + "</h2><img src=\"finawa.gif\" loop=infinite></div></div></body></html>" + "\n";

	s_content_length = std::to_string(s_content.length());
}

void Response::get_method(Request &req, std::vector<Server *> &server)
{

	if (req.header.path == "/") // if path == /
	{
		std::ifstream file2;
		for (int i = 0; i < server[0]->location[0]->index.size();i++) // Looping over config index
		{
			file2.open(server[0]->location[0]->root + "/" + server[0]->location[0]->index[i]);
			
			if (file2.is_open()) //If any index file opens
			{
				s_content_type = get_content_type(server[0]->location[0]->root + "/" + server[0]->location[0]->index[i]) + "\n";
				std::stringstream s;
				s << file2.rdbuf();
				s_content = s.str();
				s.seekg(0, std::ios::end);
				s_content_length += s.tellg();
				// s_content_length = std::to_string(s_content.length());
				break ;
			}
		}
		if (file2.is_open())
			file2.close();
		else // no files from index found
		{
			req.header.status = "403";
			response_error(req);
		}
	}
	else //If path isnt "/"
	{
		std::ifstream file1(server[0]->location[0]->root + req.header.path);
		if (file1.is_open()) // if we have permission to open the file
		{
			s_content_type = get_content_type(req.header.path) + "\n";
			DIR *dir;
			
			if ((dir = opendir((server[0]->location[0]->root  + req.header.path).c_str()))) // If it's a Directory 
				if_directory(req, dir, server);
			else // if its a file
			{
				if (s_content_type == "application/octet-stream\n")
				{
					// CGI GOES HERE AND WHENEVER I CALL GET_CONTENT_TYPE 
					// PREFERABLY MAKE IT IN A CALLING FUNCTION
					std::cout << "CGI" << std::endl;
				}
				else
				{
					std::stringstream s;
					s << file1.rdbuf();
					s_content = s.str();
					s.seekg(0, std::ios::end); // this puts a pointer at the end of the stream
					s_content_length += s.tellg(); // and this returns the position of the pointer aka the length of the stream
					file1.close();
				}
			}
		}
		else //Permission error
		{
			req.header.status = "403";
			response_error(req);
		}
	}

}

Response::Response (Request req, std::vector<Server *> &server)
{
	set_map();
    s_http = "HTTP/1.1" ;
	s_status = map_status[req.header.status];
    s_content_length = "";
    s_content = "";
    content_length = 0;
	std::cout << "Header " + req.header.status << "Path: " << req.header.path << std::endl;
	
	if (req.header.status != "201" && req.header.status != "200")
		response_error(req);
	else if (req.header.method == "GET")
		get_method(req, server);
}

std::string Response::get_response()
{
	std::string response = s_http + s_status + "\n" + "Content-type: " + s_content_type + "Content-length: " + s_content_length + "\n\n" + s_content;

	return response;
}

void Response::open_directory(DIR *dir, Request req_obj)
{
	std::vector<std::string> files;
	struct dirent *diread;

	while ((diread = readdir(dir)))
		files.push_back(diread->d_name);
	closedir(dir);
	s_content_type = get_content_type("public/index.html") + "\n";
	s_content = "<html><head><link rel=\"stylesheet\" href=\"autoindex.css\"></head><body><h1 id=\"auto\">Index of " + req_obj.header.path + "</h1><ul>";

	for (int i = 0; i < files.size(); i++)
		s_content += "<li class=\"li\"><a href=\"" + req_obj.header.path + "/" + files[i] + "\"><p>" + files[i] + "</p></a></li>";
	s_content_length = std::to_string(s_content.length());
}


void Response::if_directory(Request &req, DIR *dir, std::vector<Server *> &server)
{

	std::ifstream file2;
	for (int i = 0; i < server[0]->location[0]->index.size() ; i++)
	{
		
		file2.open(server[0]->location[0]->root +  "/" + server[0]->location[0]->index[i]);
		if (file2.is_open())
		{
			
			s_content_type = get_content_type(server[0]->location[0]->root +  "/" + server[0]->location[0]->index[i]) + "\n";
			std::stringstream s;
			s << file2.rdbuf();
			s_content = s.str();
			s.seekg(0, std::ios::end); // this puts a pointer at the end of the stream
			s_content_length += s.tellg();
			//s_content_length += std::to_string(s_content.length());
			break ;
		}
	}

	if (file2.is_open())
		file2.close();
	else if (server[0]->location[0]->auto_index)
		open_directory(dir, req);
	else
	{
		req.header.status = "403";
		response_error(req);
	}

}