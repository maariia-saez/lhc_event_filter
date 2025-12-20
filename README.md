# LHC Event Filter & Analyzer

## Overview
This project simulates a **High-Throughput Data Acquisition System (DAQ)** typical of a high-energy physics experiment like those at CERN.

It consists of a modular C application designed to parse, filter, and analyze massive strams of particle collision data. The system demonstrates efficient memory management using **dynamic data structures** to handle unoredictable data volumnes, compying with strict ANSI C standards.

## KEY FEATURES
* **Dynamic Memory MAnagement:** Implementation of resizable arrays (vectors) to handle growing datasets efficiently without memory leaks.
* **Binary & Text I/O:** Robust parsing of raw data streams.
* **Modular Architecture:** Separation of concerns between generation, storage, and analysis logic.
* **Automated Build System:** Professional `Makefile`with strict compilation flags (`-Wall -Werror -Wextra -ansi -pedantic`).

## Project Structure
```text
.
├── src/            # Source files (.c) - Core logic
├── include/        # Header files (.h) - API definitions
├── data/           # Simulation datasets
├── scripts/        # Bash automation scripts
├── obj/            # Compiled object files (auto-generated)
├── bin/            # Executables (auto-generated)
└── Makefile        # Build automation
```

## How to Build
TO compile the entire project, simply run:
```bash
make
```

This will generate two executables in the bin/ directory:
* `generator`: Creates synthetic collision data.
* `analyzer`: Processes and filters the events.

## Usage
*(Pending implementation)*
```bash
./bin/generator > data/raw_events.dat
./bin/analyzer < data/raw_events.dat
```

## Author
María Sáez Díaz *Mathematics and Computer Science Student*
