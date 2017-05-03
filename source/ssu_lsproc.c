#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
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
	
	// option f : 
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
			exit(0);	// child end
		}
		else{				//fork err
			fprintf(stderr, "fork err\n");
			return;
		}
	}

	// option t :
	if(opt.is_t >= 0){
		pid = fork();
		if(pid > 0){	// parents
			wait(&status);
		}
		else if(pid == 0){	// child
			if(opt.is_t == 0){
				opt.t_pid[0] = getpid();
				opt.is_t++;
			}

			for(i=0; i<opt.is_t; i++){
				if(opt.is_t > 1)
					printf("([/proc/%d/status])\n", opt.t_pid[i]);
				rst = optionT(opt.t_pid[i]);
			}
			exit(0);	// child end
		}
		else{	//fork err
			fprintf(stderr, "fork() err\n");
			return;
		}
	}

	// option c :
	if(opt.is_c >= 0){
		pid = fork();
		if(pid > 0){	// parents
			wait(&status);
		}
		else if(pid == 0){	// child
			if(opt.is_c == 0){
				opt.c_pid[0] = getpid();
				opt.is_c++;
			}

			for(i=0; i<opt.is_c; i++){
				if(opt.is_c > 1)
					printf("([/proc/%d/cmdline])\n", opt.c_pid[i]);
				rst = optionC(opt.c_pid[i]);
			}
			exit(0);	// child end
		}
		else{	//fork err
			fprintf(stderr, "fork() err\n");
			return;
		}
	}

	// option n :
	if(opt.is_n >= 0){
		pid = fork();
		if(pid > 0){	// parents
			wait(&status);
		}
		else if(pid == 0){	//child
			if(opt.is_n == 0){
				opt.n_pid[0] = getpid();
				opt.is_n++;
			}

			for(i=0; i<opt.is_n; i++){
				if(opt.is_n > 1)
					printf("([/proc/%d/io])\n", opt.n_pid[i]);
				rst = optionN(opt.n_pid[i]);
			}
			exit(0);	// child end
		}
		else{	//fork err
			fprintf(stderr, "fork() err\n");
			return;
		}
	}

	// option m :
	if(opt.is_m >= 0){
		pid = fork();
		if(pid > 0){	// parents
			wait(&status);
		}
		else if(pid == 0){	//child
			if(opt.is_m == 0){
				opt.m_pid[0] = getpid();
				opt.is_m++;
			}

			for(i=0; i<opt.is_m; i++){
				if(opt.is_m > 1)
					printf("([/proc/%d/environ])\n", opt.m_pid[i]);
				rst = optionM(opt.m_pid[i], opt.is_k, opt.key);
			}
			exit(0);	// child end
		}
		else{	//fork err
			fprintf(stderr, "fork() err\n");
			return;
		}
	}

	// option w:
	if(opt.is_w >= 0){
		pid = fork();
		if(pid > 0){	// parents
			wait(&status);
		}
		else if(pid == 0){ // child
			rst = optionW();
			exit(0);
		}
		else{ // fork err
			fprintf(stderr, "fork() err\n");
			return;
		}
	}

	// option e:
	if(opt.is_e >= 0){
		pid = fork();
		if(pid > 0){	// parents
			wait(&status);
		}
		else if(pid == 0){ // child
			rst = optionE();
			exit(0);
		}
		else{ // fork err
			fprintf(stderr, "fork() err\n");
			return;
		}
	}

	// option l:
	if(opt.is_l >= 0){
		pid = fork();
		if(pid > 0){	// parents
			wait(&status);
		}
		else if(pid == 0){ // child
			rst = optionL();
			exit(0);
		}
		else{ // fork err
			fprintf(stderr, "fork() err\n");
			return;
		}
	}

	// option v:
	if(opt.is_v >= 0){
		pid = fork();
		if(pid > 0){	// parents
			wait(&status);
		}
		else if(pid == 0){ // child
			rst = optionV();
			exit(0);
		}
		else{ // fork err
			fprintf(stderr, "fork() err\n");
			return;
		}
	}

	printf(">: ssu_lsproc terminated. :<\n");
}

