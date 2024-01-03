# Astro

Library to help working with [SPICE](https://naif.jpl.nasa.gov/pub/naif/toolkit_docs/C/index.html)
and [ERFA](https://github.com/liberfa/erfa) libraries

## Installation

It is a bit more complicated then normal lib so pay attention:

- instal ERFA library
  - <https://github.com/liberfa/erfa?tab=readme-ov-file#building-and-installing-erfa>
- add `ex_astro` to `mix.exs`

```elixir
  def deps do
    [
      ...
      {:ex_astro, "~> 0.1"},
      ...
    ]
  end
```

- download SPICE kernels and add them to the config

```bash
mix astro.kernels
```
