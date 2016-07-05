/*
// Copyright (c) 2016 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#ifdef OC_SECURITY
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "port/oc_storage.h"

#define STORE_PATH_SIZE 64

char store_path[STORE_PATH_SIZE];
int store_path_len;
bool path_set = false;

void
oc_storage_config(const char *store)
{
  store_path_len = strlen(store);
  if (store_path_len >= STORE_PATH_SIZE)
    return;

  strncpy(store_path, store, store_path_len);
  path_set = true;
}

void
oc_storage_read(const char *store, uint8_t *buf, size_t *size)
{
  FILE *fp = 0;
  size_t store_len = strlen(store);

  if (!path_set || (store_len + store_path_len >= STORE_PATH_SIZE))
    goto err;

  strncpy(store_path + store_path_len, store, store_len);
  store_path[store_path_len + store_len] = '\0';
  fp = fopen(store_path, "rb");

  if (fp) {
    size_t read_size;
    read_size = fread(buf, 1, *size, fp);

    *size = read_size;
    fclose(fp);
    return;
  }

err:
  *size = 0;
}

void
oc_storage_write(const char *store, uint8_t *buf, size_t size)
{
  FILE *fp;
  size_t store_len = strlen(store);

  if (!path_set || (store_len + store_path_len >= STORE_PATH_SIZE))
    return;

  strncpy(store_path + store_path_len, store, store_len);
  store_path[store_path_len + store_len] = '\0';
  fp = fopen(store_path, "wb");

  if (fp) {
    fwrite(buf, 1, size, fp);
    fclose(fp);
  }
}
#endif /* OC_SECURITY */