int optionV(void){
	int rst;
	char *path = "/proc/version";
	char buf[1024];
	FILE *fp;

	if((rst=access(path, F_OK)) < 0){	// not exist
		printf("%s doesn't read.\n", path);
		return 1;
	}
	if((rst=access(path, R_OK)) < 0){	// can't read
		printf("%s can't be read.\n", path);
		return 1;
	}

	if((fp = fopen(path, "r")) == NULL){	// check fopen() err
		fprintf(stderr, "fopen() err\n");
		return 1;
	}

	fscanf(fp, "%[^\n]s", buf);
	printf("%s\n", buf);

	return 0;
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
		fprintf(stderr, "opendir() err\n");	
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
		
		if(lstat(nPath, &statbuf) < 0){	// check lstat() err
			fprintf(stderr, "lstat() err\n");
			return 1;		
		}
		
		if(S_ISLNK(statbuf.st_mode) != 0){
			memset(linkfile, 0, 30);
			if(rst = readlink(nPath, linkfile, 30) < 0){ //check readlink() err
				fprintf(stderr, "readlink() err\n");
				continue;	
			}
			printf("File Descriptor: %s, Opened File: %s\n", dirp->d_name, linkfile);
		}
		
		free(nPath);
	}
	closedir(dp);

	return 0;
}

int optionT(pid_t pid){
	int i, len, rst;
	char path1[10];
	char path2[26] = "/proc/";
	char *status = "/status";
	char *nPath;
	FILE *fp;
	char str1[100];
	char str2[100];
	char *Name = "Name:";
	char *State = "State:";
	char *Tgid = "Tgid:";
	char *Ngid = "Ngid:";
	char *Pid = "Pid:";
	char *PPid = "PPid:";
	char *TracerPid = "TracerPid:";
	char *Uid = "Uid:";
	char *Gid = "Gid:";

	ssu_atoi(pid, path1);
	len = strlen(path1);
	for(int i=0; i<len; i++)
		path2[6+i] = path1[i];
	path2[6+len] = '\0';
	strcat(path2, status);
	
	if((rst=access(path2, F_OK)) < 0){	// not exist
		printf("%s doesn't read.\n", path2);
		return 1;
	}
	if((rst=access(path2, R_OK)) < 0){	// can't read
		printf("%s can't be read.\n", path2);
		return 1;
	}

	if((fp = fopen(path2, "r")) == NULL){	// check fopen() err
		fprintf(stderr, "open err\n");
		exit(1);
	}

	i=0;
	// print
	while(1){
		if(fscanf(fp, "%s", str1) == EOF){
			// check file end || fscanf() err
			if(!feof(fp)){
				fprintf(stderr, "fscanf() err\n");
			}
			break;
		}
		if(fscanf(fp, "%s", str2) == EOF){
			// check file end || fscanf() err
			if(!feof(fp)){
				fprintf(stderr, "fscanf() err\n");
			}
			break;
		}

		// print proc/[pid]/status
		if(!strcmp(str1, Name)){
			printf("%s %s\n", str1, str2);
			i++;
		}
		else if(!strcmp(str1, State)){
			printf("%s ", str1);
			switch(str2[0]){
				case 'R':
					printf("Runing\n");
					break;
				case 'S':
					printf("Sleeping\n");
					break;
				case 'D':
					printf("Disk sleep\n");
					break;
				case 'T':
					if(str2[2] == 's')
						printf("Stopped\n");
					else if(str2[2] == 't')
						printf("Tracing stop\n");
					else
						printf("%s\n", str2);
					break;
				case 'Z':
					printf("Zombie\n");
					break;
				case 'X':
					printf("Dead\n");
					break;
				default:
					printf("%s\n", str2);
					break;
			}
			fscanf(fp, "%s", str2);
			i++;
		}
		else if(!strcmp(str1, Tgid)){
			printf("%s %s\n", str1, str2);
			i++;
		}
		else if(!strcmp(str1, Ngid)){
			printf("%s %s\n", str1, str2);
			i++;
		}
		else if(!strcmp(str1, Pid)){
			printf("%s %s\n", str1, str2);
			i++;
		}
		else if(!strcmp(str1, PPid)){
			printf("%s %s\n", str1, str2);
			i++;
		}
		else if(!strcmp(str1, TracerPid)){
			printf("%s %s\n", str1, str2);
			i++;
		}
		else if(!strcmp(str1, Uid)){
			printf("%s %s\n", str1, str2);
			fscanf(fp, "%s", str2);
			fscanf(fp, "%s", str2);
			fscanf(fp, "%s", str2);
			i++;
		}
		else if(!strcmp(str1, Gid)){
			printf("%s %s\n", str1, str2);
			fscanf(fp, "%s", str2);
			fscanf(fp, "%s", str2);
			fscanf(fp, "%s", str2);
			i++;
		}

		if(i > 8)
			break;
	}
	return 0;
}

