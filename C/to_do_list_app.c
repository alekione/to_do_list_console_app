/**
 *simple to-do list application where users can add, edit, and delete tasks wthin cmd.
 */
 
 #include <stdio.h>
 #include <string.h>
 #include <stdbool.h>
 #include <stdlib.h>
 #include <windows.h>
 #include <shlobj.h>
 
 struct Node;
 int process_normal();
 int process_file(char *, char *, char *, char *);
 int init();
 char* enc_dec(char *, char *);
 bool is_command(char*);
 void free_node();
 void _free(struct Node **);
 void shift_node(struct Node **, char *);

		
struct Node {
	char *task;
	char *date;
	char *comment;
	struct Node *prev;
	struct Node *next;
};

struct Node *head;
const char *filename;

/**
 * init - iniializes basic functions of the program
 */
int init() {
	FILE *fd;
	char rd, trans, *p;
	int i = 0, count = 0;
	
	if ((fd = fopen(filename, "r")) == NULL)
		return(0);
	struct Node n = {NULL, NULL, NULL, NULL, NULL};
	head = (struct Node *)malloc(sizeof(struct Node));
	if (head == NULL) {
		perror("Error - unable to allocate required memory\n");
		free(p);
		fclose(fd);
		return (-1);
	}
	head = &n;
	// check if the file is empty before proceeding
	fseek(fd, 0, SEEK_END);
	if (ftell(fd) <= 0)
		return (0);
	fseek(fd, 0, SEEK_SET);
	
	p = (char *)malloc(1024 * sizeof(char));
	if (p == NULL) {
		perror("Error - unable to allocate required memory\n");
		fclose(fd);
		return(-1);
	}
	while(1) {
		rd = fgetc(fd);
		if (!(feof(fd))) {
			if (rd = ';') {
				p[i] = '\0';
				if (count = 0) {
					head->task = enc_dec("dec", p);
					count++;
				}
				else if (count == 1) {
					head->date = enc_dec("dec", p);
					count++;
				}
				else {
					head->comment = enc_dec("dec", p);
					count = 0;
					struct Node m = {NULL, NULL, NULL, NULL, NULL};
					struct Node *new = (struct Node *) malloc(sizeof(struct Node));
					if (new == NULL) {
						perror("Error - unable to allocate required memory\n");
						fclose(fd);
						free(p);
						free_node();
						return (-1);
					}
					new = &m; 
					new->prev = head;
					head->next = new;
					head = new;
				}
				i = 0;
				continue;
			}
			p[i] = rd;
			i++;
		}
		else {
			p[i] = '\0';
			if (count = 0) {
				head->task = enc_dec("dec", p);
			}
			else if (count == 1) {
				head->date = enc_dec("dec", p);
			}
			else {
				head->comment = enc_dec("dec", p);
			}
		}
	}
	fclose(fd);
	free(p);
	shift_node(&head, "up");
	return(0);
} 
 /**
  * process_normal- run as a normal file, i.e, when args are not passed from cmd
  */
 int process_normal() {
	 char *usr = NULL, *command, *task = NULL, *date = NULL, *comment = NULL;
	 int ret, count = 0, i = 0, j = 0;
	 ssize_t nread;
	 size_t len = 0;
	 
	 char *choice[6] = {"1", "2", "3", "4", NULL};
	 usr = (char *)malloc(10 * sizeof(char));
	 if (usr == NULL) {
		 perror("Error - unable to allocate required memory\n");
		 return(-1);
	 }
	 /**
	 
	 command = (char *)malloc(10 * sizeof(char));
	 if (command == NULL) {
		 perror("Error - unable to allocate required memory\n");
		 free(usr);
		 return(-1);
	 }
	 task = (char *)malloc(300 * sizeof(char));
	 if (task == NULL) {
		 perror("Error - unable to allocate required memory\n");
		 free(usr);
		 free(command);
		 return(-1);
	 }
	 date = (char *)malloc(20 * sizeof(char));
	 if (date == NULL) {
		 perror("Error - unable to allocate required memory\n");
		 free(usr);
		 free(command);
		 free(task);
		 return(-1);
	 }
	 comment = (char *)malloc(1024 * sizeof(char));
	 if (date == NULL) {
		 perror("Error - unable to allocate required memory\n");
		 free(usr);
		 free(command);
		 free(task);
		 free(date);
		 return(-1);
	 }
	 */
	 while (scanf("%s", usr)) {
		 if (strcmpi(usr, "exit") == 0) {
			 free(usr);
			 return(-1);
		 }
		 for (i = 0; choice[i] != NULL; i++) {
			 if (strcmp(choice[i], usr) == 0) {
				 j++;
				 break;
			 }
		 }
		 if (j > 0)
			 break;
		 printf("invalid command\n");
	 }
	 if (strcmp(usr,"1") == 0) {
		 command = "add";
		 printf("Task: ");
		 nread = getline(&task, &len, stdin);
		 task[nread - 1] = '\0';
		 len = 0;
		 printf("date: ");
		 nread = getline(&date, &len, stdin);
		 date[nread - 1] = '\0';
		 len = 0;
		 printf("comment: ");
		 nread = getline(&comment, &len, stdin);
		 comment[nread - 1] = '\0';
	 }
	 else if (strcmp(usr,"2") == 0 || strcmp(usr,"3") == 0) {
		 if (strcmp(usr,"2") == 0)
			 command = "edit";
		 else
			 command = "delete";
		 printf("task to %s: ", command);
		 nread = getline(&task, &len, stdin);
		 task[nread - 1] = '\0';
	 }
	 else {
		 command = "print";
	 }
	 ret = process_file(command, task, date, comment);
	 free(usr);
	 free(command);
	 free(task);
	 free(date);
	 free(comment);
	 return (ret);
 }
 
 /**
  * process_file - all file processing and encoding are done here
  */
 int process_file(char *command, char *task, char *date, char *comment) {
	 FILE *fd;
	 int num = 0;
	 char *sep = ";", *val;
	 
	 if ((fd = fopen(filename, "r")) == NULL)
		 fd = fopen(filename, "w");
	 fclose(fd);
	 if (strcmp(command, "print") == 0) {
		 if (head->task == NULL) {
			 printf("No task to show.\n");
			 return(0);
		 }
		 printf("task\t\tdate\t\tcomment\t\t\n");
		 while (head != NULL) {
			 printf("%d  ", num++);
			 printf("%s\t%s\t%s\n", head->task, head->date, head->comment);
			 head = head->next;
		 }
		 shift_node(&head, "up");
		 return (0);
	 }
	 else if (strcmp(command, "add") == 0) {
		 struct Node *new = (struct Node *)malloc(sizeof(struct Node));
		 if (new == NULL) {
			 perror("Error - unable to allocate memory. Task not added.\n");
			 return (0);
		 }
		 shift_node(&head, "down");
		 new->task = strdup(task);
		 new->date = strdup(date);
		 new->comment = strdup(comment);
		 new->next = NULL;
		 new->prev = head;
		 head->next = new;
		 shift_node(&head, "up");
		 fd = fopen(filename, "a");
		 if (fd == NULL) {
			 perror("error - unable to open file for writting. please try again.\n");
			 _free(&new);
			 return (0);
		 }
		 if (ftell(fd) > 0)
			 fwrite(sep, 1, sizeof(sep), fd);
		 val = enc_dec("enc", new->task);
		 fwrite(val, 1, sizeof(val), fd);
		 fwrite(sep, 1, sizeof(sep), fd);
		 free(val);
		 val = enc_dec("enc", new->date);
		 fwrite(val, 1, sizeof(val), fd);
		 fwrite(sep, 1, sizeof(sep), fd);
		 free(val);
		 val = enc_dec("enc", new->comment);
		 fwrite(val, 1, sizeof(val), fd);
		 free(val);
		 return (0);
	 }
	 else if (strcmp(command, "edit") == 0) {
		 struct Node *ptr;
		 
		 shift_node(&head, "up");
		 if (head == NULL) {
			 printf("Task not found.\n");
			 return(0);
		 }
		 while (1) {
			 if (strcmp(head->task, task) == 0) {
				 if (date != NULL) {
					 head->date = date;
				 }
				 if (comment != NULL){
					 head->comment = comment;
				 }
				 process_file("rewrite", NULL, NULL, NULL);
				 break;
			 }
			 if (head->next == NULL) {
				 printf("Task not found.\n");
				 break;
			 }
			 ptr = head;
			 head = ptr->next;
		 }
		 shift_node(&head, "up");
		 return (0);
	 }
	 else if (strcmp(command, "delete") == 0) {
		 struct Node *ptr;
		 
		 shift_node(&head, "up");
		 if (head == NULL) {
			 printf("Task not found.\n");
			 return(0);
		 }
		 while(1) {
			 if (strcmp(head->task, task) == 0) {
				 ptr = head;
				 head = ptr->next;
				 head->prev = ptr->prev;
				 ptr->prev->next = head;
				 _free(&ptr);
				 process_file("rewrite", NULL, NULL, NULL);
				 break;
			 }
			 if (head->next == NULL) {
				 printf("Task not found\n");
				 break;
			 }
		 }
	 }
	 else {
		 // write all changes into the file
		 shift_node(&head, "up");
		 
		 fd = fopen(filename, "w");
		 while (1) {
			 val = enc_dec("enc", head->task);
			 fwrite(val, 1, sizeof(val), fd);
			 fwrite(sep, 1, sizeof(sep), fd);
			 val = enc_dec("enc", head->date);
			 fwrite(val, 1, sizeof(val), fd);
			 fwrite(sep, 1, sizeof(sep), fd);
			 val = enc_dec("enc", head->comment);
			 fwrite(val, 1, sizeof(val), fd);
			 if (head->next != NULL)
				 fwrite(sep, 1, sizeof(sep), fd);
			 if (head->next == NULL)
				 break;
			 head = head->next;
		 }
		 fclose(fd);
		 shift_node(&head, "up");
	 }
	 return (0);
 }
 
 /**
  * enc_dec - used to encode or decode strings for file writting.
  */
 char *enc_dec(char *command, char *str) {
	 char *opt = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz ,.?)'/\"&@!%("};
	 char val, *ret = (char *)malloc(strlen(str) * sizeof(char));
	 int jump = 8;
	 int i, j, len = strlen(opt);
	 
	 if (ret == NULL) {
		 perror("Error - unable to allocate memory\n");
		 return ("");
	 }
	 for (j = 0; str[j] != '\0'; j++) {
		 val = str[j];
		 if (command == "enc") {
			for  (i = 0; opt[i] != '\0'; i++) {
				if (opt[i] == val) {
					if (i + jump < len)
						ret[j] = opt[i + jump];
					ret[j] = opt[(i + jump) - len];
				}
			}
		 }
		 else {
			for (i = 0; opt[i] != '\0'; i++) {
				if (opt[i] == val) {
					if (i - jump >= 0) {
						ret[j] = opt[i - jump];
					}
					ret[j] = opt[(i - jump) + len];
				}
			}
		 }
	 }
	 ret[j] = '\0';
	 return(ret);
 }
 
 /**
  *is_command - checks whether te passed string is a command.
  * Return: true  or false
  */
 bool is_command(char *command) {
	 char *cmd[6] = {"print", "help", "delete", "edit", "add", NULL};
	 
	 int i = 0;
	 while (cmd[i] != NULL) {
		 if (cmd[i] == command)
			 return (true);
		 i++;
	 }
	 return (false);
 }
 
 /**
  * shift_node - is used to shift head position. either at the beginning or at the end.
  */
 void shift_node(struct Node **hpsn, char *psn) {
	 struct Node *ptr, *hptr;
	 
	 hptr = *hpsn;
	 if (strcmp(psn, "up") == 0) {
		 while (hptr->prev != NULL) {
			 ptr = hptr;
			 hptr = ptr->prev;
		 }
		 *hpsn = hptr;
	 }
	 else {
		 while (hptr->next != NULL) {
			 ptr = hptr;
			 hptr = ptr->next;
		 }
		 *hpsn = hptr;
	 }
 }
 
 
 /**
  free_node - used to free struct Node memory on exit
  */
 void free_node() {
	struct Node *ptr;
	
	shift_node(&head, "up");
	while(head != NULL) {
		ptr = head;
		head = ptr->next;
		_free(&ptr);
	}
 }
 
 /**
  * _free - used to  free  a siglr node.
  */
 void _free(struct Node **p) {
	 struct Node *ptr;
	 ptr = *p;
	 free(ptr->task);
	 free(ptr->date);
	 free(ptr->comment);
	 free(ptr);
 }
 
/**
 * main function.
 */
 int main(int argc, char *argv[]) {
	 
	 char *task = NULL, *date = NULL, *comment = NULL;
	 int ret;
	 
	 TCHAR szPath[MAX_PATH];
     if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, szPath))) {
        strcat(szPath, "\\to_do_list\\example.txt");
	 }
	 else {
		 printf("Error getting Documents folder path.\n");
		 return (0);
	 }
	 
	 filename = szPath;
	 printf("filename = %s]\n", filename);
	 return(0);
	 if ((ret = init()) < 0)
		 return (ret);
	 ret = 0;
	 while (ret != -1) {
		 printf("What would you like to do\n1. add task\n2. edit task\n3. delete task\n4.view saved tasks\n5.exit\n");
		 ret = process_normal();
	 }
	 return(0);
 }