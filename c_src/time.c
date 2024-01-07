#include "utils.h"

static ERL_NIF_TERM
dtf2d(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  int iy, im, id, ihr, imn;
  double sec, d1, d2;

  if (!enif_get_int(env, argv[0], &iy) ||
      !enif_get_int(env, argv[1], &im) ||
      !enif_get_int(env, argv[2], &id) ||
      !enif_get_int(env, argv[3], &ihr) ||
      !enif_get_int(env, argv[4], &imn) ||
      !enif_get_double(env, argv[5], &sec))
    return enif_make_badarg(env);

  eraDtf2d("UTC", iy, im, id, ihr, imn, sec, &d1, &d2);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_double(env, d1 + d2);
}

static ERL_NIF_TERM
utc2tai(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  double utc, tai1, tai2;
  if (!enif_get_double(env, argv[0], &utc))
    return enif_make_badarg(env);
  eraUtctai(j2000_c(), utc - j2000_c(), &tai1, &tai2);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_double(env, tai1 + tai2);
}

static ERL_NIF_TERM
tai2tt(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  double tai, tt1, tt2;
  if (!enif_get_double(env, argv[0], &tai))
    return enif_make_badarg(env);
  eraTaitt(j2000_c(), tai - j2000_c(), &tt1, &tt2);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_double(env, tt1 + tt2);
}

static ERL_NIF_TERM
tai2utc(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  double tai, utc1, utc2;
  if (!enif_get_double(env, argv[0], &tai))
    return enif_make_badarg(env);
  eraTaiutc(j2000_c(), tai - j2000_c(), &utc1, &utc2);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_double(env, utc1 + utc2);
}

static ERL_NIF_TERM
tt2tai(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  double tt, tai1, tai2;
  if (!enif_get_double(env, argv[0], &tt))
    return enif_make_badarg(env);
  eraTttai(j2000_c(), tt - j2000_c(), &tai1, &tai2);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_double(env, tai1 + tai2);
}

static ERL_NIF_TERM
tt2tcg(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  double tt, tcg1, tcg2;
  if (!enif_get_double(env, argv[0], &tt))
    return enif_make_badarg(env);
  eraTttcg(j2000_c(), tt - j2000_c(), &tcg1, &tcg2);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_double(env, tcg1 + tcg2);
}

static ERL_NIF_TERM
tt2tdb(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  double tt, dtr, g, tdb1, tdb2;
  if (!enif_get_double(env, argv[0], &tt))
    return enif_make_badarg(env);

  // calculation from:
  // https://www.stjarnhimlen.se/comp/time.html
  // https://lweb.cfa.harvard.edu/~jzhao/times.html#TDB
  g = 357.53 + 0.9856003 * (tt - j2000_c());
  dtr = 0.001658 * sin(g) + 0.000014 * sin(2 * g);

  eraTttdb(j2000_c(), tt - j2000_c(), dtr, &tdb1, &tdb2);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_double(env, tdb1 + tdb2);
}

static ERL_NIF_TERM
tcg2tt(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  double tcg, tt1, tt2;
  if (!enif_get_double(env, argv[0], &tcg))
    return enif_make_badarg(env);
  eraTcgtt(j2000_c(), tcg - j2000_c(), &tt1, &tt2);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_double(env, tt1 + tt2);
}

static ERL_NIF_TERM
tdb2tt(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  double tdb, dtr, g, tt1, tt2;
  if (!enif_get_double(env, argv[0], &tdb))
    return enif_make_badarg(env);

  // calculation from:
  // https://www.stjarnhimlen.se/comp/time.html
  // https://lweb.cfa.harvard.edu/~jzhao/times.html#TDB
  g = 357.53 + 0.9856003 * (tdb - j2000_c());
  dtr = 0.001658 * sin(g) + 0.000014 * sin(2 * g);

  eraTdbtt(j2000_c(), tdb - j2000_c(), dtr, &tt1, &tt2);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_double(env, tt1 + tt2);
}

