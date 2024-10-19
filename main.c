#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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
  char *json = "{\"test\":\"str\",\"string\":\"str\"}";
  JsonArray arr;
  arr.size = 16;
  parse_json(json, &arr);
  JsonObject *obj = getObject(&arr, "test");
  JsonObject *secObj = getObject(&arr, "string");
  if (obj != NULL) {

    printf("Object with key %s in the array and value %s  \n", obj->key,
           obj->value);
  }
  if (secObj != NULL) {

    printf("Second Object with key %s in the array and value %s  \n",
           secObj->key, secObj->value);
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
    strcpy(array->jsonObjects[index].value, value);
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
    printf("ERROR: No such object with key %s , suck ass!\n", key);
    return NULL;
  }
}

JsonArray parse_json(char *json, JsonArray *array) {
  char *value = NULL;
  int starting = 0;
  int ending = 0;
  bool start = true;
  char *key = NULL;
  bool valueStarting = false;
  for (int x = 0; x < strlen(json); x++) {
    if (json[x] == '\"') {
      // parsing string
      if (start) {
        starting = x + 1;
        start = false;
      } else {
        ending = x - 1;
        start = true;
      }
    }
    if (json[x] == ':') {
      int key_len = ending - starting;
      key = (char *)malloc(key_len + 1);
      strncpy(key, &json[starting], key_len + 1);
      starting = 0;
      ending = 0;
    }
    if (json[x] == ',' || json[x] == '}') {
      int valueLength = ending - starting;
      value = (char *)malloc(valueLength + 1);
      strncpy(value, &json[starting], valueLength + 1);
      put_in_JsonObject(array, key, value);
      starting = 0;
      ending = 0;
      free(key);
      free(value);
    }
  }
}
