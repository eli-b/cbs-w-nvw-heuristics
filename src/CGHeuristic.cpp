#include "CBSHeuristic.h"

int CGHeuristic::minimumVertexCover(const std::vector<int>& CG, int old_mvc, int cols, int num_of_CGedges)
{
	clock_t t = clock();
	int rst = 0;
	if (num_of_CGedges < 2)
		return num_of_CGedges;
	// Compute #CG nodes that have edges
	int num_of_CGnodes = 0;
	for (int i = 0; i < cols; i++)
    {
      for (int j = 0; j < cols; j++)
        {
          if (CG[i * cols + j] > 0)
            {
              num_of_CGnodes++;
              break;
            }
        }
    }

	if (old_mvc == -1)
    {
      for (int i = 1; i < num_of_CGnodes; i++)
        {
          if (KVertexCover(CG, num_of_CGnodes, num_of_CGedges, i, cols))
            {
              rst = i;
              break;
            }
        }
      assert(rst > 0);
    }
	else
    {
      if (KVertexCover(CG, num_of_CGnodes, num_of_CGedges, old_mvc - 1, cols))
        rst = old_mvc - 1;
      else if (KVertexCover(CG, num_of_CGnodes, num_of_CGedges, old_mvc, cols))
        rst = old_mvc;
      else
        rst = old_mvc + 1;
    }
	runtime_solve_MVC += (double) (clock() - t) / CLOCKS_PER_SEC;
	return rst;
}


// Whether there exists a k-vertex cover solution
bool CGHeuristic::KVertexCover(const std::vector<int>& CG, int num_of_CGnodes, int num_of_CGedges, int k, int cols)
{
	double runtime = (double)(clock() - start_time) / CLOCKS_PER_SEC;
	if (runtime > time_limit)
		return true; // run out of time
	if (num_of_CGedges == 0)
		return true;
	else if (num_of_CGedges > k * num_of_CGnodes - k)
		return false;

	std::vector<int> node(2);
	bool flag = true;
	for (int i = 0; i < cols - 1 && flag; i++) // to find an edge
	{
		for (int j = i + 1; j < cols && flag; j++)
		{
			if (CG[i * cols + j] > 0)
			{
				node[0] = i;
				node[1] = j;
				flag = false;
			}
		}
	}
	for (int i = 0; i < 2; i++)
	{
		std::vector<int> CG_copy(CG.size());
		CG_copy.assign(CG.cbegin(), CG.cend());
		int num_of_CGedges_copy = num_of_CGedges;
		for (int j = 0; j < cols; j++)
		{
			if (CG_copy[node[i] * cols + j] > 0)
			{
				CG_copy[node[i] * cols + j] = 0;
				CG_copy[j * cols + node[i]] = 0;
				num_of_CGedges_copy--;
			}
		}
		if (KVertexCover(CG_copy, num_of_CGnodes - 1, num_of_CGedges_copy, k - 1, cols))
			return true;
	}
	return false;
}

int CGHeuristic::computeInformedHeuristicsValue(CBSNode& curr, double time_limit){

  int h = -1;
	int num_of_CGedges = 0;
	vector<int> HG(num_of_agents * num_of_agents, 0); // heuristic graph
  buildCardinalConflictGraph(curr, HG, num_of_CGedges);
  // Minimum Vertex Cover
  if (curr.parent == nullptr || num_of_CGedges > ILP_edge_threshold) // root node of CBS tree or the CG is too large
    h = minimumVertexCover(HG);
  else
    h = minimumVertexCover(HG, curr.parent->h_val, num_of_agents, num_of_CGedges);
  return h;

}

void CGHeuristic::buildCardinalConflictGraph(CBSNode& curr, vector<int>& CG, int& num_of_CGedges)
{
	num_of_CGedges = 0;
	for (const auto& conflict : curr.conflicts)
    {
      if (conflict->priority == conflict_priority::CARDINAL)
        {
          int a1 = conflict->a1;
          int a2 = conflict->a2;
          if (!CG[a1 * num_of_agents + a2])
            {
              CG[a1 * num_of_agents + a2] = true;
              CG[a2 * num_of_agents + a1] = true;
              num_of_CGedges++;
            }
        }
    }
	runtime_build_dependency_graph += (double)(clock() - start_time) / CLOCKS_PER_SEC;
}
