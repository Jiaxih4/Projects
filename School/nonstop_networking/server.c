/**
 * nonstop_networking
 * CS 341 - Fall 2023
 */
#include "common.h"
#include "format.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include "./includes/dictionary.h"
#include "./includes/vector.h"

typedef struct C_info {
	verb method;
	size_t ss[3];
	char ff[2][1024];
} C_info;

static int efd;
static dictionary  *dict;
static char* dk;
static size_t dkl;
static vector *fv;
static dictionary  *fs;


int error_testing(ssize_t rc, char* c);
size_t head_reader(int socket, char *c, size_t max);
void e_change(int cfd);
int change_lev(C_info * cinfo);
void set_path(C_info * cinfo, char* path, int len);
void get_w(size_t wc, size_t size, int cfd, FILE * rr);
size_t either(size_t one, size_t two);
void retrieve(int cfd, FILE* rr, char* filename);
int process_GET(int cfd, C_info *cinfo);
ssize_t find_index(C_info* cinfo, char* filename);
int process_DELETE(int cfd, C_info * cinfo, char *filename);
size_t find_size();
void print_list(int cfd);
void process_LIST(int cfd);
void cleanup(int cfd);
int process_PUT(int cfd, C_info* cinfo);
void process_cmd(int cfd, C_info *cinfo);
ssize_t add_file( size_t size, size_t rc, char* path, int cfd);
int do_put(int cfd, C_info *cinfo);
void fill_cinfo(verb method, C_info * cinfo, int size);
int lev2(char* header);
void read_header(int cfd, C_info *cinfo);
void write_error(int status, int cfd, const char* err, size_t size, int lev);
void do_error(C_info * cinfo, int cfd);
void run_client(int cfd);
int delete_file(char *filename);
void clear();
void freeall(vector*infos);
void shutdown_server();
int make_server(char* port);
void make_epoll(int socket_fd);
void run_server(char* port);

int error_testing(ssize_t rc, char* c) {
	if (rc == 0) {
        return 1;
    } 
	if (rc == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        return 1;
    }
	if (c[strlen(c) - 1] == '\n') {
		return 1;
	}
	if (rc == -1 && errno == EINTR) {
        return 2;
    } 
	if (rc == -1 && errno != EINTR) {
        return 1;
    }
	return 0;
}

size_t head_reader(int socket, char *c, size_t max){ 
	size_t  count = 0;
    ssize_t rc    = 0;
    while (count < max) {
        rc = read(socket, (void*)c, 1);
        int er = error_testing(rc, c);
		if (er == 1) {
			break;
		} else if (er == 2) {
			continue;
		}
        count  += rc;
        c += rc;
    }

    return count;
}

void e_change(int cfd) {
	struct epoll_event temp;
  	temp.events = EPOLLOUT;
  	temp.data.fd = cfd;
	epoll_ctl(efd, EPOLL_CTL_MOD, cfd, &temp);
}

int change_lev(C_info * cinfo) {
	if (cinfo->method == GET) {
		return 0;
	} else if (cinfo->method == PUT) {
		return 1;
	} else if (cinfo->method == DELETE) {
		return 2;
	} else if (cinfo->method == LIST) {
		return 3;
	}
	return -1;
}

void set_path(C_info * cinfo, char* path, int len) {
	memset(path, 0, len);
	sprintf(path, "%s/%s", dk, cinfo->ff[0]);
}

void get_w(size_t wc, size_t size, int cfd, FILE * rr) {
	while (1) {
		if (wc >= size) {
			break;
		}
		size_t diff = size - wc;
  		size_t ind = diff;
      	if( diff > 1024){
    		ind = 1024;
  		}
    	char buffer[ind + 1];
      	fread(buffer, 1, ind, rr);
    	write_socket(cfd, buffer, ind);
  		wc += ind;
    }
}

size_t either(size_t one, size_t two) {
	size_t output = one;
    if ( (one) > two){
		output = two;
  	}
	return output;
}

