# C Fundamentals Exercises

Welcome! These exercises will help you get familiar with basic C concepts. Start with Exercise 1 and work your way through. Each exercise builds on previous concepts.

---

## Exercise 1: Variables and Data Types

**Objective:** Learn how to declare and use variables in C.

### Example Code

```c
#include <stdio.h>

int main() {
    int age = 42;
    float pi = 3.14;
    char grade = 'A';

    printf("Age: %d\n", age);
    printf("Pi: %f\n", pi);
    printf("Grade: %c\n", grade);

    return 0;
}
```

**Output:**
```
Age: 42
Pi: 3.140000
Grade: A
```

### Your Task

Create a file named `exercise1.c` and modify the example above to:

1. Create three new variables: `height` (float), `score` (int), and `initial` (char)
2. Assign your own values to each
3. Print them with descriptive labels

**Expected Output Format:**
```
Height: 5.90
Score: 95
Initial: V
```

**Hints:**
- Use `%d` for integers
- Use `%f` for floats
- Use `%c` for characters
- Don't forget `#include <stdio.h>`

---

## Exercise 2: Basic Arithmetic

**Objective:** Perform simple calculations and understand operator precedence.

### Example Code

```c
#include <stdio.h>

int main() {
    int a = 10;
    int b = 3;

    printf("%d + %d = %d\n", a, b, a + b);
    printf("%d - %d = %d\n", a, b, a - b);
    printf("%d * %d = %d\n", a, b, a * b);
    printf("%d / %d = %d\n", a, b, a / b);
    printf("%d %% %d = %d\n", a, b, a % b);

    return 0;
}
```

**Output:**
```
10 + 3 = 13
10 - 3 = 7
10 * 3 = 30
10 / 3 = 3
10 % 3 = 1
```

### Your Task

Create a file named `exercise2.c`. Modify the example to use different numbers and add two more operations:

1. Change `a` to 15 and `b` to 4
2. Keep addition, subtraction, multiplication, division, and modulo
3. Add one more calculation: `(a + b) * 2`

**Expected Output:**
```
15 + 4 = 19
15 - 4 = 11
15 * 4 = 60
15 / 4 = 3
15 % 4 = 3
(15 + 4) * 2 = 38
```

**Key Concepts:**
- `/` performs integer division (truncates decimal)
- `%` gives the remainder
- `%%` prints a literal % symbol

---

## Exercise 3: Working with Strings

**Objective:** Learn how to declare and manipulate character arrays (strings).

### Example Code

```c
#include <stdio.h>
#include <string.h>

int main() {
    char name[] = "Hello";
    int length = strlen(name);

    printf("String: %s\n", name);
    printf("Length: %d\n", length);

    return 0;
}
```

**Output:**
```
String: Hello
Length: 5
```

### Your Task

Create a file named `exercise3.c`. Modify the example to:

1. Create a string with your name (or any name you want)
2. Print the string
3. Print its length
4. Print the first character using `name[0]`
5. Print the last character using `name[length-1]`

**Expected Output (if name is "Vincent"):**
```
String: Vincent
Length: 7
First character: V
Last character: t
```

**Hints:**
- Include `<string.h>` for `strlen()`
- Use `%s` for strings
- Use `%c` for individual characters
- Array indexing starts at 0

---

## Exercise 4: Arrays

**Objective:** Work with arrays of integers.

### Example Code

```c
#include <stdio.h>

int main() {
    int numbers[5] = {10, 20, 30, 40, 50};
    int sum = 0;

    printf("Array elements: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
        sum += numbers[i];
    }
    printf("\n");

    printf("Sum: %d\n", sum);

    return 0;
}
```

**Output:**
```
Array elements: 10 20 30 40 50
Sum: 150
```

### Your Task

Create a file named `exercise4.c`. Modify the example to:

1. Create an array with 5 different numbers (your choice)
2. Print all elements
3. Calculate and print the sum
4. Find and print the largest element
5. Find and print the smallest element

**Expected Output (if using 5 15 3 22 8):**
```
Array elements: 5 15 3 22 8
Sum: 53
Largest: 22
Smallest: 3
```

**Hints:**
- Use a loop to find max and min
- Initialize `max` to a very small number and `min` to a very large number
- Compare each element with current max/min

---

## Exercise 5: Loops and Conditionals

**Objective:** Practice if statements and loops.

### Example Code

