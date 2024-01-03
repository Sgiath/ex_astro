#include "utils.h"

static ERL_NIF_TERM
spkezr(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  // inputs
  SpiceDouble et;
  SpiceChar *target = load_string(env, argv[1]);
  SpiceChar *observer = load_string(env, argv[2]);
  SpiceChar *reference_frame = load_string(env, argv[3]);
  SpiceChar *abcorr = load_string(env, argv[4]);

  if (!enif_get_double(env, argv[0], &et))
    return enif_make_badarg(env);

  // ignored output
  SpiceDouble lt;

  // output
  SpiceDouble position[6];

  // retrieve state vector at the time
  spkezr_c(target, et, reference_frame, abcorr, observer, position, &lt);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_tuple2(
      env,
      enif_make_tuple3(
          env,
          enif_make_double(env, position[0]),
          enif_make_double(env, position[1]),
          enif_make_double(env, position[2])),
      enif_make_tuple3(
          env,
          enif_make_double(env, position[3]),
          enif_make_double(env, position[4]),
          enif_make_double(env, position[5])));
}

static ERL_NIF_TERM
spkez(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  // inputs
  SpiceDouble et;
  SpiceInt target, observer;

  if (!enif_get_double(env, argv[0], &et) ||
      !enif_get_int(env, argv[1], &target) ||
      !enif_get_int(env, argv[2], &observer))
    return enif_make_badarg(env);

  SpiceChar *reference_frame = load_string(env, argv[3]);
  SpiceChar *abcorr = load_string(env, argv[4]);

  // ignored output
  SpiceDouble lt;

  // output
  SpiceDouble position[6];

  // retrieve state vector at the time
  spkez_c(target, et, reference_frame, abcorr, observer, position, &lt);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_tuple2(
      env,
      enif_make_tuple3(
          env,
          enif_make_double(env, position[0]),
          enif_make_double(env, position[1]),
          enif_make_double(env, position[2])),
      enif_make_tuple3(
          env,
          enif_make_double(env, position[3]),
          enif_make_double(env, position[4]),
          enif_make_double(env, position[5])));
}

static ERL_NIF_TERM
spkgeo(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  // inputs
  SpiceDouble et;
  SpiceInt target, observer;

  if (!enif_get_double(env, argv[0], &et) ||
      !enif_get_int(env, argv[1], &target) ||
      !enif_get_int(env, argv[2], &observer))
    return enif_make_badarg(env);

  SpiceChar *reference_frame = load_string(env, argv[3]);

  // ignored output
  SpiceDouble lt;

  // output
  SpiceDouble position[6];

  // retrieve state vector at the time
  spkgeo_c(target, et, reference_frame, observer, position, &lt);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_tuple2(
      env,
      enif_make_tuple3(
          env,
          enif_make_double(env, position[0]),
          enif_make_double(env, position[1]),
          enif_make_double(env, position[2])),
      enif_make_tuple3(
          env,
          enif_make_double(env, position[3]),
          enif_make_double(env, position[4]),
          enif_make_double(env, position[5])));
}

static ErlNifFunc nif_funcs[] = {
    {"spkezr", 5, spkezr},
    {"spkez", 5, spkez},
    {"spkgeo", 4, spkgeo},
};

ERL_NIF_INIT(Elixir.Astro.Ephemeris, nif_funcs, &load, NULL, &upgrade, &unload)
