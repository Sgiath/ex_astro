defmodule Astro.Ephemeris do
  @moduledoc """
  Calculation state of objects
  """
  use Astro.NIF, "ephemeris"

  require Logger

  def position(target) do
    DateTime.utc_now()
    |> DateTime.to_iso8601()
    |> Astro.Time.utc2et()
    |> spkgeo(target, 0, "J2000")
  end

  @type vec() :: {float(), float(), float()}

  @doc """
  Determine the apparent, true, or geometric state of a body with respect to another body relative
  to a user specified reference frame.
  """
  @spec spkezr(
          et :: float(),
          target :: String.t(),
          observer :: String.t(),
          ref_plane :: String.t(),
          aberration_correction :: String.t()
        ) :: {position :: vec(), velocity :: vec()} | {:error, String.t()}
  def spkezr(_et, _target, _observer, _ref_plane, _ab_corr),
    do: :erlang.nif_error({:error, :not_loaded})

  @doc """
  The function spkez/5 performs the same functions as spkezr/5. The only difference is the means by
  which objects are specified. spkez/5 requires that the target and observing bodies be specified
  using the NAIF integer ID codes for those bodies.

  spkez/5 is useful in those situations when you have ID codes for objects stored as integers. There
  is also a modest efficiency gain when using integer ID codes instead of character strings to
  specify targets and observers.
  """
  @spec spkez(
          et :: float(),
          target :: integer(),
          observer :: integer(),
          ref_plane :: String.t(),
          aberration_correction :: String.t()
        ) :: {position :: vec(), velocity :: vec()} | {:error, String.t()}
  def spkez(_et, _target, _observer, _ref_plane, _ab_corr),
    do: :erlang.nif_error({:error, :not_loaded})

  @doc """
  The function spkgeo/4 returns only geometric (uncorrected) states.

  spkgeo/4 involves slightly less overhead than does spkez/5 and thus may be marginally faster than
  calling spkez/5
  """
  @spec spkgeo(
          et :: float(),
          target :: integer(),
          observer :: integer(),
          ref_plane :: String.t()
        ) :: {position :: vec(), velocity :: vec()} | {:error, String.t()}
  def spkgeo(_et, _target, _observer, _ref_plane),
    do: :erlang.nif_error({:error, :not_loaded})
end
