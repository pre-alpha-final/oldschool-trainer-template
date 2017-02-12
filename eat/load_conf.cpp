#include "load_conf.h"

int load_conf(char *t)
{
 //remove "s
 char text[255];
 memset(text, 0, 255);
 for (int i=0, j=0; i<255; j++, i++)
 {
  if (t[i] == '\"')
  {
   j--;
   continue;
  }
  text[j] = t[i];
  if (t[i] == 0x00)
   break;
 }
 
 FILE *f;
 int size;
 char tmp[100];
 f = fopen(text, "rt+");
 if (f == NULL)
  return 1;
 fseek(f, 0, 2);
 size = ftell(f);
 rewind(f);

 //read data line by line
 memset(tmp, 0, 100);
 while (fgets(tmp, 100, f))
 {
  if (tmp[strlen(tmp)-1] == '\n') //remove new line
   tmp[strlen(tmp)-1] = 0x00;
  if (strlen(tmp) < 1) //remove empty lines
   continue;
  if (tmp[0] == ';') //remove comments
   continue;
  analyze_conf_line(tmp);
  memset(tmp, 0, 100);
 }
 fclose(f);
}

int analyze_conf_line(char *text)
{
/*change according to program
 char tmp[100];

 //checking a single line of config file
 strcpy(tmp, "port ");
 if (!strncmp(text, tmp, strlen(tmp)))
  strcpy(port, (char*)&text[strlen(tmp)]);
 strcpy(tmp, "speed ");
 if (!strncmp(text, tmp, strlen(tmp)))
  speed = atoi(&text[strlen(tmp)]);
 return 0;
*/
}
