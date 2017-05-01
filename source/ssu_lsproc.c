#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "ssu_lsproc.h"
#include "ssu_usage.h"
#include "ssu_test.h"
#include "ssu_function.h"

void ssu_lsproc(int argc, char* argv[]){
	struct lsproc_opt opt;
	char befoOpt='\0';
	int i, rst;
	int pid, status;
	init_lsproc(&opt);

	// parsing
	if((rst = parsing_lsproc(argc, argv, &opt)) < 0){
		lsprocUsage();
		return;
	}

	// check parsing
	//ssu_test(&opt);

	// start lsproc
	printf(">: ssu_lsproc start. :<\n");
	
	// f : 
	if(opt.is_f >= 0){
		pid = fork();
		if(pid > 0){		// parents
			wait(&status);
		}
		else if(pid == 0){	// child
			if(opt.is_f == 0){
				opt.f_pid[0] = getpid();
				opt.is_f++;
			}

			for(i=0; i<opt.is_f; i++){
				if(opt.is_f > 1)
					printf("([/proc/%d/fd])\n", opt.f_pid[i]);
				rst = optionF(opt.f_pid[i]);
			}

			exit(1);
		}
		else{				//fork err
		}
	}

	printf(">: ssu_lsproc terminated. :<\n");
}

int optionF(pid_t pid){
	int i, len, rst, p, d;
	char path1[10];
	char path2[21]="/proc/";
	char *fd = "/fd";
	char *nPath;
	char linkfile[30];
	DIR *dp;
	struct dirent *dirp;
	struct stat statbuf;

	ssu_atoi(pid, path1);
	len = strlen(path1);
	for(int i=0; i<len; i++)
		path2[6+i] = path1[i];
	path2[6+len] = '\0';
	strcat(path2, fd);
	
	if((rst=access(path2, F_OK)) < 0){	// not exist
		printf("%s doesn't read.\n", path2);
		return 1;
	}
	if((rst=access(path2, R_OK)) < 0){	// can't read
		printf("%s can't be read.\n", path2);
		return 1;
	}

	if((dp = opendir(path2)) == NULL){	// check opendir() err
		// print err msg	
		return 1;	
	}
	while((dirp = readdir(dp)) != NULL){
		if(strcmp(dirp->d_name, ".") == 0){
			continue;
		}
		if(strcmp(dirp->d_name, "..") == 0){
			continue;
		}
		p = strlen(path2);
		d = strlen(dirp->d_name);
		
		nPath = (char *)malloc(p+d+2);
		memset(nPath, '\0', p+d+2);
		strcpy(nPath, path2);
		nPath[p] = '/';
		strcpy(&nPath[p+1], dirp->d_name);
		nPath[p+d+1] = '\0';
		
		if(lstat(nPath, &statbuf) < 0){	// check stat() err
			// print err msg
			return 1;		
		}
		
		if(S_ISLNK(statbuf.st_mode) != 0){
			memset(linkfile, 0, 30);
			if(rst = readlink(nPath, linkfile, 30) < 0){ //check readlink() err
				// print err msg;
				continue;	
			}
			printf("File Descriptor: %s, Opened File: %s\n", dirp->d_name, linkfile);
		}
		
		free(nPath);
	}
	closedir(dp);
}

void init_lsproc(struct lsproc_opt *opt){
	// init struct lsproc_opt
	int i;
	opt->is_f=-1;
	opt->is_t=-1;
	opt->is_c=-1;
	opt->is_n=-1;
	opt->is_m=-1;
	opt->is_k=-1;
	opt->is_w=-1;
	opt->is_e=-1;
	opt->is_l=-1;
	opt->is_v=-1;
	opt->is_r=-1;
	opt->is_s=-1;
	opt->is_o=-1;
	opt->s_filedes = -1;
	opt->s_cmdline = -1;
	opt->s_io = -1;
	opt->s_stat = -1;
	opt->s_environ = -1;
	for(i=0; i<16; i++){
		opt->f_pid[i] = -1;
		opt->t_pid[i] = -1;
		opt->c_pid[i] = -1;
		opt->n_pid[i] = -1;
		opt->m_pid[i] = -1;
		opt->key[i] = '\0';
	}
	opt->o_fname = '\0';
}

