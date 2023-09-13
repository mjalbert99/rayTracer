#include "RTScene.h"
#include "RTCube.h"
#include <vector>
#include "RTObj.h"
#include "RTGeometry.h"

using namespace glm;

#include "RTScene.inl"

void RTScene::buildTriangleSoup() {

    camera->computeMatrices();
    shader->view = camera->view;
    shader->projection = camera->proj;
    shader->nlights = light.size();
    shader->lightpositions.resize(shader->nlights);
    shader->lightcolors.resize(shader->nlights);
    int count = 0;
    for (std::pair<std::string, Light*> entry : light) {
        shader->lightpositions[count] = (entry.second)->position;
        shader->lightcolors[count] = (entry.second)->color;
        count++;
    }

    // Define stacks for depth-first search (DFS)
    std::stack < RTNode* > dfs_stack;
    std::stack < glm::mat4 >  matrix_stack; // HW3: You will update this matrix_stack during the depth-first search while loop.

    // Initialize the current state variable for DFS
    RTNode* cur = node["world"]; // root of the tree
    glm::mat4 cur_VM = camera->view; // HW3: You will update this current modelview during the depth first search.  Initially, we are at the "world" node, whose modelview matrix is just camera's view matrix.




     // HW3: The following is the beginning of the depth-first search algorithm.
     // HW3: The depth-first search for the node traversal has already been implemented (cur, dfs_stack).
     // HW3: All you have to do is to also update the states of (cur_VM, matrix_stack) alongside the traversal.  You will only need to modify starting from this line.
    dfs_stack.push(cur);
    matrix_stack.push(cur_VM);

    // Compute total number of connectivities in the graph; this would be an upper bound for
    // the stack size in the depth first search over the directed acyclic graph
    int total_number_of_edges = 0;
    for (const auto& n : node) total_number_of_edges += n.second->childnodes.size();

    while (!dfs_stack.empty()) {
        // Detect whether the search runs into infinite loop by checking whether the stack is longer than the number of edges in the graph.
        if (dfs_stack.size() > total_number_of_edges) {
            std::cerr << "Error: The scene graph has a closed loop." << std::endl;
            exit(-1);
        }

        // top-pop the stacks
        cur = dfs_stack.top();  dfs_stack.pop();
        cur_VM = matrix_stack.top(); matrix_stack.pop();

        // draw all the models at the current node
        for (size_t i = 0; i < cur->models.size(); i++) {
            // Prepare to draw the geometry. Assign the modelview and the material.
            shader->modelview = (cur_VM * cur->modeltransforms[i]); // TODO: HW3: Without updating cur_VM, modelview would just be camera's view matrix.
            shader->material = (cur->models[i])->material;

            // The draw command
            shader->setUniforms();

            for (Triangle t : (cur->models[i])->geometry->elements) {
                //std::cout << "Inside Triangle Soup" << std::endl;
                t.material = (cur->models[i])->material;        //setting triangle material

                glm::mat4 viewI = glm::inverse(shader->view);

                t.P[0] = vec3(viewI * shader->modelview * vec4(t.P[0], 1));
                t.P[1] = vec3(viewI * shader->modelview * vec4(t.P[1], 1));
                t.P[2] = vec3(viewI * shader->modelview * vec4(t.P[2], 1));


                t.N[0] = vec3(normalize(inverse(transpose(mat4(viewI * shader->modelview))) * vec4(t.N[0], 0)));
                t.N[1] = vec3(normalize(inverse(transpose(mat4(viewI * shader->modelview))) * vec4(t.N[1], 0)));
                t.N[2] = vec3(normalize(inverse(transpose(mat4(viewI * shader->modelview))) * vec4(t.N[2], 0)));

                triangle_soup.push_back(t);
            }

        }

        // Continue the DFS: put all the child nodes of the current node in the stack
        for (size_t i = 0; i < cur->childnodes.size(); i++) {
            dfs_stack.push(cur->childnodes[i]);
            matrix_stack.push(cur_VM * cur->childtransforms[i]);
        }

    } // End of DFS while loop. 

}