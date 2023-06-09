#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>

using namespace std;

typedef struct node {
    char ch;
    float pro;
    int arr[20];
    string code;
    int top;
} node;

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

int prob_value(node *p,FILE *fp){
    float t = 0,x = 0;
    char ch;
    while((ch = fgetc(fp)) != EOF){
        p[ch].pro++; t++;
        p[ch].ch = (char)ch;
    }
    int l = 0;
    for(int i = 0;i < 256;i++){
        if(p[i].pro != 0) {
            l++;
            p[i].pro = p[i].pro / t;
            x += p[i].pro;
        }
    }
    cout << endl << "Prob 123 : " << x << endl;
    return l;
}

void printarr(node arr[],int l){
    for(int i = 0;i < l;i++) {
        if (arr[i].pro == 0) {}
        else {
            cout << arr[i].ch << " : " << arr[i].pro << endl;
        }
    }
    cout << endl;
}

void printarr1(node arr[],int l){
    string s; int m;
    for(int i = 0;i < l;i++) {
        cout << arr[i].ch << " : ";
        s = "";
        for(int j = 0;j <= arr[i].top;j++){
            s += (char)(arr[i].arr[j] + 48);
        }
        arr[i].code = s;
        cout << arr[i].code << endl;
    }
    cout << endl;
}

void swap(node &n1,node &n2){
    node temp;
    temp.pro = n1.pro;
    temp.ch = n1.ch;
    n1.pro = n2.pro;
    n1.ch = n2.ch;
    n2.pro = temp.pro;
    n2.ch = temp.ch;
}

int max_top(node arr[],int t){
    int max = 0;
    for(int i = 0;i < t;i++){
        if(max < arr[i].top){max = arr[i].top;}
    }
    return max;
}

void sortByProbability(node p[],int n){
    int i, j;
    node temp;
    for (j = 1; j < n; j++) {
        for (i = 0; i < n - 1; i++) {
            if ((p[i].pro) < (p[i + 1].pro)) {
                swap(p[i],p[i+1]);
            }
        }
    }
}

void shanon(node p[],int l,int h){
    float prob1 = 0,prob2 = 0,prob3 = 0;
    int k;
    if(l >= h){return;}
    else if(h == l+1){
        p[l].arr[++p[l].top] = 0;
        p[h].arr[++p[h].top] = 1;
    }
    else{
        for(int i = l;i <= h;i++){
            prob1 = prob1 + p[i].pro;
        }
        for(int i = l;i <= h;i++){
            prob3 = prob2;
            prob2 = prob2 + p[i].pro;
            if(prob2 >= prob1/2){
                if(prob2 - prob1/2 > prob1/2 - prob3) {
                    k = i-1; break;
                }
                else{
                    k = i; break;
                }
            }
        }
        for(int i = l;i <= k;i++){
            p[i].arr[++p[i].top] = 0;
        }
        for(int i = k+1;i <= h;i++){
            p[i].arr[++p[i].top] = 1;
        }
        shanon(p,l,k);
        shanon(p,k+1,h);
    }
}

int loc(node arr[],string d,int l){
    for(int i = 0;i < l;i++){
        if(arr[i].code == d){return i;}
    }
    return -1;
}

int iloc(node arr[],char ch,int l){
    for(int i = 0;i < l;i++){
        if(arr[i].ch == ch){return i;}
    }
    return -1;
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
                fprintf(fc,"%c",p[d].ch);
                //cout << p[d].ch;
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
        x -= p[i].pro * log2(p[i].pro);
        y += (p[i].top+1) * p[i].pro;
    }
    a = x;
    b = (a/y)*100;
    cout << "Entropy : " << a << endl;
    cout << "Expected length : " << y << endl;
    cout << "Optimality : " <<  b << " % " << endl;
}

int main(){
    FILE *fp = fopen("d.txt","r");
    FILE *fc = fopen("b.txt","wb");
    FILE *fz = fopen("c.txt","wb");
    node *arr = (node*)malloc(sizeof(node)*256);
    for(int i = 0;i < 256;i++){
        arr[i].pro = 0;
        arr[i].top = -1;
    }
    int t,l,ec;
    t = prob_value(arr,fp);
    cout << "Count : " << t << endl;
    sortByProbability(arr, 256);
    printarr(arr,t);
    shanon(arr,0,t-1);
    l = max_top(arr,t);
    cout << "Max top : " << l << endl;
    printarr1(arr,t);
    ec = encode(fp,fc,arr,t);
    fclose(fp);
    fclose(fc);
    FILE *fd = fopen("b.txt","rb");
    decode(ec,fd,fz,arr,t,l);
    optimality(arr,t);
    fclose(fp); fclose(fz); fclose(fd);
    free(arr);
    return 0;
}
