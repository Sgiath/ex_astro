defmodule Astro.NIF do
  @moduledoc false

  defmacro __using__(name) do
    quote do
      require Logger

      @on_load :load_nifs

      @kernels Application.compile_env(:ex_astro, :spice_kernels, [])

      defp load_nifs do
        {kernels, missing_files} = Enum.split_with(@kernels, &File.exists?/1)

        Enum.each(missing_files, fn path ->
          Logger.warning("Kernel '#{path}' configured but file doesn't exists. Skipping")
        end)

        :ex_astro
        |> Application.app_dir("priv/#{unquote(name)}")
        |> String.to_charlist()
        |> :erlang.load_nif(kernels)
      end
    end
  end
end
