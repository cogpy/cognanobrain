"""
NanoBrain Python Bindings

Python interface to the NanoBrain C++ cognitive architecture.

Example usage:
    import nanobrain as nb
    
    config = nb.UnifiedConfig()
    config.time_crystal_dimensions = 11
    
    kernel = nb.UnifiedKernel(config)
    kernel.initialize()
    
    atom = kernel.create_atom("ConceptNode", "Cat", 0.9, 0.8)
    print(f"Created atom: {atom}")
"""

__version__ = "1.0.0"
__author__ = "NanoBrain Team"

# Import core classes - will be populated by C++ bindings
try:
    from nanobrain._core import (
        UnifiedConfig,
        UnifiedKernel,
        TimeCrystalConfig,
        TimeCrystalKernel,
        Atom,
        Link,
        TruthValue,
        AttentionValue,
    )
except ImportError:
    # C++ bindings not built yet
    pass

def get_version():
    """Return NanoBrain version string."""
    return __version__
