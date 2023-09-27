# Custom_Shell_with_HPC_Calculator
This project involves developing a custom shell in the C programming language that allows executing commonly used Linux commands. The shell will utilize various system calls and techniques for inter-process communication and synchronization.

Once launched from the BASH terminal, the shell will display a personalized command prompt. When the user enters a specific command in the shell, the system will execute the corresponding command. For example, instead of using the BASH command "ls" to list files and directories in the current directory, the user can enter the command "Lister" in the custom shell. Similarly, to delete a file, the user can use the command "Supprimer" instead of the BASH command "rm".

The shell will employ different techniques for inter-process communication and synchronization to ensure proper command execution. It will use pipes for inter-process communication, signals for synchronization, and other Inter-Process Communication (IPC) mechanisms to facilitate information exchange between processes.

One of the key features of the shell will be the "Calculer" command. This command will allow executing complex calculation operations involving multiple tasks. The user can input initial values, assign each task to a process, and the shell will utilize IPC techniques to perform parallel calculations. The final result will be displayed, along with the execution time for the multiprocess calculation. The shell will also display the execution time for the same operation calculated in sequential mode for comparison purposes.
