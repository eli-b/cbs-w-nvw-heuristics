# CBS With Near-Vertex-Weighted Heuristics

## Requirements:

  1. BOOST (https://www.boost.org/).
  2. OSI (https://github.com/coin-or/Osi), as abstract interface to a MIP solver, is also required.
  3. Currently, Gurobi (https://www.gurobi.com/) is required as the concrete solver library, but integrating another library should be easy.
 
## Parameters:
 1. Heuristics (*choose one*)
    * Zero
    * CG
    * NVW-CG
    * DG
    * NVW-DG
    * WDG
    * NVW-WDG
 2. Symmetry reasoning  (*choose multiple*)
    * None
    * Rectangle
    * Corridor
    * Target
    * Mutex
 
 Many other parameters exist, but are irrelevant to the work of this paper.