int optionC(pid_t pid){
	int i, len, rst;
	char c;
	char path1[10];
	char path2[30]="/proc/";
	char *cmdline = "/cmdline";
	char *nPath;
	char buf[1040];
	FILE *fp;

	ssu_atoi(pid, path1);
	len = strlen(path1);
	for(int i=0; i<len; i++)
		path2[6+i] = path1[i];
	path2[6+len] = '\0';
	strcat(path2, cmdline);
	
	if((rst=access(path2, F_OK)) < 0){	// not exist
		printf("%s doesn't read.\n", path2);
		return 1;
	}
	if((rst=access(path2, R_OK)) < 0){	// can't read
		printf("%s can't be read.\n", path2);
		return 1;
	}
	
	if((fp = fopen(path2, "r")) == NULL){ // check fopen() err
		fprintf(stderr, "fopen() err\n");
		return 1;
	}
	
	i=0, len=0;
	while((c = fgetc(fp)) != EOF){ // read until file end
		if(i==0)
			printf("argv[%d] = ", len);
		
		if(c == '\0'){
			printf("\n");
			i=0;
			len++;
		}
		else{
			printf("%c",c);
			i++;
		}
	}
	return 0;
}

int optionN(pid_t pid){
	int i, len, rst;
	char path1[10];
	char path2[21]="/proc/";
	char *io = "/io";
	char str1[30];
	char str2[30];
	char *nPath;
	char *rchar = "rchar:";
	char *wchar = "wchar:";
	char *syscr = "syscr:";
	char *syscw = "syscw:";
	char *read_bytes = "read_bytes:";
	char *write_bytes = "write_bytes:";
	char *cancelled_write_bytes = "cancelled_write_bytes:";
	FILE *fp;

	ssu_atoi(pid, path1);
	len = strlen(path1);
	for(int i=0; i<len; i++)
		path2[6+i] = path1[i];
	path2[6+len] = '\0';
	strcat(path2, io);
	
	if((rst=access(path2, F_OK)) < 0){	// not exist
		printf("%s doesn't read.\n", path2);
		return 1;
	}
	if((rst=access(path2, R_OK)) < 0){	// can't read
		printf("%s can't be read.\n", path2);
		return 1;
	}

	if((fp=fopen(path2, "r")) == NULL){
		fprintf(stderr, "fopen() err\n");
		return 1;
	}

	// print
	i=0;
	while(1){
		if(fscanf(fp, "%s", str1) == EOF){
			// check file end || fscanf() err
			if(!feof(fp)){
				fprintf(stderr, "fscanf() err\n");
			}
			break;
		}
		if(fscanf(fp, "%s", str2) == EOF){
			// check file end || fscanf() err
			if(!feof(fp)){
				fprintf(stderr, "fscanf() err\n");
			}
			break;
		}

		if(!strcmp(str1, rchar)){
			printf("Characters read : %s\n", str2);
		}
		else if(!strcmp(str1, wchar)){
			printf("Characters write : %s\n", str2);
		}
		else if(!strcmp(str1, syscr)){
			printf("Read syscalls : %s\n", str2);
		}
		else if(!strcmp(str1, syscw)){
			printf("Write syscalls : %s\n", str2);
		}
		else if(!strcmp(str1, read_bytes)){
			printf("Bytes read : %s\n", str2);
		}
		else if(!strcmp(str1, write_bytes)){
			printf("Bytes written : %s\n", str2);
		}
		else if(!strcmp(str1, cancelled_write_bytes)){
			printf("Cancelled write bytes : %s\n", str2);
		}
	}
	return 0;
}

int optionM(pid_t pid, int keyNum, char **key){
	int i, len, rst, print, num;
	char path1[10];
	char path2[30]="/proc/";
	char *environ = "/environ";
	char *nPath;
	char c;
	char str[1000];
	char *k[16];
	FILE *fp;

	ssu_atoi(pid, path1);
	len = strlen(path1);
	for(int i=0; i<len; i++)
		path2[6+i] = path1[i];
	path2[6+len] = '\0';
	strcat(path2, environ);
	
	if((rst=access(path2, F_OK)) < 0){	// not exist
		printf("%s doesn't read.\n", path2);
		return 1;
	}
	if((rst=access(path2, R_OK)) < 0){	// can't read
		printf("%s can't be read.\n", path2);
		return 1;
	}

	if((fp=fopen(path2, "r")) == NULL){ // check fopen() err
		fprintf(stderr, "fopen() err\n");
		return 1;
	}

	if(keyNum > 0){
		for(i=0; i<keyNum; i++){
			k[i] ='\0';
		}
	}
	
	len=0; print=-1; num=0;
	while((c = fgetc(fp)) != EOF){	// read until file end
		if(num > keyNum)
			break;
		
		if(c == '\0'){
			str[len] = '\0';
			if(keyNum > 0 && print >= 0){
				if(len == 0){
					k[print] = '\0';
				}
				else{
					k[print] = malloc(len);
					strcpy(k[print], str);
				}
				//k[print] = malloc(len);
				//strcpy(k[print], str);

				print = -1;
				num++;
			}
			else if(keyNum <= 0){
				printf("%s\n", str);
			}
			len = 0;
		}
		else if(c == '='){
			str[len] = '\0';
			if(keyNum > 0){
				for(i=0; i<keyNum; i++){
					if(!strcmp(str, key[i])){
						print = i;
						break;
					}
				}
			}
			else{
				printf("%s=", str);
			}
			len=0;
		}
		else{
			str[len] = c;
			len++;
		}
	}
	
	for(i = 0; i<keyNum; i++){
		if(k[i] != '\0'){
			printf("%s=%s\n", key[i], k[i]);
			free(k[i]);
		}
	}

	return 0;
}

