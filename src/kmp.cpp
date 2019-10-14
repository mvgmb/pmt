#include <bits/stdc++.h>
using namespace std;

void borders(char* pat, int* b){
    const int  pat_len = strlen(pat);
    b[0]=-1;
    int i=1, j=0;
    while (i < pat_len){
        while(j<pat_len & i+j<pat_len & pat[i+j]== pat[j]){
            j++;
            b[i+j] = j;
            //cout << b[i+j]<< endl;
        }
        i += max(1,j-b[j]);
        j = max(0,b[j]);
    }
}

void kmp(char* txt, char* pat){
    const int txt_len = strlen(txt), pat_len = strlen(pat);
    int b[pat_len+1] = {0};
    borders(pat, b);
    int j = 0;
    for(int i = 0; i <= txt_len-pat_len; i+=max(1,j-b[j])){
        for(j = 0;j < pat_len & txt[i+j] == pat[j] ;j++){
            if(j== pat_len-1){
                cout << "O pattern ocorreu no index " << i << endl;
            }
        }
        j=max(0,b[j]);
    }
}

int main(){
    char txt[] = "abracadabra";
    char pat[] = "abra";
    kmp(txt, pat);
}