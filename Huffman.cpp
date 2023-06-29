#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

#define MAX_TREE_HT 1000

typedef struct node {
    char st;
    string code;
    int freq;
    float pro;
    int top;
} node;

typedef struct MinHNode {
    int freq;
    char st;
    struct MinHNode *left, *right;
}MinHNode;

typedef struct MinH {
    unsigned size;
    unsigned capacity;
    MinHNode **array;
}MinH;

int dec(int n){
    int dec_value = 0;
    int base = 1;
    int temp = n;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;
        dec_value += last_digit * base;
        base = base * 2;
    }
    return dec_value;
}

string bin(int n){
    string s = "",m = "";
    while (n > 0) {
        s += char(n % 2 + 48);
        n = n / 2;
    }
    int d = s.size();
    int l = s.size();
    while(d < 8){m += "0";d++;}
    for(int i = 0;i < s.size();i++){
        m += s[l - 1 - i];
    }
    return m;
}

int loc(node arr[],string d,int l){
    for(int i = 0;i < l;i++){
        if(arr[i].code == d){return i;}
    }
    return -1;
}

int iloc(node arr[],char ch,int l){
    for(int i = 0;i < l;i++){
        if(arr[i].st == ch){return i;}
    }
    return -1;
}

int freq_value(node *p,FILE *fp){
    int ch,l = 0;
    float t = 0;
    while((ch = fgetc(fp)) != EOF){
        p[ch].st = (char)ch;
        p[ch].freq++;
        p[ch].pro++;
        t++;
    }
    for(int i = 0;i < 256;i++){
        if(p[i].freq != 0) { l++; }
        p[i].pro = p[i].pro / t;
    }
    return l;
}

void printarr(node arr[],int l){
    for(int i = 0;i < l;i++) {
        if (arr[i].freq == 0) {}
        else {
            cout << arr[i].st << " : " << arr[i].freq << endl;
        }
    }
    cout << endl;
}

void printarr1(node arr[],int l){
    for(int i = 0;i < l;i++) {
        cout << arr[i].st << " : ";
        cout << arr[i].code << endl;
    }
    cout << endl;
}

void swap(node &n1,node &n2){
    node temp;
    temp.freq = n1.freq;
    temp.pro = n1.pro;
    temp.st = n1.st;
    n1.freq = n2.freq;
    n1.pro = n2.pro;
    n1.st = n2.st;
    n2.freq = temp.freq;
    n2.pro = temp.pro;
    n2.st = temp.st;
}

void sortByFreq(node p[],int n){
    int i, j;
    node temp;
    for (j = 1; j < n; j++) {
        for (i = 0; i < n - 1; i++) {
            if ((p[i].freq) < (p[i + 1].freq)) {
                swap(p[i],p[i+1]);
            }
        }
    }
}

MinHNode *newNode(char c, int freq) {
    MinHNode *temp = (MinHNode*)malloc(sizeof(MinHNode));
    temp->left = temp->right = nullptr;
    temp->st = c;
    temp->freq = freq;
    return temp;
}

MinH *createMinH(int capacity) {
    MinH *minHeap = (MinH*)malloc(sizeof(MinH));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHNode**)malloc(capacity*sizeof(MinHNode*));
    return minHeap;
}

