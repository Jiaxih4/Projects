# Welcome to Homework 0!

For these questions you'll need the mini course and  "Linux-In-TheBrowser" virtual machine (yes it really does run in a web page using javascript) at -

http://cs-education.github.io/sys/

Let's take a look at some C code (with apologies to a well known song)-
```C
// An array to hold the following bytes. "q" will hold the address of where those bytes are.
// The [] mean set aside some space and copy these bytes into teh array array
char q[] = "Do you wanna build a C99 program?";

// This will be fun if our code has the word 'or' in later...
#define or "go debugging with gdb?"

// sizeof is not the same as strlen. You need to know how to use these correctly, including why you probably want strlen+1

static unsigned int i = sizeof(or) != strlen(or);

// Reading backwards, ptr is a pointer to a character. (It holds the address of the first byte of that string constant)
char* ptr = "lathe"; 

// Print something out
size_t come = fprintf(stdout,"%s door", ptr+2);

// Challenge: Why is the value of away equal to 1?
int away = ! (int) * "";


// Some system programming - ask for some virtual memory

int* shared = mmap(NULL, sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
munmap(shared,sizeof(int*));

// Now clone our process and run other programs
if(!fork()) { execlp("man","man","-3","ftell", (char*)0); perror("failed"); }
if(!fork()) { execlp("make","make", "snowman", (char*)0); execlp("make","make", (char*)0)); }

// Let's get out of it?
exit(0);
```

## So you want to master System Programming? And get a better grade than B?
```C
int main(int argc, char** argv) {
	puts("Great! We have plenty of useful resources for you, but it's up to you to");
	puts(" be an active learner and learn how to solve problems and debug code.");
	puts("Bring your near-completed answers the problems below");
	puts(" to the first lab to show that you've been working on this.");
	printf("A few \"don't knows\" or \"unsure\" is fine for lab 1.\n"); 
	puts("Warning: you and your peers will work hard in this class.");
	puts("This is not CS225; you will be pushed much harder to");
	puts(" work things out on your own.");
	fprintf(stdout,"This homework is a stepping stone to all future assignments.\n");
	char p[] = "So, you will want to clear up any confusions or misconceptions.\n";
	write(1, p, strlen(p) );
	char buffer[1024];
	sprintf(buffer,"For grading purposes, this homework 0 will be graded as part of your lab %d work.\n", 1);
	write(1, buffer, strlen(buffer));
	printf("Press Return to continue\n");
	read(0, buffer, sizeof(buffer));
	return 0;
}
```
## Watch the videos and write up your answers to the following questions

**Important!**

The virtual machine-in-your-browser and the videos you need for HW0 are here:

http://cs-education.github.io/sys/

The coursebook:

http://cs341.cs.illinois.edu/coursebook/index.html

