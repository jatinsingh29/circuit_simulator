Here’s a clean **README.md** in Markdown form for your repo:

```markdown
# 🔌 Circuit Simulator

A **graph-based digital circuit simulator in C++** that can parse and evaluate digital circuits described in both **ISCAS’85 style** and **modern/standard style** `.bench` benchmark files.  

The simulator:  
- Parses `.bench` netlists  
- Evaluates gates using **topological ordering**  
- Supports **primary input assignment** from files or user prompt  
- Outputs values for **all intermediate and final nodes**  
- Exports a **Graphviz DOT description** for visualization  

---

## 📂 Project Structure

```

circuit_simulator/
├── bench_sim.cpp        # Main simulator source file
├── ISCAS'85_form/       # ISCAS'85 style benchmark circuits (.bench)
├── standard_form/       # Converted benchmarks in modern style (.bench)
├── example_inputs/      # Example input vectors for circuits

````

- **ISCAS'85_form/** → classic benchmark style:
  ```bench
  Sum = XOR(A, B)
  Carry = AND(A, B)
````

* **standard_form/** → modern style:

  ```bench
  XOR(Sum, A, B)
  AND(Carry, A, B)
  ```

---

## ⚙️ Build & Run

### Compile

```bash
g++ bench_sim.cpp -o bench_sim
```

### Run with a netlist + input vector file

```bash
./bench_sim ISCAS'85_form/c17.bench example_inputs/c17_inp.txt
```

---

## 📊 Visualization

After running the simulator, the program will print **DOT format** lines.
Copy these into the [Graphviz Online Visualizer](https://dreampuf.github.io/GraphvizOnline/) to see the circuit graph.

**Node color codes:**

* 🟦 Light Blue → Primary Inputs
* 🟩 Light Green → Logic 1
* 🟥 Red → Logic 0
* ⬜ Gray → Unknown (X)

---

## 👤 Author

Developed as part of a project to explore **graph-based digital simulation** and parsing of **benchmark circuits**.

```
