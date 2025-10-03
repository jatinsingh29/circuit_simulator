# Circuit Simulator

A graph-based digital circuit simulator in C++ that can parse and evaluate digital circuits described in both ISCAS’85 style and modern/standard style `.bench` benchmark files. The simulator evaluates gates using topological ordering, supports primary input assignment from files or user prompt, and outputs values for all intermediate and final nodes. It also exports a Graphviz DOT description for visualization.

## Project Structure

circuit_simulator/
├── bench_sim.cpp      # Main simulator source file
├── ISCAS'85_form/     # ISCAS'85 style benchmark circuits (.bench)
├── standard_form/     # Converted benchmarks in modern style (.bench)
└── example_inputs/    # Example input vectors for circuits


- **ISCAS'85_form/**: Classic benchmark style:
Sum = XOR(A, B)
Carry = AND(A, B)


- **standard_form/**: Modern style:
XOR(Sum, A, B)
AND(Carry, A, B)


## Build & Run

### Compile
```bash
g++ bench_sim.cpp -o bench_sim
Run
To run the simulator with a netlist and an input vector file:

Bash

./bench_sim ISCAS'85_form/c17.bench example_inputs/c17_inp.txt
Visualization
After the simulator run, simply copy and paste the .dot format lines generated in the terminal into the Graphviz Online visualizer.

Each node in the visualization is color-coded:

Light blue → Primary Inputs

Light green → Logic 1

Red → Logic 0

Gray → Unknown (X)