Questions? Comments? Use Ed: (you'll need to accept the sign up link I sent you)
https://edstem.org/

The in-browser virtual machine runs entirely in Javascript and is fastest in Chrome. Note the VM and any code you write is reset when you reload the page, *so copy your code to a separate document.* The post-video challenges (e.g. Haiku poem) are not part of homework 0 but you learn the most by doing (rather than just passively watching) - so we suggest you have some fun with each end-of-video challenge.

HW0 questions are below. Copy your answers into a text document (which the course staff will grade later) because you'll need to submit them later in the course. More information will be in the first lab.

## Chapter 1

In which our intrepid hero battles standard out, standard error, file descriptors and writing to files.

### Hello, World! (system call style)
1. Write a program that uses `write()` to print out "Hi! My name is `<Your Name>`".
// write(1, "Hi! My name is Jiaxi Huang", 26);
### Hello, Standard Error Stream!
2. Write a function to print out a triangle of height `n` to standard error.
   - Your function should have the signature `void write_triangle(int n)` and should use `write()`.
   - The triangle should look like this, for n = 3:
   ```C
   *
   **
   ***
   ```
//
void write_triangle(int n) {
    for(int c = 0; c < n + 1; c++) {
        for(int i = 0; i < c; i++) {
            write(1, "*", 1);
        }
        write(1,"\n", 1);
    }
}
//
### Writing to files
3. Take your program from "Hello, World!" modify it write to a file called `hello_world.txt`.
   - Make sure to to use correct flags and a correct mode for `open()` (`man 2 open` is your friend).
//
int f = open("hello_world.txt", O_APPEND, S_IRWXU);
write(f, "Hi! My name is Jiaxi Huang", 26);
close(f);
//
### Not everything is a system call
4. Take your program from "Writing to files" and replace `write()` with `printf()`.
   - Make sure to print to the file instead of standard out!
//
close(1);
int f = open("hello_world.txt", O_APPEND, S_IRWXU);
printf("Hello world!\n");
close(f);
//
5. What are some differences between `write()` and `printf()`?
//
Some differences between write and printf is that write only writes data as strings while printf can write data as many formats, such as integers and floats. Also, write is a system call while printf is not so it doesn't cost as much to the system as printf.
//
## Chapter 2

Sizing up C types and their limits, `int` and `char` arrays, and incrementing pointers

### Not all bytes are 8 bits?
1. How many bits are there in a byte?
//
at least 8 bits in a byte
//
2. How many bytes are there in a `char`?
//
1 byte in a char
//
3. How many bytes the following are on your machine?
   - `int`, `double`, `float`, `long`, and `long long`
//
int - 4 bytes
double - 8 bytes
float - 4 bytes
long - 4 bytes
longlong - 8 bytes
//
### Follow the int pointer
4. On a machine with 8 byte integers:
```C
int main(){
    int data[8];
} 
```
If the address of data is `0x7fbd9d40`, then what is the address of `data+2`?
//
0x7fbd9d50
//
5. What is `data[3]` equivalent to in C?
   - Hint: what does C convert `data[3]` to before dereferencing the address?
//
*(data+3)
//
### `sizeof` character arrays, incrementing pointers
  
Remember, the type of a string constant `"abc"` is an array.

6. Why does this segfault?
```C
char *ptr = "hello";
*ptr = 'J';
```
//
You get segfault because the instructions for the cpu can only be read so ptr can only be read and can't write.
//
7. What does `sizeof("Hello\0World")` return?
//
12
//
8. What does `strlen("Hello\0World")` return?
//
5
//
9. Give an example of X such that `sizeof(X)` is 3.
//
sizeof("ab")
//
10. Give an example of Y such that `sizeof(Y)` might be 4 or 8 depending on the machine.
//
sizeof(1)
//
## Chapter 3

Program arguments, environment variables, and working with character arrays (strings)

### Program arguments, `argc`, `argv`
1. What are two ways to find the length of `argv`?
//
argc stores the length of argv. Another way is to use a loop and go through argv until you reach the end. Everytime you loop, you increment count by one and the result of count is the length of argv.
//
2. What does `argv[0]` represent?
//
argv represents the execution name of the program.
//
### Environment Variables
3. Where are the pointers to environment variables stored (on the stack, the heap, somewhere else)?
//
environment variables are internally stored above the stack of main() function.
//
### String searching (strings are just char arrays)
4. On a machine where pointers are 8 bytes, and with the following code:
```C
char *ptr = "Hello";
char array[] = "Hello";
```

What are the values of `sizeof(ptr)` and `sizeof(array)`? Why?
//
sizeof(ptr) would be 8 because pointers are 8 bytes on the machine. sizeof(array) would be 6 because the array need 6 bytes for each char and an extra byte for null char at the end.
//

### Lifetime of automatic variables

5. What data structure manages the lifetime of automatic variables?
//
A stack data structure manages the lifetime of automatic variables
//
## Chapter 4

Heap and stack memory, and working with structs

### Memory allocation using `malloc`, the heap, and time
1. If I want to use data after the lifetime of the function it was created in ends, where should I put it? How do I put it there?
//
You should put it inside heap by using malloc(number of bytes) and strcpy
//
2. What are the differences between heap and stack memory?
//
The difference between stack and heap is that stack is inflexible and stores the order of method execution, while heap is flexible and stores the dynamic memory allocation and deallocation.
//
3. Are there other kinds of memory in a process?
//
There is also global/static memory
//
4. Fill in the blank: "In a good C program, for every malloc, there is a ___".
//
"In a good C program, for every malloc, there is a free".
//
### Heap allocation gotchas
5. What is one reason `malloc` can fail?
//
One reason malloc can fail is that there is not enough heap memory
//
6. What are some differences between `time()` and `ctime()`?
//
the difference between time and ctime is that time returns a time_t object while ctime returns a string/char*
//
7. What is wrong with this code snippet?
```C
free(ptr);
free(ptr);
```
//
You can't free a pointer twice because it can confuse the bookkeeping memory of which memory is free
//
8. What is wrong with this code snippet?
```C
free(ptr);
printf("%s\n", ptr);
```
//
You can't use a memory after it has been freed because that pointer has become invalid.
//
9. How can one avoid the previous two mistakes? 
//
One strategy is that once you free a memory, set it to NULL
//
### `struct`, `typedef`s, and a linked list
10. Create a `struct` that represents a `Person`. Then make a `typedef`, so that `struct Person` can be replaced with a single word. A person should contain the following information: their name (a string), their age (an integer), and a list of their friends (stored as a pointer to an array of pointers to `Person`s).

//
struct Person {
char* name;
int age;
struct Person* friends[50];
};
typedef struct Person Person;
//
11. Now, make two persons on the heap, "Agent Smith" and "Sonny Moore", who are 128 and 256 years old respectively and are friends with each other.
### Duplicating strings, memory allocation and deallocation of structures
Create functions to create and destroy a Person (Person's and their names should live on the heap).
//
int main() {
   Person* p1 = (Person) malloc(sizeof(Person));
   Person p2 = (Person*) malloc(sizeof(Person));
   p1->name = "Agent Smith";
   p1->age = 128;
   p2->name =  "Sonny Moore";
   p2->age = 256;
   p1->friends[0] = p2;
   p2->friends[0] = p1;
   return 0;
}
//
12. `create()` should take a name and age. The name should be copied onto the heap. Use malloc to reserve sufficient memory for everyone having up to ten friends. Be sure initialize all fields (why?).
//
Person* person_create(char* n, int a) {
    Person* p1 = (Person) malloc(sizeof(Person));
    p1 -> name = strdup(n);
    p1 -> age = a;
    for(int i = 0; i < 10; i++) {
        p1->friends[i] = NULL;
    }
    return p1;
}
//
13. `destroy()` should free up not only the memory of the person struct, but also free all of its attributes that are stored on the heap. Destroying one person should not destroy any others.
//
void person_destroy(Person p) {
    free(p->name);
    for (int i = 0; i < 10; i++) {
        free(p->friends[i]);
    }
    memset(p, 0, sizeof(Person));
    free(p);
}
//

## Chapter 5 

Text input and output and parsing using `getchar`, `gets`, and `getline`.

### Reading characters, trouble with gets
1. What functions can be used for getting characters from `stdin` and writing them to `stdout`?
//
getschar() gets the character from stdin and putchar() put them in stdout
//
2. Name one issue with `gets()`.
//
One issue with gets() is that you can't enter more than 12 bytes
//
### Introducing `sscanf` and friends
3. Write code that parses the string "Hello 5 World" and initializes 3 variables to "Hello", 5, and "World".
//
char* d = "Hello 5 World";
char c[5];
int i = 1;
char b[5];
sscanf(d, "%s %d %s", c, &i, b);
//
### `getline` is useful
4. What does one need to define before including `getline()`?
//
you need to define _GNU_SOURCE
//
5. Write a C program to print out the content of a file line-by-line using `getline()`.
//
FILE f;
f = fopen("filename.txt", "r");
charline = NULL;
size_t i = 0;
while(getline(&line, &i, f) != -1) {
  printf("%s", line);}
fclose(f);
free(line);
//

## C Development

These are general tips for compiling and developing using a compiler and git. Some web searches will be useful here

1. What compiler flag is used to generate a debug build?
//
gcc -g
//
2. You modify the Makefile to generate debug builds and type `make` again. Explain why this is insufficient to generate a new build.
//
It might not be sufficient because modifying the makefile and not the source code will not allow the make to detect any changes so it won't make a new build.
//
3. Are tabs or spaces used to indent the commands after the rule in a Makefile?
//
You use tab to indent
//
4. What does `git commit` do? What's a `sha` in the context of git?
//
git commit records the current state of the project. "SHA" stands for Simple Hashing Algorithm and it gives the commit a unique ID.
//
5. What does `git log` show you?
//
git log shows a list of past commits
//
6. What does `git status` tell you and how would the contents of `.gitignore` change its output?
//
git status shows the state of the current project and what has been changed. gitignore changes what is shown and what is not shown. it will hide whatever you can to ignore.
//
7. What does `git push` do? Why is it not just sufficient to commit with `git commit -m 'fixed all bugs' `?
//
git push uploads the current commit from a local repository to a remote repository. it is not just sufficient to commit with git commit -m 'fixed all bugs' because you need to be more specific with the changes that you made so that you or others will know why you commit.
//
8. What does a non-fast-forward error `git push` reject mean? What is the most common way of dealing with this?
//
a non-fast-forward error git push reject means that the git cannot commit to the remote repository. the most common way of dealing with this is to use git pull.
//

## Optional (Just for fun)
- Convert your a song lyrics into System Programming and C code and share on Ed.
- Find, in your opinion, the best and worst C code on the web and post the link to Ed.
- Write a short C program with a deliberate subtle C bug and post it on Ed to see if others can spot your bug.
- Do you have any cool/disastrous system programming bugs you've heard about? Feel free to share with your peers and the course staff on Ed.