void swapMinHNode(MinHNode **a,MinHNode **b) {
    MinHNode *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinH *minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;
    if (smallest != idx) {
        swapMinHNode(&minHeap->array[smallest],
                     &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

MinHNode *extractMin(MinH *minHeap) {
    MinHNode *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinH *minHeap,MinHNode *minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(MinH *minHeap) {
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

int isLeaf(MinHNode *root) {
    return !(root->left) && !(root->right);
}

MinH *createAndBuildMinHeap(node p[], int size) {
    MinH *minHeap = createMinH(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(p[i].st, p[i].freq);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

MinHNode *buildHfTree(node p[], int size) {
    MinHNode *left, *right, *top;
    MinH *minHeap = createAndBuildMinHeap(p, size);
    while (minHeap->size != 1) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}
void printHCodes(MinHNode *root, int arr[], int top,node *p) {
    if (root->left) {
        arr[top] = 0;
        printHCodes(root->left, arr, top + 1,p);
    }
    if (root->right) {
        arr[top] = 1;
        printHCodes(root->right, arr, top + 1,p);
    }
    if (isLeaf(root)) {
        string s = "";
        for(int i = 0;i < top;i++){
            s += (char)(arr[i]+48);
        }
        int d = iloc(p,root->st,128);
        p[d].code = s;
        p[d].top = s.size() - 1;
    }
}

void huffman(node p[], int size) {
    MinHNode *root = buildHfTree(p, size);
    int arr[MAX_TREE_HT], top = 0;
    printHCodes(root, arr, top,p);
}

int max_top(node arr[],int t){
    int max = 0;
    for(int i = 0;i < t;i++){
        if(max < arr[i].code.size()){max = arr[i].code.size();}
    }
    return max - 1;
}

int encode(FILE *fp,FILE *fc,node p[],int t){
    fseek(fp,0,SEEK_SET);
    string s = "",st;
    char ch; int d,m = 0,x,de,ec = 0;
    while((ch = fgetc(fp)) != EOF){
        d = iloc(p,ch,t);
        s += p[d].code; m += p[d].top + 1;
        ec += 1;
    }
    int n = m % 8;
    while(n < 8){
        s += "0"; n++;
    }
    for(int i = 0;i < s.size();i = i + 8){
        st = "";
        for(int j = 0;j < 8;j++){
            st += s[i+j];
        }
        x = stoi(st);
        de = dec(x);
        fprintf(fc,"%c",(char)de);
        //cout << de << " ";
    }
    //cout << endl << endl;
    return ec;
}

void decode(int ec,FILE *fp,FILE *fc,node p[],int t,int l){
    fseek(fp,0,SEEK_SET);
    string s = "",st = "",y;
    int dx = 0;
    int ch,v; int d;
    while((ch = fgetc(fp)) != EOF){
        s = bin(ch);
        //cout << ch << " ";
        st += s;
    }
    //cout << endl << st << endl;
    for(int i = 0;i < st.size();i = i+v){
        v = 0;
        while(v <= l+1){
            y = "";
            for(int j = 0;j < v;j++){
                y += st[i+j];
            }
            d = loc(p,y,t);
            if(d != -1){
                fprintf(fc,"%c",p[d].st);
                //cout << p[d].st;
                dx++;
                break;
            }
            else{v++;}
        }
        if(dx >= ec){break;}
    }
}

void optimality(node p[],int l){
    float x = 0,y = 0,z = 0,a,b;
    for(int i = 0;i < l;i++){
        z += p[i].freq;
        x -= p[i].pro * log2(p[i].freq);
        y += p[i].code.size() * p[i].pro;
    }
    a = x + log2(z);
    b = (a/y)*100;
    cout << "Entropy : " << a << endl;
    cout << "Expected length : " << y << endl;
    cout << "Optimality : " << b << " % " << endl;
}

int main(){
    FILE *fp = fopen("a.txt","rb");
    FILE *fd = fopen("d.txt","rb");
    FILE *fc = fopen("b.txt","wb");
    FILE *fz = fopen("c.txt","wb");
    node *arr = (node*)malloc(sizeof(node)*256);
    for(int i = 0;i < 256;i++){
        arr[i].freq = 0;
        arr[i].top = -1;
    }
    int t,l,ec;
    t = freq_value(arr,fd);
    cout << "Count : " << t << endl;
    sortByFreq(arr, 256);
    printarr(arr,t);
    huffman(arr,t);
    l = max_top(arr,t);
    cout << "Max top : " << l << endl;
    printarr1(arr,t);
    ec = encode(fp,fc,arr,t);
    fclose(fc);
    FILE *fe = fopen("b.txt","rb");
    decode(ec,fe,fz,arr,t,l);
    optimality(arr,t);
    free(arr);
    return 0;
}
