## Mutual Exclusion in Linux

### GWU CSCI 3411 - Fall 2020 - Lab 5

---

## Topics

1.  Introduction

2.  Exercises

3.  Discussion

4.  Linux multi-threading and mutual exclusion functions

# 1. Introduction

In previous class exercises, we've practiced multi-threading in Linux using `pthreads` and shared memory. In this lab, we will study behaviors in multi-threading when operating on shared resources, and then we will practice mutual exclusion.

# 2. Exercises

In these exercises, we will examine race conditions, identify critical sections, and add mutexes to protect access of shared resources.

## 2.1 Validate Linux Environment

**All group members** should individually start your VMs and run the following

```sh
lscpu
```

Confirm that the operating system reports that the system is operating with more than one core similar to the following:

```sh
Architecture:        x86_64
CPU op-mode(s):      32-bit, 64-bit
Byte Order:          Little Endian
CPU(s):              4
On-line CPU(s) list: 0-3
Thread(s) per core:  2
Core(s) per socket:  2
Socket(s):           1
NUMA node(s):        1
Vendor ID:           AuthenticAMD
```

If any student only has a single CPU, please work alongside a student on a multicore setup.

## 2.2 Install Missing Manual Pages

In a previous lab, we stressed the importance of referring to the man pages while doing work for this class. By default, Ubuntu only includes some of the pthread manpages. Install them all by running the following:

`sudo apt install glibc-doc`

## 2.3 Discuss `race.c` single-core execution

The output of this program was either:

```
Elf
Magic
<repeats 1000 times>
Zombie
Children
<repeats 1000 times>
```

or

```
Zombie
Children
<repeats 1000 times>
Elf
Magic
<repeats 1000 times>
```

While the order might differ run to run depending on the order that the scheduler encounters the threads, the text did not interleave.

Examine the `race.c` file.

As a group, discuss the following:

- What is the shared resource?
- What is the critical section?
- What mechanism is preventing the two threads from interleaving their results?
- Can you think of situations where a single-core system might still interleave results?

## 2.4 Discuss `race.c` multi-core execution

Now let's compile on your multi-core system.

Before you do this, as a group discuss the following:

- How do you predict the program will behave?
- What would be different if your system have 50 cores? 500 cores?

Compile using the provided `Makefile` then run the program `race`.

- Does the program behave as you predicted?
- Why does the program produce the output it does?

_Note: You should look closely at the output and note any exceptional interleaving._

## 2.5 Protect `race.c` with a Mutex

Your objective is to protect the Elf Magic from being eaten by the Zombie Children.

As a first step, add a mutex to `race.c` such that the following prints:

```
Elf
Magic
<repeats 1000 times>
Zombie
Children
<repeats 1000 times>
```

or

```
Zombie
Children
<repeats 1000 times>
Elf
Magic
<repeats 1000 times>
```

- How many times does the mutex get locked and unlocked?
- What does this mean for mutual exclusion, progress, and bounded wait / fairness?

Your solution is using a "course lock." In some ways, this resembles a [big-lock](https://en.wikipedia.org/wiki/Giant_lock), as used by older operating systems.

From now on, we will consider the two print statements together in each thread as the critical section. Modify the mutex to guard this smaller critical section. You should lock the mutex preceding the critical section and unlock the mutex following the critical section.

How do you predict the program will behave now?

Compile and run the program again.

_Note: Again look closely at the output and note any exceptional interleaving._

- Does the program behave as you predicted?
- Why does the program produce the output it does?

## 2.4 Exercise 4

A second source file `race2.c` has been provided. You may use `race.c` as a reference, but you should attempt to write `race2.c` yourself to better familiarize yourself with the pthread library.

_Note: You are not using a mutex in this program, yet!_

Declare the following shared variable:

```c
volatile int var = 0;
```

The `var` variable can be changed by either thread. Compiler optimization may not realize that this variable may change out of context. Thus the compiler may optimize out code that may be essential. The `volatile` keyword gives this information to the compiler. In this case, such an optimization might not happen but it is a good practice to declare shared memory variables as `volatile`.

Create two threads. In each thread increment `var` then decrement `var`. Following the decrement of `var`, use an assertion to verify that `var` is zero. For example:

```c
assert(var == 0);
```

Thinking about control flow sequentially, what values should `var` have? Is this the correct way to conceptualize the behavior? Why or why not?

Compile and run `race2`.

- What behavior does this program exhibit?
- Why does it behave this way? What is the critical section?

Now, add a mutex to `race2.c` and protect the critical section in each thread. Compile and run `race2` again.

- What behavior does this program exhibit?
- Why does it behave this way?
- Is this correct behavior?

## 2.5 Exercise 5

Modify `thread_fn` to match the following:

```c
static void *
thread_fn(void *arg)
{
    while (true) {
        lock();
        var++;
        unlock();

        lock();
        var--;
        assert(var == 0);
        unlock();
    }

    return NULL;
}
```

As a group, discuss what you think will happen and then run this.

Fix this by changing the expression inside of the assert to match what values you think `var` could possibly be at this point.

_fin_
