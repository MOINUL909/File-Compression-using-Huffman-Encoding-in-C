#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
UserId=Moinul
Password=Moinul909
*/

// Define login credentials
#define USER_ID "Moinul"
#define PASSWORD "Moinul909"

// Function to verify login credentials
int login() {
    char inputUserId[50];
    char inputPassword[50];

    printf("Enter User ID: ");
    scanf("%s", inputUserId);

    printf("Enter Password: ");
    scanf("%s", inputPassword);

    // Check if the entered credentials match the predefined credentials
    if (strcmp(inputUserId, USER_ID) == 0 && strcmp(inputPassword, PASSWORD) == 0) {
        printf("Login successful!\n\n");
        return 1;  // Return 1 for successful login
    } else {
        printf("Invalid User ID or Password.\n");
        return 0;  // Return 0 for failed login
    }
}

// Define a Huffman Tree node
struct MinHeapNode {
    char data;                // One of the characters in the input file
    unsigned freq;            // Frequency of the character
    struct MinHeapNode *left, *right;  // Left and right children
};

// Define a Min Heap: Collection of Huffman Tree nodes
struct MinHeap {
    unsigned size;            // Current size of the min heap
    unsigned capacity;        // Capacity of the min heap
    struct MinHeapNode **array;  // Array of minheap node pointers
};

// Create a new min heap node
struct MinHeapNode* createNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// Create a min heap with given capacity
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// Swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

// Standard min heapify function
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Check if the size of heap is one
int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// Extract minimum value node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// Insert a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// Build a min heap
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// Print an array of size n
void printArr(int arr[], int n) {
    for (int i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
}

// Utility function to check if this node is a leaf
int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = createNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

// Builds the Huffman Tree and returns its root
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;

    // Step 1: Create a min heap of capacity equal to size
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap)) {
        // Step 2: Extract the two minimum frequency items from heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Step 3: Create a new internal node with frequency equal to the sum of the two nodes' frequencies.
        // Make the two extracted node as left and right children of this new node. Add this node to the min heap.
        top = createNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    // Step 4: The remaining node is the root node and the tree is complete.
    return extractMin(minHeap);
}

// Prints Huffman codes from the root of Huffman Tree. It uses arr[] to store codes
void printCodes(struct MinHeapNode* root, int arr[], int top) {
    // Assign 0 to left edge and recur
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    // Assign 1 to right edge and recur
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    // If this is a leaf node, then it contains one of the input characters, print the character and its code from arr[]
    if (isLeaf(root)) {
        printf("%c: ", root->data);
        printArr(arr, top);
    }
}

// Builds a Huffman Tree and prints codes by traversing the built Huffman Tree
void HuffmanCodes(char data[], int freq[], int size) {
    // Construct Huffman Tree
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

    // Print Huffman codes using the Huffman tree built above
    int arr[100], top = 0;

    printCodes(root, arr, top);
}

// Main function to test the implementation
int main() {
    // Login mechanism
    if (login()) {
        printf("Access granted. You can proceed with Huffman Encoding.\n\n");

        // Huffman Encoding data
        char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
        int freq[] = { 5, 9, 12, 13, 16, 45 }; // Frequency of the above characters

        int size = sizeof(arr) / sizeof(arr[0]);

        // Call the function to perform Huffman Encoding
        HuffmanCodes(arr, freq, size);
    } else {
        printf("Access denied. Exiting program.\n");
    }

    return 0;
}
