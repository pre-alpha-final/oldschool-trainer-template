#include "exy_strutils.h"
#include <windows.h>
//debug functions
extern void ShowMessage(const char *mes);
extern void ShowMessage(int mes);
//end debug functions

void sfl(string *str, list<string> *lst)
{
 list<string>::iterator it;
 str->clear();
 for (it = lst->begin(); it != lst->end(); it++)
 {
  *str += *it;
  *str += "\r\n";
 }
}

void lfs(list<string> *lst, string *str)
{
 lst->clear();
 char *dup = strdup(str->c_str());
 char tmp[500000];
 memset(tmp, 0, 500000);
 for (int i=0, j=0; i<strlen(dup); i++)
 {
  if (dup[i] == '\r')
  {
   lst->push_back(tmp);
   memset(tmp, 0, 500000);
   j = 0;
   i++;
   continue;
  }
  if (dup[i] == '\n')
  {
   lst->push_back(tmp);
   memset(tmp, 0, 500000);
   j = 0;
   continue;
  }
  tmp[j] = dup[i];
  j++;
 }
 if (strlen(tmp)>0) //push last line
  lst->push_back(tmp);
 free(dup);
}

string exyload(char *name, list<string> *lf)
{
 FILE *f;
 string sf;
 int size;
 char *cf;
 
 f = fopen(name, "rb");
 fseek(f, 0, 2);
 size = ftell(f);
 cf = new char[size+1];
 memset(cf, 0, size+1);
 rewind(f);     
 fread(cf, size, 1, f);
 fclose(f);
 sf = cf;
 delete[] cf;
 lfs(lf, &sf);
 return sf;
}

void exysave(char* name, string text)
{
 FILE *f;
 f = fopen(name, "wb");
 fwrite(text.c_str(), text.length(), 1, f);
 fclose(f);
 return;
}

void exyerase(string *text, char c)
{
 if (text->find(c)!=string::npos)
  text->erase(text->find_first_of(c), 1);
 return;
}

int strinstr(string t1, string t2)
{
 return t1.find(t2);
}
