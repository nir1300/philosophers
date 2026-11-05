# Philosophers

A 42 School project implementing the **Dining Philosophers Problem** in C using POSIX threads (`pthread`) and mutexes.

The goal is to simulate philosophers sitting around a table, sharing forks, and repeatedly eating, sleeping, and thinking — while preventing data races and deadlocks and detecting when a philosopher dies from starvation.

## Table of Contents

- [The Problem](#the-problem)
- [How It Works](#how-it-works)
- [Compilation](#compilation)
- [Usage](#usage)
- [Arguments](#arguments)
- [Simulation Output](#simulation-output)
- [Implementation Details](#implementation-details)
- [Project Structure](#project-structure)

## The Problem

Each philosopher has their own thread and follows the same routine:

1. **Take forks** — a philosopher needs the two forks adjacent to them to eat.
2. **Eat** — eating takes a specified amount of time.
3. **Sleep** — after eating, the philosopher sleeps.
4. **Think** — after sleeping, the philosopher thinks before trying to eat again.

A philosopher dies if they do not start eating again within `time_to_die` milliseconds after their last meal.

The simulation ends when either:
- A philosopher dies.
- Every philosopher has eaten at least the required number of meals, if a meal limit was provided.

## How It Works

The program creates one thread per philosopher and a separate monitoring thread.

- Each philosopher thread executes its own routine.
- Each fork is represented by a mutex, ensuring that only one philosopher can hold a fork at a time.
- Shared data is protected by mutexes to avoid concurrent access.
- The monitoring thread regularly checks whether a philosopher has exceeded their time to die or whether all philosophers have reached the meal limit.

## Compilation

Compile the project using the Makefile:

```bash
make
```

Other available Makefile rules:

```bash
make clean   # Remove object files
make fclean  # Remove object files and the executable
make re      # Rebuild the project
```

The executable is named `philo`.

## Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

Example:

```bash
./philo 5 800 200 200
```

Run with a meal limit:

```bash
./philo 5 800 200 200 7
```

## Arguments

| Argument | Description |
|---|---|
| `number_of_philosophers` | Number of philosophers (and forks) participating in the simulation. |
| `time_to_die` | Maximum time in milliseconds a philosopher can go without starting a meal. |
| `time_to_eat` | Time in milliseconds a philosopher spends eating. |
| `time_to_sleep` | Time in milliseconds a philosopher spends sleeping. |
| `number_of_times_each_philosopher_must_eat` | Optional. The simulation stops once every philosopher has eaten at least this many times. |

All supplied arguments must be positive integers. The optional meal limit can be omitted.

## Simulation Output

Each line reports an event, with the timestamp measured in milliseconds from the start of the simulation.

```text
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
200 1 is sleeping
200 2 is thinking
```

The possible messages are:

- `has taken a fork`
- `is eating`
- `is sleeping`
- `is thinking`
- `died`

The philosopher number starts at `1`.

## Implementation Details

### Threads

Each philosopher is represented by a `t_philos` structure and runs in its own POSIX thread. A separate monitoring thread checks the simulation state.

### Mutexes

The implementation uses mutexes for:

- **Forks (`forks`)** — one mutex per fork, preventing multiple philosophers from using the same fork simultaneously.
- **Output (`output`)** — prevents messages from different threads from being printed simultaneously.
- **Access (`access`)** — protects shared philosopher data, including meal counts, timestamps, and state.
- **Alive (`alive`)** — protects the simulation's running state.

### Fork acquisition

The order in which philosophers acquire forks depends on whether the number of philosophers is even or odd.

- **Even number of philosophers:** fork indices are sorted before locking, so each philosopher locks the lower-indexed fork first. This establishes a consistent lock order.
- **Odd number of philosophers:** even- and odd-numbered philosophers use different acquisition orders, with small delays to stagger contention.

The implementation also staggers the initial start of philosopher routines to reduce the likelihood of all philosophers competing for forks at the same time.

### Monitoring

The monitoring thread repeatedly checks two conditions:

1. **Starvation:** calculates how long each philosopher has gone without eating. If this reaches `time_to_die`, the philosopher is reported as dead and the simulation stops.
2. **Meal limit:** if a meal limit was supplied, checks whether every philosopher has eaten at least that many times. If so, the simulation stops.

### Special case: one philosopher

With only one philosopher, there is only one available fork. The philosopher takes it, but cannot acquire a second fork to eat. The simulation waits until their time to die expires.

### Cleanup

After the simulation ends, the program joins the philosopher and monitoring threads, destroys the mutexes, and frees the allocated memory.

## Project Structure

```text
philo/
├── Makefile
├── philo.h
├── main.c
├── init.c
├── threads.c
├── routine.c
├── routine_helpers.c
├── monitoring.c
├── helpers.c
├── time.c
├── exit.c
└── libft_helpers.c
```

| File | Responsibility |
|---|---|
| `main.c` | Entry point and simulation startup. |
| `init.c` | Argument validation, memory allocation, and initialization. |
| `threads.c` | Thread creation, joining, and mutex destruction. |
| `routine.c` | Main philosopher routine: taking forks, eating, sleeping, and releasing forks. |
| `routine_helpers.c` | Special handling for one philosopher and staggered starts. |
| `monitoring.c` | Death detection and meal-limit monitoring. |
| `helpers.c` | Shared-state checks and fork-locking helpers. |
| `time.c` | Timestamp functions and millisecond-based sleeping. |
| `exit.c` | Cleanup and error handling. |
| `libft_helpers.c` | Small utility functions used by the project. |
| `philo.h` | Structures, includes, and function declarations. |

## Technologies

- C
- POSIX threads (`pthread`)
- Mutexes
- `gettimeofday()` for time measurement
- Make
