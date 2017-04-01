#include <iostream>
#include <cstdio>
#define NUM 99
#define INFINITY 65535
int ARRAY[NUM];
int front;
using namespace std;
struct Node{
    int pageNumber;
    bool isFree;
    int count; //only for lru
};
bool optInsert(Node *optArray,int n,int position){
    int insertPage = ARRAY[position];
    int i;
    for(i=0;i<n;i++){
        if(!optArray[i].isFree && optArray[i].pageNumber==insertPage)
            return false;
        else if(optArray[i].isFree)
            break;
    }
    if(i<n){
        optArray[i].isFree = false;
        optArray[i].pageNumber = insertPage;
    }else{
        int max = -INFINITY; int index = -1;
        for(int j=0;j<n;j++){
            int page = optArray[i].pageNumber;
            int k;
            for(k=position + 1;k<NUM;k++){
                if(ARRAY[k]==page)
                    break;
            }
            if(k-position>max){
                max = k-position;
                index = j;
            }
        }
        optArray[index].pageNumber = insertPage;
    }
    return true;
}
bool lruInsert(Node *lruArray,int n,int position){
    int insertPage = ARRAY[position];
    int i;
    for(i=0;i<n;i++){
        if(!lruArray[i].isFree && lruArray[i].pageNumber==insertPage){
            for(int j=0;j<n;j++){
                if(!lruArray[i].isFree)
                    lruArray[i].count++;
                else
                    break;
            }
            lruArray[i].count = 0;
            return false;
        }else if(lruArray[i].isFree)
            break;
    }
    if(i<n){
        lruArray[i].isFree = false;
        lruArray[i].pageNumber = insertPage;
        lruArray[i].count = 0;
        for(int j=0;j<i;j++){
            lruArray[j].count++;
        }
    }else{
        int max = -INFINITY; int index = -1;
        for(int j=0;j<n;j++){
            if(lruArray[j].count>max){
                index = j;
                max = lruArray[j].count;
            }
            lruArray[j].count++;
        }
        lruArray[index].pageNumber = insertPage;
        lruArray[index].count = 0;
    }
    return true;
}
bool fifoInsert(Node *fifoArray,int n,int position){
    int insertPage = ARRAY[position];
    int i;
    for(i=0;i<n;i++){
        if(!fifoArray[i].isFree && fifoArray[i].pageNumber==insertPage)
            return false;
        else if(fifoArray[i].isFree){
            break;
        }
    }
    if(i<n){
        fifoArray[i].isFree = false;
        fifoArray[i].pageNumber = insertPage;
    }else{
        fifoArray[front].pageNumber = insertPage;
        front = (front+1)%n;
    }
    return true;
}
int main(void)
{
    FILE *in = fopen("data.txt","r");
    FILE *out = fopen("out.txt","w");
    for(int i=0;i<NUM;i++){
        fscanf(in,"%d",&ARRAY[i]);
    }
    for(int n=1;n<=10;n++){
        fprintf(out,"***when n is %d,the lack page num is as follows***\n",n);
        Node *optArray = new Node[n]; int optCount = 0;
        Node *lruArray = new Node[n]; int lruCount = 0;
        Node *fifoArray = new Node[n];int fifoCount = 0; front = 0;
        for(int i=0;i<n;i++){
            optArray[i].isFree = lruArray[i].isFree = fifoArray[i].isFree = true;
            lruArray[i].count = 0;
        }

        for(int i=0;i<NUM;i++){//首先执行OPT,然后执行LRU,再然后执行FIFO
            if(optInsert(optArray,n,i)) optCount++;
            if(lruInsert(lruArray,n,i)) lruCount++;
            if(fifoInsert(fifoArray,n,i)) fifoCount++;
        }
        fprintf(out,"\tOPT lack page:%d\n",optCount);
        fprintf(out,"\tLRU lack page:%d\n",lruCount);
        fprintf(out,"\tFIFO lack page:%d\n",fifoCount);
        delete[] optArray;
        delete[] lruArray;
        delete[] fifoArray;
    }
    fclose(in);
    fclose(out);
    return 0;
}