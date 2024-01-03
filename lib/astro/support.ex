defmodule Astro.Support do
  @moduledoc """
  Support functions
  """
  use Astro.NIF, "support"

  def named_objects do
    @kernels
    |> Enum.map(&spkobj/1)
    |> Enum.filter(&is_list/1)
    |> List.flatten()
    |> Enum.uniq()
    |> Enum.map(fn id -> {id, bodc2n(id)} end)
    |> Enum.reject(fn
      {_id, {:error, _msg}} -> true
      _otherwise -> false
    end)
    |> Enum.into(%{})
  end

  def bodc2n(_code), do: :erlang.nif_error({:error, :not_loaded})
  def bodn2c(_name), do: :erlang.nif_error({:error, :not_loaded})
  def spkobj(_file), do: :erlang.nif_error({:error, :not_loaded})
end
