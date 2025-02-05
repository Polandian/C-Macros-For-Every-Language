#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#define true 1
#define false 0
#define error -1
#define bool _Bool
#define _f(x) free(x)
#define _ff(x) x[0]='\0'
#define arr_size 64
#define typeof(x) _Generic((x), \
    int:     "int",   \
    float:   "float", \
    short:   "short", \
    long:    "long",  \
    char:    "char",  \
    char*:   "char*", \
    char**:  "char**",\
    default: "other")
#define _m(x) malloc(x)
#define ARGC_ERR "not enough arguments"
#define ARGV_ERR "not proper arguments"

#define ac(s, c){\
    s[strlen(s)] = c;\
    s[strlen(s)+1] = '\0';\
}
#define wq(){\
    d++; \
    c=l[d];\
}

char* replace(const char* s, const char* oldW, const char* newW, bool n){ 
    char* result; 
    int i, cnt = 0; 
    int newWlen = strlen(newW); 
    int oldWlen = strlen(oldW); 
 
    for (i = 0; s[i] != '\0'; i++) { 
        if (strstr(&s[i], oldW) == &s[i]) { 
            cnt++; 
            i += oldWlen - 1; 
        } 
    } 
 
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1); 
 
    i = 0; 
    while (*s) { 
        if(strstr(s, oldW) == s) { 
            strcpy(&result[i], newW); 
            i += newWlen; 
            s += oldWlen; 
            n=true;
        } 
        else
            result[i++] = *s++; 
    } 
 
    result[i] = '\0'; 
    return result; 
} 


void* _e(const char *n, const int d){
    printf("jsmacros.exe: %s\nerror: %d exit status",n,d);
    abort();
}

void preproccessor(const char* file){
    char m_name[72][72],m_value[72][72];
    char inc[72],c_macro[16],filet[128],l[256],c;
    strcat(filet, file);
    strcat(filet, "temp");
    FILE *fr = fopen(file, "r");
    FILE *fw = fopen(filet, "a");
    int d=0,ma=0;
    bool ifdef=false;
    while(fgets(l, 256, fr)){
        c=l[d];
        //skip tabs
        while(c==' '){
            wq();
        }
        if(c=='#'&&ifdef==false){
            //start reading macro name
            int i=0;
            while(c!=' '){
                wq();
                c_macro[i++]=c;
            };
            wq();
            c_macro[--i]='\0';
            //start reading first macro value
            if(strcmp(c_macro, "include")==0){
                int t=0;
                while(l[d+1]!='>'){
                    wq();
                    inc[t++]=c;
                }
                inc[t]='\0';
                
                //add said file to the new javascript file
                FILE *inf = fopen(inc, "r");
                char ln[256];
                while(fgets(ln, 256, inf)){
                    fprintf(fw, ln);
                }
                fprintf(fw, "\n");
                fclose(inf);
                _f(inf);
            }
            else if(strcmp(c_macro,"define")==0){
                int f=0,g=0,i=0;
                bool b=false;
                while(c!=' '){
                    inc[f++]=c;
                    wq();
                }
                inc[f]='\0';
                wq();
                for(;i<ma;i++){
                    if(strcmp(inc,m_name[i])==0){
                        b=true;
                        break;
                    }
                }

                if(b==true){
                    m_value[i][0]='\0';
                    while(c!=' '&&d!=strlen(l)+1){
                        m_value[i][g++]=c;
                        wq();
                    }
                    m_value[i][g]='\0';
                    if((int)m_value[i][g-2]==10)
                        m_value[i][g-2]='\0';
                }
                else{
                    while(c!=' '&&d!=strlen(l)+1){
                        m_value[ma][g++]=c;
                        wq();
                    }
                    m_value[ma][g]='\0';
                    if((int)m_value[ma][g-2]==10)
                        m_value[ma][g-2]='\0';
                    strcpy(m_name[ma], inc);
                    ++ma;
                }
            }
            else if(strcmp(c_macro,"definef")==0){
                //TODO function macros
                //just like c ones, read one line (that is seperated to different lines via \) and then replace it like #define macros
            }
            else if(strcmp(c_macro,"ifdef")==0){
                bool q=false;
                int t=0;
                while(l[t+1]!=' '&&d<strlen(l)){
                    inc[t++]=c;
                    wq();
                }
                inc[t]='\0';
                if((int)inc[t-2]==10)
                    inc[t-2]='\0';
                for(int z=0;z<ma;z++){
                    if(strcmp(m_name[ma], inc)==0){
                        q=true;
                        break;
                    }
                }
                if(q==true) ifdef=true;
            }
            else if(strcmp(c_macro,"ifndef")==0){
                bool q=false;
                int t=0;
                while(l[t+1]!=' '&&t<strlen(l)){
                    inc[t++]=c;
                    wq();
                }
                inc[t]='\0';
                if((int)inc[t-2]==10)
                    inc[t-2]='\0';
                for(int z=0;z<ma;z++){
                    if(strcmp(m_name[ma], inc)==0){
                        q=true;
                        break;
                    }
                }
                if(q==false) ifdef=true;
            }
        }
        else if(ifdef==true){
            c=l[d];
            char z[6];
            int d=0,f=0;
            //skip tabs
            while(c==' '){
                wq();
                f++;
            }
            for(;d<6;d++){
                z[d]=l[f+d];
            }
            if(strcmp(z,"#endif")) ifdef=false;
        }
        else{
            bool t=false;
            for(int j=0;j<ma;++j){
                char ll[256];
                memcpy(ll,l,256);
                l[0]='\0';
                strcat(l, replace(ll, m_name[j], m_value[j], t));
                if(t==true){
                    printf("found one");
                    break;
                }
            }
            fprintf(fw, l);
        }        


        _ff(c_macro);
        _ff(inc);
        d=0;
    }
    fclose(fr);
    fclose(fw);
    _f(fr);
    _f(fw);
    bool rm=remove(file);
    if(rm!=0)perror("Error");
    bool rn=rename(filet, file);
    if(rn!=0)perror("Error");
}

int main(int argc, char** argv){
    if(argc>=2) {preproccessor(argv[1]);preproccessor(argv[1]);}
    else _e(ARGC_ERR, error);
    return 0;
}
