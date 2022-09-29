# WebServer
## because reinventing the wheel is an option 
### Features:
- HTTP 1.1 compliant 
- Never block and client can be bounced properly
- Non-blocking I/O operations
- Listen to multiple IPs
- Listen to multiple ports
- Methods: GET, POST, DELETE
- Able to upload files
- Able to serve a fully static website.
- Supports multiple CGIs.
- Custom error pages 
- High availablity
- Resilient! Server never dies :slightly_smiling_face:
- `poll()` as the default event monitor
### Usage: 
``` bash
$ make -C server/
$ ./webserver server/webserver.conf
```

### Configue file setup:  
 ```yaml
 server:
	host: 127.0.0.1
	port: 5050
	server_name: example.com
	location:
		allowed_method: get delete post
		path: "/route"
		root: "/root"
		index: index.html index.php ..
		upload: "upload/path"
		auto_index: on || off
		cgi:
			php: "path/to/php-cgi"
			py: "path/to/python"
			perl: "path/to/perl"
		redirection:
			status: redirection_code
			url: 'url'
	error_page:
        status_code: custom_error_page.html
```
**Authors:**  
*[Ayoub Maatouch](https://github.com/AyoubMaatouch)*  
*[Marouane Ougnou](https://github.com/MaroIsLife)*  
*[ElMehdi Mallah](https://github.com/mallah-elmehdi)*  
