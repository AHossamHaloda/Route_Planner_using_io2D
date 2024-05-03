#include "route_planner.h"
#include <algorithm>

using std::abs;

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y)
    : 
    m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    /* Store the nodes you find in the RoutePlanner's start_node and end_node attributes. */
    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);
    
}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->distance(*end_node);
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    /* add all unvisited neighbors to the neighbors vector */
    current_node->FindNeighbors();

   /* Loop over these neighbors */
   for(auto node: current_node->neighbors)
    {
        /* Calculate h, g values */
        node->parent = current_node;
        node->h_value = CalculateHValue(node);
        node->g_value = current_node->g_value + current_node->distance(*node);
        
        /* add the neighbor to the open list */
        open_list.emplace_back(node);
        node->visited = true;
    }
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

RouteModel::Node *RoutePlanner::NextNode() {
    RouteModel::Node *node {};
    /* Check if the open_list is empty */
    if (open_list.empty()) {
        node = nullptr; // Return nullptr if the open_list is empty
    }

    /*
    * std::sort can be used instead of selection sort
    * std::sort(open_list.begin(), open_list.end(), [](const auto& a, const auto& b) {
        return a->h_value + a->g_value > b->h_value + b->g_value;
        });      
    */
    /* Selection sort algorithm to find the node with the lowest sum of g_value and h_value */
    for (size_t i = 0; i < open_list.size() - 1; ++i) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < open_list.size(); ++j) {
            /* Compare nodes based on the sum of g_value and h_value */
            if ((open_list[j]->g_value + open_list[j]->h_value) < (open_list[minIndex]->g_value + open_list[minIndex]->h_value)) {
                minIndex = j;
            }
        }
        /* Swap the nodes at minIndex and i */
        if (minIndex != i) {
            RouteModel::Node *temp = open_list[minIndex];
            open_list[minIndex] = open_list[i];
            open_list[i] = temp;
        }
    }

    /* Create a pointer to the node with the lowest sum */
     node = open_list.front();

    /* Remove the node from the open_list */
    open_list.erase(open_list.begin());

    return node;
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found{};

    /* Construct the path by adding distances of nodes until we reach the start_node */    
    while (current_node != start_node) {
        path_found.push_back(*current_node);   // path_found.insert(path_found.begin(), *current_node);
        distance += current_node->distance(*current_node->parent);
        current_node = current_node->parent;
    }
    path_found.push_back(*current_node);    // path_found.insert(path_found.begin(), *current_node);
    distance *= m_Model.MetricScale();
    std::cout<<"\n Path is Completed \n";
    /* Reverse the nodes in path_found to keep them in order */
    std::reverse(path_found.begin(), path_found.end());
    return path_found;  
}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.
void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = start_node;

    /* Add first node to the open list and marked it as visited */
    open_list.emplace_back(start_node);
    current_node->visited = true;

    /* Loop over open_list to find the end_node */
    while (!open_list.empty()){
        if (current_node != end_node){
            AddNeighbors(current_node);
            current_node = NextNode();
        }
        else{
            std::cout<<"\n End Node Found !\n";
            break;
        }
    } 
    m_Model.path = ConstructFinalPath(current_node);
}