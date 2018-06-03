# Function Solving by Creating Postfix Form


This program solving function by create postfix form of function.


# Reading Function


This program read function from "input.txt" file which in the same folder with executable form of this.


# Sample Input Txt and Sample Output

Variables must be one character, every part of function must be divided by whitespace

a = 3 ;

b = 2 ;

c = a + b * 4 ;

b = c + b * 2 ;

d = a * ( b - c ) / 2 ;


The Final State of Variables


a = 3, b = 15, c = 11, d = 6


The Postfix Expressions


1. Postfix : 3

2. Postfix : 2

3. Postfix : a b 4 * +

4. Postfix : c b 2 * +

5. Postfix : a b c - * 2 /