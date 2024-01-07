#include "utils.h"

static ERL_NIF_TERM
spkezr(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  // inputs
  SpiceChar *target, *reference_frame, *abcorr, *observer;
  SpiceDouble et;

  if (!load_string(env, argv[0], &target) ||
      !enif_get_double(env, argv[1], &et) ||
      !load_string(env, argv[2], &reference_frame) ||
      !load_string(env, argv[3], &abcorr) ||
      !load_string(env, argv[4], &observer))
    return enif_make_badarg(env);

  // output
  SpiceDouble state[6];
  SpiceDouble lt;

  // retrieve state vector at the time
  spkezr_c(target, et, reference_frame, abcorr, observer, state, &lt);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return ok_result2(env, make_list(env, state, 6), enif_make_double(env, lt));
}

static ERL_NIF_TERM
spkez(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  // inputs
  SpiceDouble et;
  SpiceInt target, observer;
  SpiceChar *reference_frame, *abcorr;

  if (!enif_get_int(env, argv[0], &target) ||
      !enif_get_double(env, argv[1], &et) ||
      !load_string(env, argv[2], &reference_frame) ||
      !load_string(env, argv[3], &abcorr) ||
      !enif_get_int(env, argv[4], &observer))
    return enif_make_badarg(env);

  // output
  SpiceDouble state[6];
  SpiceDouble lt;

  // retrieve state vector at the time
  spkez_c(target, et, reference_frame, abcorr, observer, state, &lt);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return ok_result2(env, make_list(env, state, 6), enif_make_double(env, lt));
}

static ERL_NIF_TERM
spkgeo(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  // inputs
  SpiceDouble et;
  SpiceInt target, observer;
  SpiceChar *reference_frame;

  if (!enif_get_int(env, argv[0], &target) ||
      !enif_get_double(env, argv[1], &et) ||
      !load_string(env, argv[2], &reference_frame) ||
      !enif_get_int(env, argv[3], &observer))
    return enif_make_badarg(env);

  // output
  SpiceDouble state[6];
  SpiceDouble lt;

  // retrieve state vector at the time
  spkgeo_c(target, et, reference_frame, observer, state, &lt);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return ok_result2(env, make_list(env, state, 6), enif_make_double(env, lt));
}

static ERL_NIF_TERM
oscelt(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  SpiceDouble *state;
  SpiceDouble et;
  SpiceDouble mu;
  SpiceDouble elts[8];

  if (!load_list(env, argv[0], 6, state) ||
      !enif_get_double(env, argv[1], &et) ||
      !enif_get_double(env, argv[2], &mu))
    return enif_make_badarg(env);

  // retrieve state vector at the time
  oscelt_c(state, et, mu, elts);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return ok_result(env, make_list(env, elts, 8));
}

static ERL_NIF_TERM
conics(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  SpiceDouble *elts, et, state[6];

  if (!load_list(env, argv[0], 8, elts) ||
      !enif_get_double(env, argv[1], &et))
    return enif_make_badarg(env);

  // retrieve state vector at the time
  conics_c(elts, et, state);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return ok_result(env, make_list(env, state, 6));
}

static ErlNifFunc nif_funcs[] = {
    {"spkezr", 5, spkezr},
    {"spkez", 5, spkez},
    {"spkgeo", 4, spkgeo},
    {"oscelt", 3, oscelt},
    {"conics", 2, conics},
};

ERL_NIF_INIT(Elixir.Astro.Ephemeris, nif_funcs, &load, NULL, &upgrade, &unload)