void retrieve(int cfd, FILE* rr, char* filename) {
	write_socket(cfd, "OK\n", 3);
	size_t size;
	size = *(size_t *) dictionary_get(fs, filename);
	write_socket(cfd, (char*)&size, sizeof(size_t));
	size_t wc = 0;
	while (1) {
		if (wc >= size) {
			break;
		}
		size_t ind = either(size - wc, 1024);
    	char c[ind + 1];
      	fread(c, 1, ind, rr);
    	write_socket(cfd, c, ind);
  		wc += ind;
    }
}

int process_GET(int cfd, C_info *cinfo) {
	int len = dkl + cinfo->ss[1] + 1;
	char path[len];
	set_path(cinfo, path, len);
	if ((access(path, F_OK) != 0)) {
		cinfo->ss[0] = -3;
		return 1;
	}
	FILE *r_mode = fopen(path, "r");
	retrieve(cfd, r_mode, cinfo->ff[0]);
	fclose(r_mode);
	return 0;
} 

ssize_t find_index(C_info* cinfo, char* filename) {
	ssize_t i = 0;
	int exist = 0;
	VECTOR_FOR_EACH(fv, name, {
		char *fn = (char*)name;
        if (!strcmp(fn, filename)) {
			exist = 1;
			break;
		}
		i++;
 	});

	if (exist == 0) {
		cinfo->ss[0] = -3;
		i = -1;
	}
	return i;
}

int process_DELETE(int cfd, C_info * cinfo, char *filename) {
	int len = dkl + cinfo->ss[1] + 2;
	char path[len];
	set_path(cinfo, path, len);
	int r = remove(path);
	if (r == -1) {
		cinfo->ss[0] = -3;
		return 1;
	}
	ssize_t i = find_index(cinfo, filename);
	if (i == -1) {
		return 1;
	}
	vector_erase(fv, i);
	dictionary_remove(fs, filename);
	write_socket(cfd, "OK\n", 3);
	return 0;
}

size_t find_size() {
	size_t size = 0;
	VECTOR_FOR_EACH(fv, name, {
	    size += strlen(name)+1;
	});
	if (size != 0) {
		size--;
	}
	return size;
}

void print_list(int cfd) {
	int count = 0;
	int size = vector_size(fv);
	VECTOR_FOR_EACH(fv, name, {
		write_socket(cfd, name, strlen(name));
		count++;
		if (count != size) {
			write_socket(cfd, "\n", 1);
		}
	});
}

void process_LIST(int cfd) {
	write_socket(cfd, "OK\n", 3);
	size_t size = find_size();
	write_socket(cfd, (char*)& size, sizeof(size_t));
	print_list(cfd);
}

void cleanup(int cfd) {
	epoll_ctl(efd, EPOLL_CTL_DEL, cfd, NULL);
  	free(dictionary_get(dict, &cfd));
	dictionary_remove(dict, &cfd);
  	shutdown(cfd, SHUT_RDWR);
	close(cfd);
}

int process_PUT(int cfd, C_info* cinfo) {
	if (do_put(cfd, cinfo) != 0) {
		cinfo->ss[0] = -2;
		e_change(cfd);
		return 1;
	}
	write_socket(cfd, "OK\n", 3);
	return 0;
}

void process_cmd(int cfd, C_info *cinfo) {

  	//GET 
  	int lev = change_lev(cinfo);
	if (lev == 0) {
		int i = process_GET(cfd, cinfo);
		if (i == 1) {
			return;
		}
    // PUT
	} else if (lev == 1) {
		int i = process_PUT(cfd, cinfo);
		if (i == 1) {
			return;
		}
		
    // DELETE
	} else if (lev == 2) {
		int i = process_DELETE(cfd, cinfo, cinfo->ff[0]);
		if (i == 1) {
			return;
		}
    // LIST
	} else if (lev == 3) {
		process_LIST(cfd);
	}
	cleanup(cfd);
}