```c
#include <stdio.h>

int main() {
    for (int i = 1; i <= 5; i++) {
        if (i % 2 == 0) {
            printf("%d is even\n", i);
        } else {
            printf("%d is odd\n", i);
        }
    }

    return 0;
}
```

**Output:**
```
1 is odd
2 is even
3 is odd
4 is even
5 is odd
```

### Your Task

Create a file named `exercise5.c`. Modify the example to:

1. Loop from 1 to 10 (not 5)
2. Print each number with "even" or "odd"
3. Additionally, print "FIZZ" if the number is divisible by 3
4. Print "BUZZ" if the number is divisible by 5
5. Print all applicable messages

**Expected Output:**
```
1 is odd
2 is even
3 is odd FIZZ
4 is even
5 is odd BUZZ
6 is even FIZZ
7 is odd
8 is even
9 is odd FIZZ
10 is even BUZZ
```

**Hints:**
- Use `num % 2 == 0` to check if even
- Use `num % 3 == 0` to check if divisible by 3
- Use `num % 5 == 0` to check if divisible by 5

---

## Exercise 6: Functions

**Objective:** Write and call your own function.

### Example Code

```c
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int main() {
    int result1 = add(5, 3);
    int result2 = multiply(5, 3);

    printf("5 + 3 = %d\n", result1);
    printf("5 * 3 = %d\n", result2);

    return 0;
}
```

**Output:**
```
5 + 3 = 8
5 * 3 = 15
```

### Your Task

Create a file named `exercise6.c`. Write three functions and call them from main:

1. `add(a, b)` - returns a + b
2. `subtract(a, b)` - returns a - b
3. `divide(a, b)` - returns a / b (integer division)

Then in `main()`, call all three functions with values 20 and 4, and print the results.

**Expected Output:**
```
20 + 4 = 24
20 - 4 = 16
20 / 4 = 5
```

**Hints:**
- Functions must be declared BEFORE main or you need a function prototype
- Use `return` to send a value back to the caller
- Function format: `return_type function_name(parameters) { body }`

---

## Exercise 7: Pointers

**Objective:** Understand pointers and the address-of operator.

### Example Code

```c
#include <stdio.h>

int main() {
    int x = 42;
    int *ptr = &x;

    printf("Value of x: %d\n", x);
    printf("Address of x: %p\n", &x);
    printf("Value at pointer: %d\n", *ptr);

    return 0;
}
```

**Output:**
```
Value of x: 42
Address of x: 0x7fff5fbff8ac
Value at pointer: 42
```

### Your Task

Create a file named `exercise7.c`. Modify the example to:

1. Create two integer variables: `age = 25` and `score = 95`
2. Create pointers to both variables
3. Print:
   - The value of age and its address
   - The value of score and its address
   - The values using the pointers (dereferenced)

**Expected Output:**
```
Age: 25, Address: 0x7fff5fbff8ac
Score: 95, Address: 0x7fff5fbff8b0
Age via pointer: 25
Score via pointer: 95
```

**Key Concepts:**
- `&variable` = address of variable
- `*pointer` = value at the pointer (dereference)
- `%p` = pointer format specifier
- Pointers store memory addresses

---

## Exercise 8: Structs

**Objective:** Create and use structures.

### Example Code

```c
#include <stdio.h>

typedef struct {
    char name[50];
    int age;
    float height;
} Person;

int main() {
    Person person1;
    person1.age = 25;
    person1.height = 5.60;
    strcpy(person1.name, "Alice");

    printf("Name: %s\n", person1.name);
    printf("Age: %d\n", person1.age);
    printf("Height: %.2f\n", person1.height);

    return 0;
}
```

**Output:**
```
Name: Alice
Age: 25
Height: 5.60
```

### Your Task

Create a file named `exercise8.c`. Create a struct and:

1. Define a struct called `Book` with:
   - `char title[100]` - title of the book
   - `char author[50]` - author name
   - `int pages` - number of pages
   - `float rating` - rating out of 10

2. Create two instances of the Book struct
3. Assign values to both books
4. Print all information for both books

**Expected Output:**
```
Book 1:
Title: The C Programming Language
Author: Kernighan & Ritchie
Pages: 274
Rating: 4.80

Book 2:
Title: Clean Code
Author: Robert Martin
Pages: 464
Rating: 4.60
```

**Hints:**
- Use `strcpy()` for string assignment (from `<string.h>`)
- Access struct members with the dot operator: `person.age`
- Use `%.2f` to print floats with 2 decimal places

