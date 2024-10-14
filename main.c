#include <stdio.h>
#include <string.h>

// iterate if " start of string " end of string
struct map {};

struct map parse_json(char *json);

int main(void) {
  char *json = "{\"test\":5}";
  return 1;
}

struct map parse_json(char *json) {
  char *value;
  int starting = 0;
  int ending = 0;
  int startOrEnd = 1;
  for (int x = 0; x < sizeof(json); x++) {
    printf("%c  start: %d , end:%d \n", json[x], starting, ending);
    if (json[x] == '\"') {
      if (startOrEnd == 1) {
        starting = x;
        startOrEnd = 0;
      } else {
        ending = x;
        startOrEnd = 1;
      }
    }
    if (json[x] == ':') {
      printf("we are getting the key\n");
      int i = 0;
      char *key[ending];
      memcpy(key, &json[starting], ending);
      key[ending - 1] = "/0";
      printf("key is %s \n", &key);
    }
  }
}
