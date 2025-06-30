<h1 align="center">IAED Project – Parking Management System</h1>

## Project Overview

This repository contains the implementation of a parking management system developed for the IAED (Introdução a Algoritmos e Estruturas de Dados) course @IST Instituto Superior Técnico (2023/2024). The project aims to facilitate the management of parking lots, allowing for the creation of parks, registration of vehicle entries and exits, historical consultations, and billing according to a specific tariff structure.

### 📁 Project Details
- **Goal**: Implement a parking management system in **C** that manages up to 20 parking lots, each with a name, capacity, and billing regime (tariff based on 15-minute intervals).

### Key Features
- **Command-Line Interface**: The program interacts with users through commands read from standard input (stdin), including:
  | Command | Description                           |
  |---------|-------------------------------------|
  | `q`     | Terminates the program              |
  | `p`     | Creates/lists parking lots          |
  | `e`     | Registers vehicle entry              |
  | `s`     | Registers vehicle exit               |
  | `v`     | Lists vehicle entries/exits         |
  | `f`     | Displays billing for a parking lot  |
  | `r`     | Removes a parking lot from the system|

## Requirements and Specifications

- **Implementation Language**: C, utilizing dynamic memory allocation.
- **Data Validation**: Rigorous validation of input data (dates, times, license plates, capacity, tariffs).
- **Chronological Order**: The system maintains the chronological order of records.
- **Billing Calculation**: Billing is calculated based on 15-minute intervals, with differentiated tariffs for the first hour, subsequent periods, and a daily maximum limit.
- **Special Consideration**: The system accounts for February 29 (parking lot closed, no charges).
- **Library Restrictions**: Only the following libraries are permitted: `stdio.h`, `stdlib.h`, `ctype.h`, `string.h`.
- **Prohibited Constructs**: The use of `goto`, `extern`, or `qsort` is not allowed.

## Compilation and Execution

To compile the project, use `gcc` with the recommended flags:

```bash
gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o proj1 *.c
```

## Notes

- The project was developed following best practices in software engineering, including iterative testing and validation.
- All functionalities were rigorously tested to ensure reliability and performance.

