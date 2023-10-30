# cmpsc472-project-1
Command Line Interfce App for Process Manipulation

## Main Command Line Options:

- Create a new process - proc_create
- List child processes - proc_list
- Monitor Process Memory Usage - proc_monit
- Terminate Child Process - proc_kill (use list function in order to list available processes)
- Create thread - thread_create
- Synchronize threads - thread_sync (threads are created and critical section of code is used to share data scross multiple threads)
- Two processes created to communicate a string between parent and child using pipe - ipc_pipe_share
-  To exit CLI type 8 - Exit

## Process Spawning is done using fork() function 

## Output Example:

### Main Menu

```
Menu:
1. proc_create
2. proc_list
3. proc_monit
4. proc_kill
5. thread_create
6. thread_sync
7. ipc_pipe_share
8. Exit

Enter your choice: 
```

### Process Creation:
```
Enter your choice: 1
Process creation function called.

Menu:
1. proc_create
2. proc_list
3. proc_monit
4. proc_kill
5. thread_create
6. thread_sync
7. ipc_pipe_share
8. Exit

Enter your choice: Child process 38926 running...
```

### Process listing:
```
Enter your choice: 
2
Process list function called.
1. PID: 38926
2. PID: 39161
3. PID: 39171
```

### Memory Monitoring
```
Enter your choice: 3

Enter index of child to monitor: 2
Process monitor function called.
For current process (PID: 39161):
VIRT: 5740 kB, RES: 1024 kB, SHR: 1024 kB
```

### Thead Creation

```
Enter your choice: 5
Thread creation function called.
Thread 0 created.
```

### Thread Sync

```
Thread synchronization function called.
Producer 0 produced 84. Buffer: 84 0 0 0 0 
Consumer 0 consumed 84. Buffer: 0 0 0 0 0 
Producer 0 produced 16. Buffer: 0 16 0 0 0 
Consumer 1 consumed 16. Buffer: 0 0 0 0 0 
Producer 0 produced 87. Buffer: 0 0 87 0 0 
Consumer 0 consumed 87. Buffer: 0 0 0 0 0 
Producer 0 produced 22. Buffer: 0 0 0 22 0 
Consumer 1 consumed 22. Buffer: 0 0 0 0 0 
Producer 0 produced 91. Buffer: 0 0 0 0 91 
Consumer 0 consumed 91. Buffer: 0 0 0 0 0 
Producer 0 produced 27. Buffer: 27 0 0 0 0 
Consumer 1 consumed 27. Buffer: 0 0 0 0 0 
Producer 0 produced 73. Buffer: 0 73 0 0 0 
Consumer 0 consumed 73. Buffer: 0 0 0 0 0 
Producer 0 produced 69. Buffer: 0 0 69 0 0 
Consumer 1 consumed 69. Buffer: 0 0 0 0 0 
Producer 0 produced 83. Buffer: 0 0 0 83 0 
Consumer 0 consumed 83. Buffer: 0 0 0 0 0 
Producer 0 produced 24. Buffer: 0 0 0 0 24 
Consumer 1 consumed 24. Buffer: 0 0 0 0 0 
Producer 0 produced 30. Buffer: 30 0 0 0 0 
Consumer 0 consumed 30. Buffer: 0 0 0 0 0 
Producer 0 produced 59. Buffer: 0 59 0 0 0 
Consumer 1 consumed 59. Buffer: 0 0 0 0 0 
Producer 0 produced 94. Buffer: 0 0 94 0 0 
Consumer 0 consumed 94. Buffer: 0 0 0 0 0 
Producer 0 produced 43. Buffer: 0 0 0 43 0 
Consumer 1 consumed 43. Buffer: 0 0 0 0 0 
Producer 0 produced 22. Buffer: 0 0 0 0 22 
Consumer 0 consumed 22. Buffer: 0 0 0 0 0 
Producer 0 produced 38. Buffer: 38 0 0 0 0 
Consumer 1 consumed 38. Buffer: 0 0 0 0 0 
Producer 0 produced 16. Buffer: 0 16 0 0 0 
Consumer 0 consumed 16. Buffer: 0 0 0 0 0 
Producer 0 produced 27. Buffer: 0 0 27 0 0 
Consumer 1 consumed 27. Buffer: 0 0 0 0 0 
Producer 0 produced 57. Buffer: 0 0 0 57 0 
Consumer 0 consumed 57. Buffer: 0 0 0 0 0 
Producer 0 produced 71. Buffer: 0 0 0 0 71 
Consumer 1 consumed 71. Buffer: 0 0 0 0 0
```

### IPC Pipe Communication

```
Enter your choice: 7
IPC using pipe function called.

Piped message: hey there, this is a message from child process!
```

