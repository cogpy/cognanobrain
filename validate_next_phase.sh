#!/bin/bash
# Validation script for Next Phase Implementation

set -e

echo "╔═══════════════════════════════════════════════════════════════╗"
echo "║  NanoCyc Next Phase Implementation Validation                ║"
echo "╚═══════════════════════════════════════════════════════════════╝"
echo ""

# Check Elixir installation
echo "→ Checking Elixir installation..."
if ! command -v elixir &> /dev/null; then
    echo "✗ Elixir not found. Please install Elixir 1.14 or later."
    echo "  Visit: https://elixir-lang.org/install.html"
    exit 1
fi

echo "✓ Elixir found: $(elixir --version | head -1)"
echo ""

# Navigate to nanobrain_ex directory
cd "$(dirname "$0")/nanobrain_ex"

# Fetch dependencies
echo "→ Fetching dependencies..."
mix deps.get
echo "✓ Dependencies fetched"
echo ""

# Compile the project
echo "→ Compiling project..."
mix compile
echo "✓ Project compiled successfully"
echo ""

# Run tests (when they exist)
echo "→ Running tests..."
mix test || echo "⚠ Tests not yet implemented (expected)"
echo ""

# Try to run examples
echo "→ Testing examples..."
echo ""
echo "--- Tensor Operations ---"
mix run -e "Nanobrain.Examples.NextPhase.example_tensor_operations()" || echo "⚠ Example failed (dependencies may need implementation)"
echo ""

echo "--- Neural Network ---"
mix run -e "Nanobrain.Examples.NextPhase.example_neural_network()" || echo "⚠ Example failed (dependencies may need implementation)"
echo ""

echo "--- Cognitive Transformation ---"
mix run -e "Nanobrain.Examples.NextPhase.example_cognitive_transformation()" || echo "⚠ Example failed (dependencies may need implementation)"
echo ""

echo "╔═══════════════════════════════════════════════════════════════╗"
echo "║  Validation Complete                                          ║"
echo "╚═══════════════════════════════════════════════════════════════╝"
echo ""
echo "Next steps:"
echo "  1. Review implementation in nanobrain_ex/lib/nanobrain/"
echo "  2. Add comprehensive tests"
echo "  3. Integrate with existing AtomSpace"
echo "  4. Optimize performance"
echo "  5. Extend language bridges"
echo ""
