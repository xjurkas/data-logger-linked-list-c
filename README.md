# Data Logger Linked List Manager

A command-line application written in C for loading, displaying, inserting, deleting, swapping, and sorting measurement records stored in a singly linked list.

The project was created as a procedural programming assignment and demonstrates dynamic memory management, file processing, structures, pointers, linked-list operations, and basic sorting algorithms.

## Features

- loads measurement records from a text file,
- stores records in a dynamically allocated singly linked list,
- displays all currently loaded records,
- inserts a new record at a selected position,
- deletes records with a specified identifier,
- swaps records at two selected positions,
- sorts records by date and time in ascending or descending order,
- releases all dynamically allocated memory before termination.

## Data model

Each measurement is represented by the `RECORD` structure:

```c
typedef struct record {
    char oznacenie;
    int cislovanie;
    char druh;
    float latitude;
    float longitude;
    char typ[3];
    float value;
    int time;
    int date;
    struct record *next;
} RECORD;
```

A record contains:

- a five-character measurement identifier,
- latitude and longitude,
- a two-character measurement type,
- the measured value,
- time,
- date,
- a pointer to the next record.

## Commands

The application is controlled by entering a single character:

| Command | Description |
| --- | --- |
| `n` | Load records from `dataloger_V2.txt` into the linked list |
| `v` | Display all records currently stored in the list |
| `p` | Insert a new record at a selected position |
| `z` | Delete all records matching a specified ID |
| `r` | Swap records at two selected positions |
| `u` | Sort records by date and time |
| `k` | Exit the application and release allocated memory |

For sorting, enter:

- `0` for ascending order,
- `1` for descending order.

## Requirements

- GCC, Clang, or another C compiler
- C11 support
- the input file `dataloger_V2.txt` placed in the program's working directory

The submitted archive contains the source code only. The required data file is not included in the repository and must be supplied separately before using the `n` command.

## Compilation

### Linux and macOS

```bash
gcc -std=c11 -Wall -Wextra -pedantic 127776_2023_pt2.c -lm -o data_logger
```

Run the program:

```bash
./data_logger
```

### Windows with MinGW GCC

```powershell
gcc -std=c11 -Wall -Wextra -pedantic 127776_2023_pt2.c -lm -o data_logger.exe
```

Run the program:

```powershell
.\data_logger.exe
```

## Example workflow

```text
n   Load records from the input file
v   Display the loaded records
p   Insert a new measurement
u   Sort records by date and time
v   Display the sorted list
k   Exit and release memory
```

The prompts and console output in the application are written in Slovak.

## Implementation details

### Loading records

The `n` command opens `dataloger_V2.txt`, removes any previously loaded list, parses the input records, and dynamically allocates one linked-list node for every measurement.

### Inserting records

The `p` command creates a new node and inserts it at the requested position. When the position is beyond the list length, the record is appended near the end of the list according to the implemented position handling.

### Deleting records

The `z` command traverses the complete list and removes every node whose measurement ID matches the entered ID. Memory belonging to deleted nodes is immediately released.

### Swapping records

The `r` command locates two positions and changes their placement in the linked list by modifying node links.

### Sorting records

The `u` command combines the numeric date and time values for comparison and uses bubble sort to order records chronologically. The current implementation has quadratic time complexity:

```text
O(n²)
```

### Memory management

Records are dynamically allocated with `malloc`. Existing nodes are released before a file is reloaded, deleted nodes are freed immediately, and the remaining list is freed when the program exits.

## Repository structure

```text
.
├── 127776_2023_pt2.c
└── README.md
```

## Known limitations

- The input filename is hard-coded as `dataloger_V2.txt`.
- The input file is not included in the submitted archive.
- Input validation is limited, so records must follow the expected format.
- Sorting uses bubble sort and therefore does not scale efficiently for large datasets.
- The application uses numeric date and time values instead of dedicated date-time structures.
- The project is an educational implementation rather than a production-ready data-management system.

## Educational focus

The project demonstrates the following C programming concepts:

- structures and structure pointers,
- singly linked lists,
- dynamic memory allocation,
- file input,
- string and numeric conversion,
- list traversal and mutation,
- pointer manipulation,
- sorting,
- memory cleanup.

## Author

**Dominik Jurkas**
