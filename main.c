#include <stdio.h>
#include <string.h>

typedef struct {
  char *value;
  char *key;
  int valid;
} JsonObject;

typedef struct {
  JsonObject jsonObjects[16];
  unsigned int size;
} JsonArray;

unsigned long calculate_hash(char *key);
unsigned int put_in_JsonObject(JsonArray *array, JsonObject jsonObject);
JsonObject *getObject(JsonArray *array, char *key);

JsonArray parse_json(char *json, JsonArray *array);

int main(void) {
  char *json = "{\"test\":5,\"string\":\"str\"}";
  JsonArray arr;
  parse_json(json, &arr);
  printf("Key in the array is %s \n", getObject(&arr, "test")->key);
  return 1;
}

unsigned long calculate_hash(char *key) {
  unsigned long hash = 5351;
  int c;
  while (c = *key++) {
    c = ((hash << 5) + hash) + c;
  }

  return hash;
}

unsigned int put_in_JsonObject(JsonArray *array, JsonObject jsonObject) {
  unsigned int index = calculate_hash(jsonObject.key) & array->size;
  if (array->jsonObjects[index].valid == 1) {
    printf("ERROR: The cell its not empty!\n");
  } else {
    array->jsonObjects[index] = jsonObject;
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
    JsonObject temp = {0};
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
      memcpy(key, &json[starting], ending);
      strncpy(temp.key, key, sizeof(temp.key) - 1);
      printf("the key is %s\n", key);
      temp.valid = 1;
      printf("Putting in array with key %s\n", temp.key);
      put_in_JsonObject(array, temp);
    }
  }
}
