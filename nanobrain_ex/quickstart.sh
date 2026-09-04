#!/bin/bash

# NanoBrain Elixir Quick Start Script

echo "============================================================"
echo "NanoBrain Pure Elixir - Quick Start"
echo "============================================================"
echo ""

# Check if Elixir is installed
if ! command -v elixir &> /dev/null; then
    echo "❌ Elixir not found. Please install Elixir first:"
    echo "   Ubuntu: sudo apt-get install elixir"
    echo "   macOS:  brew install elixir"
    echo "   Other:  https://elixir-lang.org/install.html"
    exit 1
fi

echo "✅ Elixir $(elixir --version | grep Elixir | cut -d' ' -f2) found"
echo ""

# Install dependencies
echo "📦 Installing dependencies..."
mix local.hex --force --if-missing
mix local.rebar --force --if-missing
mix deps.get

echo ""
echo "🔨 Compiling project..."
mix compile

echo ""
echo "🧪 Running tests..."
mix test

echo ""
echo "🎯 Running examples..."
mix run -e "Nanobrain.Examples.run_all()"

echo ""
echo "============================================================"
echo "✅ NanoBrain Elixir is ready to use!"
echo "============================================================"
echo ""
echo "Try it in IEx:"
echo "  iex -S mix"
echo ""
echo "Or check the README.md for more information."
echo ""
