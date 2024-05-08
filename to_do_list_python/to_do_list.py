import os
from pathlib import Path
task_dict = dict()
filename = Path.home()
b_filename = Path.home()
if os.name == "nt":
	filename = str(filename) + "\\Documents\\to_do_list.txt"
	b_filename = str(b_filename) + "\\Documents\\to_do_list_backup.txt"
else:
	filename = str(filename) + "/Documents/to_do_list.txt"
	b_filename = str(b_fiprintlename) + "/Documents/to_do_list_backup.txt"
	

def process_file():
	try:
		with open(filename, 'r') as fd:
			rd = fd.readline()
			if rd == "":
				return
			vals = rd.split(":")
			length = len(vals)
			i = 0
			while i < length - 1:
				if i % 2 == 0:
					task_dict[enc_dec("dec", vals[i])] = enc_dec("dec", vals[i + 1])
				i += 1
			
			check_backup()
	except FileNotFoundError:
		try:
			with open(filename, "w") as fd:
				pass
		except FileNotFoundError:
			print("File directory not found.")
		except PermissionError:
			print("file permission denied.")
		
	except PermissionError:
		print("file permission denied.")
			

def get_input():
	while True:
		usr = input("> ")
		if usr not in ['add', 'edit', 'delete', "print", "exit"]:
			print("invalid command")
			continue
		break
	if usr == 'add':
		add_task()
	elif usr == 'edit':
		edit_task()
	elif usr == 'delete':
		delete_task()
	elif usr == 'print':
		print_task()
	else:
		on_exit()

def add_task():
	while True:
		task = input("Task: ")
		if task == "":
			print("Task cannot be empty. Type exit to cancel.")
			continue
		if task == "exit":
			return
		break
	date = input("Date: ")
	comment = input("comment: ")
	task_dict[task] = date + ',' + comment
	with open(b_filename, 'a') as fd:
		fd.write(enc_dec("enc", "add:"))
		fd.write(enc_dec("enc", task))
		fd.write(':')
		fd.write(enc_dec("enc", date + "," + comment))
		fd.write('\n')

def edit_task():
	while True:
		task = input("Task: ")
		if task == "":
			print("Task cannot be empty. Type exit to cancel.")
			continue
		if task == "exit":
			return
		break
	if task not in task_dict:
		print("'"+ task + "'", "task not found.")
		return
	date = input("Date: ")
	if date == "":
		date = task_dict[task].split(",", maxsplit=1)[0]
	comment = input("comment: ")
	if comment == "":
		comment = task_dict[task].split(",", maxsplit=1)[1]
	task_dict[task] = date + ',' + comment
	with open(b_filename, 'a') as fd:
		fd.write(enc_dec("enc", "edit:"))
		fd.write(enc_dec("enc", task))
		fd.write(':')
		fd.write(enc_dec("enc", date + "," + comment))
		fd.write('\n')
		

def delete_task():
	while True:
		task = input("Task: ")
		if task == "":
			print("Task cannot be empty. Type 'exit' to cancel, 'print' to view saved task(s)")
			continue
		if task == "exit":
			return
		if task == "print":
			print_task()
			continue
		break
	if task not in task_dict:
		print("'"+ task + "'", "task not found")
		return
	del(task_dict[task])
	with open(b_filename, 'w') as fd:
		for key, val in task_dict:
			fd.write(enc_dec("enc", "del:"))
			fd.write(enc_dec("enc", key))
			fd.write(":")
			fd.write(enc_dec("enc", val))
			fd.write("\n")

def print_task():
	if len(task_dict) == 0:
		print("No task to print")
		return
	counter = 0
	print("No. Task\t\tDate\t\tcomment")
	for key in task_dict:
		val = task_dict[key]
		date = val.split(",", maxsplit=1)[0]
		comment = task_dict[key].split(",", maxsplit=1)[1]
		counter += 1
		print(counter, key, "\t", date, "\t", comment)

def enc_dec(command, str_val):
	n_code = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ.,)/?'(&"
	val = 12
	length = len(n_code)
	ret_val = ""
	for char in str_val:
		if char not in n_code:
			ret_val += char
			continue
		index = n_code.find(char)
		if command == "enc":
			if index + val < length:
				ret_val += n_code[index + val]
			else:
				ret_val += n_code[(index + val) - length]
		else:
			if index - val >= 0:
				ret_val += n_code[index - val]
			else:
				ret_val += n_code[(index - val) + length]
	return(ret_val)

def check_backup():
	try:
		with open(b_filename, 'r') as bfd:
			while True:
				rd = bfd.readline()
				if rd == "":
					return
				vals = rd.split(':')
				index = 0
				length = len(vals) - 1
				while(index < length):
					if vals[index] in ["add", "edit"]:
						index += 1
						task_dict[vals[index]] = vals[index + 1] + ',' + vals[index + 2]
					if vals[index] == "del":
						del task_dict[index + 1]
					index += 1
				try:
					with open(filename, 'w') as fd:
						for key, val in task_dict:
							fd.write(enc_dec("enc", task))
							fd.write(":")
							fd.write(enc_dec("enc", val))
							fd.write(":")
				except FileNotFoundError:
					print("File directory not found.")
					exit(1)
				except PermissionError:
					print("file permission denied.")
					exit(1)
	except FileNotFoundError:
		try:
			with open(b_filename, 'w') as bfd:
				pass
		except FileNotFoundError:
			print("backup File directory not found.")
			exit(1)
		except PermissionError:
			print("backup file permission denied.")
			exit(1)
	except PermissionError:
		print("backup file permission denied.")
		exit(1)
	with open(b_filename, 'w') as fd:
		pass

def on_exit():
	with open(filename, 'w') as fd:
		for key, val in task_dict:
			fd.write(enc_dec("enc", task))
			fd.write(":")
			fd.write(enc_dec("enc", val))
			fd.write(":")
	with open(b_filename, 'w') as fd:
		pass
	exit(0)

process_file()
print("Welcome to to do list App.\n\nType 'add', 'edit', 'delete', 'print' to get started.")
print(" add - add task(s)\n edit - edit saved task(s)\n delete - deletes saved task(s)\n print - displays saved task(s)\n exit - closes the program")
while True:
	get_input()
