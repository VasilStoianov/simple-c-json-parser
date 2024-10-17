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
unsigned int put_in_JsonObject(JsonArray *array, char *key, char *value);
JsonObject *getObject(JsonArray *array, char *key);

JsonArray parse_json(char *json, JsonArray *array);

int main(void) {
  char *json = "{\"test\":5,\"string\":\"str\"}";
  JsonArray arr;
  arr.size = 16;
  parse_json(json, &arr);
  JsonObject *obj = getObject(&arr, "testa");
  if (obj != NULL) {

    printf("Key in the array is %s \n", obj->key);
  }
  return 1;
}

unsigned int calculate_hash(char *key) {
  unsigned int hash = 0;
  int c;

  while (c = *key++)
    hash += c;

  return hash;
}

unsigned int put_in_JsonObject(JsonArray *array, char *key, char *value) {
  unsigned int index = calculate_hash(key) % array->size;
  if (array->jsonObjects[index].valid == 1) {
    printf("ERROR: The cell its not empty!\n");
  } else {
    strcpy(array->jsonObjects[index].key, key);
    array->jsonObjects[index].valid = 1;
    return 1;
  }
}

JsonObject *getObject(JsonArray *array, char *key) {
  unsigned int index = calculate_hash(key) % array->size;
  JsonObject *result = &array->jsonObjects[index];
  if (result->valid == 1) {
    return result;
  } else {
    printf("ERROR: No such object, suck ass!\n");
    return NULL;
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
        starting = x + 1;
        startOrEnd = 0;
      } else {
        ending = x - 1;
        startOrEnd = 1;
      }
    }
    if (json[x] == ':') {
      int i = 0;
      char key[ending - 1];
      char value[ending];
      memcpy(key, &json[starting], ending - 1);
      printf("the key is %s\n", key);

      put_in_JsonObject(array, key, value);
    }
  }
}
