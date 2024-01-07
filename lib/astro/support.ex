defmodule Astro.Support do
  @moduledoc """
  Support functions
  """
  use Astro.NIF, "support"

  def bodc2n(_code), do: :erlang.nif_error({:error, :not_loaded})
  def bodn2c(_name), do: :erlang.nif_error({:error, :not_loaded})
  def spkobj(_file), do: :erlang.nif_error({:error, :not_loaded})
  def bodvcd(_code, _item), do: :erlang.nif_error({:error, :not_loaded})
  def bodvrd(_name, _item), do: :erlang.nif_error({:error, :not_loaded})
end
