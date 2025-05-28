# lib-management
A simple C-based Library Management System for managing books, users, and book issues/returns.
Features include:

-Add, delete, and search books
-Add and edit users
-Issue and return books
-Data persistence using binary files

How to Build

1. Open a terminal in the project directory.
2. Compile the code using:
```
gcc library.c -o library
```
3. Run the program:
```
./library
```
On Windows:
```
library.exe
```
Data Files
books.dat, users.dat, and issues.dat are created automatically for persistent storage.
Requirements
GCC or compatible C compiler
Usage
Follow the on-screen menu to manage books and users in the library system.
