#!/usr/bin/env elixir

# Load dependencies
Mix.install([])

# Compile the project
Code.compiler_options(ignore_module_conflict: true)

# Load all modules manually since we can't use the Application
for file <- Path.wildcard("lib/**/*.ex") do
  Code.require_file(file)
end

# Run examples
Nanobrain.Examples.run_all()
