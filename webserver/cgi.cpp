#include "response.hpp"
#include <fcntl.h>

void Response::cgi_method(Request &req, Server &server, std::string index)
{
    int pipefd[2];
    std::string s_cgi_content("");
    std::string script  = req.header.path;
    if (!replace_in_uri(script, _server_location.path, _server_location.root))
		throw std::runtime_error("replacing in uri failes!");
    script = index.empty() ? script : index;
    size_t in = s_content_type.find("/");
    size_t len = s_content_type.find("\r\n");
    std::string ext = s_content_type.substr(in + 1, (len - in) - 1);
    ext = _server_location.cgi[ext];
    // check here if not found return not allowed gci return not implemented
    char *const parm[] = {(char *const )ext.c_str(), (char *const )script.c_str(), NULL};
    pid_t pid = fork();
    std::string query_string = "";

    pipe(pipefd);
    if (pid == 0)
    {
        // child process
        if (req.header.method == "GET")
        {
            close(pipefd[0]);
            dup2(pipefd[1], 1);
            close(pipefd[1]);
        }
        else
        {
            // dup file to stdin

            int fd = open(req.file_name.c_str(), O_RDONLY);

            if (fd < 0)
            {
                throw std::runtime_error("505 Internal Server Error " + (req.file_name) + "  << " + strerror(errno) );
            }
            setenv("CONTENT_TYPE", req.header.content_type.c_str(),1);
            setenv("CONTENT_LENGTH", to_string(req.header.content_length).c_str(),1);
            dup2(fd, 0);
            close(pipefd[0]);
            dup2(pipefd[1], 1);
            close(pipefd[1]);
        }
        setenv("REQUEST_METHOD",req.header.method.c_str(), 1);
        setenv("SCRIPT_FILENAME",script.c_str(),1);
        setenv("SCRIPT_NAME",req.header.path.c_str(),1);
        setenv("QUERY_STRING",req.header.q_string.c_str(),1);             
        setenv("SERVER_PROTOCOL","HTTP/1.1",1);
        setenv("SERVER_SOFTWARE","Njinx",1);
        setenv("GATEWAY_INTERFACE","CGI/1.1",1);

        // execv inherits the environment variables of the parent process

        execv((char *const )ext.c_str(), parm);
        exit(0);
    }   
    else
    {
        // parent process
        close(pipefd[1]);
        char buf[1024];
        int n;
        while ((n = read(pipefd[0], buf, 1024)) > 0)
        {
            s_cgi_content.append(buf, n);
        }
        close(pipefd[0]);
        waitpid(pid, NULL, 0);
    }
    
    std::string line;
    std::stringstream ss(s_cgi_content);


    location =  "" ;
    while (std::getline(ss, line))
    {
        std::cout << line << std::endl;
        if (line.find("X-Powered-By:") != std::string::npos)
            continue;
        if (line.find("Status:") != std::string::npos)
        {
            std::string status = line.substr(line.find(":") + 1);
            s_status = status;
        }
        else if (line.find("Content-type:") != std::string::npos)
        {
            std::string content_type = line.substr(line.find(":") + 1);
            s_content_type = content_type + "\r\n";
        }
        else if (line.find("location:") != std::string::npos)
        {
            location = line + "\r\n";
        }
        else if (line.find("Content-length:") != std::string::npos)
        {
            std::string content_length = line.substr(line.find(":") + 1);
            s_content_length = content_length;
        }
        else
            s_content.append(line);
    
    }
    s_content_length = to_string(s_content.length());    
}
