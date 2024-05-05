/**
 *simple to-do list application where users can add, edit, and delete tasks wthin cmd.
 */
 
 #include <stdio.h>
 #include <string.h>
 #include <stdbool.h>
 #include <stdlib.h>
 #include <windows.h>
 #include <shlobj.h>
 
/**
 * Structure representing a node in a linked list.
 * 
 * Each node contains information about a task, including its description, date, and comments.
 * The 'task', 'date', and 'comment' fields store the respective information as strings.
 * 'prev' and 'next' pointers are used for linking nodes in a doubly linked list.
 */
struct Node {
    char *task;             // Description of the task
    char *date;             // Date of the task
    char *comment;          // Additional comments for the task
    struct Node *prev;      // Pointer to the previous node in the linked list
    struct Node *next;      // Pointer to the next node in the linked list
};

 int process_normal();
 int process_file(char *, char *, char *, char *);
 int init();
 char* enc_dec(char *, char *);
 char *win_directory(char *);
 char *get_input();
 bool is_command(char*);
 void free_node();
 void n_free(struct Node **);
 void shift_node(struct Node **, char *);
	


struct Node *head;
char *filename;

/**
 * Main function of the program. Initializes the to-do list application and 
 * controls the main execution flow.
 * 
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Returns 0 on successful execution.
 */
 int main(int argc, char *argv[]) {
	 int ret;
	 
	 head = NULL;
     filename = win_directory(argv[0]);
	 if ((ret = init()) < 0)
		 return (ret);
	 ret = 0;
	 while (ret != -1) {
		 ret = process_normal();
		 printf("val of ret in main = %d\n", ret);
	 }
	 free(filename);
	 return(0);
 }

/**
 * Initializes basic functions of the program, including reading existing tasks from file.
 * 
 * @return Returns 0 on successful initialization, -1 on failure.
 */
