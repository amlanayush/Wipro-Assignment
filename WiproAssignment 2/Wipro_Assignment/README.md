# CustomShell  
*A Minimal Unix-Style Shell Written in C++ (Capstone Project)*

This project implements a custom interactive shell similar to Bash, supporting:
- ✅ Running commands with arguments  
- ✅ Background execution using `&`  
- ✅ Job control: `jobs`, `fg %n`, `bg %n`  
- ✅ Process completion tracking  
- ✅ Single pipeline: `A | B`  
- ✅ Input redirection `<`  
- ✅ Output redirection `>`  
- ✅ Simple combined redirection `< file > file`  
- ✅ REPL shell loop (like bash prompt)

All functionality is built from scratch using **fork**, **execvp**, **pipe**, **dup2**, **waitpid**, and signal handling.

---

# ✅ Project Features

### **1. Command Execution**
Run system commands with arguments:
