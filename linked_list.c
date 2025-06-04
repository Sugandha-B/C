#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Node structure for singly linked list
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Linked list structure
typedef struct LinkedList {
    Node* head;
    int size;
} LinkedList;

// Function prototypes
LinkedList* createList();
Node* createNode(int data);
void insertAtBeginning(LinkedList* list, int data);
void insertAtEnd(LinkedList* list, int data);
void insertAtPosition(LinkedList* list, int data, int position);
bool deleteAtBeginning(LinkedList* list);
bool deleteAtEnd(LinkedList* list);
bool deleteAtPosition(LinkedList* list, int position);
bool deleteByValue(LinkedList* list, int value);
void traverseList(LinkedList* list);
void traverseReverse(LinkedList* list, Node* node);
int searchElement(LinkedList* list, int value);
int getSize(LinkedList* list);
bool isEmpty(LinkedList* list);
void clearList(LinkedList* list);
void destroyList(LinkedList* list);
void displayMenu();

// Create a new linked list
LinkedList* createList() {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list == NULL) {
        printf("Error: Memory allocation failed for list\n");
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    return list;
}

// Create a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Error: Memory allocation failed for node\n");
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Insert element at the beginning
void insertAtBeginning(LinkedList* list, int data) {
    if (list == NULL) {
        printf("Error: List is NULL\n");
        return;
    }
    
    Node* newNode = createNode(data);
    if (newNode == NULL) return;
    
    newNode->next = list->head;
    list->head = newNode;
    list->size++;
    printf("Element %d inserted at beginning\n", data);
}

// Insert element at the end
void insertAtEnd(LinkedList* list, int data) {
    if (list == NULL) {
        printf("Error: List is NULL\n");
        return;
    }
    
    Node* newNode = createNode(data);
    if (newNode == NULL) return;
    
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        Node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    list->size++;
    printf("Element %d inserted at end\n", data);
}

// Insert element at specific position (0-indexed)
void insertAtPosition(LinkedList* list, int data, int position) {
    if (list == NULL) {
        printf("Error: List is NULL\n");
        return;
    }
    
    if (position < 0 || position > list->size) {
        printf("Error: Invalid position %d. Valid range: 0-%d\n", position, list->size);
        return;
    }
    
    if (position == 0) {
        insertAtBeginning(list, data);
        return;
    }
    
    Node* newNode = createNode(data);
    if (newNode == NULL) return;
    
    Node* current = list->head;
    for (int i = 0; i < position - 1; i++) {
        current = current->next;
    }
    
    newNode->next = current->next;
    current->next = newNode;
    list->size++;
    printf("Element %d inserted at position %d\n", data, position);
}

// Delete element from beginning
bool deleteAtBeginning(LinkedList* list) {
    if (list == NULL || list->head == NULL) {
        printf("Error: List is empty or NULL\n");
        return false;
    }
    
    Node* temp = list->head;
    int data = temp->data;
    list->head = list->head->next;
    free(temp);
    list->size--;
    printf("Element %d deleted from beginning\n", data);
    return true;
}

// Delete element from end
bool deleteAtEnd(LinkedList* list) {
    if (list == NULL || list->head == NULL) {
        printf("Error: List is empty or NULL\n");
        return false;
    }
    
    if (list->head->next == NULL) {
        return deleteAtBeginning(list);
    }
    
    Node* current = list->head;
    while (current->next->next != NULL) {
        current = current->next;
    }
    
    int data = current->next->data;
    free(current->next);
    current->next = NULL;
    list->size--;
    printf("Element %d deleted from end\n", data);
    return true;
}

// Delete element at specific position (0-indexed)
bool deleteAtPosition(LinkedList* list, int position) {
    if (list == NULL || list->head == NULL) {
        printf("Error: List is empty or NULL\n");
        return false;
    }
    
    if (position < 0 || position >= list->size) {
        printf("Error: Invalid position %d. Valid range: 0-%d\n", position, list->size - 1);
        return false;
    }
    
    if (position == 0) {
        return deleteAtBeginning(list);
    }
    
    Node* current = list->head;
    for (int i = 0; i < position - 1; i++) {
        current = current->next;
    }
    
    Node* nodeToDelete = current->next;
    int data = nodeToDelete->data;
    current->next = nodeToDelete->next;
    free(nodeToDelete);
    list->size--;
    printf("Element %d deleted from position %d\n", data, position);
    return true;
}

// Delete first occurrence of a value
bool deleteByValue(LinkedList* list, int value) {
    if (list == NULL || list->head == NULL) {
        printf("Error: List is empty or NULL\n");
        return false;
    }
    
    if (list->head->data == value) {
        return deleteAtBeginning(list);
    }
    
    Node* current = list->head;
    while (current->next != NULL && current->next->data != value) {
        current = current->next;
    }
    
    if (current->next == NULL) {
        printf("Element %d not found in list\n", value);
        return false;
    }
    
    Node* nodeToDelete = current->next;
    current->next = nodeToDelete->next;
    free(nodeToDelete);
    list->size--;
    printf("Element %d deleted from list\n", value);
    return true;
}

// Traverse and display the list
void traverseList(LinkedList* list) {
    if (list == NULL || list->head == NULL) {
        printf("List is empty\n");
        return;
    }
    
    printf("List contents: ");
    Node* current = list->head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf(" -> NULL\n");
    printf("Size: %d\n", list->size);
}

// Traverse and display the list in reverse (using recursion)
void traverseReverse(LinkedList* list, Node* node) {
    if (list == NULL) {
        printf("List is NULL\n");
        return;
    }
    
    if (node == NULL) {
        if (list->head == NULL) {
            printf("List is empty\n");
        }
        return;
    }
    
    traverseReverse(list, node->next);
    printf("%d ", node->data);
}

// Search for an element and return its position (-1 if not found)
int searchElement(LinkedList* list, int value) {
    if (list == NULL || list->head == NULL) {
        return -1;
    }
    
    Node* current = list->head;
    int position = 0;
    
    while (current != NULL) {
        if (current->data == value) {
            printf("Element %d found at position %d\n", value, position);
            return position;
        }
        current = current->next;
        position++;
    }
    
    printf("Element %d not found in list\n", value);
    return -1;
}

// Get the size of the list
int getSize(LinkedList* list) {
    if (list == NULL) {
        return 0;
    }
    return list->size;
}

// Check if the list is empty
bool isEmpty(LinkedList* list) {
    return (list == NULL || list->head == NULL);
}

// Clear all elements from the list
void clearList(LinkedList* list) {
    if (list == NULL) {
        return;
    }
    
    Node* current = list->head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    
    list->head = NULL;
    list->size = 0;
    printf("List cleared successfully\n");
}

// Destroy the entire list structure
void destroyList(LinkedList* list) {
    if (list == NULL) {
        return;
    }
    
    clearList(list);
    free(list);
    printf("List destroyed successfully\n");
}

// Display menu options
void displayMenu() {
    printf("\n=== SINGLY LINKED LIST OPERATIONS ===\n");
    printf("1.  Insert at beginning\n");
    printf("2.  Insert at end\n");
    printf("3.  Insert at position\n");
    printf("4.  Delete from beginning\n");
    printf("5.  Delete from end\n");
    printf("6.  Delete at position\n");
    printf("7.  Delete by value\n");
    printf("8.  Display list\n");
    printf("9.  Display list in reverse\n");
    printf("10. Search element\n");
    printf("11. Get list size\n");
    printf("12. Check if empty\n");
    printf("13. Clear list\n");
    printf("0.  Exit\n");
    printf("=====================================\n");
    printf("Enter your choice: ");
}

// Main function with interactive menu
int main() {
    LinkedList* list = createList();
    if (list == NULL) {
        printf("Failed to create linked list\n");
        return 1;
    }
    
    int choice, data, position;
    
    printf("Singly Linked List Implementation in C\n");
    printf("======================================\n");
    
    while (1) {
        displayMenu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("Enter data to insert at beginning: ");
                scanf("%d", &data);
                insertAtBeginning(list, data);
                break;
                
            case 2:
                printf("Enter data to insert at end: ");
                scanf("%d", &data);
                insertAtEnd(list, data);
                break;
                
            case 3:
                printf("Enter data: ");
                scanf("%d", &data);
                printf("Enter position: ");
                scanf("%d", &position);
                insertAtPosition(list, data, position);
                break;
                
            case 4:
                deleteAtBeginning(list);
                break;
                
            case 5:
                deleteAtEnd(list);
                break;
                
            case 6:
                printf("Enter position to delete: ");
                scanf("%d", &position);
                deleteAtPosition(list, position);
                break;
                
            case 7:
                printf("Enter value to delete: ");
                scanf("%d", &data);
                deleteByValue(list, data);
                break;
                
            case 8:
                traverseList(list);
                break;
                
            case 9:
                printf("List in reverse: ");
                traverseReverse(list, list->head);
                printf("\n");
                break;
                
            case 10:
                printf("Enter element to search: ");
                scanf("%d", &data);
                searchElement(list, data);
                break;
                
            case 11:
                printf("List size: %d\n", getSize(list));
                break;
                
            case 12:
                printf("List is %s\n", isEmpty(list) ? "empty" : "not empty");
                break;
                
            case 13:
                clearList(list);
                break;
                
            case 0:
                printf("Exiting program...\n");
                destroyList(list);
                return 0;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}