static ERL_NIF_TERM
tdb2tcb(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  double tdb, tcb1, tcb2;
  if (!enif_get_double(env, argv[0], &tdb))
    return enif_make_badarg(env);
  eraTdbtcb(j2000_c(), tdb - j2000_c(), &tcb1, &tcb2);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_double(env, tcb1 + tcb2);
}

static ERL_NIF_TERM
tcb2tdb(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  double tcb, tdb1, tdb2;
  if (!enif_get_double(env, argv[0], &tcb))
    return enif_make_badarg(env);
  eraTcbtdb(j2000_c(), tcb - j2000_c(), &tdb1, &tdb2);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_double(env, tdb1 + tdb2);
}

static ERL_NIF_TERM
jd2dt(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  double jd, fd;
  int iy, im, id, ihmsf[4];
  char sign = '+';

  if (!enif_get_double(env, argv[0], &jd))
    return enif_make_badarg(env);

  eraJd2cal(j2000_c(), jd - j2000_c(), &iy, &im, &id, &fd);
  eraD2tf(6, fd, &sign, ihmsf);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_tuple7(
      env,
      enif_make_int(env, iy),
      enif_make_int(env, im),
      enif_make_int(env, id),
      enif_make_int(env, ihmsf[0]),
      enif_make_int(env, ihmsf[1]),
      enif_make_int(env, ihmsf[2]),
      enif_make_int(env, ihmsf[3]));
}

static ERL_NIF_TERM
str2et(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  SpiceDouble et;
  SpiceChar *timstr;

  if (!load_string(env, argv[0], &timstr))
    return enif_make_badarg(env);

  // convert to TDB (ET)
  str2et_c(timstr, &et);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_double(env, et);
}

static ERL_NIF_TERM
utc2et(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  double et;
  char *utcstr;

  if (!load_string(env, argv[0], &utcstr))
    return enif_make_badarg(env);

  // convert to TDB (ET)
  utc2et_c(utcstr, &et);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_double(env, et);
}

static ERL_NIF_TERM
unitim(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  SpiceDouble epoch;
  SpiceChar *insys, *outsys;

  if (!enif_get_double(env, argv[0], &epoch) ||
      !load_string(env, argv[1], &insys) ||
      !load_string(env, argv[2], &outsys))
    return enif_make_badarg(env);

  SpiceDouble result = unitim_c(epoch, insys, outsys);

  // check for any errors
  if (failed_c())
    return handle_error(env);

  return enif_make_double(env, result);
}

static ERL_NIF_TERM
sec2day(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  double j_sec;
  if (!enif_get_double(env, argv[0], &j_sec))
    return enif_make_badarg(env);

  return enif_make_double(env, (j_sec / spd_c()) + j2000_c());
}

static ERL_NIF_TERM
day2sec(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
  double j_day;
  if (!enif_get_double(env, argv[0], &j_day))
    return enif_make_badarg(env);

  return enif_make_double(env, (j_day - j2000_c()) * spd_c());
}

static ErlNifFunc nif_funcs[] = {
    {"dtf2d", 6, dtf2d},
    {"utc2tai", 1, utc2tai},
    {"tai2tt", 1, tai2tt},
    {"tai2utc", 1, tai2utc},
    {"tt2tai", 1, tt2tai},
    {"tt2tcg", 1, tt2tcg},
    {"tt2tdb", 1, tt2tdb},
    {"tcg2tt", 1, tcg2tt},
    {"tdb2tt", 1, tdb2tt},
    {"tdb2tcb", 1, tdb2tcb},
    {"tcb2tdb", 1, tcb2tdb},
    {"jd2dt", 1, jd2dt},
    {"str2et", 1, str2et},
    {"utc2et", 1, utc2et},
    {"unitim", 3, unitim},
    {"sec2day", 1, sec2day},
    {"day2sec", 1, day2sec},
};

ERL_NIF_INIT(Elixir.Astro.Time, nif_funcs, &load, NULL, &upgrade, &unload)
