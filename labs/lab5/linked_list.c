#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int data;
    struct Node* next;
    } Node;

typedef struct LinkedList{
    Node* head;
    int size;
}    

/*Using malloc to allocate space in heap. Dynamic Array to Store Data*/ 
Node* createNode(int data){
    Node *node = malloc(sizeof(*node));
    (*node).data = data;
    (*node).next = NULL;
    return node;
}

LinkedList* createLinkedList(){
    LinkedList *list = malloc(sizeof(*list));
    list->head = NULL;
    list->size = 0;
    return list;
}

LinkedList* appendLinkedList(LinkedList *list, int data){
    Node* node = createNode(data);
    LinkedList *newLinkedList = malloc(sizeof(list)+1);
    Node* current = list->head;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = node;
    current->size = sizeof(newLinkedList);
}



void printNode(Node* node){
    printf("Node: address %p, data: %d, next: %p\n", node, (*node).data, (*node).next);
}
    
int main(){
    // Node firstNode = {1, NULL};
    // Node secondNode = {2, NULL};
    Node* firstNode = createNode(1);
    Node* secondNode = createNode(2);
    firstNode->next = secondNode;
    /* Function arguments are passed by value in C. 
    When you call printNode(firstNode), the function receives a copy of the entire,
    not the original firstNode inside main. So the two node will print the same address
    Warning: need to use pointer(address) here */
    printNode(firstNode);
    printNode(secondNode);

    free(secondNode);
    free(firstNode);

    return 0;

}