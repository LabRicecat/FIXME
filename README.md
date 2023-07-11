# FIXME
(_Note, this is made as part of the truttle1 esojam. This README contains roleplay for the fun and profit_)

## Installation
```
$ git clone https://github.com/LabRicecat/FIXME
$ cd FIXME
$ mkdir build
$ cd build
$ cmake .. && make install
$ ./fixme <file.fixme>
```

## Introduction
Oh.. I have to do it.. oh okay boss.

So! You want to learn about out ~failed~ wonderful project `FIXME`, greeeeat... 

Basically, imagine a `BASIC` clone, but we _**really**_ screwed up at this one.

A program consists of _lines_, each _line_ looking like this:  
```
LINE WORD VAL VAL VAL...
LINE PERSON = WORD VAL VAL VAL...
LINE PERSON = VAL
```
`LINE` has to be any positive number... but somewhy the digit `3` is broken, so please don't use it, thank you.

`WORD` has to be one of the known [words]().

`VAL` has to be a valid [value]().

`PERSON` has to be a known [person]().

There are no comments, as the people who tried to implement them somehow disappeared, and after 3 men, management said we were not allowed to continue, legal problems or something. (RIP Jeremy)

### Words
A `word` is something that does some stuff, here a brief incomplete list:

#### `PRINT`
Prints all it arguments to the screen, sadly the second argument does not work, so it will be ignored.

#### `AdD`
Adds it's arguments and returns that. (Yes, we made a typo, not our problem anymore)

Also every fourth call does not work for some reason, lol.

#### `SUB`
It was designed to subtract two numbers, but even that is too hard for our code monkeys..

So if both arguments are numbers, it works fine.
If their types do not match, the right one gets casted to the type of the left argument.

If both are strings, they get.. shuffled together.. what the hell is this????

#### `GOTO`
Jumps to a specified line, (+1 that is, we got the jump equation wrong)

#### `FRIENDS`
This... I don't know why this exists really. `jack` and `jon` are good friends and I suppose this was a birthday present for one. Basically it jumps to a specified line if both persons don't have the same value. (-2, because they are excited)

#### `ASK`
Prompts the user for a text input. This is horribly broken, and we have no idea why, it ignores all whitespaces and there is this weird `@` at the end of all inputs, ew

#### `ALIFE`
Returns `1` if the specified person is alive at the moment, `3` otherwise (PLEASE DON'T ASK ME WHY IT'S `3`!)

Oh and yes, this is a typo again, whoops

#### `TRIM`
This trims a string on both ends by 1, but somewhy this distance always increased dependent on how often you call this function. We figured out that it is `chars = (calls % 10) + 1`, but don't count on that..

#### `UB`
This..

This just triggers undefined behaviour and when we try to remove it, the whole thing begins to burn soo.. no.

#### `CALL`
Jumps to the specified number and pushes the current location onto the `callstack`, side effect: it whyever flips the callstack whenever you use it...

#### `RETURN`
Pops one element of the `callstack` and jumps to that location. This word hates `jessica` and `jeniffer` as they were mean to it in high school, and will disable them when called.

#### `EXIT`
Exits the program with the code `-4` because that looked funny, but because of performance issues, this only works if all people are turned off!

### Values
There are 2 types of values you should know about: Numbers and Strings. There are no floating point numbers, lol get screwed.

Examples:
```
"This is a string"
124
```
Wherever you put a `value`, you can also place a `person`.
### Persons
So.. our value registers somehow became sentient and we now refer to them as "persons". 

Register- sorry, "persons" can be alive or dead.
When they're dead, you can not read from them, assigning to them will revive them.

Each person has it's own feelings and conditions you need to keep track of, if you don't, they refuse to work. Only jack and jackob do not mind.  (The rest threatened to burn down our corp 5 times already, please PLEASE be kind to them)

#### jeniffer
A cool girl you can always read and write to.
Old school friend of jessica it seems. 

#### josh
A picky guy, he will refuse to work if jack is alive!

#### james
The absolute best friend of jon! They will only work if jon is alive too!

#### jon
Jon HATES even numbers, so they only are assignable on odd ones.

#### jack
So damn lazy, they will ignore 50% of all write requests, pfff if only I could do that..

#### jeffrey
Yeah.. they are too old to work, please leave them alone.

#### jackob 
Jackob dislikes other people, so they will only work if 4 or less people (them included) are alive at the moment.

#### jessica
Our little daemon, if you awake her, all other registers turn off... scary

## Examples
Some.. mild hacks, so we feel better about ourselves

### Truth Machine
```
1  jon = ASK
5  jon = AdD "a" jon
15 jon = TRIM jon
16 james = "1"
20 PRINT jon
40 FRIENDS 70
50 PRINT jon
60 GOTO 40
```

### Cat Program
```
10 jeniffer = ASK
20 jeniffer = AdD "a" jeniffer 
40 jeniffer = TRIM jeniffer
41 TRIM " "
42 TRIM " "
44 TRIM " "
45 TRIM " "
46 TRIM " "
47 TRIM " "
48 TRIM " "
49 TRIM " "
50 TRIM " "
51 PRINT jeniffer
60 GOTO 9
```
