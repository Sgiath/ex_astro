defmodule Astro.MixProject do
  use Mix.Project

  @version "0.2.1"

  def project do
    [
      # Library
      app: :ex_astro,
      version: @version,

      # Elixir
      elixir: "~> 1.16",
      build_embedded: Mix.env() == :prod,
      start_permanent: Mix.env() == :prod,
      consolidate_protocols: Mix.env() != :test,
      deps: deps(),

      # Elixir make
      compilers: [:elixir_make] ++ Mix.compilers(),
      make_clean: ["clean"],

      # Docs
      name: "ex_astro",
      source_url: "https://github.com/Sgiath/ex_astro",
      homepage_url: "https://sgiath.dev/libraries#ex_astro",
      description: """
      Library wrapping around SPICE and ERFA libraries
      """,
      package: package(),
      docs: docs()
    ]
  end

  def application do
    [
      extra_applications: [:logger]
    ]
  end

  defp deps do
    [
      # HTTP client
      {:req, "~> 0.4"},

      # C compilation
      {:elixir_make, "~> 0.7", runtime: false},

      # Development
      {:ex_check, "~> 0.15", only: [:dev], runtime: false},
      {:credo, "~> 1.7", only: [:dev], runtime: false},
      {:dialyxir, "~> 1.4", only: [:dev], runtime: false},
      {:ex_doc, "~> 0.31", only: [:dev], runtime: false},
      {:mix_test_watch, "~> 1.1", only: [:dev], runtime: false}
    ]
  end

  defp package do
    [
      name: "ex_astro",
      maintainers: ["Sgiath <astro@sgiath.dev>"],
      files: ~w(lib LICENSE mix.exs README* CHANGELOG* c_src/*.[ch] Makefile),
      licenses: ["WTFPL"],
      links: %{
        "GitHub" => "https://github.com/Sgiath/ex_astro",
        "SPICE" => "https://naif.jpl.nasa.gov/pub/naif/toolkit_docs/C/index.html",
        "ERFA" => "https://github.com/liberfa/erfa"
      }
    ]
  end

  defp docs do
    [
      authors: ["sgiath <astro@sgiath.dev>"],
      main: "readme",
      api_reference: false,
      extras: [
        "README.md": [filename: "readme", title: "Overview"],
        "CHANGELOG.md": [filename: "changelog", title: "Changelog"]
      ],
      formatters: ["html"],
      source_ref: "v#{@version}",
      source_url: "https://github.com/Sgiath/ex_astro"
    ]
  end
end
