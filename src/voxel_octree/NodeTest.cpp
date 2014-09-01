#include <iostream>
#include <cstdlib>
#include "Node.h"


int main()
{
    Node * tree = new Node();


    std::cout<< "Starting.." << std::endl;

    for(int i = 0; i < 100000; i++)
    {
        vec3 pos(rand()%65536,rand()%65536,rand()%65536);

        if(tree->GetChild(pos, MAX_DEPTH_LEVEL))
        {
            printf("Already exists at pos: %u %u %u\n", pos.x, pos.y, pos.z);
        }
        else
        {
            //printf("Adding at pos: %u %u %u\n", pos.x, pos.y, pos.z);
        }

        Node * added = tree->AddChild(pos, MAX_DEPTH_LEVEL);
        added->data = i+10;

        Node * n = tree->GetChild(pos, MAX_DEPTH_LEVEL);

        if(added==nullptr)
        {
            std::cout << "Failed to add node" << std::endl;
            continue;
        }

        if(n==nullptr)
        {
            std::cout << "Failed to get node" << std::endl;
            continue;
        }

        if(added->data != n->data)
        {
            std::cout << "Incorrect data, expected: " << (int)added->data << "; Got: " << (int) n->data << std::endl;
        }
        else
        {
            //std::cout << "Success, expected: " << (int)added->data << "; Got: " << (int) n->data << std::endl;
        }
    }

    std::cout<< std::endl << std::endl;
    Node::TraverseLeafNodes(tree, MAX_DEPTH_LEVEL, [](Node * n){ });
    std::cout << "Done" << std::endl;

    delete tree;

    return 0;
}
