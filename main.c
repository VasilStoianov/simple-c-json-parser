#include <stdio.h>
#include <string.h>

typedef struct {
  char value[50];
  char key[50];
  int valid;
} JsonObject;

typedef struct {
  JsonObject jsonObjects[16];
  unsigned int size;
} JsonArray;

unsigned int calculate_hash(char *key);
unsigned int put_in_JsonObject(JsonArray *array, char key[], char *value);
JsonObject *getObject(JsonArray *array, char *key);

JsonArray parse_json(char *json, JsonArray *array);

int main(void) {
  char *json = "{\"test\":5,\"string\":\"str\"}";
  JsonArray arr;
  parse_json(json, &arr);
  printf("Key in the array is %s \n", getObject(&arr, "test")->key);
  return 1;
}

unsigned int calculate_hash(char *key) {
  unsigned int hash = 0;
  int c;

  while (c = *key++)
    hash += c;

  return hash;
}

unsigned int put_in_JsonObject(JsonArray *array, char key[], char *value) {
  printf("This is %s \n", key);
  unsigned int index = calculate_hash(key) & array->size;
  if (array->jsonObjects[index].valid == 1) {
    printf("ERROR: The cell its not empty!\n");
  } else {
    strcpy(array->jsonObjects[index].key, key);
    array->jsonObjects[index].valid = 1;
    printf("key in the objet before putting it is %s \n",
           array->jsonObjects[index].key);
    array->size++;
    return 1;
  }
}

JsonObject *getObject(JsonArray *array, char *key) {
  int index = calculate_hash(key) & array->size;
  JsonObject *result = &array->jsonObjects[index];

  if (result->valid == 0) {
    printf("ERROR: No such object, suck ass!\n");
    return result;
  } else {
    return result;
  }
}

JsonArray parse_json(char *json, JsonArray *array) {
  char *value;
  int starting = 0;
  int ending = 0;
  int startOrEnd = 1;
  for (int x = 0; x < sizeof(json); x++) {
    // printf("%c  start: %d , end:%d \n", json[x], starting, ending);
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
      int i = 0;
      char key[ending];
      char value[ending];
      memcpy(key, &json[starting], ending);
      printf("the key is %s\n", key);

      put_in_JsonObject(array, key, value);
    }
  }
}