ssize_t add_file( size_t size, size_t rc, char* path, int cfd) {
    FILE *ww =NULL;
	int w = 0;
	ssize_t count;
	size_t diff = size - rc;
	while (rc < size) {
		size_t ind = diff;
    	if(diff > 1024){
      		ind = 1024;
    	}
		char c[1025] = {0};
		count = read_socket(cfd, c, ind);
		
		if (count == -1) continue;
		rc += count;
        if (w == 0 && count != 0) {
            w = 1;
	        ww = fopen(path, "w");

	        if (!ww) {
		        perror("fopen");
		        return -1;
	        }
        }
        if (ww != NULL) {

		    fprintf(ww, "%s", c);
        }

		if( count == 0) break;
	}
	if (ww != NULL) {
	    fclose(ww);
    }
	return rc;
}

int do_put(int cfd, C_info *cinfo) {
	int len = dkl + cinfo->ss[1] + 2;
	char path[len];
	set_path(cinfo,path, len);
	if ((access(path, F_OK)) != 0) {
		vector_push_back(fv, cinfo->ff[0]);
	}

	size_t size = 0;
	read_socket(cfd, (char*) & size, sizeof(size_t));
	ssize_t ad = add_file( size + 5, 0, path, cfd);
	if (ad == -1) {
		return 1;
	}
	if (ad == 0 && (size_t)ad != size) {
		print_connection_closed();
		return 1;
	}
	dictionary_set(fs, cinfo->ff[0], &size);
	return 0;
}

void fill_cinfo(verb method, C_info * cinfo, int size) {
	cinfo->method = method;
	if (method != LIST) {
		strcpy(cinfo->ff[0], cinfo->ff[1] + size);
		cinfo->ss[1] = strlen(cinfo->ff[0]);
		cinfo->ff[0][cinfo->ss[1] - 1] = '\0';
	}
}

int lev2(char* header) {
	if (!strncmp(header, "GET", 3)) {
		return 1;
	} else if (!strncmp(header, "PUT", 3)) {
		return 2;
	} else if (!strncmp(header, "DELETE", 6)) {
		return 3;
	} else if (!strncmp(header, "LIST", 4)) {
		return 4;
	} else {
		return 5;
	}
}

void read_header(int cfd, C_info *cinfo) {
	size_t count = head_reader(cfd, cinfo->ff[1], 1024);

	if (count == 1024) {
		cinfo->ss[0] = -1;
		e_change(cfd);
		return;
	}
	cinfo->ss[2] = strlen(cinfo->ff[1]);
	int lev = lev2(cinfo->ff[1]);

	switch(lev) {
		case 1:
			fill_cinfo(GET, cinfo, 4);
			break;
		case 2:
			fill_cinfo(PUT, cinfo, 4);
			break;
		case 3:
			fill_cinfo(DELETE, cinfo, 7);
			break;
		case 4:
			fill_cinfo(LIST, cinfo, 1);
			break;
		case 5:
			print_invalid_response();
			cinfo->ss[0] = -1;
			e_change(cfd);
			return;
	}

	cinfo->ss[0] = 1;
	e_change(cfd);
}

void write_error(int status, int cfd, const char* err, size_t size, int lev) {
	if (status == lev) {
		write_socket(cfd, err, size);
	}
}

void do_error(C_info * cinfo, int cfd) {
	write_error(cinfo->ss[0], cfd, err_bad_request, strlen(err_bad_request), -1);
	write_error(cinfo->ss[0], cfd, err_bad_file_size, strlen(err_bad_file_size), -2);
	write_error(cinfo->ss[0], cfd, err_no_such_file, strlen(err_no_such_file), -3);
}