---

## Exercise 9: File I/O

**Objective:** Read and write files.

### Example Code

```c
#include <stdio.h>

int main() {
    FILE *file = fopen("output.txt", "w");

    fprintf(file, "Line 1\n");
    fprintf(file, "Line 2\n");
    fprintf(file, "Line 3\n");

    fclose(file);

    file = fopen("output.txt", "r");
    char line[100];

    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    fclose(file);

    return 0;
}
```

**Output:**
```
Line 1
Line 2
Line 3
```

### Your Task

Create a file named `exercise9.c`:

1. Open a file for writing called `student_info.txt`
2. Write information about 3 students (name, grade, GPA)
3. Close the file
4. Open the file for reading
5. Read and print all lines back

**File Content (student_info.txt should have):**
```
Alice, Grade: A, GPA: 3.9
Bob, Grade: B, GPA: 3.5
Charlie, Grade: A-, GPA: 3.8
```

**Expected Console Output:**
```
Reading from file:
Alice, Grade: A, GPA: 3.9
Bob, Grade: B, GPA: 3.5
Charlie, Grade: A-, GPA: 3.8
```

**Hints:**
- Use `fopen(filename, "w")` to write, `fopen(filename, "r")` to read
- Always check if file opened successfully: `if (file == NULL)`
- Use `fprintf()` to write to file
- Use `fgets()` to read from file
- Always use `fclose()` when done

---

## Exercise 10: Character Classification

**Objective:** Use `ctype.h` functions to classify characters.

### Example Code

```c
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char text[] = "Hello123!";
    int length = strlen(text);

    for (int i = 0; i < length; i++) {
        char c = text[i];
        if (isupper(c)) {
            printf("%c - uppercase letter\n", c);
        } else if (islower(c)) {
            printf("%c - lowercase letter\n", c);
        } else if (isdigit(c)) {
            printf("%c - digit\n", c);
        } else {
            printf("%c - special character\n", c);
        }
    }

    return 0;
}
```

**Output:**
```
H - uppercase letter
e - lowercase letter
l - lowercase letter
l - lowercase letter
o - lowercase letter
1 - digit
2 - digit
3 - digit
! - special character
```

### Your Task

Create a file named `exercise10.c`. Modify the example to:

1. Use a different string: `char text[] = "Test2025@Code";`
2. Loop through each character
3. Classify and print each character
4. At the end, print a summary count:
   - Total uppercase letters
   - Total lowercase letters
   - Total digits
   - Total special characters

**Expected Output:**
```
T - uppercase letter
e - lowercase letter
s - lowercase letter
t - lowercase letter
2 - digit
0 - digit
2 - digit
5 - digit
@ - special character
C - uppercase letter
o - lowercase letter
d - lowercase letter
e - lowercase letter

Summary:
Uppercase: 2
Lowercase: 8
Digits: 4
Special: 1
```

**Hints:**
- Use `isupper()`, `islower()`, `isdigit()` from `<ctype.h>`
- Keep counters for each category
- Use `strlen()` to get string length

---

## Bonus Challenges

Once you've completed exercises 1-10, try these challenges:

### Challenge 1: Temperature Converter
Write a program that converts Celsius to Fahrenheit. Formula: `F = (C * 9/5) + 32`

### Challenge 2: Simple Calculator
Create a calculator that:
1. Takes two numbers as input
2. Takes an operator (+, -, *, /)
3. Performs the operation and prints the result

### Challenge 3: Find Longest Word
Given a sentence, find and print the longest word in it.

### Challenge 4: Sort an Array
Write a function that sorts an array of integers from smallest to largest.

### Challenge 5: Count Vowels
Count the number of vowels in a string and print the result.

---

## Tips for Success

1. **Start simple** - Make sure each exercise compiles and runs before moving on
2. **Use printf() for debugging** - Print variables to see what's happening
3. **Read error messages** - They usually tell you exactly what's wrong
4. **Compile often** - Don't write 50 lines then compile. Compile every 5-10 lines
5. **Use meaningful variable names** - `age` is better than `a`
6. **Add comments** - Explain what your code does

---

## How to Compile and Run

For each exercise:

```bash
gcc -o exercise1 exercise1.c
./exercise1
```

Or use the Makefile if provided.

---

Good luck! You're learning one of the most powerful programming languages ever created!
