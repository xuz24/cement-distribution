# 🏗️ Cement Distribution

## 📄 Description
This project models the optimal distribution of cement from one state to another based on given weights and supply/demand.

## 📦 Dependencies
- C++17 or newer
- Standard C++ libraries
- `make` and a working C++ compiler (e.g., `g++`)

## 🔧 Build Instructions
To compile the program, run:

```bash
make cement_flow
```

Make sure make and a C++ compiler are installed.

## 🚀 Usage
```bash
./cement_flow -m {MIN|MAX|CITIES} -i {INPUT_FILE} -s {SUPPLY_DEMAND_FILE} > {OUTPUT_FILE (optional)}
```
Arguments:

`-m` Mode of operation
- `MIN` uses averaged minimum border distances as weights

- `MAX` uses averaged maximum border distances as weights

- `CITIES` uses averaged 5 largest cities by population distances as weights

`-i` Input file
- A CSV file with cement plant distances (e.g., CementPlantDists.csv)

`-s` Supply/Demand file
- A CSV file defining the supply and demand at states (e.g., supply_demand.csv)

`> {OUTPUT_FILE}` (Optional) Redirect output to a file

💡 Example:
```
./cement_flow -m MIN -i CementPlantDists.csv -s supply_demand.csv > min_output.txt
```