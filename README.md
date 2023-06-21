# C Multi-User Database

## Introduction
---
### What this program does
C multi-user server that stores keys assigned with values, (using the terminal).

### Where this repository comes from
This repository is the work from, and for, the cs214 the third group project: *"Network Communication"*.   
If interested, the file *"assignment_page.png"*   
*(The team member will not be mentioned for their privacy)*

[This course]( https://www.cs.rutgers.edu/academics/undergraduate/course-synopses/course-details/01-198-214-systems-programming ),
was taken during Spring 2021.

&nbsp;

&nbsp;

## Downloading the repository
---
Run the following in the terminal:    
` git clone https://github.com/AdamFariello/c_multi-user_database `

## Compiling (via terminal)
Enter the folder where the repository was downloaded.   
(Terminal command: ` cd c_multi-user_database ` )

### Compiling the main program
` make `

### (ALTERNATIVELY) Compiling the new version of the main program 
` make allNew `

**This is just server.c, but with a different coding format*

### (OPTIONAL) Checking dependencies
` make allDepends `   

*The command compiles, and then deletes the outputed programs.*   
*This is to make sure the dependencies are working.*

&nbsp;

&nbsp;

## Running the program
---
### Starting the server   
` ./server [port #] `

### Connecting to the server 
#### Using telnet (Localhost)
` telnet localhost [server port #]  `

#### Using telnet (Online)
` telnet [computer ipaddress] [server port #]  `

&nbsp;

&nbsp;

## Commands to run (As outlined in "p3.pdf" section 3)
---
### SET
#### Description
Set a value to a key.   
When done succesfully, 
the command outputs to the user: `OKS` 

#### Use case
`SET` **{press enter}**   
[`Combine length of [key] and [value]`, or higher] **{press enter}**    
`[key]` **{press enter}**    
`[value]` **{press enter}**      

#### Example
`SET`   
`11`   
`[key]`   
`[value]`   

&nbsp;
 
### GET
#### Description
Get the value assigned to a key.   
When done succesfully,
the command outputs to the user the keys value 

#### Use Case   
`GET` **{press enter}**   
[`Length of [key]`, or higher] **{press enter}**   
`[key]` **{press enter}**  

#### Example
`GET`   
`4`    
`day`    

&nbsp;

### DEL
#### Description
Delete the key from the servers storage.   
When the key is requested for deletion, two situations can occur:    
1. If a key is in the server, it outputs the value of the key.   
2. Otherwise: `KNF`; "key-not-found" error.

#### Use Case
`DEL` **{press enter}**   
[Length of `[key]`, or higher] **{press enter}**  
`[key]` **{press enter}**  

#### Example (Server has the key)
`DEL`   
`3`   
`day`   
Sunday   

#### Example (Server doesn't have the key)
`DEL`   
`3`   
`day`    
KNF
