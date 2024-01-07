#include <stdbool.h>
#include <string.h>
#include <erl_nif.h>
#include <erfa.h>
#include "SpiceUsr.h"

static bool
load_string(ErlNifEnv *env, ERL_NIF_TERM arg, char **result)
{
  ErlNifBinary bin;

  if (!enif_inspect_binary(env, arg, &bin))
    return false;

  *result = malloc(sizeof(char) * (bin.size + 1));
  memcpy(*result, bin.data, bin.size);
  (*result)[bin.size] = '\0';

  return true;
}

static bool
load_list(ErlNifEnv *env, ERL_NIF_TERM arg, size_t l, double *result)
{
  size_t len;
  if (!enif_get_list_length(env, arg, &len) || len != l)
    return false;

  result = enif_alloc(len * sizeof(double));
  ERL_NIF_TERM head, tail = arg;

  for (int i = 0; i < len; i++)
  {
    if (!enif_get_list_cell(env, tail, &head, &tail) ||
        enif_get_double(env, head, &result[i]))
      return false;
  }

  return true;
}

static ERL_NIF_TERM
make_list(ErlNifEnv *env, double *list, size_t len)
{
  ERL_NIF_TERM result[len];

  for (int i = 0; i < len; i++)
  {
    result[i] = enif_make_double(env, list[i]);
  }

  return enif_make_list_from_array(env, result, len);
}

static ERL_NIF_TERM
make_binary(ErlNifEnv *env, char *data)
{
  ErlNifBinary bin;

  // Assuming 'data' is a null-terminated char*
  size_t length = strlen(data);

  // Create a binary term from the data
  if (enif_alloc_binary(length, &bin))
  {
    memcpy(bin.data, data, length);
    return enif_make_binary(env, &bin);
  }
  else
  {
    // Return an error term if allocation fails
    return enif_make_badarg(env);
  }
}

static int
load(ErlNifEnv *env, void **priv, ERL_NIF_TERM load_info)
{
  errdev_c("SET", 0, "NULL");
  errprt_c("SET", 0, "ALL");
  erract_c("SET", 0, "RETURN");

  SpiceChar *path;
  ERL_NIF_TERM head;

  while (enif_get_list_cell(env, load_info, &head, &load_info))
  {
    if (!load_string(env, head, &path))
      return 1;

    furnsh_c(path);
    free(path);
  }

  return 0;
}

static int
upgrade(ErlNifEnv *env, void **priv, void **old_priv, ERL_NIF_TERM load_info)
{
  return 1;
}

static void
unload(ErlNifEnv *env, void *priv)
{
  return;
}

static ERL_NIF_TERM
error_result(ErlNifEnv *env, const char *error_msg)
{
  return enif_make_tuple2(env, enif_make_atom(env, "error"), make_binary(env, error_msg));
}

static ERL_NIF_TERM
handle_error(ErlNifEnv *env)
{
  // Get the error message (1840 is max message length)
  SpiceChar error[1841];
  getmsg_c("LONG", 1840, error);

  // reset the error status
  reset_c();

  // return error
  return error_result(env, error);
}

static ERL_NIF_TERM
ok_result(ErlNifEnv *env, ERL_NIF_TERM r)
{
  return enif_make_tuple2(env, enif_make_atom(env, "ok"), r);
}

static ERL_NIF_TERM
ok_result2(ErlNifEnv *env, ERL_NIF_TERM r1, ERL_NIF_TERM r2)
{
  return enif_make_tuple3(env, enif_make_atom(env, "ok"), r1, r2);
}