int init() {
	FILE *fd;
	char rd, trans, *p;
	int i = 0, count = 0;
	
	if ((fd = fopen(filename, "r")) == NULL)
		return(0);
	
	// check if the file is empty before proceeding
	fseek(fd, 0, SEEK_END);
	if (ftell(fd) <= 0) {
		fclose(fd);
		return (0);
	}
	fseek(fd, 0, SEEK_SET);
	head = (struct Node *)malloc(sizeof(struct Node));
	if (head == NULL) {
		printf("Error - unable to allocate required memory\n");
		fclose(fd);
		return (-1);
	}
	
	p = (char *)malloc(1024 * sizeof(char));
	if (p == NULL) {
		printf("Error - unable to allocate required memory\n");
		fclose(fd);
		return(-1);
	}
	while(1) {
		rd = fgetc(fd);
		if (rd != -1) {
			if (rd == ';') {
				p[i] = '\0';
				if (count ==0) {
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
					struct Node *new = (struct Node *) malloc(sizeof(struct Node));
					if (new == NULL) {
						printf("Error - unable to allocate required memory\n");
						fclose(fd);
						free(p);
						free_node();
						return (-1);
					} 
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
			if (count == 0) {
				head->task = enc_dec("dec", p);
				head->date = NULL;
				head->comment = NULL;
			}
			else if (count == 1) {
				head->date = enc_dec("dec", p);
				head->comment = NULL;
			}
			else {
				head->comment = enc_dec("dec", p);
			}
			break;
		}
	}
	fclose(fd);
	free(p);
	shift_node(&head, "up");
	return(0);
}

/**
 * Processes user input and performs corresponding actions based on the selected command.
 * 
 * @return Returns -1 to exit the program, 0 otherwise.
 */
 int process_normal() {
	 char *usr = NULL, *command = NULL, *task = NULL, *date = NULL, *comment = NULL;
	 int ret, count = 0, i = 0, j = 0;
	 
	 char *choice[6] = {"1", "2", "3", "4", NULL};
	 usr = (char *)malloc(10 * sizeof(char));
	 if (usr == NULL) {
		 printf("Error - unable to allocate required memory\n");
		 return(-1);
	 }
	 printf("What would you like to do\n1. add task\n2. edit task\n3. delete task\n4.view saved tasks\n5.exit\n");
	 while (scanf("%s", usr)) {
		 if (strcmpi(usr, "5") == 0) {
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
	 command = (char *)malloc(10 * sizeof(char));
	 if (command == NULL) {
		 printf("Error - unable to allocate required memory\n");
		 free(usr);
		 return (-1);
	 }
	 if (strcmp(usr,"1") == 0) {
		 strcpy(command, "add");
		 printf("Task: ");
		 task = get_input();
		 if (task == NULL || strcmp(task, "") == 0) {
			 if (task == NULL) printf("Error - failed to get user input\n");
			 else printf("task cannot be empty\n");
			 free(command);
			 free(task);
			 return (0);
		 }
		 printf("date: ");
		 date = get_input();
		 if (date == NULL) {
			 printf("Error - failed to get user input\n");
			 free(task);
			 free(command);
			 return (0);
		 }
		 printf("comment: ");
		 comment = get_input();
		 if (comment == NULL) {
			 printf("Error - failed to get user input\n");
			 free(date);
			 free(task);
			 free(command);
			 return (0);
		 }
	 }
	 else if (strcmp(usr,"2") == 0 || strcmp(usr,"3") == 0) {
		 if (strcmp(usr,"2") == 0)
			 strcpy(command, "edit");
		 else
			strcpy(command, "delete");
		 printf("task to %s: ", command);
		 task = get_input();
		 if (task == NULL) {
			 free(command);
			 printf("Error - failed to get user input\n");
			 return (0);
		 }
	 }
	 else {
		 strcpy(command, "print");
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
 * Handles file processing and encoding/decoding operations for task management.
 * 
 * @param command The command to execute (e.g., "add", "edit", "delete", "print").
 * @param task The task description.
 * @param date The task date.
 * @param comment Additional comments for the task.
 * @return Returns 0 on successful operation, -1 on failure.
 */
 int process_file(char *command, char *task, char *date, char *comment) {
	 FILE *fd;
	 int num = 0;
	 char *sep = ";", *val;
	 
	 printf("in process file vals =\n%s\n%s\n%s\n%s\n", command, task, date, comment);
	 if ((fd = fopen(filename, "r")) == NULL) {
		 fd = fopen(filename, "w");
		 if (fd == NULL) {
			 printf("failed to create file. Try again.\n");
			 return (0);
		 }
	 }
	 printf("opened file for reading then close\n");
	 fclose(fd);
	 if (strcmp(command, "print") == 0) {
		 if (head == NULL) {
			 printf("No task to show.\n");
			 return(0);
		 }
		 printf("task\t\tdate\t\tcomment\t\t\n");
		 while (1) {
			 printf("%d  ", num++);
			 printf("%s\t%s\t%s\n", head->task, head->date, head->comment);
			 if (head->next == NULL)
				 break;
			 head = head->next;
		 }
		 shift_node(&head, "up");
		 return (0);
	 }
	 else if (strcmp(command, "add") == 0) {
		 printf("command is add_\n");
		 struct Node *new = (struct Node *)malloc(sizeof(struct Node));
		 if (new == NULL) {
			 printf("Error - unable to allocate memory. Task not added.\n");
			 return (0);
		 }
		 if (head != NULL) shift_node(&head, "down");
		 printf("created new node\n");
		 new->task = strdup(task);
		 new->date = strdup(date);
		 new->comment = strdup(comment);
		 new->next = NULL;
		 new->prev = head;
		 if (head != NULL) head->next = new;
		 else head = new;
		 shift_node(&head, "up");
		 fd = fopen(filename, "a");
		 if (fd == NULL) {
			 printf("error - unable to open file for writting. please try again.\n");
			 n_free(&new);
			 return (0);
		 }
		 printf("opened file for appending");
		 if (ftell(fd) > 0)
			 fwrite(sep, 1, strlen(sep), fd);
		 
		 val = enc_dec("enc", new->task);
		 fwrite(val, 1, strlen(val), fd);
		 fwrite(sep, 1, strlen(sep), fd);
		 free(val);
		 val = enc_dec("enc", new->date);
		 fwrite(val, 1, strlen(val), fd);
		 fwrite(sep, 1, strlen(sep), fd);
		 free(val);
		 val = enc_dec("enc", new->comment);
		 fwrite(val, 1, strlen(val), fd);
		 free(val);
		 fclose(fd);
		 return (0);
	 }
	 else if (strcmp(command, "edit") == 0) {
		 struct Node *ptr;
		 
		 if (head == NULL) {
			 printf("Task not found.\n");
			 return(0);
		 }
		 shift_node(&head, "up");
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
		 process_file("other",NULL, NULL, NULL);
		 return (0);
	 }
	 else if (strcmp(command, "delete") == 0) {
		 if (head == NULL) {
			 printf("Task not found.\n");
			 return(0);
		 }
		 struct Node *ptr;
		 shift_node(&head, "up");
		 while(1) {
			 if (strcmp(head->task, task) == 0) {
				 ptr = head;
				 head = ptr->next;
				 head->prev = ptr->prev;
				 ptr->prev->next = head;
				 n_free(&ptr);
				 process_file("rewrite", NULL, NULL, NULL);
				 break;
			 }
			 if (head->next == NULL) {
				 printf("Task not found\n");
				 break;
			 }
			 head = head->next;
		 }
		 return (0);
	 }
	 else {
		 // write all changes into the file
		 shift_node(&head, "up");
		 
		 fd = fopen(filename, "w");
		 if (fd == NULL) {
			 printf("Error - failed to write changes to the file\n");
			 return (-1);
		 }
		 while (1) {
			 val = enc_dec("enc", head->task);
			 fwrite(val, 1, strlen(val), fd);
			 fwrite(sep, 1, strlen(sep), fd);
			 val = enc_dec("enc", head->date);
			 fwrite(val, 1, strlen(val), fd);
			 fwrite(sep, 1, strlen(sep), fd);
			 val = enc_dec("enc", head->comment);
			 fwrite(val, 1, strlen(val), fd);
			 if (head->next != NULL)
				 fwrite(sep, 1, strlen(sep), fd);
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
 * Encodes or decodes strings for file writing.
 * 
 * @param command The command to indicate encoding or decoding.
 * @param str The string to encode or decode.
 * @return Returns the encoded or decoded string.
 */
 char *enc_dec(char *command, char *str) {
	 char *opt = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz ,.?)'/\"&@!%("};
	 char val;
	 int jump = 8;
	 int i, j, len = strlen(opt);
	 
	 if (str == NULL) return (NULL);
	 char *ret = (char *)malloc((strlen(str) + 1) * sizeof(char));
	 if (ret == NULL) {
		 printf("Error - unable to allocate memory\n");
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
 * Checks whether the passed string is a valid command.
 * 
 * @param command The string to check.
 * @return Returns true if the string is a valid command, false otherwise.
 */
 bool is_command(char *command) {
	 char *cmd[6] = {"print", "help", "delete", "edit", "add", NULL};
	 
	 int i = 0;
	 while (cmd[i] != NULL) {
		 if (strcmp(cmd[i], command) == 0)
			 return (true);
		 i++;
	 }
	 return (false);
 }
 
/**
 * Shifts the head position of the linked list of tasks.
 * 
 * @param hpsn Pointer to the head of the linked list.
 * @param psn Position to shift ("up" or "down").
 */
 void shift_node(struct Node **hpsn, char *psn) {
	 struct Node *ptr, *hptr;
	 
	 hptr = *hpsn;
	 if (*hpsn == NULL)
		 return;
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
 * Constructs a full directory path for the to-do list file.
 * 
 * @param directory The directory path where the program is located.
 * @return Returns the full directory path as a string.
 */
 char *win_directory(char *directory) {
	int i, val, len;
	char *f;
	
	len = strlen(directory) + 15;
	f = (char *)malloc(len * sizeof(char));
	if (f == NULL) {
		printf("Error - unable to allocate required memory\n");
		return (NULL);
	}
	i = 0;
	while(1) {
		if (directory[i] == '\0') break;
		if (directory[i] == 92) val = i;
		i++;
	}
	directory[val] = '\0';
	strcpy(f, directory);
	
	return (strcat(f, "\\to_do_list.txt"));
 }
 
/**
 * Reads user input from stdin and returns it as a string.
 * 
 * @return Returns the user input string.
 */
 char *get_input() {
	 int c, i = 0;
	 size_t max_val = 150;
	 char *str;
	 
	 str = (char *)malloc(max_val * sizeof(char));
	 if (str == NULL) {
		 printf("Error - failed to allocate required memory\n");
		 return (NULL);
	 }
	 // Clear input buffer
	 while ((c = getchar()) != '\n' && c != EOF) {
		 str[i] = c;
		 i++;
		 break;
	 }
	 while (1) {
		 c = getchar();
		 if (c == '\n') {
			 str[i] = '\0';
			 break;
		 }
		 if (i == max_val - 1) {
			 max_val = max_val + 150;
			 str = realloc(str, max_val);
			 if (str == NULL) {
				 printf("Error - failed to reallocate required memory\n");
				 free(str);
				 return (NULL);
			 }
		 }
		 str[i] = c;
		 i++;
	 }
	 return (str);
 }
 
 /**
 * Frees memory allocated for the linked list nodes and their contents.
 */
 void free_node() {
	struct Node *ptr;
	
	shift_node(&head, "up");
	while(head != NULL) {
		ptr = head;
		head = ptr->next;
		n_free(&ptr);
	}
 }
 
 /**
 * Frees memory allocated for a single node in the linked list.
 * 
 * @param p Pointer to the node to be freed.
 */
 void n_free(struct Node **p) {
    if (*p == NULL) return;

    struct Node *ptr = *p;
    if (ptr->task!= NULL) free(ptr->task);
    if (ptr->date!= NULL) free(ptr->date);
    if (ptr->comment!= NULL) free(ptr->comment);
    free(ptr);
    *p = NULL; // set the original pointer to NULL to avoid dangling pointers
}