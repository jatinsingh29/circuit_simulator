# Circuit Simulator

A graph-based digital circuit simulator in C++ that can parse and evaluate digital circuits described in both ISCAS'85 style and modern/standard style `.bench` benchmark files. The simulator evaluates gates using topological ordering, supports primary input assignment from files or user prompt, and outputs values for all intermediate and final nodes. It also outputs a Graphviz DOT description for visualization on the run-terminal.

## 📂 Project Structure

circuit_simulator/
├── bench_sim.cpp # Main simulator source file 
├── ISCAS'85_form/ # ISCAS'85 style benchmark circuits (.bench)
├── standard_form/ # Converted benchmarks in modern style (.bench)
└── example_inputs/ # Example input vectors for circuits

### File Format Examples

**ISCAS'85_form/**: classic benchmark style
Sum = XOR(A, B)
Carry = AND(A, B)


**standard_form/**: modern style
XOR(Sum, A, B)
AND(Carry, A, B)

## Compile
g++ bench_sim.cpp -o bench_sim

## Run with a benchmark + input vector file
./bench_sim ISCAS'85_form/c17.bench example_inputs/c17_inp.txt


## 📊 Visualization

After the simulator run, simply copy and paste the `.dot` format lines generated in the compiler into the [Graphviz Online visualizer](https://dreampuf.github.io/GraphvizOnline/)

### Node Color Coding:
- **Light blue** → Primary Inputs
- **Light green** → Logic 1
- **Red** → Logic 0
- **Gray** → Unknown (X)
