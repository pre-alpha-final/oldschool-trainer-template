#include "exy_find.h"

string exyfind(string text, int start, char a, char b, char c, char d,
               int offset, char pass1, char pass2, char stop, int* index)
{
 const char *ctext = text.c_str();
 char gotyou[500];
 memset(gotyou, 0, sizeof(gotyou));
 bool found = 0;
 for (unsigned int j=start, k=0; j<strlen(ctext); j++)
 {
  if (!found)
   if (a == 0 || ctext[j]==a)
    if (b == 0 || ctext[j+1]==b)
     if (c == 0 || ctext[j+2]==c)
      if (d == 0 || ctext[j+3]==d)
      {
       found = 1;
       j+=offset;
       while (pass1 != 0 && ctext[j] == pass1)
        j++;
       while (pass2 != 0 && ctext[j] == pass2)
        j++;
       while (pass1 != 0 && ctext[j] == pass1)
        j++;
      }
  //copy till stop
  if (found)
  {
   if (ctext[j] == stop)
   {
    if (index != NULL)
     *index = j;
    break;
   }
   gotyou[k]=ctext[j];
   k++;
  }
 }
 if (!found && index != NULL)
  *index = -1;
 string response;
 response = gotyou;
 return response;
}

