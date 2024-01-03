#include <string.h>
#include <erl_nif.h>
#include <erfa.h>
#include "SpiceUsr.h"

static char *
load_string(ErlNifEnv *env, ERL_NIF_TERM arg)
{
  ErlNifBinary bin; // Binary structure to hold the string

  // Get the binary argument from the Erlang term
  enif_inspect_binary(env, arg, &bin);

  // Allocate memory for the string (+1 for null terminator)
  char *str = enif_alloc(bin.size + 1);

  // Copy the binary data to the allocated memory and add null terminator
  memcpy(str, bin.data, bin.size);
  str[bin.size] = '\0';

  return str;
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
  ERL_NIF_TERM head;

  errdev_c("SET", 0, "NULL");
  errprt_c("SET", 0, "ALL");
  erract_c("SET", 0, "RETURN");

  while (enif_get_list_cell(env, load_info, &head, &load_info))
  {
    char *path = load_string(env, head);
    furnsh_c(path);
    enif_free(path);
  }

  return 0;
}

static int
upgrade(ErlNifEnv *env, void **priv, void **old_priv, ERL_NIF_TERM load_info)
{
  return 0;
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