int parsing_lsproc(int argc, char* argv[], struct lsproc_opt * opt){
	// parsing
	int i, j, n;
	char *tmp;
	char befoOpt = '\0';

	for(i=1; i<argc; i++){
		n=0;
		if(argv[i][0] == '-'){
			switch(argv[i][1]){
				case 'f':
					if(opt->is_f >= 0) return -1;
					while(++i < argc && argv[i][0] != '-' && isdigit(argv[i][0])){
						if(n > 15) return -1;
						opt->f_pid[n] = atoi(argv[i]);
						n++;
					}
					opt->is_f = n;
					i--;
					befoOpt = 'f';
					break;
				case 't':
					if(opt->is_t >= 0) return -1;
					while(++i < argc && argv[i][0] != '-' && isdigit(argv[i][0])){
						if(n > 15) return -1;
						opt->t_pid[n] = atoi(argv[i]);
						n++;
					}
					opt->is_t = n;
					i--;
					befoOpt = 't';
					break;
				case 'c':
					if(opt->is_c >= 0) return -1;
					while(++i < argc && argv[i][0] != '-' && isdigit(argv[i][0])){
						if(n > 15) return -1;
						opt->c_pid[n] = atoi(argv[i]);
						n++;
					}
					opt->is_c = n;
					i--;
					befoOpt = 'c';
					break;
				case 'n':
					if(opt->is_n >= 0) return -1;
					while(++i < argc && argv[i][0] != '-' && isdigit(argv[i][0])){
						if(n > 15) return -1;
						opt->n_pid[n] = atoi(argv[i]);
						n++;
					}
					opt->is_n = n;
					i--;
					befoOpt = 'n';
					break;
				case 'm':
					if(opt->is_m >= 0) return -1;
					while(++i < argc && argv[i][0] != '-' && isdigit(argv[i][0])){
						if(n > 15) return -1;
						opt->m_pid[n] = atoi(argv[i]);
						n++;
					}
					opt->is_m = n;
					i--;
					befoOpt = 'm';
					break;
				case 'k':
					if(befoOpt != 'm') return -1;
					if(opt->is_k >= 0) return -1;
					while(++i< argc && argv[i][0] != '-'){
						if(n > 15) return -1;
						opt->key[n] = argv[i];
						n++;
					}
					opt->is_k = n;
					i--;
					befoOpt = 'k';
					break;
				case 'w':
					if(opt->is_w >= 0) return -1;
					opt->is_w = 0;
					befoOpt = 'w';
					break;
				case 'e':
					if(opt->is_e >= 0) return -1;
					opt->is_e = 0;
					befoOpt = 'e';
					break;
				case 'l':
					if(opt->is_l >= 0) return -1;
					opt->is_l = 0;
					befoOpt = 'l';
					break;
				case 'v':
					if(opt->is_v >= 0) return -1;
					opt->is_v = 0;
					befoOpt = 'v';
					break;
				case 'r':
					if(opt->is_r >= 0) return -1;
					opt->is_r = 0;
					befoOpt = 'r';
					break;
				case 's':
					if(opt->is_s >= 0) return -1;
					while(++i<argc && argv[i]){
						if(n > 4) return -1;
						if(strcmp(argv[i], "FILEDES") == 0){
							opt->s_filedes = 1;
						}
						else if(strcmp(argv[i], "CMDLINE") == 0){
							opt->s_cmdline = 1;
						}
						else if(strcmp(argv[i], "IO") == 0){
							opt->s_io = 1;
						}
						else if(strcmp(argv[i], "STAT") == 0){
							opt->s_stat = 1;
						}
						else if(strcmp(argv[i], "ENVIRON") == 0){
							opt->s_environ = 1;
						}
						else{
							return -1;
						}
						n++;
					}
					i--;
					opt->is_s = 0;
					befoOpt = 's';
					break;
				case 'o':
					if(opt->is_o >= 0) return -1;
					if(i+1 < argc)
						opt->o_fname = argv[++i];
					opt->is_o = 0;
					befoOpt = 'o';
					break;
				default:
					// can't fine that option
					return -1;
					break;
			}
		}
		else{ // err
			return -1;
		}
	}

	// TODO if r option, sort here
	if(opt->is_r==0){
		if(opt->is_f > 1){
			for(i=0; i<opt->is_f; i++){
				for(j=1; j<opt->is_f; j++){
					if(opt->f_pid[j-1] > opt->f_pid[j]){
						n = opt->f_pid[j];
						opt->f_pid[j] = opt->f_pid[j-1];
						opt->f_pid[j-1] = n;
					}
				}
			}
		}

		if(opt->is_t > 1){
			for(i=0; i<opt->is_t; i++){
				for(j=1; j<opt->is_t; j++){
					if(opt->t_pid[j-1] > opt->t_pid[j]){
						n = opt->t_pid[j];
						opt->t_pid[j] = opt->t_pid[j-1];
						opt->t_pid[j-1] = n;
					}
				}
			}
		}

		if(opt->is_c > 1){
			for(i=0; i<opt->is_c; i++){
				for(j=1; j<opt->is_c; j++){
					if(opt->c_pid[j-1] > opt->c_pid[j]){
						n = opt->c_pid[j];
						opt->c_pid[j] = opt->c_pid[j-1];
						opt->c_pid[j-1] = n;
					}
				}
			}
		}

		if(opt->is_n > 1){
			for(i=0; i<opt->is_n; i++){
				for(j=1; j<opt->is_n; j++){
					if(opt->n_pid[j-1] > opt->n_pid[j]){
						n = opt->n_pid[j];
						opt->n_pid[j] = opt->n_pid[j-1];
						opt->n_pid[j-1] = n;
					}
				}
			}
		}

		if(opt->is_m > 1){
			for(i=0; i<opt->is_m; i++){
				for(j=1; j<opt->is_m; j++){
					if(opt->m_pid[j-1] > opt->m_pid[j]){
						n = opt->m_pid[j];
						opt->m_pid[j] = opt->m_pid[j-1];
						opt->m_pid[j-1] = n;
					}
				}
			}
		}

		if(opt->is_k > 1){
			for(i=0; i<opt->is_k; i++){
				for(j=1; j<opt->is_k; j++){
					if(strcmp(opt->key[j-1], opt->key[j]) > 0){
						tmp = opt->key[j];
						opt->key[j] = opt->key[j-1];
						opt->key[j-1] = tmp;
					}
				}
			}
		}
	}
	return 0;
}