void run_client(int cfd) {
	C_info* cinfo = dictionary_get(dict, &cfd);

	if (cinfo->ss[0] == 0) {
		read_header(cfd, cinfo);
	}else if (cinfo->ss[0] == 1) {
		process_cmd(cfd, cinfo);
	}else{
		write_socket(cfd, "ERROR\n", 6);
		do_error(cinfo, cfd);
	  	epoll_ctl(efd, EPOLL_CTL_DEL, cfd, NULL);
	  	free(cinfo);
	  	dictionary_remove(dict, &cfd);
	  	shutdown(cfd, SHUT_RDWR);
	  	close(cfd);
	}
}

int delete_file(char *filename){
	int len = dkl + strlen(filename);
	len += 2;
    char temp[len];
    memset(temp, 0, len);
    sprintf(temp, "%s/%s", dk, filename);

   	return unlink(temp); 
}

void clear() {
	if (vector_size(fv) > 0) {
        VECTOR_FOR_EACH(fv, node, {
            char *fn = (char*)node;
            delete_file(fn);
        }); 
    }
}

void freeall(vector*infos) {
	VECTOR_FOR_EACH(infos, cinfo, {
    	free(cinfo);
	});
}

void shutdown_server() {
	close(efd);
    clear();
	vector *infos = dictionary_values(dict);
	freeall(infos);
    
	vector_destroy(infos);
	dictionary_destroy(dict);
	vector_destroy(fv);
	dictionary_destroy(fs);
    rmdir(dk);
	exit(1);
}

int make_server(char* port) {
	int socket_fd  = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (socket_fd == -1) {
		return -1;
	}
	struct addrinfo hints, *result;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;       
	hints.ai_socktype = SOCK_STREAM; 
	hints.ai_flags = AI_PASSIVE;
	int s = getaddrinfo(NULL, port, &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		return -39;
	}
	int optval = 1;
	if (setsockopt(socket_fd, SOL_SOCKET,  SO_REUSEADDR, &optval, sizeof(optval))) {
    	return -39;
	}
	if (bind(socket_fd, result->ai_addr, result->ai_addrlen) != 0 ) {
    	return -39;
	}
	if (listen(socket_fd, 100) != 0 ) {
    	return -39;
	}
	freeaddrinfo(result);
	return socket_fd;
}

void make_epoll(int socket_fd) {
	struct epoll_event et;
  	et.events = EPOLLIN;
  	et.data.fd = socket_fd;
	epoll_ctl(efd, EPOLL_CTL_ADD, socket_fd, &et);
}

void run_server(char* port){
  	int socket_fd = make_server(port);
	if (socket_fd < 0) {
		exit(1);
	}
	efd = epoll_create(39);
	if (efd == -1) {
		exit(1);
	}
	make_epoll(socket_fd);
	struct epoll_event e_ar[123];
	while (1) {
		int n = epoll_wait(efd, e_ar, 123, -1);
		if (n == -1) {
			exit(1);
		}
		int i = 0;
		while (1) {
			if (i >= n) {
				break;
			}
			if (e_ar[i].data.fd == socket_fd) {
				int cfd = accept(socket_fd, NULL, NULL);
				if (cfd < 0) {
					exit(1);
				}
				make_epoll(cfd);
				C_info *cinfo = calloc(1, sizeof(C_info));
				cinfo->ss[0] = 0;
				cinfo->ss[1] = 0;
				cinfo->ss[2] = 0;
        		dictionary_set(dict, &cfd, cinfo);
			} else {
				run_client(e_ar[i].data.fd);
			}
			i++;
		}
	}
}



int main(int argc, char **argv) {
    // good luck!
  if(argc != 2){
    print_server_usage();
    exit(1);  
  }
  
  struct sigaction s;
  memset(&s, '\0', sizeof(s));
  s.sa_handler = shutdown_server;
  if (sigaction(SIGINT, &s, NULL) != 0) {
	exit(1);
  }

  char d[] = "XXXXXX";
  dk = mkdtemp(d);
  dkl = strlen(dk);
  print_temp_directory(dk);

  dict = int_to_shallow_dictionary_create();
  fs = string_to_unsigned_long_dictionary_create();
  fv = string_vector_create();
	
  run_server(argv[1]);

}