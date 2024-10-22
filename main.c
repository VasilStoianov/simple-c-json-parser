#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  JSON_VALUE_STRING,
  JSON_VALUE_INT,
  JSON_VALUE_BOOLEAN,
  JSON_VALUE_OBJECT,
  JSON_VALUE_ARRAY
} JsonValueType;

typedef struct {
  char *key;
  JsonValueType valueType;
  union {
    int intValue;
    char *stringValue;
    bool boolValue;
  } value;
  int valid;
} JsonObject;

typedef struct {
  JsonObject jsonObjects[16];
  unsigned int size;
} JsonArray;

unsigned int calculate_hash(char *key);
unsigned int put_in_JsonObject(JsonArray *array, char *key, char *value,
                               JsonValueType type);
JsonObject *getObject(JsonArray *array, char *key);

void parse_json(char *json, JsonArray *array);

int main(void) {
  char *json = "{\"test\":\"str\",\"intvalue\":55555,\"string\":\"str\"}";
  JsonArray arr;
  arr.size = 16;
  parse_json(json, &arr);
  JsonObject *obj = getObject(&arr, "test");
  JsonObject *secObj = getObject(&arr, "string");
  JsonObject *thirdObj = getObject(&arr, "intvalue");
  if (obj != NULL) {

    printf("Object with key %s in the array and value %s  \n", obj->key,
           obj->value);
  }
  if (secObj != NULL) {

    printf("Second Object with key %s in the array and value %s  \n",
           secObj->key, secObj->value);
  }
  if (thirdObj != NULL) {
    printf("Third int object key %s value %d \n", thirdObj->key,
           thirdObj->value);
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

unsigned int put_in_JsonObject(JsonArray *array, char *key, char *value,
                               JsonValueType valueType) {
  unsigned int index = calculate_hash(key) % array->size;
  JsonObject obj;
  if (array->jsonObjects[index].valid == 1) {
    printf("ERROR: The cell its not empty! Rewriting the old with new one!\n");
  }
  switch (valueType) {
  case JSON_VALUE_STRING: {
    obj.valueType = JSON_VALUE_STRING;
    obj.key = strdup(key);
    obj.value.stringValue = strdup(value);
    obj.valid = 1;
    array->jsonObjects[index] = obj;
    return 1;
  }
  case JSON_VALUE_INT: {

    obj.valueType = JSON_VALUE_INT;
    obj.key = strdup(key);
    obj.valid = 1;
    obj.value.intValue = atoi(value);
    array->jsonObjects[index] = obj;
    return 1;
  }
  }
  return 0;
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

void parse_json(char *json, JsonArray *array) {
  char *value = NULL;
  int starting = 0;
  int ending = 0;
  bool start = true;
  char *key = NULL;
  bool parsingStarted = true;
  bool validJson = false;
  bool stopLoop = false;
  for (int x = 0; x < strlen(json) && !stopLoop; x++) {
    if (json[x] == '{' && parsingStarted) {
      printf("Starting to parse\n");
      validJson = true;
      parsingStarted = false;
    }
    if (validJson) {
      switch (json[x]) {
      case '\"': {
        // parsing string
        if (start) {
          starting = x + 1;
          start = false;
        } else {
          ending = x - 1;
          start = true;
        }
        break;
      };
      case ':': {
        int key_len = ending - starting;
        key = (char *)malloc(key_len + 1);

        strncpy(key, &json[starting], key_len + 1);
        ending = 0;
        if (isdigit(json[x + 1])) {
          starting = x + 1;
        } else {
          starting = 0;
        }
        break;
      };
      case ',':
        if (json[x + 1] == ',' || json[x - 1] == ',') {
          printf("ERROR: Invalid Json");
          stopLoop = true;
          break;
        }
      case '}': {
        JsonValueType valueType;
        if (ending == 0 && isdigit(json[x - 1]) && starting != 0) {

          valueType = JSON_VALUE_INT;
          ending = x - 1;
        }
        if (json[starting] == 't' || json[starting] == 'T' ||
            json[starting] == 'f' || json[starting] == 'F') {
          valueType = JSON_VALUE_BOOLEAN;
        }
        if (isalpha(json[starting])) {
          valueType = JSON_VALUE_STRING;
        }
        int valueLength = ending - starting;
        value = (char *)malloc(valueLength + 1);
        strncpy(value, &json[starting], valueLength + 1);
        put_in_JsonObject(array, key, value, valueType);
        starting = 0;
        ending = 0;
        free(key);
        free(value);
        break;
      };
      default: {
        break;
      }
      }
    } else {
      printf("ERROR: Invalid json");
      return;
    }
  }
}