int optionW(void){
	char *path = "/proc/interrupts";
	int i, rst, len, cpuNum=0;
	FILE *fp;
	char c;
	char str1[1024];
	char str2[1024];
	double avg = 0;
	int input = 0;

	if((rst=access(path, F_OK)) < 0){	// not exist
		printf("%s doesn't read.\n", path);
		return 1;
	}
	if((rst=access(path, R_OK)) < 0){	// can't read
		printf("%s can't be read.\n", path);
		return 1;
	}

	if((fp = fopen(path, "r")) == NULL){ // check fopen() err
		fprintf(stderr, "fopen() err\n");
		return 1;
	}

	while((c = fgetc(fp)) != '\n'){ // check CPUNum 
		if(c != ' '){
			cpuNum++;
			while((c = fgetc(fp)) != ' '){
				if(c == '\n'){
					ungetc(c, fp);
					break;
				}
			}
		}
	}

	printf("---Number of CPUs : %d ---\n", cpuNum);
	printf("      [Average] : [Description]\n");
	
	while((c = fgetc(fp)) != EOF){	// until read file end.
		if(c == ' '){
			// pass	
		}
		else if(isdigit(c)){
			fscanf(fp, "%[^\n]s", str2);
		}
		else if(isalpha(c)){
			input = 0;
			avg = 0;
			ungetc(c, fp);
			fscanf(fp, "%s", str1);
			len = strlen(str1);
			str1[len-1] = '\0';
			for(i=0; i<cpuNum; i++){
				str2[0] = '\0';
				fscanf(fp, "%s", str2);
				input = atoi(str2);
				avg += (double)input;
			}
			avg = avg / (double)cpuNum;
			str2[0] = '\0';
			fscanf(fp, "%[^\n]s", str2);
			printf("%15.3f : <%s> %s\n", avg, str1, str2);
		}
	}
	return 0;
}

int optionE(void){
	char *path = "/proc/filesystems";
	FILE *fp;
	char *nodev = "nodev";
	char str[20];
	int i=0, rst;

	if((rst=access(path, F_OK)) < 0){	// not exist
		printf("%s doesn't read.\n", path);
		return 1;
	}
	if((rst=access(path, R_OK)) < 0){	// can't read
		printf("%s can't be read.\n", path);
		return 1;
	}

	if((fp = fopen(path, "r")) == NULL){ // check fopen() err
		fprintf(stderr, "fopen() err\n");
		return 1;
	}
	
	printf("<<Supported Filesystems>>\n");
	while(!(fscanf(fp, "%s", str) == EOF)){
		if(!strcmp(nodev, str)){
			fscanf(fp, "%s", str);	
		}
		else{
			printf("%s ", str);
			i++;
			if(i % 5 == 0){
				printf("\n");
			}
		}
	}
	printf("\n");
	return 0;
}

int optionL(void){
	int i, rst;
	char *path = "/proc/uptime";
	FILE *fp;
	double worktime, idletime;

	if((rst=access(path, F_OK)) < 0){	// not exist
		printf("%s doesn't read.\n", path);
		return 1;
	}
	if((rst=access(path, R_OK)) < 0){	// can't read
		printf("%s can't be read.\n", path);
		return 1;
	}
	
	if((fp = fopen(path, "r")) == NULL){ // check fopen() err
		fprintf(stderr, "fopen() err\n");
		return 1;
	}

	fscanf(fp, "%lf", &worktime);
	fscanf(fp, "%lf", &idletime);

	printf("Process worked time : %.2f(sec)\n", worktime);
	printf("Process worked time : %.2f(sec)\n", idletime);
	
	return 0;
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
	int i, j, k, n;
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

	// if r option, sort here
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

